/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "engines/nancy/action/recordtypes.h"
#include "engines/nancy/nancy.h"
#include "engines/nancy/scene.h"
#include "engines/nancy/logic.h"
#include "engines/nancy/nancy.h"
#include "engines/nancy/graphics.h"
#include "engines/nancy/audio.h"
#include "engines/nancy/input.h"
#include "engines/nancy/resource.h"

#include "common/str.h"

namespace Nancy {

// Simple helper function to read rectangles
static void readRect(Common::SeekableReadStream &stream, Common::Rect &inRect) {
    inRect.left = stream.readUint32LE();
    inRect.top = stream.readUint32LE();
    inRect.right = stream.readUint32LE();
    inRect.bottom = stream.readUint32LE();
}

void HotspotDesc::readData(Common::SeekableReadStream &stream) {
    frameID = stream.readUint16LE();
    readRect(stream, coords);
}

void BitmapDesc::readData(Common::SeekableReadStream &stream) {
    frameID = stream.readUint16LE();
    readRect(stream, src);
    readRect(stream, dest);
}

void SecondaryVideoDesc::readData(Common::SeekableReadStream &stream) {
    frameID = stream.readUint16LE();
    readRect(stream, srcRect);
    readRect(stream, destRect);
    stream.skip(0x20);
}

void EventFlagsDesc::readData(Common::SeekableReadStream &stream) {
    for (uint i = 0; i < 10; ++i) {
        descs[i].label = stream.readSint16LE();
        descs[i].flag = (PlayState::Flag)stream.readUint16LE();
    }
}

void EventFlagsDesc::execute(NancyEngine *engine) {
    for (uint i = 0; i < 10; ++i) {
        if (descs[i].label != -1) {
            engine->playState.eventFlags[descs[i].label] = descs[i].flag;
        }
    }
}

uint16 SceneChange::readData(Common::SeekableReadStream &stream) {
    sceneID = stream.readUint16LE();
    frameID = stream.readUint16LE();
    verticalOffset = stream.readUint16LE();
    doNotStartSound = (bool)(stream.readUint16LE());
    return 8;
}

void SceneChange::execute(NancyEngine *engine) {
    engine->sceneManager->changeScene(sceneID, frameID, verticalOffset, doNotStartSound);
    isDone = true;
}

uint16 HotMultiframeSceneChange::readData(Common::SeekableReadStream &stream) {
    uint16 ret = SceneChange::readData(stream);
    uint16 numHotspots = stream.readUint16LE();

    for (uint i = 0; i < numHotspots; ++i) {
        hotspots.push_back(HotspotDesc());
        HotspotDesc &newDesc = hotspots[i];
        newDesc.readData(stream);
    }

    return ret + (numHotspots * 0x12) + 2;
}

void HotMultiframeSceneChange::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            // turn main rendering on
            state = kRun;
            // fall through
        case kRun:
            hasHotspot = false;
            for (uint i = 0; i < hotspots.size(); ++i) {
                if (hotspots[i].frameID == engine->playState.currentViewFrame) {
                    hasHotspot = true;
                    hotspot = hotspots[i].coords;
                }
            }
            break;
        case kActionTrigger:
            SceneChange::execute(engine);
            break;
    }
}

uint16 Hot1FrSceneChange::readData(Common::SeekableReadStream &stream) {
    SceneChange::readData(stream);
    hotspotDesc.readData(stream);
    return 0x1A;
}

void Hot1FrSceneChange::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            hotspot = hotspotDesc.coords;
            state = kRun;
            // fall through
        case kRun:
            if (hotspotDesc.frameID == engine->playState.currentViewFrame) {
                hasHotspot = true;
            } else {
                hasHotspot = false;
            }
            break;
        case kActionTrigger:
            SceneChange::execute(engine);
            break;
    }
}

uint16 HotMultiframeMultisceneChange::readData(Common::SeekableReadStream &stream) {
    stream.seek(0x14, SEEK_CUR);
    uint size = stream.readUint16LE() * 0x12 + 0x16;
    stream.seek(-0x16, SEEK_CUR);

    return readRaw(stream, size); // TODO
}

