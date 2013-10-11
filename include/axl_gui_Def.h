// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_DEF_H

#include "axl_rtl_BitIdx.h"

namespace axl {
namespace gui {

class CEngine;

//.............................................................................

enum EMouseButton
{
	EMouseButton_None   = 0x00,
	EMouseButton_Left   = 0x01,
	EMouseButton_Right  = 0x02,
	EMouseButton_Medium = 0x04,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EMouseButton
GetFirstMouseButton (uint_t Buttons)
{
	return (EMouseButton) rtl::GetLoBitIdx32 (Buttons);
}

//.............................................................................

enum EModifierKey
{
	EModifierKey_None   = 0x00,
	EModifierKey_Shift  = 0x01,
	EModifierKey_Ctrl   = 0x02,
	EModifierKey_Alt    = 0x04,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EModifierKey
GetFirstModifierKey (uint_t ModifierKeys)
{
	return (EModifierKey) rtl::GetLoBitIdx32 (ModifierKeys);
}

//.............................................................................

enum EKey
{
	EKey_Backspace   = 0x08,
	EKey_Tab         = 0x09,
	EKey_Enter       = 0x0d,	
	EKey_Esc         = 0x1b,

	EKey_Pause       = 0x13,	
	EKey_Print       = 0x2a,	

	EKey_CapsLock    = 0x14,
	EKey_NumLock     = 0x90,
	EKey_ScrollLock  = 0x91,

	EKey_Shift       = 0x10,
	EKey_Ctrl        = 0x11,
	EKey_Alt         = 0x12,

	EKey_PageUp      = 0x21,
	EKey_PageDown    = 0x22,
	EKey_End         = 0x23,
	EKey_Home        = 0x24,

	EKey_Left        = 0x25,
	EKey_Up          = 0x26,
	EKey_Right       = 0x27,
	EKey_Down        = 0x28,

	EKey_Insert      = 0x2d,
	EKey_Delete      = 0x2e,

	EKey_F1          = 0x70,
	EKey_F2          = 0x71,
	EKey_F3          = 0x72,
	EKey_F4          = 0x73,
	EKey_F5          = 0x74,
	EKey_F6          = 0x75,
	EKey_F7          = 0x76,
	EKey_F8          = 0x77,
	EKey_F9          = 0x78,
	EKey_F10         = 0x79,
	EKey_F11         = 0x7a,
	EKey_F12         = 0x7b,
};

//.............................................................................

struct TPoint
{
	union
	{
		struct
		{
			int m_x;
			int m_y;
		};

		struct
		{
			uint_t m_x_u;
			uint_t m_y_u;
		};
	};

	TPoint ()
	{
		m_x = 0;
		m_y = 0;
	}

	TPoint (
		int x,
		int y
		)
	{
		m_x = x;
		m_y = y;
	}

	TPoint 
	operator + (const TPoint& Point2)
	{
		return TPoint (m_x + Point2.m_x, m_y + Point2.m_y);
	}

	TPoint 
	operator - (const TPoint& Point2)
	{
		return TPoint (m_x - Point2.m_x, m_y - Point2.m_y);
	}

	void
	Setup (
		int x,
		int y
		)
	{
		m_x = x;
		m_y = y;
	}

	void
	ClipNegative ()
	{
		if (m_x < 0)
			m_x = 0;

		if (m_x < 0)
			m_x = 0;
	}
};

//.............................................................................

struct TSize
{
	union
	{
		struct 
		{
			int m_Width;
			int m_Height;
		};

		struct 
		{
			uint_t m_Width_u;
			uint_t m_Height_u;
		};
	};

	TSize ()
	{
		m_Width = 0;
		m_Height = 0;
	}

	TSize (
		int Width,
		int Height
		)
	{
		m_Width = Width;
		m_Height = Height;
	}

	TSize 
	operator + (const TSize& Size2)
	{
		return TSize (m_Width + Size2.m_Width, m_Height + Size2.m_Height);
	}

	TSize 
	operator - (const TSize& Size2)
	{
		return TSize (m_Width - Size2.m_Width, m_Height - Size2.m_Height);
	}

	void
	Setup (
		int Width,
		int Height
		)
	{
		m_Width = Width;
		m_Height = Height;
	}

	void
	ClipNegative ()
	{
		if (m_Width < 0)
			m_Width = 0;

		if (m_Height < 0)
			m_Height = 0;
	}
};

//.............................................................................

enum EFormFactor
{
	EFormFactor_Landscape,
	EFormFactor_Portrait,
	EFormFactor_LineStrip,
	EFormFactor_ColumnStrip,
	EFormFactor__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EFormFactor
GetFormFactor (
	int Width,
	int Height, 
	int StripThreshold = 32
	)
{
	return 
		Height > Width * StripThreshold ? EFormFactor_LineStrip :
		Width > Height * StripThreshold ? EFormFactor_ColumnStrip :
		Height > Width ? EFormFactor_Portrait : EFormFactor_Landscape;
}

inline
EFormFactor
GetFormFactor (
	const TSize& Size, 
	int StripThreshold = 32
	)
{
	return GetFormFactor (Size.m_Width, Size.m_Height, StripThreshold);
}

//.............................................................................

struct TRect
{
	union 
	{
		struct 
		{
			int m_Left;
			int m_Top;
			int m_Right;
			int m_Bottom;
		};

