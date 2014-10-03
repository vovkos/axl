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
	ColorFlag_Transparent = 0x80000000,
	ColorFlag_Index       = 0x40000000,
	ColorFlag_IndexMask   = 0x0fffffff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline 
uint_t 
getColorIndex (uint_t color)
{ 
	ASSERT ((color & ColorFlag_Index) && !(color & ColorFlag_Transparent));
	
	return color & ColorFlag_IndexMask;
}

//.............................................................................

enum StdColor
{
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

//.............................................................................

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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		m_colorArray = getStdPalColorArray ();
		m_count = StdPalColor__Count;
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
	getColorRgb (uint_t color)
	{
		ASSERT (!(color & ColorFlag_Transparent));
		
		if (!(color & ColorFlag_Index))
			return color;

		size_t i = color & ColorFlag_IndexMask;
		color = i < m_count ? m_colorArray [i] : ColorFlag_Transparent;
			
		if (color & ColorFlag_Index) // allow two-staged index lookup
		{
			i = color & ColorFlag_IndexMask;
			color = i < m_count ? m_colorArray [i] : ColorFlag_Transparent;
			ASSERT (!(color & ColorFlag_Index)); // bad palette!
		}

		return color;
	}
};

//.............................................................................

inline
uint_t
overlayColor (
	uint_t baseColor,
	uint_t overlayColor
	)
{
	return (overlayColor & ColorFlag_Transparent) ? baseColor : overlayColor;
}

uint_t
parseColorString (
	const char* string,
	const char** end = NULL
	);

//.............................................................................

struct ColorAttr
{
	uint_t m_foreColor;
	uint_t m_backColor;

	ColorAttr ()
	{
		m_foreColor = ColorFlag_Transparent;
		m_backColor = ColorFlag_Transparent;
	}

	ColorAttr (
		uint_t foreColor,
		uint_t backColor = ColorFlag_Transparent
		)
	{
		setup (foreColor, backColor);
	}

	ColorAttr (const char* string)
	{
		parse (string);
	}

	int 
	cmp (const ColorAttr& attr)
	{
		return memcmp (this, &attr, sizeof (ColorAttr));
	}

	void
	clear ()
	{
		m_foreColor = ColorFlag_Transparent;
		m_backColor = ColorFlag_Transparent;
	}

	void
	setup (
		uint_t foreColor,
		uint_t backColor
		)
	{
		m_foreColor = foreColor;
		m_backColor = backColor;
	}

	void
	overlay (
		const ColorAttr& baseAttr,
		const ColorAttr& overlayAttr
		)
	{
		m_foreColor = overlayColor (baseAttr.m_foreColor, overlayAttr.m_foreColor);
		m_backColor = overlayColor (baseAttr.m_backColor, overlayAttr.m_backColor);
	}

	void
	overlay (const ColorAttr& overlayAttr)
	{
		overlay (*this, overlayAttr);
	}

	void
	parse (
		const char* string,
		const char** end = NULL
		);

	void
	parseOverlay (
		const ColorAttr& baseAttr,
		const char* string,
		const char** end = NULL
		);

	void
	parseOverlay (
		const char* string,
		const char** end = NULL
		)
	{
		parseOverlay (*this, string, end);
	}
};

//.............................................................................


} // namespace gui
} // namespace axl

