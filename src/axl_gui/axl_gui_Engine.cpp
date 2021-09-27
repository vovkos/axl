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

#include "pch.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//..............................................................................

Engine::Engine() {
	static uint_t colorInitTable[StdPalColor__Count] = {
		StdColor_AnsiBlack,         // StdColor_AnsiBlack,
		StdColor_AnsiRed,           // StdPalColor_AnsiRed,
		StdColor_AnsiGreen,         // StdPalColor_AnsiGreen,
		StdColor_AnsiYellow,        // StdPalColor_AnsiYellow,
		StdColor_AnsiBlue,          // StdPalColor_AnsiBlue,
		StdColor_AnsiMagenta,       // StdPalColor_AnsiMagenta,
		StdColor_AnsiCyan,          // StdPalColor_AnsiCyan,
		StdColor_AnsiWhite,         // StdPalColor_AnsiWhite,

		StdColor_AnsiBrightBlack,   // StdPalColor_AnsiBrightBlack,
		StdColor_AnsiBrightRed,     // StdPalColor_AnsiBrightRed,
		StdColor_AnsiBrightGreen,   // StdPalColor_AnsiBrightGreen,
		StdColor_AnsiBrightYellow,  // StdPalColor_AnsiBrightYellow,
		StdColor_AnsiBrightBlue,    // StdPalColor_AnsiBrightBlue,
		StdColor_AnsiBrightMagenta, // StdPalColor_AnsiBrightMagenta,
		StdColor_AnsiBrightCyan,    // StdPalColor_AnsiBrightCyan,
		StdColor_AnsiBrightWhite,   // StdPalColor_AnsiBrightWhite,

		0x000000,                   // StdPalColor_WidgetText,
		0xffffff,                   // StdPalColor_WidgetBack,
		0x808080,                   // StdPalColor_GrayText,
		0xffffff,                   // StdPalColor_SelectionText,
		0x4060e0,                   // StdPalColor_SelectionBack,
		0xc0c0c0,                   // StdPalColor_3DFace,
		0x808080,                   // StdPalColor_3DShadow,
		0x000000,                   // StdPalColor_3DDarkShadow,
		0xe0e0e0,                   // StdPalColor_3DLight,
		0xffffff,                   // StdPalColor_3DHiLight,

		StdColor_Black,             // StdPalColor_Black,
		StdColor_White,             // StdPalColor_White,
		StdColor_Gray,              // StdPalColor_Gray,
		StdColor_Red,               // StdPalColor_Red,
		StdColor_Green,             // StdPalColor_Green,
		StdColor_Blue,              // StdPalColor_Blue,
		StdColor_Purple,            // StdPalColor_Purple,
		StdColor_Cyan,              // StdPalColor_Cyan,

		StdColor_LightRed,          // StdPalColor_LightRed,
		StdColor_LightGreen,        // StdPalColor_LightGreen,
		StdColor_LightBlue,         // StdPalColor_LightBlue,
		StdColor_LightPurple,       // StdPalColor_LightPurple,
		StdColor_LightCyan,         // StdPalColor_LightCyan,

		StdColor_PastelGray,        // StdPalColor_PastelGray,
		StdColor_PastelRed,         // StdPalColor_PastelRed,
		StdColor_PastelOrange,      // StdPalColor_PastelOrange,
		StdColor_PastelYellow,      // StdPalColor_PastelYellow,
		StdColor_PastelGreen,       // StdPalColor_PastelGreen,
		StdColor_PastelCyan,        // StdPalColor_PastelCyan,
		StdColor_PastelBlue,        // StdPalColor_PastelBlue,
		StdColor_PastelPurple,      // StdPalColor_PastelPurple,
	};

	ASSERT(sizeof(colorInitTable) == sizeof(m_stdPalColorTable));
	memcpy(m_stdPalColorTable, colorInitTable, sizeof(m_stdPalColorTable));
};

//..............................................................................

} // namespace gui
} // namespace axl