uint16 StartFrameNextScene::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x4); // TODO
}

uint16 StartStopPlayerScrolling::readData(Common::SeekableReadStream &stream) {
    type = stream.readByte();    
    return 1;
}

uint16 PlayPrimaryVideoChan0::readData(Common::SeekableReadStream &stream) {
    uint16 bytesRead = stream.pos();
    stream.read(videoData, 0x69C);

    uint16 numResponses = stream.readUint16LE();
    if (numResponses > 0) {
        for (uint i = 0; i < numResponses; ++i) {
            uint16 numConditionFlags = stream.readUint32LE();
            responses.push_back(ResponseStruct());
            ResponseStruct &response = responses[responses.size()-1];

            if (numConditionFlags > 0) {
                for (uint16 j = 0; j < numConditionFlags; ++j) {
                    response.conditionFlags.push_back(ConditionFlags());
                    ConditionFlags &flags = response.conditionFlags[response.conditionFlags.size()-1];
                    stream.read(flags.unknown, 5);
                }
            }

            stream.read(response.unknown, 0x1D8);
        }
    }

    uint16 numSceneBranchStructs = stream.readUint16LE();
    if (numSceneBranchStructs > 0) {
        // TODO
    }

    uint16 numFlagsStructs = stream.readUint16LE();
    if (numFlagsStructs > 0)  {
        for (uint16 i = 0; i < numFlagsStructs; ++i) {
            uint16 numConditionFlags = stream.readUint16LE();
            flagsStructs.push_back(FlagsStruct());
            FlagsStruct &flagsStruct = flagsStructs[flagsStructs.size()-1];

            if (numConditionFlags > 0) {
                // Not sure about this
                if (numConditionFlags > 0) {
                for (uint16 j = 0; j < numConditionFlags; ++j) {
                    flagsStruct.conditionFlags.push_back(ConditionFlags());
                    ConditionFlags &flags = flagsStruct.conditionFlags[flagsStruct.conditionFlags.size()-1];
                    stream.read(flags.unknown, 5);
                }
            }
            }

            flagsStruct.unknown = stream.readUint32LE();
        }
    }

    bytesRead = stream.pos() - bytesRead;
    return bytesRead;
}

uint16 PlaySecondaryVideo::readData(Common::SeekableReadStream &stream) {
    char buf[10];
    stream.read(buf, 10);
    filename = Common::String(buf);
    stream.skip(0x14);
    loopFirstFrame = stream.readUint16LE();
    loopLastFrame = stream.readUint16LE();
    onHoverFirstFrame = stream.readUint16LE();
    onHoverLastFrame = stream.readUint16LE();
    onHoverEndFirstFrame = stream.readUint16LE();
    onHoverEndLastFrame = stream.readUint16LE();
    SceneChange::readData(stream);
    stream.skip(1);

    uint16 numVideoDescs = stream.readUint16LE();
    for (uint i = 0; i < numVideoDescs; ++i) {
        videoDescs.push_back(SecondaryVideoDesc());
        videoDescs[i].readData(stream);
    }

    return 0x35 + (numVideoDescs * 0x42);
}

