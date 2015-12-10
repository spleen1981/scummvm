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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This code is based on Labyrinth of Time code with assistance of
 *
 * Copyright (c) 1993 Terra Nova Development
 * Copyright (c) 2004 The Wyrmkeep Entertainment Co.
 *
 */

#include "lab/lab.h"

#include "lab/dispman.h"
#include "lab/eventman.h"
#include "lab/image.h"
#include "lab/interface.h"
#include "lab/labsets.h"
#include "lab/music.h"
#include "lab/processroom.h"
#include "lab/resource.h"
#include "lab/utils.h"

namespace Lab {

/*---------------------------------------------------------------------------*/
/*------------------------------ The Map stuff ------------------------------*/
/*---------------------------------------------------------------------------*/

enum MapFloor {
	kFloorNone,
	kFloorLower,
	kFloorMiddle,
	kFloorUpper,
	kFloorMedMaze,
	kFloorHedgeMaze,
	kFloorSurMaze,
	kFloorCarnival
};

/**
 * Loads in the map data.
 */
void LabEngine::loadMapData() {
	Common::File *mapImages = _resource->openDataFile("P:MapImage");

	_imgMap = new Image(mapImages);
	_imgRoom = new Image(mapImages);
	_imgUpArrowRoom = new Image(mapImages);
	_imgDownArrowRoom = new Image(mapImages);
	_imgHRoom = new Image(mapImages);
	_imgVRoom = new Image(mapImages);
	_imgMaze = new Image(mapImages);
	_imgHugeMaze = new Image(mapImages);

	_imgMapNorth = new Image(mapImages);
	_imgMapEast = new Image(mapImages);
	_imgMapSouth = new Image(mapImages);
	_imgMapWest = new Image(mapImages);
	_imgPath = new Image(mapImages);
	_imgBridge = new Image(mapImages);

	_mapGadgetList.push_back(createButton( 8,  _utils->vgaScaleY(105), 0, VKEY_LTARROW, new Image(mapImages), new Image(mapImages)));	// back
	_mapGadgetList.push_back(createButton( 55, _utils->vgaScaleY(105), 1, VKEY_UPARROW, new Image(mapImages), new Image(mapImages)));	// up
	_mapGadgetList.push_back(createButton(101, _utils->vgaScaleY(105), 2, VKEY_DNARROW, new Image(mapImages), new Image(mapImages)));	// down

	delete mapImages;

	Common::File *mapFile = _resource->openDataFile("Lab:Maps", MKTAG('M', 'A', 'P', '0'));
	_music->updateMusic();
	if (!_music->_doNotFilestopSoundEffect)
		_music->stopSoundEffect();

	_maxRooms = mapFile->readUint16LE();
	_maps = new MapData[_maxRooms];	// will be freed when the user exits the map
	for (int i = 0; i < _maxRooms; i++) {
		_maps[i]._x = mapFile->readUint16LE();
		_maps[i]._y = mapFile->readUint16LE();
		_maps[i]._pageNumber = mapFile->readUint16LE();
		_maps[i]._specialID = mapFile->readUint16LE();
		_maps[i]._mapFlags = mapFile->readUint32LE();
	}

	delete mapFile;
}

void LabEngine::freeMapData() {
	freeButtonList(&_mapGadgetList);

	delete _imgMap;
	delete _imgRoom;
	delete _imgUpArrowRoom;
	delete _imgDownArrowRoom;
	delete _imgBridge;
	delete _imgHRoom;
	delete _imgVRoom;
	delete _imgMaze;
	delete _imgHugeMaze;
	delete _imgPath;
	delete _imgMapNorth;
	delete _imgMapEast;
	delete _imgMapSouth;
	delete _imgMapWest;
	// _imgXMark is a pointer to one of the four buffers above
	delete[] _maps;

	_imgMap = nullptr;
	_imgRoom = nullptr;
	_imgUpArrowRoom = nullptr;
	_imgDownArrowRoom = nullptr;
	_imgBridge = nullptr;
	_imgHRoom = nullptr;
	_imgVRoom = nullptr;
	_imgMaze = nullptr;
	_imgHugeMaze = nullptr;
	_imgPath = nullptr;
	_imgMapNorth = nullptr;
	_imgMapEast = nullptr;
	_imgMapSouth = nullptr;
	_imgMapWest = nullptr;
	_imgXMark = nullptr;
	_maps = nullptr;
}

/**
 * Figures out what a room's coordinates should be.
 */
Common::Rect LabEngine::roomCoords(uint16 curRoom) {
	Image *curRoomImg = nullptr;

	switch (_maps[curRoom]._specialID) {
	case NORMAL:
	case UPARROWROOM:
	case DOWNARROWROOM:
		curRoomImg = _imgRoom;
		break;
	case BRIDGEROOM:
		curRoomImg = _imgBridge;
		break;
	case VCORRIDOR:
		curRoomImg = _imgVRoom;
		break;
	case HCORRIDOR:
		curRoomImg = _imgHRoom;
		break;
	default:
		// Some rooms (like the map) do not have an image
		break;
	}

	int x1 = _utils->mapScaleX(_maps[curRoom]._x);
	int y1 = _utils->mapScaleY(_maps[curRoom]._y);
	int x2 = x1;
	int y2 = y1;

	if (curRoomImg) {
		x2 += curRoomImg->_width;
		y2 += curRoomImg->_height;
	}

	return Common::Rect(x1, y1, x2, y2);
}

/**
 * Draws a room map.
 */
void LabEngine::drawRoomMap(uint16 curRoom, bool drawMarkFl) {
	uint16 drawX, drawY, offset;

	uint16 x = _utils->mapScaleX(_maps[curRoom]._x);
	uint16 y = _utils->mapScaleY(_maps[curRoom]._y);
	uint32 flags = _maps[curRoom]._mapFlags;

	switch (_maps[curRoom]._specialID) {
	case NORMAL:
	case UPARROWROOM:
	case DOWNARROWROOM:
		if (_maps[curRoom]._specialID == NORMAL)
			_imgRoom->drawImage(x, y);
		else if (_maps[curRoom]._specialID == DOWNARROWROOM)
			_imgDownArrowRoom->drawImage(x, y);
		else
			_imgUpArrowRoom->drawImage(x, y);

		offset = (_imgRoom->_width - _imgPath->_width) / 2;

		if ((NORTHDOOR & flags) && (y >= _imgPath->_height))
			_imgPath->drawImage(x + offset, y - _imgPath->_height);

		if (SOUTHDOOR & flags)
			_imgPath->drawImage(x + offset, y + _imgRoom->_height);

		offset = (_imgRoom->_height - _imgPath->_height) / 2;

		if (EASTDOOR & flags)
			_imgPath->drawImage(x + _imgRoom->_width, y + offset);

		if (WESTDOOR & flags)
			_imgPath->drawImage(x - _imgPath->_width, y + offset);

		drawX = x + (_imgRoom->_width - _imgXMark->_width) / 2;
		drawY = y + (_imgRoom->_height - _imgXMark->_height) / 2;

		break;

	case BRIDGEROOM:
		_imgBridge->drawImage(x, y);

		drawX = x + (_imgBridge->_width - _imgXMark->_width) / 2;
		drawY = y + (_imgBridge->_height - _imgXMark->_height) / 2;

		break;

	case VCORRIDOR:
		_imgVRoom->drawImage(x, y);

		offset = (_imgVRoom->_width - _imgPath->_width) / 2;

		if (NORTHDOOR & flags)
			_imgPath->drawImage(x + offset, y - _imgPath->_height);

		if (SOUTHDOOR & flags)
			_imgPath->drawImage(x + offset, y + _imgVRoom->_height);

		offset = (_imgRoom->_height - _imgPath->_height) / 2;

		if (EASTDOOR & flags)
			_imgPath->drawImage(x + _imgVRoom->_width, y + offset);

		if (WESTDOOR & flags)
			_imgPath->drawImage(x - _imgPath->_width, y + offset);

		if (EASTBDOOR & flags)
			_imgPath->drawImage(x + _imgVRoom->_width, y - offset - _imgPath->_height + _imgVRoom->_height);

		if (WESTBDOOR & flags)
			_imgPath->drawImage(x - _imgPath->_width, y - offset - _imgPath->_height + _imgVRoom->_height);

		offset = (_imgVRoom->_height - _imgPath->_height) / 2;

		if (EASTMDOOR & flags)
			_imgPath->drawImage(x + _imgVRoom->_width, y - offset - _imgPath->_height + _imgVRoom->_height);

		if (WESTMDOOR & flags)
			_imgPath->drawImage(x - _imgPath->_width, y - offset - _imgPath->_height + _imgVRoom->_height);

		drawX = x + (_imgVRoom->_width - _imgXMark->_width) / 2;
		drawY = y + (_imgVRoom->_height - _imgXMark->_height) / 2;

		break;

	case HCORRIDOR:
		_imgHRoom->drawImage(x, y);

		offset = (_imgRoom->_width - _imgPath->_width) / 2;

		if (NORTHDOOR & flags)
			_imgPath->drawImage(x + offset, y - _imgPath->_height);

		if (SOUTHDOOR & flags)
			_imgPath->drawImage(x + offset, y + _imgRoom->_height);

		if (NORTHRDOOR & flags)
			_imgPath->drawImage(x - offset - _imgPath->_width + _imgHRoom->_width, y - _imgPath->_height);

		if (SOUTHRDOOR & flags)
			_imgPath->drawImage(x - offset - _imgPath->_width + _imgHRoom->_width, y + _imgRoom->_height);

		offset = (_imgHRoom->_width - _imgPath->_width) / 2;

		if (NORTHMDOOR & flags)
			_imgPath->drawImage(x - offset - _imgPath->_width + _imgHRoom->_width, y - _imgPath->_height);

		if (SOUTHMDOOR & flags)
			_imgPath->drawImage(x - offset - _imgPath->_width + _imgHRoom->_width, y + _imgRoom->_height);

		offset = (_imgRoom->_height - _imgPath->_height) / 2;

		if (EASTDOOR & flags)
			_imgPath->drawImage(x + _imgHRoom->_width, y + offset);

		if (WESTDOOR & flags)
			_imgPath->drawImage(x - _imgPath->_width, y + offset);

		drawX = x + (_imgHRoom->_width - _imgXMark->_width) / 2;
		drawY = y + (_imgHRoom->_height - _imgXMark->_height) / 2;

		break;

	default:
		return;
	}

	if (drawMarkFl)
		_imgXMark->drawImage(drawX, drawY);
}

/**
 * Checks if a floor has been visited.
 */
bool LabEngine::onFloor(uint16 floorNum) {
	for (uint16 i = 1; i <= _maxRooms; i++) {
		if ((_maps[i]._pageNumber == floorNum) && _roomsFound->in(i) && _maps[i]._x)
			return true;
	}

	return false;
}

/**
 * Figures out which floor, if any, should be gone to if the up arrow is hit
 */
bool LabEngine::getUpFloor(uint16 *floorNum) {
	do {
		if (*floorNum < kFloorUpper)
			(*floorNum)++;
		else {
			*floorNum   = kFloorCarnival + 1;
			return false;
		}
	} while ((!onFloor(*floorNum)) && (*floorNum <= kFloorCarnival));

	return true;
}

/**
 * Figures out which floor, if any, should be gone to if the down arrow is
 * hit.
 */
bool LabEngine::getDownFloor(uint16 *floorNum) {
	do {
		if ((*floorNum == kFloorLower) || (*floorNum == kFloorNone)) {
			*floorNum = kFloorNone;
			return false;
		} else if (*floorNum > kFloorUpper) {
			// Labyrinth specific code
			if (*floorNum == kFloorHedgeMaze)
				*floorNum = kFloorUpper;
			else if ((*floorNum == kFloorCarnival) || (*floorNum == kFloorMedMaze))
				*floorNum = kFloorMiddle;
			else if (*floorNum == kFloorSurMaze)
				*floorNum = kFloorLower;
			else {
				*floorNum = kFloorNone;
				return false;
			}
		} else
			(*floorNum)--;

	} while ((!onFloor(*floorNum)) && (*floorNum != kFloorNone));

	return true;
}

/**
 * Draws the map
 */
void LabEngine::drawMap(uint16 curRoom, uint16 curMsg, uint16 floorNum, bool fadeOut, bool fadeIn) {
	_event->mouseHide();

	if (fadeOut)
		_graphics->fade(false, 0);

	_graphics->setAPen(0);
	_graphics->rectFill(0, 0, _graphics->_screenWidth - 1, _graphics->_screenHeight - 1);

	_imgMap->drawImage(0, 0);
	drawGadgetList(&_mapGadgetList);

	for (uint16 i = 1; i <= _maxRooms; i++) {
		if ((_maps[i]._pageNumber == floorNum) && _roomsFound->in(i) && _maps[i]._x) {
			drawRoomMap(i, (bool)(i == curRoom));
			_music->updateMusic();
		}
	}

	// Makes sure the X is drawn in corridors
	// NOTE: this here on purpose just in case there's some weird
	// condition, like the surreal maze where there are no rooms
	if ((_maps[curRoom]._pageNumber == floorNum) && _roomsFound->in(curRoom) && _maps[curRoom]._x)
		drawRoomMap(curRoom, true);

	uint16 tempfloor = floorNum;
	bool noOverlay = getUpFloor(&tempfloor);

	Gadget *upGadget = _event->getGadget(1);
	Gadget *downGadget = _event->getGadget(2);

	if (noOverlay)
		enableGadget(upGadget);
	else
		disableGadget(upGadget, 12);

	tempfloor = floorNum;
	noOverlay = getDownFloor(&tempfloor);

	if (noOverlay)
		enableGadget(downGadget);
	else
		disableGadget(downGadget, 12);

	// Labyrinth specific code
	if (floorNum == kFloorLower) {
		if (onFloor(kFloorSurMaze))
			_imgMaze->drawImage(_utils->mapScaleX(538), _utils->mapScaleY(277));
	} else if (floorNum == kFloorMiddle) {
		if (onFloor(kFloorCarnival))
			_imgMaze->drawImage(_utils->mapScaleX(358), _utils->mapScaleY(72));

		if (onFloor(kFloorMedMaze))
			_imgMaze->drawImage(_utils->mapScaleX(557), _utils->mapScaleY(325));
	} else if (floorNum == kFloorUpper) {
		if (onFloor(kFloorHedgeMaze))
			_imgHugeMaze->drawImage(_utils->mapScaleX(524), _utils->mapScaleY(97));
	} else if (floorNum == kFloorSurMaze) {
		char *sptr = (char *)_resource->getStaticText(kTextSurmazeMessage).c_str();
		_graphics->flowText(_msgFont, 0, 7, 0, true, true, true, true, _utils->mapScaleX(360), 0, _utils->mapScaleX(660), _utils->mapScaleY(450), sptr);
	}

	if ((floorNum >= kFloorLower) && (floorNum <= kFloorCarnival)) {
		char *sptr = (char *)_resource->getStaticText(floorNum - 1).c_str();
		_graphics->flowTextScaled(_msgFont, 0, 5, 3, true, true, true, true, 14, 75, 134, 97, sptr);
	}

	if (_rooms[curMsg]._roomMsg)
		_graphics->flowTextScaled(_msgFont, 0, 5, 3, true, true, true, true, 14, 148, 134, 186, _rooms[curMsg]._roomMsg);

	if (fadeIn)
		_graphics->fade(true, 0);

	_event->mouseShow();
}

/**
 * Processes the map.
 */
void LabEngine::processMap(uint16 curRoom) {
	uint32 place = 1;
	uint16 curMsg = curRoom;
	uint16 curFloor = _maps[curRoom]._pageNumber;

	while (1) {
		// Make sure we check the music at least after every message
		_music->updateMusic();
		IntuiMessage *msg = getMsg();

		if (!msg) {
			_music->updateMusic();

			byte newcolor[3];

			if (place <= 14) {
				newcolor[0] = 14 << 2;
				newcolor[1] = place << 2;
				newcolor[2] = newcolor[1];
			} else {
				newcolor[0] = 14 << 2;
				newcolor[1] = (28 - place) << 2;
				newcolor[2] = newcolor[1];
			}

			waitTOF();
			_graphics->writeColorRegs(newcolor, 1, 1);
			_event->updateMouse();
			waitTOF();
			_event->updateMouse();
			waitTOF();
			_event->updateMouse();
			waitTOF();
			_event->updateMouse();

			place++;

			if (place >= 28)
				place = 1;

		} else {
			uint32 msgClass  = msg->_msgClass;
			uint16 msgCode   = msg->_code;
			uint16 gadgetID  = msg->_gadgetID;
			uint16 qualifier = msg->_qualifier;
			uint16 mouseX    = msg->_mouseX;
			uint16 mouseY    = msg->_mouseY;

			if (((msgClass == MOUSEBUTTONS) && (IEQUALIFIER_RBUTTON & qualifier)) || ((msgClass == RAWKEY) && (msgCode == 27)))
				return;

			if (msgClass == GADGETUP) {
				if (gadgetID == 0) {
					// Quit menu button
					return;
				} else if (gadgetID == 1) {
					// Up arrow
					uint16 oldFloor = curFloor;
					bool drawmap = getUpFloor(&curFloor);

					if (drawmap) {
						_graphics->fade(false, 0);
						drawMap(curRoom, curMsg, curFloor, false, false);
						_graphics->fade(true, 0);
					} else
						curFloor = oldFloor;
				} else if (gadgetID == 2) {
					// Down arrow
					uint16 oldFloor = curFloor;
					bool drawmap = getDownFloor(&curFloor);

					if (drawmap) {
						_graphics->fade(false, 0);
						drawMap(curRoom, curMsg, curFloor, false, false);
						_graphics->fade(true, 0);
					} else
						curFloor = oldFloor;
				}
			} else if ((msgClass == MOUSEBUTTONS) && (IEQUALIFIER_LEFTBUTTON & qualifier)) {
				if ((curFloor == kFloorLower) && (mouseX >= _utils->mapScaleX(538)) && (mouseY >= _utils->mapScaleY(277))
					  && (mouseX <= _utils->mapScaleX(633)) && (mouseY <= _utils->mapScaleY(352))
					  && onFloor(kFloorSurMaze)) {
					curFloor = kFloorSurMaze;

					_graphics->fade(false, 0);
					drawMap(curRoom, curMsg, curFloor, false, false);
					_graphics->fade(true, 0);
				} else if ((curFloor == kFloorMiddle) && (mouseX >= _utils->mapScaleX(358)) && (mouseY >= _utils->mapScaleY(71))
							  && (mouseX <= _utils->mapScaleX(452)) && (mouseY <= _utils->mapScaleY(147))
							  && onFloor(kFloorCarnival)) {
					curFloor = kFloorCarnival;

					_graphics->fade(false, 0);
					drawMap(curRoom, curMsg, curFloor, false, false);
					_graphics->fade(true, 0);
				} else if ((curFloor == kFloorMiddle) && (mouseX >= _utils->mapScaleX(557)) && (mouseY >= _utils->mapScaleY(325))
						  && (mouseX <= _utils->mapScaleX(653)) && (mouseY <= _utils->mapScaleY(401))
						  && onFloor(kFloorMedMaze)) {
					curFloor = kFloorMedMaze;

					_graphics->fade(false, 0);
					drawMap(curRoom, curMsg, curFloor, false, false);
					_graphics->fade(true, 0);
				} else if ((curFloor == kFloorUpper) && (mouseX >= _utils->mapScaleX(524)) && (mouseY >=  _utils->mapScaleY(97))
						  && (mouseX <= _utils->mapScaleX(645)) && (mouseY <= _utils->mapScaleY(207))
						  && onFloor(kFloorHedgeMaze)) {
					curFloor = kFloorHedgeMaze;

					_graphics->fade(false, 0);
					drawMap(curRoom, curMsg, curFloor, false, false);
					_graphics->fade(true, 0);
				} else if (mouseX > _utils->mapScaleX(314)) {
					uint16 oldMsg = curMsg;
					Common::Rect curCoords;

					for (uint16 i = 1; i <= _maxRooms; i++) {
						curCoords = roomCoords(i);

						if ((_maps[i]._pageNumber == curFloor)
							  && _roomsFound->in(i) && curCoords.contains(Common::Point(mouseX, mouseY))) {
							curMsg = i;
						}
					}

					if (oldMsg != curMsg) {
						if (_rooms[curMsg]._roomMsg == nullptr)
							_resource->readViews(curMsg);

						char *sptr;
						if ((sptr = _rooms[curMsg]._roomMsg)) {
							_event->mouseHide();
							_graphics->setAPen(3);
							_graphics->rectFillScaled(13, 148, 135, 186);
							_graphics->flowTextScaled(_msgFont, 0, 5, 3, true, true, true, true, 14, 148, 134, 186, sptr);

							if (_maps[oldMsg]._pageNumber == curFloor)
								drawRoomMap(oldMsg, (bool)(oldMsg == curRoom));

							curCoords = roomCoords(curMsg);
							int right = (curCoords.left + curCoords.right) / 2;
							int left = right - 1;
							int top, bottom;
							top = bottom = (curCoords.top + curCoords.bottom) / 2;

							if ((curMsg != curRoom) && (_maps[curMsg]._pageNumber == curFloor)) {
								_graphics->setAPen(1);
								_graphics->rectFill(left, top, right, bottom);
							}

							_event->mouseShow();
						}
					}
				}
			}

			_graphics->screenUpdate();
		}
	}
}

/**
 * Does the map processing.
 */
void LabEngine::doMap(uint16 curRoom) {
	static uint16 amigaMapPalette[] = {
		0x0BA8, 0x0C11, 0x0A74, 0x0076,
		0x0A96, 0x0DCB, 0x0CCA, 0x0222,
		0x0444, 0x0555, 0x0777, 0x0999,
		0x0AAA, 0x0ED0, 0x0EEE, 0x0694
	};

	_graphics->FadePalette = amigaMapPalette;

	_music->updateMusic();
	loadMapData();
	_graphics->blackAllScreen();

	if (_direction == NORTH)
		_imgXMark = _imgMapNorth;
	else if (_direction == SOUTH)
		_imgXMark = _imgMapSouth;
	else if (_direction == EAST)
		_imgXMark = _imgMapEast;
	else if (_direction == WEST)
		_imgXMark = _imgMapWest;

	_event->attachGadgetList(&_mapGadgetList);
	drawMap(curRoom, curRoom, _maps[curRoom]._pageNumber, false, true);
	_event->mouseShow();
	_graphics->screenUpdate();
	processMap(curRoom);
	_event->attachGadgetList(nullptr);
	_graphics->fade(false, 0);
	_graphics->blackAllScreen();
	_event->mouseHide();
	_graphics->setAPen(0);
	_graphics->rectFill(0, 0, _graphics->_screenWidth - 1, _graphics->_screenHeight - 1);
	freeMapData();
	_graphics->blackAllScreen();
	_event->mouseShow();
	_graphics->screenUpdate();
}

} // End of namespace Lab
