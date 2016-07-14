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

#include "titanic/star_control/star_control_sub20.h"
#include "common/textconsole.h"

namespace Titanic {

CStarControlSub20::CStarControlSub20(void *src) {
	_lockCounter = 0;
	_dataP = nullptr;

	if (src) {
		copyFrom1(src);
	} else {
		_field4 = 0.0;
		_field8 = 0.0;
		_fieldC = 20.0;
		_field10 = 0.0;
		_field14 = 50000.0;
		_field18 = 1.0;
		_field1C = 1.0;
		_field20 = 0.0;
	}
}

CStarControlSub20::~CStarControlSub20() { 
	clear();
}

void CStarControlSub20::copyFrom1(void *src) {
	error("TODO: CStarControlSub20::copyFrom1");
}

void CStarControlSub20::copyFrom2(void *src) {
	error("TODO: CStarControlSub20::copyFrom2");
}

void CStarControlSub20::proc4() {
	if (!isLocked() && _field4 < _field14) {
		_field8 += _field4;
		if (_fieldC == _field8)
			_field4 -= _field8;
		else
			_field4 += _field8;
	}
}

void CStarControlSub20::proc5() {
	if (!isLocked()) {
		_field8 -= _fieldC;
		if (_field8 == _field4)
			_field4 += _field8;
		else
			_field4 -= _field8;

		if (_field8 < 0.0)
			_field8 = 0.0;
	}
}

void CStarControlSub20::proc6() {
	if (!isLocked())
		_field4 = _field14;
}

void CStarControlSub20::proc7() {
	if (!isLocked()) {
		_field4 = 0.0;
		_field8 = 0.0;
	}
}

void CStarControlSub20::proc11(CErrorCode *errorCode, void *v2, void *v3) {
	if (_field4 > 0.0) {
		warning("TODO: CStarControlSub20::proc11");
	}
}

void CStarControlSub20::setData(void *data) {
	clear();
	_dataP = data;
}

void CStarControlSub20::clear() {
	if (_dataP) {
		delete _dataP;
		_dataP = nullptr;
	}
}

void CStarControlSub20::load(SimpleFile *file, int val) {
	if (!val) {
		_field4 = file->readFloat();
		_field8 = file->readFloat();
		_fieldC = file->readFloat();
		_field10 = file->readFloat();
		_field14 = file->readFloat();
		_field18 = file->readFloat();
		_field1C = file->readFloat();
		_field20 = file->readFloat();
	}
}

void CStarControlSub20::save(SimpleFile *file, int indent) {
	file->writeFloatLine(_field4, indent);
	file->writeFloatLine(_field8, indent);
	file->writeFloatLine(_fieldC, indent);
	file->writeFloatLine(_field10, indent);
	file->writeFloatLine(_field14, indent);
	file->writeFloatLine(_field18, indent);
	file->writeFloatLine(_field1C, indent);
	file->writeFloatLine(_field20, indent);
}

void CStarControlSub20::incLockCount() {
	++_lockCounter;
}

void CStarControlSub20::decLockCount() {
	if (_lockCounter > 0)
		--_lockCounter;
}

} // End of namespace Titanic