void PlaySecondaryVideo::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            engine->graphics->loadSecondaryVideo(channelID(), filename, this);
            engine->graphics->setupSecondaryVideo(channelID(), loopFirstFrame, loopLastFrame, true);
            state = kRun;
            // fall through
        case kRun: {
            ZRenderStruct &zr = engine->graphics->getZRenderStruct("SEC VIDEO 0");
            zr.isActive = false;
            hasHotspot = false;

            int activeFrame = -1;

            for (uint i = 0; i < videoDescs.size(); ++i) {
                if (videoDescs[i].frameID == engine->playState.currentViewFrame) {
                    activeFrame = i;
                }
            }

            if (activeFrame != -1) {
                // Activate the ZRenderStruct
                zr.sourceRect = videoDescs[activeFrame].srcRect;
                zr.destRect = videoDescs[activeFrame].destRect;
                zr.destRect.left += engine->graphics->viewportDesc.destination.left;
                zr.destRect.top += engine->graphics->viewportDesc.destination.top;
                zr.destRect.top -= engine->playState.verticalScroll;
                zr.destRect.right += engine->graphics->viewportDesc.destination.left;
                zr.destRect.bottom += engine->graphics->viewportDesc.destination.top;
                zr.destRect.bottom -= engine->playState.verticalScroll;
                zr.isActive = true;

                // Activate the hotspot
                hotspot = videoDescs[activeFrame].destRect;
                hasHotspot = true;

                // check if we're hovered this frame
                bool isHovered = engine->logic->getActionRecord(engine->input->hoveredElementID) == this;

                switch (hoverState) {
                    case kEndHoverDone:
                        hoverState = kNoHover;
                        engine->graphics->setupSecondaryVideo(channelID(), loopFirstFrame, loopLastFrame, true);
                        break;
                    case kNoHover:
                        if (isHovered) {
                            // Player has just hovered over, play the hover animation once
                            hoverState = kHover;
                            engine->graphics->setupSecondaryVideo(channelID(), onHoverFirstFrame, onHoverLastFrame, false);
                        }
                        break;
                    case kHover:
                        if (!isHovered) {
                            // Player has just stopped hovering, reverse the playback and go back to frame 0
                            hoverState = kEndHover;
                            engine->graphics->setupSecondaryVideo(channelID(), onHoverEndLastFrame, onHoverEndFirstFrame, false);
                        }
                        break;
                    case kEndHover:
                        break;
                }

                engine->graphics->playSecondaryVideo(channelID());
            } else {
                engine->graphics->stopSecondaryVideo(channelID());
            }

            break;
        }
        case kActionTrigger:
            SceneChange::execute(engine);
            break;
    }
}

uint16 PlaySecondaryMovie::readData(Common::SeekableReadStream &stream) {  
    char name[10];
    stream.read(name, 10);
    videoName = Common::String(name);

    stream.skip(0x1C);
    for (uint i = 0; i < 15; ++i) {
        frameFlags[i].frameID = stream.readSint16LE();
        frameFlags[i].flagDesc.label = stream.readSint16LE();
        frameFlags[i].flagDesc.flag = (PlayState::Flag)stream.readUint16LE();
    }

    triggerFlags.readData(stream);
    stream.read(name, 10);
    soundName = Common::String(name);
    soundChannel = stream.readUint16LE();
    stream.skip(0xE);
    soundVolume = stream.readUint16LE();
    
    stream.skip(6);
    SceneChange::readData(stream);

    uint16 numVideoDescs = stream.readUint16LE();
    for (uint i = 0; i < numVideoDescs; ++i) {
        videoDescs.push_back(SecondaryVideoDesc());
        videoDescs[i].readData(stream);
    }

    return 0xD4 + numVideoDescs * 0x42; // TODO
}

void PlaySecondaryMovie::execute(NancyEngine *engine) {
    ZRenderStruct &zr = engine->graphics->getZRenderStruct("SEC MOVIE");

    switch (state) {
        case kBegin:
            engine->graphics->loadSecondaryMovie(videoName);
            if (soundName != "NO SOUND") {
                engine->sound->loadSound(soundName, soundChannel, 1, soundVolume);
            }
            state = kRun;
            // fall through
        case kRun: {
            zr.isActive = false;
            engine->graphics->getZRenderStruct("CUR IMAGE CURSOR").isActive = false;
            
            int activeFrame = -1;

            for (uint i = 0; i < videoDescs.size(); ++i) {
                if (videoDescs[i].frameID == engine->playState.currentViewFrame) {
                    activeFrame = i;
                }
            }

            if (activeFrame != -1) {
                // Activate the ZRenderStruct
                zr.sourceRect = videoDescs[activeFrame].srcRect;
                zr.destRect = videoDescs[activeFrame].destRect;
                zr.destRect.left += engine->graphics->viewportDesc.destination.left;
                zr.destRect.top += engine->graphics->viewportDesc.destination.top;
                zr.destRect.top -= engine->playState.verticalScroll;
                zr.destRect.right += engine->graphics->viewportDesc.destination.left;
                zr.destRect.bottom += engine->graphics->viewportDesc.destination.top;
                zr.destRect.bottom -= engine->playState.verticalScroll;
                zr.isActive = true;

                // Start sound if any
                if (soundName != "NO SOUND") {
                    engine->sound->pauseSound(soundChannel, false);
                }
            }

            uint16 frame = 0;
            if (!engine->graphics->playSecondaryMovie(frame)) {
                // No new frame, check for flags
                for (uint i = 0; i < 16; ++i) {
                    if (frameFlags[i].frameID == frame) {
                        engine->playState.eventFlags[frameFlags[i].flagDesc.label] = frameFlags[i].flagDesc.flag;
                    }
                }
            }

            break;
        }
        case kActionTrigger:
            triggerFlags.execute(engine);
            SceneChange::execute(engine);
            break;
    }
}

