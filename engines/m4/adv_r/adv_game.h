
/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef M4_ADV_R_ADV_GAME_H
#define M4_ADV_R_ADV_GAME_H

#include "common/serializer.h"
#include "m4/adv_r/adv.h"
#include "m4/m4_types.h"

namespace M4 {

#define KERNEL_SCRATCH_SIZE 256 // Size of game scratch area

struct GameControl {
	uint32	scratch[KERNEL_SCRATCH_SIZE];  // Scratch variables for room
	int16 room_id = 0;
	int16 new_room = 0;
	int16 previous_section = 0;
	int16 section_id = 0;
	int16 new_section = 0;
	int16 previous_room = 0;

	int32 digi_overall_volume_percent = 100;
	int32 midi_overall_volume_percent = 100;
	bool camera_pan_instant = false;

	/**
	 * Shortcut for setting new room and section
	 */
	void setRoom(int newRoom) {
		new_room = newRoom;
		new_section = newRoom / 100;
	}

	void syncGame(Common::Serializer &s);
};

} // End of namespace M4

#endif
