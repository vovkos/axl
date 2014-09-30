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
	EColorFlag_Transparent = 0x80000000,
	EColorFlag_Index       = 0x40000000,
	EColorFlag_IndexMask   = 0x0fffffff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline 
uint_t 
getColorIndex (uint_t color)
{ 
	ASSERT ((color & EColorFlag_Index) && !(color & EColorFlag_Transparent));
	
	return color & EColorFlag_IndexMask;
}

//.............................................................................

enum EStdColor
{
	EStdColor_Black        = 0x000000,
	EStdColor_White        = 0xffffff,
	EStdColor_Gray         = 0x808080,
	EStdColor_Red          = 0x800000,
	EStdColor_Green        = 0x008000,
	EStdColor_Blue         = 0x000080,
	EStdColor_Purple       = 0x800080,
	EStdColor_Cyan         = 0x008080,

	EStdColor_LightRed     = 0xff0000,
	EStdColor_LightGreen   = 0x00ff00,
	EStdColor_LightBlue    = 0x0000ff,
	EStdColor_LightPurple  = 0xff00ff,
	EStdColor_LightCyan    = 0x00ffff,

	EStdColor_PastelGray   = 0xefefef,
	EStdColor_PastelRed    = 0xffe4e1,
	EStdColor_PastelOrange = 0xfffacd,
	EStdColor_PastelYellow = 0xffffc0,
	EStdColor_PastelGreen  = 0xe1fada,
	EStdColor_PastelCyan   = 0xcdfaff,
	EStdColor_PastelBlue   = 0xe2edff,
	EStdColor_PastelPurple = 0xe2dafd,
};

//.............................................................................

enum EStdPalColor
{
	// system widget colors

	EStdPalColor_WidgetText = EColorFlag_Index | 0,
	EStdPalColor_WidgetBack,     // 1
	EStdPalColor_GrayText,       // 2
	EStdPalColor_SelectionText,  // 3
	EStdPalColor_SelectionBack,  // 4
	EStdPalColor_3DFace,         // 5
	EStdPalColor_3DShadow,       // 6
	EStdPalColor_3DDarkShadow,   // 7
	EStdPalColor_3DLight,        // 8
	EStdPalColor_3DHiLight,      // 9

	// std colors

	EStdPalColor_Black,          // 10
	EStdPalColor_White,          // 11
	EStdPalColor_Gray,           // 12
	EStdPalColor_Red,            // 13
	EStdPalColor_Green,          // 14
	EStdPalColor_Blue,           // 15
	EStdPalColor_Purple,         // 16
	EStdPalColor_Cyan,           // 17

	EStdPalColor_LightRed,       // 18
	EStdPalColor_LightGreen,     // 19
	EStdPalColor_LightBlue,      // 20
	EStdPalColor_LightPurple,    // 21
	EStdPalColor_LightCyan,      // 22
	
	EStdPalColor_PastelGray,     // 23
	EStdPalColor_PastelRed,      // 24
	EStdPalColor_PastelOrange,   // 25
	EStdPalColor_PastelYellow,   // 26
	EStdPalColor_PastelGreen,    // 27
	EStdPalColor_PastelCyan,     // 28
	EStdPalColor_PastelBlue,     // 29
	EStdPalColor_PastelPurple,   // 30

	EStdPalColor__SystemColorCount = ~EColorFlag_Index & (EStdPalColor_3DHiLight + 1),
	EStdPalColor__Count            = ~EColorFlag_Index & (EStdPalColor_PastelPurple + 1)
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

struct TPalette
{
	const uint_t* m_colorArray;
	size_t m_count;

	TPalette ()
	{
		m_colorArray = getStdPalColorArray ();
		m_count = EStdPalColor__Count;
	}

	TPalette (
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
		ASSERT (!(color & EColorFlag_Transparent));
		
		if (!(color & EColorFlag_Index))
			return color;

		size_t i = color & EColorFlag_IndexMask;
		color = i < m_count ? m_colorArray [i] : EColorFlag_Transparent;
			
		if (color & EColorFlag_Index) // allow two-staged index lookup
		{
			i = color & EColorFlag_IndexMask;
			color = i < m_count ? m_colorArray [i] : EColorFlag_Transparent;
			ASSERT (!(color & EColorFlag_Index)); // bad palette!
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
	return (overlayColor & EColorFlag_Transparent) ? baseColor : overlayColor;
}

uint_t
parseColorString (
	const char* string,
	const char** end = NULL
	);

//.............................................................................

struct TColorAttr
{
	uint_t m_foreColor;
	uint_t m_backColor;

	TColorAttr ()
	{
		m_foreColor = EColorFlag_Transparent;
		m_backColor = EColorFlag_Transparent;
	}

	TColorAttr (
		uint_t foreColor,
		uint_t backColor = EColorFlag_Transparent
		)
	{
		setup (foreColor, backColor);
	}

	TColorAttr (const char* string)
	{
		parse (string);
	}

	int 
	cmp (const TColorAttr& attr)
	{
		return memcmp (this, &attr, sizeof (TColorAttr));
	}

	void
	clear ()
	{
		m_foreColor = EColorFlag_Transparent;
		m_backColor = EColorFlag_Transparent;
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
		const TColorAttr& baseAttr,
		const TColorAttr& overlayAttr
		)
	{
		m_foreColor = overlayColor (baseAttr.m_foreColor, overlayAttr.m_foreColor);
		m_backColor = overlayColor (baseAttr.m_backColor, overlayAttr.m_backColor);
	}

	void
	overlay (const TColorAttr& overlayAttr)
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
		const TColorAttr& baseAttr,
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

