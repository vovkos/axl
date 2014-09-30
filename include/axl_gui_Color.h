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
	ColorFlagKind_Transparent = 0x80000000,
	ColorFlagKind_Index       = 0x40000000,
	ColorFlagKind_IndexMask   = 0x0fffffff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline 
uint_t 
getColorIndex (uint_t color)
{ 
	ASSERT ((color & ColorFlagKind_Index) && !(color & ColorFlagKind_Transparent));
	
	return color & ColorFlagKind_IndexMask;
}

//.............................................................................

enum StdColorKind
{
	StdColorKind_Black        = 0x000000,
	StdColorKind_White        = 0xffffff,
	StdColorKind_Gray         = 0x808080,
	StdColorKind_Red          = 0x800000,
	StdColorKind_Green        = 0x008000,
	StdColorKind_Blue         = 0x000080,
	StdColorKind_Purple       = 0x800080,
	StdColorKind_Cyan         = 0x008080,

	StdColorKind_LightRed     = 0xff0000,
	StdColorKind_LightGreen   = 0x00ff00,
	StdColorKind_LightBlue    = 0x0000ff,
	StdColorKind_LightPurple  = 0xff00ff,
	StdColorKind_LightCyan    = 0x00ffff,

	StdColorKind_PastelGray   = 0xefefef,
	StdColorKind_PastelRed    = 0xffe4e1,
	StdColorKind_PastelOrange = 0xfffacd,
	StdColorKind_PastelYellow = 0xffffc0,
	StdColorKind_PastelGreen  = 0xe1fada,
	StdColorKind_PastelCyan   = 0xcdfaff,
	StdColorKind_PastelBlue   = 0xe2edff,
	StdColorKind_PastelPurple = 0xe2dafd,
};

//.............................................................................

enum StdPalColorKind
{
	// system widget colors

	StdPalColorKind_WidgetText = ColorFlagKind_Index | 0,
	StdPalColorKind_WidgetBack,     // 1
	StdPalColorKind_GrayText,       // 2
	StdPalColorKind_SelectionText,  // 3
	StdPalColorKind_SelectionBack,  // 4
	StdPalColorKind_3DFace,         // 5
	StdPalColorKind_3DShadow,       // 6
	StdPalColorKind_3DDarkShadow,   // 7
	StdPalColorKind_3DLight,        // 8
	StdPalColorKind_3DHiLight,      // 9

	// std colors

	StdPalColorKind_Black,          // 10
	StdPalColorKind_White,          // 11
	StdPalColorKind_Gray,           // 12
	StdPalColorKind_Red,            // 13
	StdPalColorKind_Green,          // 14
	StdPalColorKind_Blue,           // 15
	StdPalColorKind_Purple,         // 16
	StdPalColorKind_Cyan,           // 17

	StdPalColorKind_LightRed,       // 18
	StdPalColorKind_LightGreen,     // 19
	StdPalColorKind_LightBlue,      // 20
	StdPalColorKind_LightPurple,    // 21
	StdPalColorKind_LightCyan,      // 22
	
	StdPalColorKind_PastelGray,     // 23
	StdPalColorKind_PastelRed,      // 24
	StdPalColorKind_PastelOrange,   // 25
	StdPalColorKind_PastelYellow,   // 26
	StdPalColorKind_PastelGreen,    // 27
	StdPalColorKind_PastelCyan,     // 28
	StdPalColorKind_PastelBlue,     // 29
	StdPalColorKind_PastelPurple,   // 30

	StdPalColorKind__SystemColorCount = ~ColorFlagKind_Index & (StdPalColorKind_3DHiLight + 1),
	StdPalColorKind__Count            = ~ColorFlagKind_Index & (StdPalColorKind_PastelPurple + 1)
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
		m_count = StdPalColorKind__Count;
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
		ASSERT (!(color & ColorFlagKind_Transparent));
		
		if (!(color & ColorFlagKind_Index))
			return color;

		size_t i = color & ColorFlagKind_IndexMask;
		color = i < m_count ? m_colorArray [i] : ColorFlagKind_Transparent;
			
		if (color & ColorFlagKind_Index) // allow two-staged index lookup
		{
			i = color & ColorFlagKind_IndexMask;
			color = i < m_count ? m_colorArray [i] : ColorFlagKind_Transparent;
			ASSERT (!(color & ColorFlagKind_Index)); // bad palette!
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
	return (overlayColor & ColorFlagKind_Transparent) ? baseColor : overlayColor;
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
		m_foreColor = ColorFlagKind_Transparent;
		m_backColor = ColorFlagKind_Transparent;
	}

	ColorAttr (
		uint_t foreColor,
		uint_t backColor = ColorFlagKind_Transparent
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
		m_foreColor = ColorFlagKind_Transparent;
		m_backColor = ColorFlagKind_Transparent;
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