uint16 PlayStaticBitmapAnimation::readData(Common::SeekableReadStream &stream) {
    // TODO
    uint16 bytesRead = stream.pos();
    byte *seek = bitmapData;
    
    uint16 currentSize = 0x72;
    stream.read(seek, currentSize);

    seek += currentSize;
    currentSize = (uint16)(bitmapData[0x18]) - (uint16)(bitmapData[0x16]);
    ++currentSize;
    currentSize *= 16;
    stream.read(seek, currentSize);

    seek += 0x252;
    currentSize = (uint16)(bitmapData[0x70]);
    currentSize *= 34;
    stream.read(seek, currentSize);

    bytesRead= stream.pos() - bytesRead;
    return bytesRead;
}

uint16 PlayIntStaticBitmapAnimation::readData(Common::SeekableReadStream &stream) {
    uint beginOffset = stream.pos();
    char name[10];
    stream.read(name, 10);
    imageName = Common::String(name);

    stream.skip(0xA);
    firstFrame = stream.readUint16LE();
    stream.skip(2);
    lastFrame = stream.readUint16LE();
    frameTime = Common::Rational(1000, stream.readUint16LE()).toInt();
    stream.skip(2);
    soundFlagDesc.label = stream.readSint16LE();
    soundFlagDesc.flag = (PlayState::Flag)stream.readUint16LE();

    SceneChange::readData(stream);

    triggerFlags.readData(stream);

    stream.read(name, 10);
    soundName = Common::String(name);
    channelID = stream.readUint16LE();

    stream.seek(beginOffset + 0x74, SEEK_SET);
    uint numFrames = stream.readUint16LE();

    for (uint i = firstFrame; i <= lastFrame; ++i) {
        frameRects.push_back(Common::Rect());
        readRect(stream, frameRects[i]);
    }

    for (uint i = 0; i < numFrames; ++i) {
        bitmaps.push_back(BitmapDesc());
        BitmapDesc &rects = bitmaps[i];
        rects.frameID = stream.readUint16LE();
        readRect(stream, rects.src);
        readRect(stream, rects.dest);
    }

    return 0x76 + numFrames * 0x22 + (lastFrame - firstFrame + 1) * 16;
}

