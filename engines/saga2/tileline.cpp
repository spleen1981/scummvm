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
 * aint32 with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * Based on the original sources
 *   Faery Tale II -- The Halls of the Dead
 *   (c) 1993-1996 The Wyrmkeep Entertainment Co.
 */

#include "saga2/std.h"
#include "saga2/tileline.h"
#include "saga2/tcoords.h"
#include "saga2/objects.h"
#include "saga2/tile.h"
#include "saga2/actor.h"

namespace Saga2 {

#if DEBUG

void TPTriangle(const TilePoint &tp1, const TilePoint &tp2, const TilePoint &tp3, int16 color) {
	TPLine(tp1, tp2, color);
	TPLine(tp2, tp3, color);
	TPLine(tp3, tp1, color);
}

void TPRectangle(const TilePoint &tp1, const TilePoint &tp2, const TilePoint &tp3, const TilePoint &tp4, int16 color) {
	TPLine(tp1, tp2, color);
	TPLine(tp2, tp3, color);
	TPLine(tp3, tp4, color);
	TPLine(tp4, tp1, color);
}

void TPCircle(const TilePoint &tp, const int radius, int16 color) {
	TPLine(tp + TilePoint(radius / 1, radius / 2, 0), tp + TilePoint(radius / 2, radius / 1, 0), color);
	TPLine(tp + TilePoint(radius / 2, radius / 1, 0), tp + TilePoint(radius / -2, radius / 1, 0), color);
	TPLine(tp + TilePoint(radius / -2, radius / 1, 0), tp + TilePoint(radius / -1, radius / 2, 0), color);
	TPLine(tp + TilePoint(radius / -1, radius / 2, 0), tp + TilePoint(radius / -1, radius / -2, 0), color);
	TPLine(tp + TilePoint(radius / -1, radius / -2, 0), tp + TilePoint(radius / -2, radius / -1, 0), color);
	TPLine(tp + TilePoint(radius / -2, radius / -1, 0), tp + TilePoint(radius / 2, radius / -1, 0), color);
	TPLine(tp + TilePoint(radius / 2, radius / -1, 0), tp + TilePoint(radius / 1, radius / -2, 0), color);
	TPLine(tp + TilePoint(radius / 1, radius / -2, 0), tp + TilePoint(radius / 1, radius / 2, 0), color);
}

#ifdef _WIN32
#include <windows.h>
#include "saga2/ftawin.h"
extern CFTWindow        *pWindow;
#endif
extern Rect16           tileRect;

//  Draw a line in TP space

void TPLine(const TilePoint &start, const TilePoint &stop) {
	Point16         startPt,
	                stopPt;

	TileToScreenCoords(start, startPt);
	TileToScreenCoords(stop, stopPt);

	startPt.x   += tileRect.x;
	stopPt.x    += tileRect.x;
	startPt.y   += tileRect.y;
	stopPt.y    += tileRect.y;

#ifdef _WIN32
	HDC     hdc;
	HPEN    hPen;

	hdc = GetDC(pWindow->m_hWnd);
	hPen = SelectObject(hdc, GetStockObject(WHITE_PEN));
	MoveToEx(hdc, startPt.x, startPt.y, NULL);
	LineTo(hdc, stopPt.x, stopPt.y);
	SelectObject(hdc, hPen);
	ReleaseDC(pWindow->m_hWnd, hdc);
#else
	mainPort.setColor(1);
	mainPort.moveTo(startPt);
	mainPort.drawTo(stopPt);
#endif
}

void TPLine(const TilePoint &start, const TilePoint &stop, int16 color);

void TPLine(const TilePoint &start, const TilePoint &stop, int16 color) {
	Point16         startPt,
	                stopPt;

	TileToScreenCoords(start, startPt);
	TileToScreenCoords(stop, stopPt);

	startPt.x   += tileRect.x;
	stopPt.x    += tileRect.x;
	startPt.y   += tileRect.y;
	stopPt.y    += tileRect.y;

#ifdef _WIN32
	HDC     hdc;
	HPEN    hPen;

	hdc = GetDC(pWindow->m_hWnd);
	hPen = SelectObject(hdc, GetStockObject(WHITE_PEN));
	MoveToEx(hdc, startPt.x, startPt.y, NULL);
	LineTo(hdc, stopPt.x, stopPt.y);
	SelectObject(hdc, hPen);
	ReleaseDC(pWindow->m_hWnd, hdc);
#else
	mainPort.setColor(color);
	mainPort.moveTo(startPt);
	mainPort.drawTo(stopPt);
#endif
}
#endif

} // end of namespace Saga2
