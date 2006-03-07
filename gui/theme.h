/* ScummVM - Scumm Interpreter
 * Copyright (C) 2006 The ScummVM project
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

#ifndef GUI_THEME_H
#define GUI_THEME_H

#include "common/stdafx.h"
#include "common/system.h"
#include "common/rect.h"
#include "common/str.h"
#include "common/config-file.h"

#include "graphics/surface.h"
#include "graphics/fontman.h"

#include "gui/eval.h"

namespace GUI {

typedef Common::String String;

// Hints to the theme engine that the widget is used in a non-standard way.

enum {
	// Indicates that this is the first time the widget is drawn.
	THEME_HINT_FIRST_DRAW = 1 << 0,

	// Indicates that the widget will be redrawn often, e.g. list widgets.
	// It may therefore be a good idea to save the background so that it
	// can be redrawn quickly.
	THEME_HINT_SAVE_BACKGROUND = 1 << 1,
	
	// Indicates that this is the launcher dialog (maybe delete this in the future)
	THEME_HINT_MAIN_DIALOG = 1 << 2,
	
	// Indicates special colorfade
	THEME_HINT_SPECIAL_COLOR = 1 << 3,
	
	// Indictaes that a shadows should be drawn around the background
	THEME_HINT_USE_SHADOW = 1 << 4
};


class Theme {
public:
	Theme() : _drawArea(), _configFile(), _loadedThemeX(0), _loadedThemeY(0) {
		Common::MemoryReadStream s((const byte *)_defaultConfigINI, strlen(_defaultConfigINI));
		_defaultConfig.loadFromStream(s);

		_evaluator = new Eval();
	}
		
	virtual ~Theme() { delete _evaluator;}

	enum kTextAlign {
		kTextAlignLeft,
		kTextAlignCenter,
		kTextAlignRight
	};

	enum kWidgetBackground {
		kWidgetBackgroundNo,
		kWidgetBackgroundPlain,
		kWidgetBackgroundBorder,
		kWidgetBackgroundBorderSmall
	};

	enum kState {
		kStateDisabled,
		kStateEnabled,
		kStateHighlight
	};

	enum kScrollbarState {
		kScrollbarStateNo,
		kScrollbarStateUp,
		kScrollbarStateDown,
		kScrollbarStateSlider,
		kScrollbarStateSinglePage
	};

	virtual bool init() = 0;
	virtual void deinit() = 0;

	virtual void refresh() = 0;

	virtual void enable() = 0;
	virtual void disable() = 0;

	virtual void openDialog() = 0;
	virtual void closeDialog() = 0;

	virtual void clearAll() = 0;
	virtual void drawAll() = 0;
	
	virtual void setDrawArea(const Common::Rect &r) { _drawArea = r; }
	// resets the draw area to the screen size
	virtual void resetDrawArea() = 0;
	
	virtual const Common::ConfigFile &getConfigFile() { return _configFile; }

	virtual const Graphics::Font *getFont() const = 0;
	virtual int getFontHeight() const = 0;
	virtual int getStringWidth(const Common::String &str) const = 0;
	virtual int getCharWidth(byte c) const = 0;

	virtual void drawDialogBackground(const Common::Rect &r, uint16 hints, kState state = kStateEnabled) = 0;
	virtual void drawText(const Common::Rect &r, const Common::String &str, kState state = kStateEnabled, kTextAlign align = kTextAlignCenter, bool inverted = false, int deltax = 0, bool useEllipsis = true) = 0;
	// this should ONLY be used by the debugger until we get a nicer solution
	virtual void drawChar(const Common::Rect &r, byte ch, const Graphics::Font *font, kState state = kStateEnabled) = 0;

	virtual void drawWidgetBackground(const Common::Rect &r, uint16 hints, kWidgetBackground background = kWidgetBackgroundPlain, kState state = kStateEnabled) = 0;
	virtual void drawButton(const Common::Rect &r, const Common::String &str, kState state = kStateEnabled) = 0;
	virtual void drawSurface(const Common::Rect &r, const Graphics::Surface &surface, kState state = kStateEnabled) = 0;
	virtual void drawSlider(const Common::Rect &r, int width, kState state = kStateEnabled) = 0;
	virtual void drawCheckbox(const Common::Rect &r, const Common::String &str, bool checked, kState state = kStateEnabled) = 0;
	virtual void drawTab(const Common::Rect &r, int tabHeight, int tabWidth, const Common::Array<Common::String> &tabs, int active, uint16 hints, kState state = kStateEnabled) = 0;
	virtual void drawScrollbar(const Common::Rect &r, int sliderY, int sliderHeight, kScrollbarState, kState state = kStateEnabled) = 0;
	virtual void drawCaret(const Common::Rect &r, bool erase, kState state = kStateEnabled) = 0;
	virtual void drawLineSeparator(const Common::Rect &r, kState state = kStateEnabled) = 0;

	Graphics::TextAlignment convertAligment(kTextAlign align) const {
		switch (align) {
		case kTextAlignLeft:
			return Graphics::kTextAlignLeft;
			break;

		case kTextAlignRight:
			return Graphics::kTextAlignRight;
			break;

		default:
			break;
		};
		return Graphics::kTextAlignCenter;
	};
	
	kTextAlign convertAligment(Graphics::TextAlignment align) const {
		switch (align) {
		case Graphics::kTextAlignLeft:
			return kTextAlignLeft;
			break;

		case Graphics::kTextAlignRight:
			return kTextAlignRight;
			break;

		default:
			break;
		}
		return kTextAlignCenter;
	}

	void processResSection(Common::ConfigFile &config, String name, bool skipDefs = false);
	void processSingleLine(const String &section, const String name, const String str);
	void setSpecialAlias(const String alias, const String &name);

	bool isThemeLoadingRequired();
	void loadTheme(Common::ConfigFile &config, bool reset = true);

	Eval *_evaluator;

protected:
	Common::Rect _drawArea;
	Common::ConfigFile _configFile;
	Common::ConfigFile _defaultConfig;

private:
	static const char *_defaultConfigINI;
	int _loadedThemeX, _loadedThemeY;
};

#define OLDGUI_TRANSPARENCY

class ThemeClassic : public Theme {
public:
	ThemeClassic(OSystem *system);
	virtual ~ThemeClassic();

	bool init();
	void deinit();

	void refresh();

	void enable();
	void disable();

	void openDialog();
	void closeDialog();

	void clearAll();
	void drawAll();
	
	void resetDrawArea();


	typedef Common::String String;

	const Graphics::Font *getFont() const { return _font; }
	int getFontHeight() const { if (_initOk) return _font->getFontHeight(); return 0; }
	int getStringWidth(const String &str) const { if (_initOk) return _font->getStringWidth(str); return 0; }
	int getCharWidth(byte c) const { if (_initOk) return _font->getCharWidth(c); return 0; }

	void drawDialogBackground(const Common::Rect &r, uint16 hints, kState state);
	void drawText(const Common::Rect &r, const String &str, kState state, kTextAlign align, bool inverted, int deltax, bool useEllipsis);
	void drawChar(const Common::Rect &r, byte ch, const Graphics::Font *font, kState state);

	void drawWidgetBackground(const Common::Rect &r, uint16 hints, kWidgetBackground background, kState state);
	void drawButton(const Common::Rect &r, const String &str, kState state);
	void drawSurface(const Common::Rect &r, const Graphics::Surface &surface, kState state);
	void drawSlider(const Common::Rect &r, int width, kState state);
	void drawCheckbox(const Common::Rect &r, const String &str, bool checked, kState state);
	void drawTab(const Common::Rect &r, int tabHeight, int tabWidth, const Common::Array<String> &tabs, int active, uint16 hints, kState state);
	void drawScrollbar(const Common::Rect &r, int sliderY, int sliderHeight, kScrollbarState, kState state);
	void drawCaret(const Common::Rect &r, bool erase, kState state);
	void drawLineSeparator(const Common::Rect &r, kState state);
private:
	void restoreBackground(Common::Rect r);
	bool addDirtyRect(Common::Rect r, bool save = false);

	void box(int x, int y, int width, int height, OverlayColor colorA, OverlayColor colorB, bool skipLastRow = false);
	void box(int x, int y, int width, int height);

	OverlayColor getColor(kState state);

	OSystem *_system;
	Graphics::Surface _screen;

#ifdef OLDGUI_TRANSPARENCY
	struct DialogState {
		Graphics::Surface screen;
	} *_dialog;

	void blendScreenToDialog();
#endif

	bool _initOk;

	const Graphics::Font *_font;
	OverlayColor _color, _shadowcolor;
	OverlayColor _bgcolor;
	OverlayColor _textcolor;
	OverlayColor _textcolorhi;
};

#ifndef DISABLE_FANCY_THEMES

class ThemeNew : public Theme {
public:
	ThemeNew(OSystem *system, String stylefile);
	virtual ~ThemeNew();

	bool init();
	void deinit();

	void refresh();

	void enable();
	void disable();

	void openDialog();
	void closeDialog();

	void clearAll();
	void drawAll();
	
	void setDrawArea(const Common::Rect &r);
	void resetDrawArea();

	const Graphics::Font *getFont() const { return _font; }
	int getFontHeight() const { if (_font) return _font->getFontHeight(); return 0; }
	int getStringWidth(const String &str) const { if (_font) return _font->getStringWidth(str); return 0; }
	int getCharWidth(byte c) const { if (_font) return _font->getCharWidth(c); return 0; }

	void drawDialogBackground(const Common::Rect &r, uint16 hints, kState state);
	void drawText(const Common::Rect &r, const String &str, kState state, kTextAlign align, bool inverted, int deltax, bool useEllipsis);
	void drawChar(const Common::Rect &r, byte ch, const Graphics::Font *font, kState state);

	void drawWidgetBackground(const Common::Rect &r, uint16 hints, kWidgetBackground background, kState state);
	void drawButton(const Common::Rect &r, const String &str, kState state);
	void drawSurface(const Common::Rect &r, const Graphics::Surface &surface, kState state);
	void drawSlider(const Common::Rect &r, int width, kState state);
	void drawCheckbox(const Common::Rect &r, const String &str, bool checked, kState state);
	void drawTab(const Common::Rect &r, int tabHeight, int tabWidth, const Common::Array<String> &tabs, int active, uint16 hints, kState state);
	void drawScrollbar(const Common::Rect &r, int sliderY, int sliderHeight, kScrollbarState, kState state);
	void drawCaret(const Common::Rect &r, bool erase, kState state);
	void drawLineSeparator(const Common::Rect &r, kState state);
private:
	bool addDirtyRect(Common::Rect r, bool backup = false, bool special = false);

	void colorFade(const Common::Rect &r, OverlayColor start, OverlayColor end, uint factor = 1);
	void drawRect(const Common::Rect &r, const Graphics::Surface *corner, const Graphics::Surface *top,
				const Graphics::Surface *left, const Graphics::Surface *fill, int alpha, bool skipLastRow = false);
	void drawRectMasked(const Common::Rect &r, const Graphics::Surface *corner, const Graphics::Surface *top,
						const Graphics::Surface *left, const Graphics::Surface *fill, int alpha,
						OverlayColor start, OverlayColor end, uint factor = 1, bool skipLastRow = false);
	void drawSurface(const Common::Rect &r, const Graphics::Surface *surf, bool upDown, bool leftRight, int alpha);
	void drawSurfaceMasked(const Common::Rect &r, const Graphics::Surface *surf, bool upDown, bool leftRight, int alpha,
							OverlayColor start, OverlayColor end, uint factor = 1);

	enum kShadowStyles {
		kShadowFull = 0,
		kShadowSmall = 1,
		kShadowButton = 2,
		kShadowSlider = 3
	};

	Common::Rect shadowRect(const Common::Rect &r, uint32 shadowStyle);
	void drawShadow(const Common::Rect &r, const Graphics::Surface *corner, const Graphics::Surface *top,
					const Graphics::Surface *left, const Graphics::Surface *fill, uint32 shadowStyle, bool full,
					bool skipLastRow = false);
	void drawShadowRect(const Common::Rect &r, const Common::Rect &area, const Graphics::Surface *corner,
						const Graphics::Surface *top, const Graphics::Surface *left, const Graphics::Surface *fill,
						int alpha, bool skipLastRow = false);

	int _shadowLeftWidth, _shadowRightWidth;
	int _shadowTopHeight, _shadowBottomHeight;

	OSystem *_system;
	Graphics::Surface _screen;
	Common::Rect _shadowDrawArea;

	bool _initOk;
	bool _forceRedraw;
	
	int _lastUsedBitMask;
	void resetupGuiRenderer();
	void setupColors();

	void restoreBackground(Common::Rect r, bool special = false);
	OverlayColor getColor(kState state);

	struct DialogState {
		Graphics::Surface screen;
	} *_dialog;

	const Graphics::Font *_font;

	enum kImageHandles {
		kDialogBkgdCorner = 0,
		kDialogBkgdTop = 1,
		kDialogBkgdLeft = 2,
		kDialogBkgd = 3,
		
		kWidgetBkgdCorner = 4,
		kWidgetBkgdTop = 5,
		kWidgetBkgdLeft = 6,
		kWidgetBkgd = 7,
		
		kCheckboxEmpty = 8,
		kCheckboxChecked = 9,
		
		kWidgetArrow = 10,
		
		kTabBkgdCorner = 11,
		kTabBkgdTop = 12,
		kTabBkgdLeft = 13,
		kTabBkgd = 14,
		
		kSliderBkgdCorner = 15,
		kSliderBkgdTop = 16,
		kSliderBkgdLeft = 17,
		kSliderBkgd = 18,
		
		kSliderCorner = 19,
		kSliderTop = 20,
		kSliderLeft = 21,
		kSlider = 22,
		
		kScrollbarBkgdCorner = 23,
		kScrollbarBkgdTop = 24,
		kScrollbarBkgdLeft = 25,
		kScrollbarBkgd = 26,
		
		kScrollbarCorner = 27,
		kScrollbarTop = 28,
		kScrollbarLeft = 29,
		kScrollbar = 30,
		
		kButtonBkgdCorner = 31,
		kButtonBkgdTop = 32,
		kButtonBkgdLeft = 33,
		kButtonBkgd = 34,
		
		kWidgetSmallBkgdCorner = 35,
		kWidgetSmallBkgdTop = 36,
		kWidgetSmallBkgdLeft = 37,
		kWidgetSmallBkgd = 38,
		
		kImageHandlesMax
	};

	const String *_imageHandles;
	const Graphics::Surface **_images;
	
	enum kColorHandles {
		kMainDialogStart = 0,
		kMainDialogEnd = 1,
		
		kDialogStart = 2,
		kDialogEnd = 3,
		
		kColorStateDisabled = 4,
		kColorStateHighlight = 5,
		kColorStateEnabled = 6,
		kColorTransparency = 7,
		
		kTextInvertedBackground = 8,
		kTextInvertedColor = 9,
		
		kWidgetBackgroundStart = 10,
		kWidgetBackgroundEnd = 11,
		kWidgetBackgroundSmallStart = 12,
		kWidgetBackgroundSmallEnd = 13,
		
		kButtonBackgroundStart = 14,
		kButtonBackgroundEnd = 15,
		kButtonTextEnabled = 16,
		kButtonTextDisabled = 17,
		kButtonTextHighlight = 18,
		
		kSliderBackgroundStart = 19,
		kSliderBackgroundEnd = 20,
		kSliderStart = 21,
		kSliderEnd = 22,
		
		kTabBackgroundStart = 23,
		kTabBackgroundEnd = 24,
		
		kScrollbarBackgroundStart = 25,
		kScrollbarBackgroundEnd = 26,
		kScrollbarButtonStart = 27,
		kScrollbarButtonEnd = 28,
		kScrollbarSliderStart = 29,
		kScrollbarSliderEnd = 30,
		
		kCaretColor = 31,
		
		kSliderHighStart = 32,
		kSliderHighEnd = 33,
		
		kButtonBackgroundHighlightStart = 34,
		kButtonBackgroundHighlightEnd = 35,
		
		kScrollbarButtonHighlightStart = 36,
		kScrollbarButtonHighlightEnd = 37,
		kScrollbarSliderHighlightStart = 38,
		kScrollbarSliderHighlightEnd = 39,
		
		kColorHandlesMax
	};
	
	OverlayColor _colors[kColorHandlesMax];
	
	enum kGradientFactors {
		kMainDialogFactor = 0,
		kDialogFactor = 1,
		kDialogSpecialFactor = 2,
		
		kWidgetSmallFactor = 3,
		kWidgetFactor = 4,
		
		kButtonFactor = 5,
		
		kSliderFactor = 6,
		kSliderBackground = 7,
		
		kTabFactor = 7,
		
		kScrollbarFactor = 8,
		kScrollbarBkgdFactor = 9,
		
		kMaxGradientFactors
	};
	
	uint _gradientFactors[kMaxGradientFactors];
};
#endif
} // end of namespace GUI

#endif // GUI_THEME_H