void PlayIntStaticBitmapAnimation::execute(NancyEngine *engine) {
    // TODO handle sound, event flags
    ZRenderStruct &zr = engine->graphics->getZRenderStruct("STATIC BITMAP ANIMATION");
    uint32 currentFrameTime = engine->getTotalPlayTime();
    switch (state) {
        case kBegin:

            // find the correct source and destination for the current viewport frame
            lastViewFrame = engine->playState.currentViewFrame;
            for (uint i = 0; i < bitmaps.size(); ++i) {
                if (lastViewFrame == bitmaps[i].frameID) {
                    currentViewFrameID = i;
                    break;
                }
            }

            currentFrame = firstFrame;
            nextFrameTime = currentFrameTime + frameTime;

            // Load the image directly into the generic surface
            // instead of inside the record; this also means we skip
            // using the source rect in srcDestRect.
            // This _may_ lead to problems (but probably won't)
            zr.sourceSurface->free();
            engine->_res->loadImage("ciftree", imageName, *zr.sourceSurface);

            // Set up the ZRenderStruct and its surface
            zr.sourceRect = frameRects[currentFrame - firstFrame];
            if (currentViewFrameID != -1) {
                zr.isActive = true;
                zr.destRect = bitmaps[currentViewFrameID].dest;
                zr.destRect.left += engine->graphics->viewportDesc.destination.left;
                zr.destRect.top += engine->graphics->viewportDesc.destination.top;
                zr.destRect.top -= engine->playState.verticalScroll;
                zr.destRect.right += engine->graphics->viewportDesc.destination.left;
                zr.destRect.bottom += engine->graphics->viewportDesc.destination.top;
                zr.destRect.bottom -= engine->playState.verticalScroll;
            }

            if (soundName != "NO SOUND") {
                warning("PlayIntStaticBitmapAnimation has a sound, please implement it!");
            }
            state = kRun;
            // fall through
        case kRun:
            // Check if we've moved the viewport
            if (lastViewFrame != engine->playState.currentViewFrame) {
                lastViewFrame = engine->playState.currentViewFrame;
                currentViewFrameID = -1;
                for (uint i = 0; i < bitmaps.size(); ++i) {
                    if (lastViewFrame == bitmaps[i].frameID) {
                        currentViewFrameID = i;
                        zr.isActive = true;
                        zr.destRect = bitmaps[currentViewFrameID].dest;
                        zr.destRect.left += engine->graphics->viewportDesc.destination.left;
                        zr.destRect.top += engine->graphics->viewportDesc.destination.top;
                        zr.destRect.top -= engine->playState.verticalScroll;
                        zr.destRect.right += engine->graphics->viewportDesc.destination.left;
                        zr.destRect.bottom += engine->graphics->viewportDesc.destination.top;
                        zr.destRect.bottom -= engine->playState.verticalScroll;
                        break;
                    }
                }
            }

            if (currentViewFrameID == -1) {
                break;
            }

            // Check the timer to see if we need to draw the next animation frame
            if (nextFrameTime <= currentFrameTime) {
                nextFrameTime = currentFrameTime + frameTime;

                currentFrame = ++currentFrame > lastFrame ? firstFrame : currentFrame;

                zr.sourceRect = frameRects[currentFrame - firstFrame];
            }
            break;
        case kActionTrigger:
            triggerFlags.execute(engine);

            SceneChange::execute(engine);
            break;
    }
}

uint16 MapCall::readData(Common::SeekableReadStream &stream) {
    stream.skip(1);
    return 1;
}

void MapCall::execute(NancyEngine *engine) {
    execType = 2;
    engine->sceneManager->stateChangeRequests |= SceneManager::kMap;
    // call base, depends on execType
    state = kBegin;
}

uint16 MapCallHot1Fr::readData(Common::SeekableReadStream &stream) {
    hotspotDesc.readData(stream);
    return 0x12;
}

void MapCallHot1Fr::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            hotspot = hotspotDesc.coords;
            state = kRun;
            // fall through
        case kRun:
            if (engine->playState.currentViewFrame == hotspotDesc.frameID) {
                hasHotspot = true;
            }
            break;
        case kActionTrigger:
            MapCall::execute(engine);
            break;
    }
}

uint16 MapCallHotMultiframe::readData(Common::SeekableReadStream &stream) {
    uint16 numDescs = stream.readUint16LE();
    for (uint i = 0; i < numDescs; ++i) {
        hotspots.push_back(HotspotDesc());
        hotspots[i].readData(stream);
    }

    return 2 + numDescs * 0x12;
}

void MapCallHotMultiframe::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            state = kRun;
            // fall through
        case kRun:
            hasHotspot = false;
            for (uint i = 0; i < hotspots.size(); ++i) {
                if (hotspots[i].frameID == engine->playState.currentViewFrame) {
                    hasHotspot = true;
                    hotspot = hotspots[i].coords;
                }
            }
            break;
        case kActionTrigger:
            MapCall::execute(engine);
            break;  
    }
}

uint16 MapLocationAccess::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x4); // TODO
}

uint16 MapSound::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x10); // TODO
}

