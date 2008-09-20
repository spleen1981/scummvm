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
 * $URL$
 * $Id$
 */

#ifndef MASSADD_DIALOG_H
#define MASSADD_DIALOG_H

#include "gui/dialog.h"
#include "common/fs.h"
#include "common/hashmap.h"
#include "common/stack.h"
#include "common/str.h"
#include "common/hash-str.h"

namespace GUI {

class StaticTextWidget;

class MassAddDialog : public Dialog {
public:
	MassAddDialog(const Common::FilesystemNode &startDir);

	//void open();
	void handleCommand(CommandSender *sender, uint32 cmd, uint32 data);
	void handleTickle();

private:
	Common::Stack<Common::FilesystemNode>  _scanStack;
	GameList _games;

	/**
	 * Map each path occuring in the config file to the target(s) using that path.
	 * Used to detect whether a potential new target is already present in the
	 * config manager.
	 */
	Common::HashMap<Common::String, Common::StringList>	_pathToTargets;

	int _dirsScanned;

	Widget *_okButton;
	StaticTextWidget *_dirProgressText;
	StaticTextWidget *_gameProgressText;
};


} // End of namespace GUI

#endif
