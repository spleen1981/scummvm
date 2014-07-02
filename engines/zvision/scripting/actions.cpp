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

#include "common/scummsys.h"

#include "zvision/scripting/actions.h"

#include "zvision/zvision.h"
#include "zvision/scripting/script_manager.h"
#include "zvision/graphics/render_manager.h"
#include "zvision/sound/zork_raw.h"
#include "zvision/video/zork_avi_decoder.h"
#include "zvision/scripting/sidefx/timer_node.h"
#include "zvision/scripting/sidefx/music_node.h"
#include "zvision/scripting/sidefx/syncsound_node.h"
#include "zvision/scripting/sidefx/animation_node.h"
#include "zvision/scripting/sidefx/ttytext_node.h"

#include "common/file.h"

#include "audio/decoders/wave.h"


namespace ZVision {

//////////////////////////////////////////////////////////////////////////////
// ActionAdd
//////////////////////////////////////////////////////////////////////////////

ActionAdd::ActionAdd(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(), "%u,%d", &_key, &_value);
}

bool ActionAdd::execute() {
	_engine->getScriptManager()->setStateValue(_key, _engine->getScriptManager()->getStateValue(_key) + _value);
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionAssign
//////////////////////////////////////////////////////////////////////////////

ActionAssign::ActionAssign(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char buf[64];
	memset(buf, 0, 64);
	sscanf(line.c_str(), "%u, %s", &_key, buf);
	_value = new ValueSlot(_engine->getScriptManager(), buf);
}

ActionAssign::~ActionAssign() {
	if (_value)
		delete _value;
}

bool ActionAssign::execute() {
	_engine->getScriptManager()->setStateValue(_key, _value->getValue());
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionAttenuate
//////////////////////////////////////////////////////////////////////////////

ActionAttenuate::ActionAttenuate(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(), "%u, %d", &_key, &_attenuation);
}

bool ActionAttenuate::execute() {
	SideFX *fx = _engine->getScriptManager()->getSideFX(_key);
	if (fx && fx->getType() == SideFX::SIDEFX_AUDIO) {
		MusicNode *mus = (MusicNode *)fx;
		mus->setVolume(255 - (abs(_attenuation) >> 7));
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionChangeLocation
//////////////////////////////////////////////////////////////////////////////

ActionChangeLocation::ActionChangeLocation(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(), "%c, %c, %c%c, %u", &_world, &_room, &_node, &_view, &_offset);
}

bool ActionChangeLocation::execute() {
	// We can't directly call ScriptManager::ChangeLocationIntern() because doing so clears all the Puzzles, and thus would corrupt the current puzzle checking
	_engine->getScriptManager()->changeLocation(_world, _room, _node, _view, _offset);
	// Tell the puzzle system to stop checking any more puzzles
	return false;
}


//////////////////////////////////////////////////////////////////////////////
// ActionCrossfade
//////////////////////////////////////////////////////////////////////////////

ActionCrossfade::ActionCrossfade(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(),
	       "%u %u %d %d %d %d %d",
	       &_keyOne, &_keyTwo, &_oneStartVolume, &_twoStartVolume, &_oneEndVolume, &_twoEndVolume, &_timeInMillis);
}

bool ActionCrossfade::execute() {
	if (_keyOne) {
		SideFX *fx = _engine->getScriptManager()->getSideFX(_keyOne);
		if (fx && fx->getType() == SideFX::SIDEFX_AUDIO) {
			MusicNode *mus = (MusicNode *)fx;
			if (_oneStartVolume >= 0)
				mus->setVolume((_oneStartVolume * 255) / 100);

			mus->setFade(_timeInMillis, (_oneEndVolume * 255) / 100);
		}
	}

	if (_keyTwo) {
		SideFX *fx = _engine->getScriptManager()->getSideFX(_keyTwo);
		if (fx && fx->getType() == SideFX::SIDEFX_AUDIO) {
			MusicNode *mus = (MusicNode *)fx;
			if (_twoStartVolume >= 0)
				mus->setVolume((_twoStartVolume * 255) / 100);

			mus->setFade(_timeInMillis, (_twoEndVolume * 255) / 100);
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionDisableControl
//////////////////////////////////////////////////////////////////////////////

ActionDisableControl::ActionDisableControl(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(), "%u", &_key);
}

bool ActionDisableControl::execute() {
	_engine->getScriptManager()->setStateFlag(_key, Puzzle::DISABLED);
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionEnableControl
//////////////////////////////////////////////////////////////////////////////

ActionEnableControl::ActionEnableControl(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(), "%u", &_key);
}

bool ActionEnableControl::execute() {
	_engine->getScriptManager()->unsetStateFlag(_key, Puzzle::DISABLED);
	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionInventory
//////////////////////////////////////////////////////////////////////////////

ActionInventory::ActionInventory(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char buf[25];
	sscanf(line.c_str(), "%25s %d", buf, &_key);

	if (strcmp(buf, "add") == 0) {
		_type = 0;
	} else if (strcmp(buf, "addi") == 0) {
		_type = 1;
	} else if (strcmp(buf, "drop") == 0) {
		_type = 2;
	} else if (strcmp(buf, "dropi") == 0) {
		_type = 3;
	} else if (strcmp(buf, "cycle") == 0) {
		_type = 4;
	}

}

bool ActionInventory::execute() {
	switch (_type) {
	case 0: // add
		_engine->getScriptManager()->invertory_add(_key);
		break;
	case 1: // addi
		_engine->getScriptManager()->invertory_add(_engine->getScriptManager()->getStateValue(_key));
		break;
	case 2: // drop
		if (_key >= 0)
			_engine->getScriptManager()->invertory_drop(_key);
		else
			_engine->getScriptManager()->invertory_drop(_engine->getScriptManager()->getStateValue(StateKey_InventoryItem));
		break;
	case 3: // dropi
		_engine->getScriptManager()->invertory_drop(_engine->getScriptManager()->getStateValue(_key));
		break;
	case 4: // cycle
		_engine->getScriptManager()->invertory_cycle();
		break;
	default:
		break;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionKill
//////////////////////////////////////////////////////////////////////////////

ActionKill::ActionKill(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	_key = 0;
	_type = 0;
	char keytype[25];
	sscanf(line.c_str(), "%25s", keytype);
	if (keytype[0] == '"') {
		if (!scumm_stricmp(keytype, "\"ANIM\""))
			_type = SideFX::SIDEFX_ANIM;
		else if (!scumm_stricmp(keytype, "\"AUDIO\""))
			_type = SideFX::SIDEFX_AUDIO;
		else if (!scumm_stricmp(keytype, "\"DISTORT\""))
			_type = SideFX::SIDEFX_DISTORT;
		else if (!scumm_stricmp(keytype, "\"PANTRACK\""))
			_type = SideFX::SIDEFX_PANTRACK;
		else if (!scumm_stricmp(keytype, "\"REGION\""))
			_type = SideFX::SIDEFX_REGION;
		else if (!scumm_stricmp(keytype, "\"TIMER\""))
			_type = SideFX::SIDEFX_TIMER;
		else if (!scumm_stricmp(keytype, "\"TTYTEXT\""))
			_type = SideFX::SIDEFX_TTYTXT;
		else if (!scumm_stricmp(keytype, "\"ALL\""))
			_type = SideFX::SIDEFX_ALL;
	} else
		_key = atoi(keytype);
}

bool ActionKill::execute() {
	if (_type)
		_engine->getScriptManager()->killSideFxType((SideFX::SideFXType)_type);
	else
		_engine->getScriptManager()->killSideFx(_key);
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionMusic
//////////////////////////////////////////////////////////////////////////////

ActionMusic::ActionMusic(ZVision *engine, int32 slotkey, const Common::String &line, bool global) :
	ResultAction(engine, slotkey),
	_volume(255),
	_universe(global) {
	uint type;
	char fileNameBuffer[25];
	uint loop;
	uint volume = 255;

	sscanf(line.c_str(), "%u %25s %u %u", &type, fileNameBuffer, &loop, &volume);

	// type 4 are midi sound effect files
	if (type == 4) {
		_soundType = Audio::Mixer::kSFXSoundType;
		_fileName = Common::String::format("midi/%s/%u.wav", fileNameBuffer, loop);
		_loop = false;
	} else {
		// TODO: See what the other types are so we can specify the correct Mixer::SoundType. In the meantime use kPlainSoundType
		_soundType = Audio::Mixer::kPlainSoundType;
		_fileName = Common::String(fileNameBuffer);
		_loop = loop == 1 ? true : false;
	}

	// Volume is optional. If it doesn't appear, assume full volume
	if (volume != 255) {
		// Volume in the script files is mapped to [0, 100], but the ScummVM mixer uses [0, 255]
		_volume = volume * 255 / 100;
	}
}

ActionMusic::~ActionMusic() {
	if (!_universe)
		_engine->getScriptManager()->killSideFx(_slotkey);
}

bool ActionMusic::execute() {
	if (_engine->getScriptManager()->getSideFX(_slotkey))
		return true;

	if (!_engine->getSearchManager()->hasFile(_fileName))
		return true;

	_engine->getScriptManager()->addSideFX(new MusicNode(_engine, _slotkey, _fileName, _loop, _volume));

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionPanTrack
//////////////////////////////////////////////////////////////////////////////

ActionPanTrack::ActionPanTrack(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey),
	_pos(0),
	_mus_slot(0) {

	sscanf(line.c_str(), "%u %d", &_mus_slot, &_pos);
}

ActionPanTrack::~ActionPanTrack() {
	_engine->getScriptManager()->killSideFx(_slotkey);
}

bool ActionPanTrack::execute() {
	if (_engine->getScriptManager()->getSideFX(_slotkey))
		return true;

	_engine->getScriptManager()->addSideFX(new PanTrackNode(_engine, _slotkey, _mus_slot, _pos));

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionPreloadAnimation
//////////////////////////////////////////////////////////////////////////////

ActionPreloadAnimation::ActionPreloadAnimation(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char fileName[25];

	// The two %*u are always 0 and dont seem to have a use
	sscanf(line.c_str(), "%25s %*u %*u %d %d", fileName, &_mask, &_framerate);

	if (_mask > 0) {
		byte r, g, b;
		Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0).colorToRGB(_mask, r, g, b);
		_mask = _engine->_pixelFormat.RGBToColor(r, g, b);
	}

	_fileName = Common::String(fileName);
}

ActionPreloadAnimation::~ActionPreloadAnimation() {
	_engine->getScriptManager()->deleteSideFx(_slotkey);
}

bool ActionPreloadAnimation::execute() {
	AnimationNode *nod = (AnimationNode *)_engine->getScriptManager()->getSideFX(_slotkey);

	if (!nod) {
		nod = new AnimationNode(_engine, _slotkey, _fileName, _mask, _framerate, false);
		_engine->getScriptManager()->addSideFX(nod);
	} else
		nod->stop();
	_engine->getScriptManager()->setStateValue(_slotkey, 2);
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionPlayAnimation
//////////////////////////////////////////////////////////////////////////////

ActionPlayAnimation::ActionPlayAnimation(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char fileName[25];

	// The two %*u are always 0 and dont seem to have a use
	sscanf(line.c_str(),
	       "%25s %u %u %u %u %u %u %d %*u %*u %d %d",
	       fileName, &_x, &_y, &_x2, &_y2, &_start, &_end, &_loopCount, &_mask, &_framerate);

	if (_mask > 0) {
		byte r, g, b;
		Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0).colorToRGB(_mask, r, g, b);
		_mask = _engine->_pixelFormat.RGBToColor(r, g, b);
	}

	_fileName = Common::String(fileName);
}

ActionPlayAnimation::~ActionPlayAnimation() {
	_engine->getScriptManager()->deleteSideFx(_slotkey);
}

bool ActionPlayAnimation::execute() {
	AnimationNode *nod = (AnimationNode *)_engine->getScriptManager()->getSideFX(_slotkey);

	if (!nod) {
		nod = new AnimationNode(_engine, _slotkey, _fileName, _mask, _framerate);
		_engine->getScriptManager()->addSideFX(nod);
	} else
		nod->stop();

	if (nod)
		nod->addPlayNode(_slotkey, _x, _y, _x2, _y2, _start, _end, _loopCount);

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionPlayPreloadAnimation
//////////////////////////////////////////////////////////////////////////////

ActionPlayPreloadAnimation::ActionPlayPreloadAnimation(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	sscanf(line.c_str(),
	       "%u %u %u %u %u %u %u %u",
	       &_controlKey, &_x1, &_y1, &_x2, &_y2, &_startFrame, &_endFrame, &_loopCount);
}

bool ActionPlayPreloadAnimation::execute() {
	AnimationNode *nod = (AnimationNode *)_engine->getScriptManager()->getSideFX(_controlKey);

	if (nod)
		nod->addPlayNode(_slotkey, _x1, _y1, _x2, _y2, _startFrame, _endFrame, _loopCount);

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionQuit
//////////////////////////////////////////////////////////////////////////////

bool ActionQuit::execute() {
	_engine->quitGame();

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionRandom
//////////////////////////////////////////////////////////////////////////////

ActionRandom::ActionRandom(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char max_buf[64];
	memset(max_buf, 0, 64);
	sscanf(line.c_str(), "%s", max_buf);
	_max = new ValueSlot(_engine->getScriptManager(), max_buf);
}

ActionRandom::~ActionRandom() {
	if (_max)
		delete _max;
}

bool ActionRandom::execute() {
	uint randNumber = _engine->getRandomSource()->getRandomNumber(_max->getValue());
	_engine->getScriptManager()->setStateValue(_slotkey, randNumber);
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionSetPartialScreen
//////////////////////////////////////////////////////////////////////////////

ActionSetPartialScreen::ActionSetPartialScreen(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char fileName[25];
	int color;

	sscanf(line.c_str(), "%u %u %25s %*u %d", &_x, &_y, fileName, &color);

	_fileName = Common::String(fileName);

	if (color >= 0) {
		byte r, g, b;
		Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0).colorToRGB(color, r, g, b);
		_backgroundColor = _engine->_pixelFormat.RGBToColor(r, g, b);
	} else {
		_backgroundColor = color;
	}

	if (color > 65535) {
		warning("Background color for ActionSetPartialScreen is bigger than a uint16");
	}
}

bool ActionSetPartialScreen::execute() {
	RenderManager *renderManager = _engine->getRenderManager();
	if (_backgroundColor >= 0)
		renderManager->renderImageToBackground(_fileName, _x, _y, _backgroundColor);
	else
		renderManager->renderImageToBackground(_fileName, _x, _y);

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionSetScreen
//////////////////////////////////////////////////////////////////////////////

ActionSetScreen::ActionSetScreen(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char fileName[25];
	sscanf(line.c_str(), "%25s", fileName);

	_fileName = Common::String(fileName);
}

bool ActionSetScreen::execute() {
	_engine->getRenderManager()->setBackgroundImage(_fileName);

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionStop
//////////////////////////////////////////////////////////////////////////////

ActionStop::ActionStop(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	_key = 0;
	sscanf(line.c_str(), "%u", &_key);
}

bool ActionStop::execute() {
	_engine->getScriptManager()->stopSideFx(_key);
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// ActionStreamVideo
//////////////////////////////////////////////////////////////////////////////

ActionStreamVideo::ActionStreamVideo(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char fileName[25];
	uint skipline;    //skipline - render video with skip every second line, not skippable.

	sscanf(line.c_str(), "%25s %u %u %u %u %u %u", fileName, &_x1, &_y1, &_x2, &_y2, &_flags, &skipline);

	_fileName = Common::String(fileName);
	_skippable = true;
}

bool ActionStreamVideo::execute() {
	ZorkAVIDecoder decoder;
	Common::File *_file = _engine->getSearchManager()->openFile(_fileName);

	if (_file) {
		if (!decoder.loadStream(_file)) {
			return true;
		}

		Common::Rect destRect = Common::Rect(_x1, _y1, _x2 + 1, _y2 + 1);

		Common::String subname = _fileName;
		subname.setChar('s', subname.size() - 3);
		subname.setChar('u', subname.size() - 2);
		subname.setChar('b', subname.size() - 1);

		Subtitle *sub = NULL;

		if (_engine->getSearchManager()->hasFile(subname))
			sub = new Subtitle(_engine, subname);

		_engine->playVideo(decoder, destRect, _skippable, sub);

		if (sub)
			delete sub;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionSyncSound
//////////////////////////////////////////////////////////////////////////////

ActionSyncSound::ActionSyncSound(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char fileName[25];
	int not_used;

	sscanf(line.c_str(), "%d %d %25s", &_syncto, &not_used, fileName);

	_fileName = Common::String(fileName);
}

bool ActionSyncSound::execute() {
	SideFX *fx = _engine->getScriptManager()->getSideFX(_syncto);
	if (!fx)
		return true;

	if (!(fx->getType() & SideFX::SIDEFX_ANIM))
		return true;

	AnimationNode *animnode = (AnimationNode *)fx;
	if (animnode->getFrameDelay() > 200) // Hack for fix incorrect framedelay in some animpreload
		animnode->setNewFrameDelay(66); // ~15fps

	_engine->getScriptManager()->addSideFX(new SyncSoundNode(_engine, _slotkey, _fileName, _syncto));
	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionTimer
//////////////////////////////////////////////////////////////////////////////

ActionTimer::ActionTimer(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char time_buf[64];
	memset(time_buf, 0, 64);
	sscanf(line.c_str(), "%s", time_buf);
	_time = new ValueSlot(_engine->getScriptManager(), time_buf);
}

ActionTimer::~ActionTimer() {
	if (_time)
		delete _time;
	_engine->getScriptManager()->killSideFx(_slotkey);
}

bool ActionTimer::execute() {
	if (_engine->getScriptManager()->getSideFX(_slotkey))
		return true;
	_engine->getScriptManager()->addSideFX(new TimerNode(_engine, _slotkey, _time->getValue()));
	return true;
}

//////////////////////////////////////////////////////////////////////////////
// ActionTtyText
//////////////////////////////////////////////////////////////////////////////

ActionTtyText::ActionTtyText(ZVision *engine, int32 slotkey, const Common::String &line) :
	ResultAction(engine, slotkey) {
	char filename[64];
	int32 x1, y1, x2, y2;
	sscanf(line.c_str(), "%d %d %d %d %s %u", &x1, &y1, &x2, &y2, filename, &_delay);
	_r = Common::Rect(x1, y1, x2, y2);
	_filename = Common::String(filename);
}

ActionTtyText::~ActionTtyText() {
	_engine->getScriptManager()->killSideFx(_slotkey);
}

bool ActionTtyText::execute() {
	if (_engine->getScriptManager()->getSideFX(_slotkey))
		return true;
	_engine->getScriptManager()->addSideFX(new ttyTextNode(_engine, _slotkey, _filename, _r, _delay));
	return true;
}

} // End of namespace ZVision