uint16 MapAviOverride::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x2); // TODO
}

uint16 MapAviOverrideOff::readData(Common::SeekableReadStream &stream) {
    overrideOffData = stream.readByte();
    return 1;
}

uint16 TextBoxWrite::readData(Common::SeekableReadStream &stream) {
    uint16 size = stream.readUint16LE();
    stream.seek(-2, SEEK_CUR);

    if (size > 0x2710) {
        error("Action Record atTextboxWrite has too many text box chars: %d", size);;
    }

    return readRaw(stream, size+2); // TODO
}

uint16 TextBoxClear::readData(Common::SeekableReadStream &stream) {
    clearData = stream.readByte();
    return 1;
}

uint16 BumpPlayerClock::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x5); // TODO
}

uint16 SaveContinueGame::readData(Common::SeekableReadStream &stream) {
    saveContinueData = stream.readByte();
    return 1;
}

uint16 TurnOffMainRendering::readData(Common::SeekableReadStream &stream) {
    turnOffData = stream.readByte();
    return 1;
}

uint16 TurnOnMainRendering::readData(Common::SeekableReadStream &stream) {
    turnOnData = stream.readByte();
    return 1;
}

uint16 ResetAndStartTimer::readData(Common::SeekableReadStream &stream) {
    stream.skip(1);
    return 1;
}

void ResetAndStartTimer::execute(NancyEngine *engine) {
    engine->playState.timerIsActive = true;
    engine->playState.timerTime = 0;
    isDone = true;
}

uint16 StopTimer::readData(Common::SeekableReadStream &stream) {
    stream.skip(1);
    return 1;
}

void StopTimer::execute(NancyEngine *engine) {
    engine->playState.timerIsActive = false;
    engine->playState.timerTime = 0;
    isDone = true;
}

uint16 EventFlags::readData(Common::SeekableReadStream &stream) {
    flags.readData(stream);
    return 0x28;
}

void EventFlags::execute(NancyEngine *engine) {
    flags.execute(engine);
    isDone = true;
}

uint16 EventFlagsMultiHS::readData(Common::SeekableReadStream &stream) {
    uint16 returnSize = EventFlags::readData(stream);
    uint16 numHotspots = stream.readUint16LE();
    for (uint16 i = 0; i < numHotspots; ++i) {
        hotspots.push_back(HotspotDesc());
        HotspotDesc &newDesc = hotspots[i];
        newDesc.readData(stream);
    }
    returnSize += numHotspots * 0x12 + 0x2;

    return returnSize;
}

void EventFlagsMultiHS::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            // turn main rendering on
            state = kRun;
            // fall through
        case kRun:
            hasHotspot = false;
            for (uint i = 0; i < hotspots.size(); ++i) {
                if (hotspots[i].frameID == engine->playState.currentViewFrame) {
                    hasHotspot = true;
                    hotspot = hotspots[i].coords;
                }
            }
            break;
        case kActionTrigger:
            hasHotspot = false;
            EventFlags::execute(engine);
            break;
        default:
            break;
    }
}

uint16 OrderingPuzzle::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x26D); // TODO
}

uint16 LoseGame::readData(Common::SeekableReadStream &stream) {
    loseData = stream.readByte();
    return 1;
}

uint16 PushScene::readData(Common::SeekableReadStream &stream) {
    pushData = stream.readByte();
    return 1;
}

uint16 PopScene::readData(Common::SeekableReadStream &stream) {
    popData = stream.readByte();
    return 1;
}

uint16 WinGame::readData(Common::SeekableReadStream &stream) {
    winData = stream.readByte();
    return 1;
}

uint16 LeverPuzzle::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x192); // TODO
}

uint16 Telephone::readData(Common::SeekableReadStream &stream) {
    rawData = new byte[0x2016];
    stream.read(rawData, 0x48C);

    int32 sizeNext = (int16)(rawData[0x48A]) * 235;
    stream.read(rawData + 0x48C, sizeNext);
    return sizeNext + 0x48C;
}

uint16 SliderPuzzle::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x544); // TODO
}

uint16 PasswordPuzzle::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0xD7); // TODO
}

