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
 * $URL$
 * $Id$
 *
 */

#ifndef LURE_H
#define LURE_H

#include "engines/engine.h"
#include "common/rect.h"
#include "sound/mixer.h"
#include "common/file.h"
#include "common/savefile.h"

#include "lure/disk.h"
#include "lure/res.h"
#include "lure/screen.h"
#include "lure/events.h"
#include "lure/menu.h"
#include "lure/strings.h"
#include "lure/room.h"
#include "lure/fights.h"

namespace Lure {

struct LureGameDescription;

class LureEngine : public Engine {
private:
	bool _initialised;
	int _gameToLoad;
	uint8 _saveVersion;
	Disk *_disk;
	Resources *_resources;
	Screen *_screen;
	Mouse *_mouse;
	Events *_events;
	Menu *_menu;
	StringData *_strings;
	Room *_room;
	FightsManager *_fights;

	const char *generateSaveName(int slotNumber);

	const LureGameDescription *_gameDescription;

public:
	LureEngine(OSystem *system, const LureGameDescription *gameDesc);
	~LureEngine();
	static LureEngine &getReference();

	virtual int init();
	virtual int go();
	virtual void pauseEngineIntern(bool pause);
	virtual void syncSoundSettings();

	Disk &disk() { return *_disk; }

	int gameToLoad() { return _gameToLoad; }
	bool loadGame(uint8 slotNumber);
	bool saveGame(uint8 slotNumber, Common::String &caption);
	Common::String *detectSave(int slotNumber);
	uint8 saveVersion() { return _saveVersion; }
	void GUIError(const char *msg, ...);

	uint32 getFeatures() const;
	Common::Language getLanguage() const;
	Common::Platform getPlatform() const;
	bool isEGA() const { return (getFeatures() & GF_EGA) != 0; }
};
	Common::String getSaveName(Common::InSaveFile *in);
} // End of namespace Lure

#endif
