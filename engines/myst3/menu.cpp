/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
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

#include "engines/myst3/cursor.h"
#include "engines/myst3/database.h"
#include "engines/myst3/menu.h"
#include "engines/myst3/myst3.h"
#include "engines/myst3/state.h"

#include "common/events.h"

#include "graphics/colormasks.h"

namespace Myst3 {

Menu::Menu(Myst3Engine *vm) :
	_vm(vm),
	_saveLoadSpotItem(0) {
}

Menu::~Menu() {
}

void Menu::updateMainMenu(uint16 action) {
	switch (action) {
	case 1: {
			// New game
			int16 choice = 1;

			// If a game is playing, ask if wants to save
			if (_vm->_state->getMenuSavedAge() != 0) {
				choice = _vm->openDialog(1080);
			}

			if (choice == 0) {
				// Go to save screen
				_vm->_state->setMenuSaveBack(1);
				_vm->_state->setMenuSaveAction(6);
				goToNode(300);
			} else if (choice == 1) {
				// New game
				goToNode(98);
			}
		}
		break;
	case 2: {
			// Load game
			int16 choice = 1;

			// If a game is playing, ask if wants to save
			if (_vm->_state->getMenuSavedAge() != 0) {
				choice = _vm->openDialog(1060);
			}

			if (choice == 0) {
				// Go to save screen
				_vm->_state->setMenuSaveBack(1);
				_vm->_state->setMenuSaveAction(3);
				goToNode(300);
			} else if (choice == 1) {
				// Load game screen
				_vm->_state->setMenuLoadBack(1);
				goToNode(200);
			}
		}
		break;
	case 3:
		// Go to save screen
		_vm->_state->setMenuSaveBack(1);
		_vm->_state->setMenuSaveAction(1);
		goToNode(300);
		break;
	case 4:
		// Settings
		_vm->_state->setMenuOptionsBack(1);
		_vm->runScriptsFromNode(599, 0, 0);
		break;
	case 5: {
			// Asked to quit
			int16 choice = 1;

			// If a game is playing, ask if wants to save
			if (_vm->_state->getMenuSavedAge() != 0) {
				choice = _vm->openDialog(1070);
			}

			if (choice == 0) {
				// Go to save screen
				_vm->_state->setMenuSaveBack(1);
				_vm->_state->setMenuSaveAction(5);
				goToNode(300);
			} else if (choice == 1) {
				// Quit
				_vm->setShouldQuit();
			}
		}
		break;
	default:
		warning("Menu action %d is not implemented", action);
		break;
	}
}

void Menu::goToNode(uint16 node) {
	if (_vm->_state->getMenuSavedAge() == 0 && _vm->_state->getLocationRoom() != 901) {
		// Entering menu, save current location
		_vm->_state->setMenuSavedAge(_vm->_state->getLocationAge());
		_vm->_state->setMenuSavedRoom(_vm->_state->getLocationRoom());
		_vm->_state->setMenuSavedNode(_vm->_state->getLocationNode());
	}

	_vm->_state->setLocationNextAge(9);
	_vm->_state->setLocationNextRoom(901);
	_vm->goToNode(node, 2);
}

Dialog::Dialog(Myst3Engine *vm, uint id):
	_vm(vm),
	_texture(0),
	_frameToDisplay(0),
	_previousframe(0) {
	const DirectorySubEntry *buttonsDesc = _vm->getFileDescription("DLGB", 1000, 0, DirectorySubEntry::kNumMetadata);
	const DirectorySubEntry *movieDesc = _vm->getFileDescription("DLOG", id, 0, DirectorySubEntry::kDialogMovie);
	const DirectorySubEntry *countDesc = _vm->getFileDescription("DLGI", id, 0, DirectorySubEntry::kNumMetadata);

	// Retrieve button count
	_buttonCount = countDesc->getMiscData(0);
	assert(_buttonCount <= 3);

	// Load available buttons
	for (uint i = 0; i < 3; i++) {
		uint32 left = buttonsDesc->getMiscData(i * 4);
		uint32 top = buttonsDesc->getMiscData(i * 4 + 1);
		uint32 width = buttonsDesc->getMiscData(i * 4 + 2);
		uint32 height = buttonsDesc->getMiscData(i * 4 + 3);
		_buttons[i] = Common::Rect(width, height);
		_buttons[i].translate(left, top);
	}

	// Load the movie
	_movieStream = movieDesc->getData();
	_bink.loadStream(_movieStream, Graphics::PixelFormat(4, 8, 8, 8, 8, 0, 8, 16, 24));

	const Graphics::Surface *frame = _bink.decodeNextFrame();
	_texture = _vm->_gfx->createTexture(frame);
}

Dialog::~Dialog() {
	_vm->_gfx->freeTexture(_texture);
}

void Dialog::draw() {
	if (_frameToDisplay != _previousframe) {
		_bink.seekToFrame(_frameToDisplay);

		const Graphics::Surface *frame = _bink.decodeNextFrame();
		_texture->update(frame);

		_previousframe = _frameToDisplay;
	}

	Common::Rect textureRect = Common::Rect(_texture->width, _texture->height);
	_vm->_gfx->drawTexturedRect2D(getPosition(), textureRect, _texture);
}

Common::Rect Dialog::getPosition() {
	Common::Rect screenRect = Common::Rect(_texture->width, _texture->height);
	screenRect.translate((Renderer::kOriginalWidth - _texture->width) / 2,
			(Renderer::kOriginalHeight - _texture->height) / 2);
	return screenRect;
}

int16 Dialog::update() {
	// Process events
	Common::Event event;
	while (_vm->_system->getEventManager()->pollEvent(event)) {
		// Check for "Hard" quit"
		if (event.type == Common::EVENT_QUIT) {
			_vm->setShouldQuit();
			return -2;
		} else if (event.type == Common::EVENT_MOUSEMOVE) {
			// Compute local mouse coordinates
			_vm->_cursor->updatePosition(event.relMouse);
			Common::Rect position = getPosition();
			Common::Point localMouse = _vm->_cursor->getPosition();
			localMouse.x -= position.left;
			localMouse.y -= position.top;

			// Display the frame corresponding to the hovered button
			for (uint i = 0; i < _buttonCount; i++) {
				if (_buttons[i].contains(localMouse)) {
					_frameToDisplay = i + 1;
					return -1;
				}
			}

			// No hovered button
			_frameToDisplay = 0;
		} else if (event.type == Common::EVENT_LBUTTONDOWN) {
			return _frameToDisplay - 1;
		} else if (event.type == Common::EVENT_KEYDOWN) {
			switch (event.kbd.keycode) {
			case Common::KEYCODE_ESCAPE:
				return -2;
				break;
			default:
				break;
			}
		}
	}

	return -1;
}

void Menu::loadMenuOpen() {
	_saveLoadFiles = _vm->getSaveFileManager()->listSavefiles("*.m3s");
	_vm->_state->setMenuSaveLoadCurrentPage(0);
	saveLoadUpdateVars();
}

void Menu::saveLoadUpdateVars() {
	int16 page = _vm->_state->getMenuSaveLoadCurrentPage();

	// Go back one page if the last element of the last page was removed
	if (page && (7 * page > (int)_saveLoadFiles.size() - 1))
		page--;
	_vm->_state->setMenuSaveLoadCurrentPage(page);

	// Set up pagination
	bool canGoLeft = (_saveLoadFiles.size() > 7) && page;
	bool canGoRight = (_saveLoadFiles.size() > 7) && (7 * (page + 1) < (int)_saveLoadFiles.size());

	_vm->_state->setMenuSaveLoadPageLeft(canGoLeft);
	_vm->_state->setMenuSaveLoadPageRight(canGoRight);
	_vm->_state->setMenuSaveLoadSelectedItem(-1);

	// Enable items
	uint16 itemsOnPage = _saveLoadFiles.size() % 7;

	if (itemsOnPage == 0 && _saveLoadFiles.size() != 0)
		itemsOnPage = 7;
	if (canGoRight)
		itemsOnPage = 7;

	for (uint i = 0; i < 7; i++)
		_vm->_state->setVar(1354 + i, i < itemsOnPage);
}

void Menu::loadMenuSelect(uint16 item) {
	_vm->_state->setMenuSaveLoadSelectedItem(item);
	int16 page = _vm->_state->getMenuSaveLoadCurrentPage();

	uint16 index = page * 7 + item;

	assert(index < _saveLoadFiles.size());
	Common::String filename = _saveLoadFiles[index];

	// Extract the age to load from the savegame
	GameState *gameState = new GameState(_vm);
	gameState->load(filename);

	uint32 age = 0;
	uint32 room = gameState->getLocationRoom();
	if (room == 901)
		age = gameState->getMenuSavedAge();
	else
		age = gameState->getLocationAge();

	delete gameState;

	// Extract the thumbnail from the save
	Common::InSaveFile *save = _vm->getSaveFileManager()->openForLoading(filename);
	saveGameReadThumbnail(save);
	delete save;

	// Look for the age name
	const DirectorySubEntry *desc = _vm->getFileDescription("AGES", 1000, 0, DirectorySubEntry::kTextMetadata);
	_saveLoadAgeName = desc->getTextData(_vm->_db->getAgeLabelId(age));
	_saveLoadAgeName.toUppercase();

	// TODO: Selecting twice loads item
}

void Menu::loadMenuLoad(uint16 item) {
	int16 page = _vm->_state->getMenuSaveLoadCurrentPage();

	uint16 index = page * 7 + item;
	assert(index < _saveLoadFiles.size());

	_vm->_state->load(_saveLoadFiles[index]);

	_vm->_state->setLocationNextAge(_vm->_state->getMenuSavedAge());
	_vm->_state->setLocationNextRoom(_vm->_state->getMenuSavedRoom());
	_vm->_state->setLocationNextNode(_vm->_state->getMenuSavedNode());
	_vm->_state->setMenuSavedAge(0);
	_vm->_state->setMenuSavedRoom(0);
	_vm->_state->setMenuSavedNode(0);

	_vm->goToNode(0, 1);
}

void Menu::draw() {
	uint16 node = _vm->_state->getLocationNode();
	uint16 room = _vm->_state->getLocationRoom();
	uint16 age = _vm->_state->getLocationAge();

	if (room != 901 || node != 200)
		return;

	int16 page = _vm->_state->getMenuSaveLoadCurrentPage();
	NodePtr nodeData = _vm->_db->getNodeData(node, room, age);

	for (uint i = 0; i < 7; i++) {
		uint itemToDisplay = page * 7 + i;

		if (itemToDisplay >= _saveLoadFiles.size())
			break;

		PolarRect rect = nodeData->hotspots[i + 1].rects[0];

		Common::String display = _saveLoadFiles[itemToDisplay];
		display.toUppercase();
		if (display.hasSuffix(".M3S")) {
			display.deleteLastChar();
			display.deleteLastChar();
			display.deleteLastChar();
			display.deleteLastChar();
		}

		while (display.size() > 17)
			display.deleteLastChar();

		_vm->_gfx->draw2DText(display, Common::Point(rect.centerPitch, rect.centerHeading));
	}

	if (!_saveLoadAgeName.empty()) {
		PolarRect rect = nodeData->hotspots[8].rects[0];
		_vm->_gfx->draw2DText(_saveLoadAgeName, Common::Point(rect.centerPitch, rect.centerHeading));
	}
}

void Menu::loadMenuChangePage() {
	saveLoadUpdateVars();
}

void Menu::saveGameReadThumbnail(Common::InSaveFile *save) {
	// Start thumbnail data
	save->seek(8580);

	static const uint kMiniatureSize = 240 * 135;
	uint8 *thumbnail = new uint8[kMiniatureSize * 3];

	// The spot item expect RGB data instead of RGBA
	uint8 *ptr = thumbnail;
	for (uint i = 0; i < kMiniatureSize; i++) {
		uint32 rgba = save->readUint32LE();
		uint8 a, r, g, b;
		Graphics::colorToARGB< Graphics::ColorMasks<8888> >(rgba, a, r, g, b);
		*ptr++ = r;
		*ptr++ = g;
		*ptr++ = b;
	}

	if (_saveLoadSpotItem)
		_saveLoadSpotItem->updateData(thumbnail);

	delete[] thumbnail;
}

} /* namespace Myst3 */
