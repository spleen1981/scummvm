/* Residual - A 3D game interpreter
 *
 * Residual is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * $URL$
 * $Id$
 *
 */

#include "common/util.h"

#include "engines/stark/xmg.h"

namespace Stark {

Graphics::Surface *XMGDecoder::decodeImage(Common::SeekableReadStream *stream) {

	_stream = stream;

	header.version = stream->readUint32LE();
	header.transColor = (stream->readByte() << 16);
	header.transColor += stream->readUint16LE();
	header.unknown = stream->readByte();
	header.width = stream->readUint32LE();
	header.height = stream->readUint32LE();
	header.scanLen = stream->readUint32LE();
	header.unknown2 = stream->readUint32LE();
	header.unknown3 = stream->readUint32LE();

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(header.width, header.height, 3);

	_width = header.width;

	_pixels = (byte *)surface->pixels;
	_currX = 0;
	_currY = 0;

	byte op1, op2;
	while (stream->pos() < stream->size()) {
		if (_currX >= header.width) {
			_currX -= 640;
			_currY += 2;
			_pixels = (byte *)surface->getBasePtr(_currX, _currY);
			if (_currY >= header.height)
				break;
		}
		op1 = stream->readByte();
		switch (op1 & 0xC0) {
			case 0x00:
				//YCrCb
				processYCrCb(op1 & 0x3F);
				break;
			case 0x40:
				//Trans
				processTrans(op1 & 0x3F);
				break;
			case 0x80:
				// RGB
				processRGB(op1 & 0x3F);
				break;
			case 0xC0:
				op2 = stream->readByte();
				switch (op1 & 0x30) {
					case 0x00:
						//YCrCb
						processYCrCb(((op1 & 0x0F) << 8) + op2);
						break;
					case 0x10:
						//Trans
						processTrans(((op1 & 0x0F) << 8) + op2);
						break;
					case 0x20:
						// RGB
						processRGB(((op1 & 0x0F) << 8) + op2);
						break;
					case 0x30:
						error("Unsupported colour mode");
						break;
				}	
				break;
		}	
		
	}

	_pixels = 0;
	return surface;
}

void XMGDecoder::processYCrCb(uint16 count){
	byte y0, y1, y2, y3;
	byte cr, cb;
	for (int i = 0; i < count; i++) {
		y0 = _stream->readByte();
		y1 = _stream->readByte();
		y2 = _stream->readByte();
		y3 = _stream->readByte();
		cr = _stream->readByte();
		cb = _stream->readByte();

		YUV2RGB(y0, cb, cr, *_pixels, *(_pixels + 1), *(_pixels + 2));
		YUV2RGB(y1, cb, cr, *(_pixels + 3), *(_pixels + 4), *(_pixels + 5));

		_pixels += _width * 3;
		YUV2RGB(y2, cb, cr, *_pixels, *(_pixels + 1), *(_pixels + 2));
		YUV2RGB(y3, cb, cr, *(_pixels + 3), *(_pixels + 4), *(_pixels + 5));

		_pixels -= _width * 3;
		_pixels += 6;
	}
	_currX += 2 * count;
	
}

void XMGDecoder::processTrans(uint16 count){
	for (int i = 0; i < count; i++){
		byte r, g, b;
		r = (header.transColor >> 16);
		g = (header.transColor >> 8) & 0xFF;
		b = (header.transColor & 0xFF);
		*_pixels = r;
		*(_pixels + 1) = g;
		*(_pixels + 2) = b;

		*(_pixels + 3) = r;
		*(_pixels + 4) = g;
		*(_pixels + 5) = b;

		_pixels += _width * 3;

		*_pixels = r;
		*(_pixels + 1) = g;
		*(_pixels + 2) = b;

		*(_pixels + 3) = r;
		*(_pixels + 4) = g;
		*(_pixels + 5) = b;

		_pixels -= _width * 3;
		_pixels += 6;

	}
	_currX += 2 * count;
}

void XMGDecoder::processRGB(uint16 count){
	for (int i = 0; i < count; i++){
		byte r, g, b;

		r = _stream->readByte();
		g = _stream->readByte();
		b = _stream->readByte();
		*_pixels = r;
		*(_pixels + 1) = g;
		*(_pixels + 2) = b;

		r = _stream->readByte();
		g = _stream->readByte();
		b = _stream->readByte();
		*(_pixels + 3) = r;
		*(_pixels + 4) = g;
		*(_pixels + 5) = b;

		_pixels += _width * 3;

		r = _stream->readByte();
		g = _stream->readByte();
		b = _stream->readByte();
		*_pixels = r;
		*(_pixels + 1) = g;
		*(_pixels + 2) = b;

		r = _stream->readByte();
		g = _stream->readByte();
		b = _stream->readByte();
		*(_pixels + 3) = r;
		*(_pixels + 4) = g;
		*(_pixels + 5) = b;

		_pixels -= _width * 3;

		_pixels += 6;
	}
	_currX += 2 * count;
}

} // end of namespace Stark
