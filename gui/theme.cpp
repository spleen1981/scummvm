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

#include "gui/theme.h"
#include "common/file.h"
#include "common/archive.h"
#include "common/unzip.h"

namespace GUI {

Theme::Theme() : _loadedThemeX(0), _loadedThemeY(0) {}

Theme::~Theme() {}

const Graphics::Font *Theme::loadFont(const char *filename) {
	const Graphics::NewFont *font = 0;
	Common::String cacheFilename = genCacheFilename(filename);
	Common::File fontFile;

	if (!cacheFilename.empty()) {
		if (fontFile.open(cacheFilename))
			font = Graphics::NewFont::loadFromCache(fontFile);
		if (font)
			return font;

#ifdef USE_ZLIB
		ZipArchive zipArchive(getThemeFileName().c_str());
		if (zipArchive.hasFile(cacheFilename)) {
			Common::FilePtr stream(zipArchive.openFile(cacheFilename));
			font = Graphics::NewFont::loadFromCache(*stream.get());
		}
#endif
		if (font)
			return font;
	}

	// normal open
	if (fontFile.open(filename)) {
		font = Graphics::NewFont::loadFont(fontFile);
	}

#ifdef USE_ZLIB
	if (!font) {
		ZipArchive zipArchive(getThemeFileName().c_str());
		if (zipArchive.hasFile(filename)) {
			Common::FilePtr stream(zipArchive.openFile(filename));
			font = Graphics::NewFont::loadFont(*stream.get());
		}
	}
#endif

	if (font) {
		if (!cacheFilename.empty()) {
			if (!Graphics::NewFont::cacheFontData(*font, cacheFilename)) {
				warning("Couldn't create cache file for font '%s'", filename);
			}
		}
	}

	return font;
}

Common::String Theme::genCacheFilename(const char *filename) {
	Common::String cacheName(filename);
	for (int i = cacheName.size() - 1; i >= 0; --i) {
		if (cacheName[i] == '.') {
			while ((uint)i < cacheName.size() - 1) {
				cacheName.deleteLastChar();
			}

			cacheName += "fcc";
			return cacheName;
		}
	}

	return "";
}

bool Theme::isThemeLoadingRequired() {
	int x = g_system->getOverlayWidth(), y = g_system->getOverlayHeight();

	if (_loadedThemeX == x && _loadedThemeY == y)
		return false;

	_loadedThemeX = x;
	_loadedThemeY = y;

	return true;
}

bool Theme::themeConfigParseHeader(Common::String header, Common::String &themeName) {	
	header.trim();
	
	if (header[0] != '[' || header.lastChar() != ']')
		return false;
		
	header.deleteChar(0);
	header.deleteLastChar();
	
	Common::StringTokenizer tok(header, ":");
	
	if (tok.nextToken() != SCUMMVM_THEME_VERSION_STR)
		return false;
		
	themeName = tok.nextToken();
	Common::String author = tok.nextToken();

	return tok.empty();
}

bool Theme::themeConfigUseable(const Common::FilesystemNode &node, Common::String &themeName) {
	Common::String stxHeader;
	bool foundHeader = false;
	
	if (ConfMan.hasKey("themepath"))
		Common::File::addDefaultDirectory(ConfMan.get("themepath"));

#ifdef DATA_PATH
	Common::File::addDefaultDirectoryRecursive(DATA_PATH);
#endif

	if (ConfMan.hasKey("extrapath"))
		Common::File::addDefaultDirectoryRecursive(ConfMan.get("extrapath"));
		
	if (node.getName().hasSuffix(".zip")) {		
#ifdef USE_ZLIB
		ZipArchive zipArchive(node.getPath().c_str());
		if (zipArchive.hasFile("THEMERC")) {
			Common::FilePtr stream(zipArchive.openFile("THEMERC"));
			stxHeader = stream->readLine();
			// TODO: Read first line of file. How?
			if (themeConfigParseHeader(stxHeader.c_str(), themeName))
				foundHeader = true;
		}
#else
		return false;
#endif	
	} else if (node.isDirectory()) {			
		Common::FilesystemNode headerfile = node.getChild("THEMERC");
		if (!headerfile.exists() || !headerfile.isReadable() || headerfile.isDirectory())
			return false;
			
		// TODO: File or FilePtr?
		Common::File f;
		f.open(headerfile);
		stxHeader = f.readLine();
		if (themeConfigParseHeader(stxHeader.c_str(), themeName))
			foundHeader = true;
	}

	return foundHeader;
}

} // End of namespace GUI

