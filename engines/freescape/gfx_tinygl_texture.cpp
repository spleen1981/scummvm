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

#include "graphics/tinygl/zblit.h"

#include "freescape/gfx_tinygl_texture.h"

namespace Freescape {

TinyGLTexture::TinyGLTexture(const Graphics::Surface *surface) {
	_width = surface->w;
	_height = surface->h;
	_format = surface->format;

	_blitImage = tglGenBlitImage();

	update(surface);
}

TinyGLTexture::~TinyGLTexture() {
	tglDeleteTextures(1, &_id);
	tglDeleteBlitImage(_blitImage);
}

void TinyGLTexture::update(const Graphics::Surface *surface) {
	tglUploadBlitImage(_blitImage, *surface, 0xA0A0A0FF, true);
}

void TinyGLTexture::updatePartial(const Graphics::Surface *surface, const Common::Rect &rect) {
	// FIXME: TinyGL does not support partial texture update
	update(surface);
}

TinyGL::BlitImage *TinyGLTexture::getBlitTexture() const {
	return _blitImage;
}

} // End of namespace Freescape
