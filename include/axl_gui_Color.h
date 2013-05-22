// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
GetColorIndex (uint_t Color)
{ 
	ASSERT ((Color & EColorFlag_Index) && !(Color & EColorFlag_Transparent));
	
	return Color & EColorFlag_IndexMask;
}

//.............................................................................

enum EStdColor
{
	EStdColor_Black        = 0x000000,
	EStdColor_White        = 0xffffff,
	EStdColor_Gray         = 0x808080,
	EStdColor_Red          = 0x000080,
	EStdColor_Green        = 0x008000,
	EStdColor_Blue         = 0x800000,
	EStdColor_Purple       = 0x800080,
	EStdColor_Cyan         = 0x808000,

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

const uint_t*
GetStdPalColorArray ();

void
UpdateStdPalSystemColors (); // call this upon theme change

//.............................................................................

struct TPalette
{
	const uint_t* m_pColorArray;
	size_t m_Count;

	TPalette ()
	{
		m_pColorArray = GetStdPalColorArray ();
		m_Count = EStdPalColor__Count;
	}

	TPalette (
		const uint_t* pColorArray,
		size_t Count
		)
	{
		m_pColorArray = pColorArray;
		m_Count = Count;
	}

	uint_t
	GetColorRgb (uint_t Color)
	{
		if (Color & EColorFlag_Transparent)
			return EColorFlag_Transparent;
		
		if (Color & EColorFlag_Index)
		{
			size_t i = Color & EColorFlag_IndexMask;
			Color = i < m_Count ? m_pColorArray [i] : EColorFlag_Transparent;
			
			if (Color & EColorFlag_Index) // allow two-staged index lookup
			{
				i = Color & EColorFlag_IndexMask;
				Color = i < m_Count ? m_pColorArray [i] : EColorFlag_Transparent;
				ASSERT (!(Color & EColorFlag_Index)); // bad palette!
			}
		}

		return Color;
	}
};

//.............................................................................

inline
uint_t
OverlayColor (
	uint_t BaseColor,
	uint_t OverlayColor
	)
{
	return (OverlayColor & EColorFlag_Transparent) ? BaseColor : OverlayColor;
}

uint_t
ParseColorString (
	const char* pString,
	const char** ppEnd = NULL
	);

//.............................................................................

struct TColorAttr
{
	uint_t m_ForeColor;
	uint_t m_BackColor;

	TColorAttr ()
	{
		m_ForeColor = EColorFlag_Transparent;
		m_BackColor = EColorFlag_Transparent;
	}

	TColorAttr (
		uint_t ForeColor,
		uint_t BackColor = EColorFlag_Transparent
		)
	{
		Setup (ForeColor, BackColor);
	}

	TColorAttr (const char* pString)
	{
		Parse (pString);
	}

	int 
	Cmp (const TColorAttr& Attr)
	{
		return memcmp (this, &Attr, sizeof (TColorAttr));
	}

	void
	Clear ()
	{
		m_ForeColor = EColorFlag_Transparent;
		m_BackColor = EColorFlag_Transparent;
	}

	void
	Setup (
		uint_t ForeColor,
		uint_t BackColor
		)
	{
		m_ForeColor = ForeColor;
		m_BackColor = BackColor;
	}

	void
	Overlay (
		const TColorAttr& BaseAttr,
		const TColorAttr& OverlayAttr
		)
	{
		m_ForeColor = OverlayColor (BaseAttr.m_ForeColor, OverlayAttr.m_ForeColor);
		m_BackColor = OverlayColor (BaseAttr.m_BackColor, OverlayAttr.m_BackColor);
	}

	void
	Overlay (const TColorAttr& OverlayAttr)
	{
		Overlay (*this, OverlayAttr);
	}

	void
	Parse (
		const char* pString,
		const char** ppEnd = NULL
		);

	void
	ParseOverlay (
		const TColorAttr& BaseAttr,
		const char* pString,
		const char** ppEnd = NULL
		);

	void
	ParseOverlay (
		const char* pString,
		const char** ppEnd = NULL
		)
	{
		ParseOverlay (*this, pString, ppEnd);
	}
};

//.............................................................................


} // namespace gui
} // namespace axl