		struct 
		{
			uint_t m_Left_u;
			uint_t m_Top_u;
			uint_t m_Right_u;
			uint_t m_Bottom_u;
		};
	};

	TRect ()
	{
		m_Left = 0;
		m_Top = 0;
		m_Right = 0;
		m_Bottom = 0;
	}

	TRect (
		int Left,
		int Top,
		int Right,
		int Bottom
		)
	{
		m_Left = Left;
		m_Top = Top;
		m_Right = Right;
		m_Bottom = Bottom;
	}

	TRect (
		int Left,
		int Top
		)
	{
		m_Left = Left;
		m_Top = Top;
		m_Right = Left;
		m_Bottom = Top;
	}

	TRect (
		const TPoint& LeftTop,
		const TPoint& RightBottom
		)
	{
		m_Left = LeftTop.m_x;
		m_Top = LeftTop.m_y;
		m_Right = RightBottom.m_x;
		m_Bottom = RightBottom.m_y;
	}

	TRect (const TPoint& LeftTop)
	{
		m_Left = LeftTop.m_x;
		m_Top = LeftTop.m_y;
		m_Right = LeftTop.m_x;
		m_Bottom = LeftTop.m_y;
	}

	TRect (
		const TPoint& LeftTop,
		const TSize& Size
		)
	{
		m_Left = LeftTop.m_x;
		m_Top = LeftTop.m_y;
		m_Right = LeftTop.m_x + Size.m_Width;
		m_Bottom = LeftTop.m_y + Size.m_Height;
	}

	bool
	IsEmpty () const
	{
		return m_Right == m_Left || m_Bottom == m_Top;
	}

	int
	GetWidth () const
	{
		return m_Right - m_Left;
	}

	int
	GetHeight () const
	{
		return m_Bottom - m_Top;
	}

	TSize
	GetSize () const
	{
		return TSize (GetWidth (), GetHeight ());
	}

	void
	Setup (
		int Left,
		int Top,
		int Right,
		int Bottom
		)
	{
		m_Left = Left;
		m_Top = Top;
		m_Right = Right;
		m_Bottom = Bottom;
	}

	void
	ClipNegative ()
	{
		if (m_Left < 0)
			m_Left = 0;

		if (m_Top < 0)
			m_Top = 0;

		if (m_Right < 0)
			m_Right = 0;

		if (m_Bottom < 0)
			m_Bottom = 0;
	}
};

//.............................................................................

struct TCursorPos
{
	union
	{
		struct
		{
			int m_Line;
			int m_Col;
		};

		struct
		{
			uint_t m_Line_u;
			uint_t m_Col_u;
		};
	};

	TCursorPos ()
	{
		m_Line = 0;
		m_Col = 0;
	}

	TCursorPos (
		int Line,
		int Col
		)
	{
		Setup (Line, Col);
	}

	bool
	operator == (const TCursorPos& Pos2) const
	{
		return Cmp (Pos2) == 0;
	}

	bool
	operator != (const TCursorPos& Pos2) const
	{
		return Cmp (Pos2) != 0;
	}

	bool
	operator < (const TCursorPos& Pos2) const
	{
		return Cmp (Pos2) < 0;
	}

	bool
	operator <= (const TCursorPos& Pos2) const
	{
		return Cmp (Pos2) <= 0;
	}

	bool
	operator > (const TCursorPos& Pos2) const
	{
		return Cmp (Pos2) > 0;
	}

	bool
	operator >= (const TCursorPos& Pos2) const
	{
		return Cmp (Pos2) >= 0;
	}

	int
	Cmp (const TCursorPos& Pos2) const
	{
		return 
			m_Line < Pos2.m_Line ? -1 : 
			m_Line > Pos2.m_Line ?  1 : 
			m_Col < Pos2.m_Col ? -1 : 
			m_Col > Pos2.m_Col ?  1 : 0;
	}

	int
	Cmp_u (const TCursorPos& Pos2) const
	{
		return 
			m_Line_u < Pos2.m_Line_u ? -1 : 
			m_Line_u > Pos2.m_Line_u ?  1 : 
			m_Col_u < Pos2.m_Col_u ? -1 : 
			m_Col_u > Pos2.m_Col_u ?  1 : 0;
	}

	void
	Setup (
		int Line,
		int Col
		)
	{
		m_Line = Line;
		m_Col = Col;
	}

	void
	ClipNegative ()
	{
		if (m_Line < 0)
			m_Line = 0;

		if (m_Col < 0)
			m_Col = 0;
	}
};

//.............................................................................

class CGuiItem
{
protected:
	CEngine* m_pEngine;

public:
	CGuiItem ()
	{
		m_pEngine = NULL;
	}

	CEngine* 
	GetEngine ()
	{
		return m_pEngine;
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

