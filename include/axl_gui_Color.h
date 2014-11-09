// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_COLOR_H

#include "axl_g_Def.h"

namespace axl {
namespace gui {
	
//.............................................................................

enum
{
	ColorFlag_Undefined   = 0x80000000,
	ColorFlag_Index       = 0x40000000,
	ColorFlag_IndexMask   = 0x3fffffff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum StdColor
{
	StdColor_Undefined    = ColorFlag_Undefined,

	StdColor_Black        = 0x000000,
	StdColor_White        = 0xffffff,
	StdColor_Gray         = 0x808080,
	StdColor_Red          = 0x800000,
	StdColor_Green        = 0x008000,
	StdColor_Blue         = 0x000080,
	StdColor_Purple       = 0x800080,
	StdColor_Cyan         = 0x008080,

	StdColor_LightRed     = 0xff0000,
	StdColor_LightGreen   = 0x00ff00,
	StdColor_LightBlue    = 0x0000ff,
	StdColor_LightPurple  = 0xff00ff,
	StdColor_LightCyan    = 0x00ffff,

	StdColor_PastelGray   = 0xefefef,
	StdColor_PastelRed    = 0xffe4e1,
	StdColor_PastelOrange = 0xfffacd,
	StdColor_PastelYellow = 0xffffc0,
	StdColor_PastelGreen  = 0xe1fada,
	StdColor_PastelCyan   = 0xcdfaff,
	StdColor_PastelBlue   = 0xe2edff,
	StdColor_PastelPurple = 0xe2dafd,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum StdPalColor
{
	// system widget colors

	StdPalColor_WidgetText = ColorFlag_Index | 0,
	StdPalColor_WidgetBack,     // 1
	StdPalColor_GrayText,       // 2
	StdPalColor_SelectionText,  // 3
	StdPalColor_SelectionBack,  // 4
	StdPalColor_3DFace,         // 5
	StdPalColor_3DShadow,       // 6
	StdPalColor_3DDarkShadow,   // 7
	StdPalColor_3DLight,        // 8
	StdPalColor_3DHiLight,      // 9

	// std colors

	StdPalColor_Black,          // 10
	StdPalColor_White,          // 11
	StdPalColor_Gray,           // 12
	StdPalColor_Red,            // 13
	StdPalColor_Green,          // 14
	StdPalColor_Blue,           // 15
	StdPalColor_Purple,         // 16
	StdPalColor_Cyan,           // 17

	StdPalColor_LightRed,       // 18
	StdPalColor_LightGreen,     // 19
	StdPalColor_LightBlue,      // 20
	StdPalColor_LightPurple,    // 21
	StdPalColor_LightCyan,      // 22
	
	StdPalColor_PastelGray,     // 23
	StdPalColor_PastelRed,      // 24
	StdPalColor_PastelOrange,   // 25
	StdPalColor_PastelYellow,   // 26
	StdPalColor_PastelGreen,    // 27
	StdPalColor_PastelCyan,     // 28
	StdPalColor_PastelBlue,     // 29
	StdPalColor_PastelPurple,   // 30

	StdPalColor__SystemColorCount = ~ColorFlag_Index & (StdPalColor_3DHiLight + 1),
	StdPalColor__Count            = ~ColorFlag_Index & (StdPalColor_PastelPurple + 1)
};

//.............................................................................

inline 
uint_t 
getColorIndex (uint_t color)
{ 
	return (color & ColorFlag_Index) ? color & ColorFlag_IndexMask : -1;
}

inline
uint_t
overlayColor (
	uint_t baseColor,
	uint_t overlayColor
	)
{
	return (overlayColor & ColorFlag_Undefined) ? baseColor : overlayColor;
}

uint_t
parseColorString (
	const char* string,
	const char** end = NULL
	);

inline
uint_t 
inverseRgb (uint_t rgb)
{
	return ((rgb & 0x0000ff) << 16) | ((rgb & 0xff0000) >> 16) | rgb & 0x00ff00;
}

const uint_t*
getStdPalColorArray ();

void
updateStdPalSystemColors (); // call this upon theme change

//.............................................................................

struct Palette
{
	const uint_t* m_colorArray;
	size_t m_count;

	Palette ()
	{
		setup (getStdPalColorArray (), StdPalColor__Count);
	}

	Palette (
		const uint_t* colorArray,
		size_t count
		)
	{
		m_colorArray = colorArray;
		m_count = count;
	}

	uint_t
	getColorRgb (uint_t color);

	void
	setup (
		const uint_t* colorArray,
		size_t count
		)
	{
		m_colorArray = colorArray;
		m_count = count;
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

