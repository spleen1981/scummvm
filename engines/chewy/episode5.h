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

#ifndef CHEWY_EPISODE5_H
#define CHEWY_EPISODE5_H

namespace Chewy {

void r70_entry();
void r70_exit(int16 eib_nr);

void r71_entry();
void r71_exit(int16 eib_nr);
int r71_proc1();
int r71_proc6();
void r71_proc7();

void r72_entry();
void r72_exit(int16 eib_nr);

void r73_entry();
void r73_exit(int16 eib_nr);
int r73_proc1();
int r73_proc2();

void r74_entry();
void r74_exit(int16 eib_nr);
int r74_proc1();

void r75_entry();
void r75_exit(int16 eib_nr);

void r76_entry();
void r76_exit();
void r76_talk1();
void r76_talk2();
int r76_proc6();
int r76_proc7();

void r77_entry();
void r77_exit();
int r77_proc1();
int r77_proc2();

void r78_entry();
void r78_exit();

void r79_entry();
void r79_exit();

void r80_entry();

void r81_entry();
void r81_exit(int16 eib_nr);
void r81_proc1();
int r81_proc2();

void r82_entry();
void r82_exit(int16 eib_nr);
void r82_talk1();
void r82_talk2();
void r82_talk3();
int r82_proc3();
int r82_proc6();
void r82_proc8();
int r82_proc9();

void r83_entry();

void r84_entry();
void r84_exit(int16 eib_nr);
void r84_talk1();
void r84_talk2();
int r84_proc4();

void r85_entry(int16 eib_nr);
void r85_exit(int16 eib_nr);

void r86_entry(int16 eib_nr);
void r86_exit(int16 eib_nr);
int r86_proc2();

void r87_entry();
void r87_exit(int16 eib_nr);
void r87_entry();
int r87_proc2();
int r87_proc4();

void r88_entry();
void r88_exit();
int r88_proc1();
int r88_proc2();
int r88_proc3();

void r89_entry();
void r89_exit();
void r89_talk1();
int r89_proc2();
int r89_proc3();
int r89_proc4();
int r89_proc5();

void r90_entry(int16 eib_nr);
void r90_exit(int16 eib_nr);
void r90_proc2();
int r90_proc3();
int r90_proc4();
int r90_proc6();

void r91_entry();
void r91_exit(int16 eib_nr);

void r92_entry();

void r93_entry();
void r93_exit();

void r94_entry();
void r94_exit();
void r94_talk1();
int r94_proc3();

void r95_entry(int16 eib_nr);
void r95_exit(int16 eib_nr);
int r95_proc2();

void r96_entry();
void r96_exit(int16 eib_nr);

void r97_entry();
void r97_exit();
void r97_proc2();
void r97_proc3();
void r97_proc4();
int r97_proc5();
int r97_proc6();
int r97_proc7();
int r97_proc8();
int r97_proc9();
int r97_proc10();
int r97_proc11();
void r97_proc12();
void r97_proc13();
int r97_proc14();
void r97_proc15();

} // namespace Chewy

#endif
