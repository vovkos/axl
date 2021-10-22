//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_GUI_COLOR_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//..............................................................................

enum {
	ColorFlag_Undefined   = 0x80000000,
	ColorFlag_Index       = 0x40000000,
	ColorFlag_IndexMask   = 0x3fffffff,
	ColorFlag_RgbMask     = 0x00ffffff,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum StdColor {
	StdColor_AnsiBlack         = 0x000000,
	StdColor_AnsiRed           = 0xbb0000,
	StdColor_AnsiGreen         = 0x00bb00,
	StdColor_AnsiYellow        = 0xbbbb00,
	StdColor_AnsiBlue          = 0x0000bb,
	StdColor_AnsiMagenta       = 0xbb00bb,
	StdColor_AnsiCyan          = 0x00bbbb,
	StdColor_AnsiWhite         = 0xbbbbbb,

	StdColor_AnsiBrightBlack   = 0x555555,
	StdColor_AnsiBrightRed     = 0xff5555,
	StdColor_AnsiBrightGreen   = 0x55ff55,
	StdColor_AnsiBrightYellow  = 0xffff55,
	StdColor_AnsiBrightBlue    = 0x5555ff,
	StdColor_AnsiBrightMagenta = 0xff55ff,
	StdColor_AnsiBrightCyan    = 0x55ffff,
	StdColor_AnsiBrightWhite   = 0xffffff,

	StdColor_Black             = 0x000000,
	StdColor_White             = 0xffffff,
	StdColor_Gray              = 0x808080,
	StdColor_Red               = 0x800000,
	StdColor_Green             = 0x008000,
	StdColor_Blue              = 0x000080,
	StdColor_Purple            = 0x800080,
	StdColor_Cyan              = 0x008080,

	StdColor_LightRed          = 0xff0000,
	StdColor_LightGreen        = 0x00ff00,
	StdColor_LightBlue         = 0x0000ff,
	StdColor_LightPurple       = 0xff00ff,
	StdColor_LightCyan         = 0x00ffff,

	StdColor_PastelGray        = 0xefefef,
	StdColor_PastelRed         = 0xffe4e1,
	StdColor_PastelOrange      = 0xfffacd,
	StdColor_PastelYellow      = 0xffffc0,
	StdColor_PastelGreen       = 0xe1fada,
	StdColor_PastelCyan        = 0xcdfaff,
	StdColor_PastelBlue        = 0xe2edff,
	StdColor_PastelPurple      = 0xe2dafd,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum StdPalColor {
	// ansi colors

	StdPalColor_AnsiBlack          = ColorFlag_Index | 0,
	StdPalColor_AnsiRed,           // 1
	StdPalColor_AnsiGreen,         // 2
	StdPalColor_AnsiYellow,        // 3
	StdPalColor_AnsiBlue,          // 4
	StdPalColor_AnsiMagenta,       // 5
	StdPalColor_AnsiCyan,          // 6
	StdPalColor_AnsiWhite,         // 7

	StdPalColor_AnsiBrightBlack,   // 8
	StdPalColor_AnsiBrightRed,     // 9
	StdPalColor_AnsiBrightGreen,   // 10
	StdPalColor_AnsiBrightYellow,  // 11
	StdPalColor_AnsiBrightBlue,    // 12
	StdPalColor_AnsiBrightMagenta, // 13
	StdPalColor_AnsiBrightCyan,    // 14
	StdPalColor_AnsiBrightWhite,   // 15

	// system widget colors

	StdPalColor_WidgetText,        // 16
	StdPalColor_WidgetBack,        // 17
	StdPalColor_GrayText,          // 18
	StdPalColor_SelectionText,     // 19
	StdPalColor_SelectionBack,     // 20
	StdPalColor_3DFace,            // 21
	StdPalColor_3DShadow,          // 22
	StdPalColor_3DDarkShadow,      // 23
	StdPalColor_3DLight,           // 24
	StdPalColor_3DHiLight,         // 25

	// axl colors

	StdPalColor_Black,             // 26
	StdPalColor_White,             // 27
	StdPalColor_Gray,              // 28
	StdPalColor_Red,               // 29
	StdPalColor_Green,             // 30
	StdPalColor_Blue,              // 31
	StdPalColor_Purple,            // 32
	StdPalColor_Cyan,              // 33

	StdPalColor_LightRed,          // 34
	StdPalColor_LightGreen,        // 35
	StdPalColor_LightBlue,         // 36
	StdPalColor_LightPurple,       // 37
	StdPalColor_LightCyan,         // 38

	StdPalColor_PastelGray,        // 39
	StdPalColor_PastelRed,         // 40
	StdPalColor_PastelOrange,      // 41
	StdPalColor_PastelYellow,      // 42
	StdPalColor_PastelGreen,       // 43
	StdPalColor_PastelCyan,        // 44
	StdPalColor_PastelBlue,        // 45
	StdPalColor_PastelPurple,      // 46

	StdPalColor__End,
	StdPalColor__Count             = ~ColorFlag_Index & StdPalColor__End
};

//..............................................................................

inline
uint_t
getColorIndex(uint_t color) {
	return (color & ColorFlag_Index) ? color & ColorFlag_IndexMask : -1;
}

inline
uint_t
overlayColor(
	uint_t baseColor,
	uint_t overlayColor
) {
	return (overlayColor & ColorFlag_Undefined) ? baseColor : overlayColor;
}

inline
uint_t
invertRgb(uint_t rgb) {
	return ((rgb & 0x0000ff) << 16) | ((rgb & 0xff0000) >> 16) | (rgb & 0x00ff00);
}

//..............................................................................

struct Palette {
	const uint_t* m_colorArray;
	size_t m_count;

	Palette() {
		setup(NULL, 0);
	}

	Palette(
		const uint_t* colorTable,
		size_t count
	) {
		m_colorArray = colorTable;
		m_count = count;
	}

	uint_t
	getColorRgb(uint_t color);

	void
	setup(
		const uint_t* colorTable,
		size_t count
	) {
		m_colorArray = colorTable;
		m_count = count;
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