uint16 AddInventoryNoHS::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x2); // TODO
}

uint16 RemoveInventoryNoHS::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x2); // TODO
}

uint16 DifficultyLevel::readData(Common::SeekableReadStream &stream) {
    difficulty = stream.readUint16LE();
    flagLabel = stream.readSint16LE();
    flagCondition = stream.readUint16LE();
    return 6;
}

void DifficultyLevel::execute(NancyEngine *engine) {
    engine->playState.difficulty = difficulty;
    if (flagLabel != -1) {
        engine->playState.eventFlags[flagLabel] = (PlayState::Flag)flagCondition;
    }
    isDone = true;
}

uint16 RotatingLockPuzzle::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x2A4); // TODO
}

uint16 ShowInventoryItem::readData(Common::SeekableReadStream &stream) {
    objectID = stream.readUint16LE();
    char name[10];
    stream.read(name, 10);
    imageName = Common::String(name);

    uint16 numFrames = stream.readUint16LE();

    for (uint i = 0; i < numFrames; ++i) {
        bitmaps.push_back(BitmapDesc());
        bitmaps[i].readData(stream);
    }

    return 0xE + 0x22 * numFrames;
}

void ShowInventoryItem::execute(NancyEngine *engine) {
    ZRenderStruct &zr = engine->graphics->getZRenderStruct("INV BITMAP");
    switch (state) {
        case kBegin:
            zr.sourceSurface->free();
            engine->_res->loadImage("ciftree", imageName, *zr.sourceSurface);
            state = kRun;
            // fall through
        case kRun:
            zr.isActive = false;
            hasHotspot = false;

            for (uint i = 0; i < bitmaps.size(); ++i) {
                if (bitmaps[i].frameID == engine->playState.currentViewFrame) {
                    hasHotspot = true;
                    hotspot = bitmaps[i].dest;
                    
                    zr.isActive = true;
                    zr.sourceRect = bitmaps[i].src;
                    zr.destRect = bitmaps[i].dest;
                    View &viewport = engine->graphics->viewportDesc;
                    zr.destRect.left += viewport.destination.left;
                    zr.destRect.right += viewport.destination.left;
                    zr.destRect.top += viewport.destination.top - engine->playState.verticalScroll;
                    zr.destRect.bottom += viewport.destination.top - engine->playState.verticalScroll;
                }
            }
            break;
        case kActionTrigger:
            // TODO play sound
            engine->sceneManager->addObjectToInventory(objectID);
            zr.isActive = false;
            hasHotspot = false;
            isDone = true;
    }
}

uint16 PlayDigiSoundAndDie::readData(Common::SeekableReadStream &stream) {
    char str[10];
    stream.read(str, 10);
    filename = Common::String(str);
    id = stream.readSint16LE();
    stream.skip(4);
    numLoops = stream.readUint16LE();
    stream.skip(4);
    volume = stream.readUint16LE();
    stream.skip(6);
    SceneChange::readData(stream);
    return 0x2B;
}

void PlayDigiSoundAndDie::execute(NancyEngine *engine) {
    switch (state) {
        case kBegin:
            engine->sound->loadSound(filename, id, numLoops, volume);
            engine->sound->pauseSound(id, false);
            state = kRun;
            break;
        case kRun:
            if (!engine->sound->isSoundPlaying(id)) {
                state = kActionTrigger;
            }
            break;
        case kActionTrigger:
            if (sceneID != 9999) {
                SceneChange::execute(engine);
            }
            break;
    }
}

uint16 PlaySoundPanFrameAnchorAndDie::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x20); // TODO
}

uint16 PlaySoundMultiHS::readData(Common::SeekableReadStream &stream) {
    stream.seek(0x2F, SEEK_CUR);
    uint16 size = stream.readUint16LE() * 0x12 + 0x31;
    stream.seek(-0x31, SEEK_CUR);

    return readRaw(stream, size); // TODO
}

uint16 HintSystem::readData(Common::SeekableReadStream &stream) {
    return readRaw(stream, 0x23); // TODO
}

} // End of namespace Nancy