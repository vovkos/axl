// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
getFirstMouseButton (uint_t buttons)
{
	return (EMouseButton) rtl::getLoBitIdx32 (buttons);
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
getFirstModifierKey (uint_t modifierKeys)
{
	return (EModifierKey) rtl::getLoBitIdx32 (modifierKeys);
}

//.............................................................................

enum EKey
{
	EKey_Backspace   = 0x08,
	EKey_Tab         = 0x09,
	EKey_Enter       = 0x0d,	
	EKey_Esc         = 0x1b,

	EKey_Pause       = 0x10000013,	
	EKey_Print       = 0x1000002a,	

	EKey_CapsLock    = 0x10000014,
	EKey_NumLock     = 0x10000090,
	EKey_ScrollLock  = 0x10000091,

	EKey_Shift       = 0x10000010,
	EKey_Ctrl        = 0x10000011,
	EKey_Alt         = 0x10000012,

	EKey_PageUp      = 0x10000021,
	EKey_PageDown    = 0x10000022,
	EKey_End         = 0x10000023,
	EKey_Home        = 0x10000024,

	EKey_Left        = 0x10000025,
	EKey_Up          = 0x10000026,
	EKey_Right       = 0x10000027,
	EKey_Down        = 0x10000028,

	EKey_Insert      = 0x1000002d,
	EKey_Delete      = 0x1000002e,

	EKey_F1          = 0x10000070,
	EKey_F2          = 0x10000071,
	EKey_F3          = 0x10000072,
	EKey_F4          = 0x10000073,
	EKey_F5          = 0x10000074,
	EKey_F6          = 0x10000075,
	EKey_F7          = 0x10000076,
	EKey_F8          = 0x10000077,
	EKey_F9          = 0x10000078,
	EKey_F10         = 0x10000079,
	EKey_F11         = 0x1000007a,
	EKey_F12         = 0x1000007b,
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
	operator + (const TPoint& point2)
	{
		return TPoint (m_x + point2.m_x, m_y + point2.m_y);
	}

	TPoint 
	operator - (const TPoint& point2)
	{
		return TPoint (m_x - point2.m_x, m_y - point2.m_y);
	}

	void
	setup (
		int x,
		int y
		)
	{
		m_x = x;
		m_y = y;
	}

	void
	clipNegative ()
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
			int m_width;
			int m_height;
		};

		struct 
		{
			uint_t m_width_u;
			uint_t m_height_u;
		};
	};

	TSize ()
	{
		m_width = 0;
		m_height = 0;
	}

	TSize (
		int width,
		int height
		)
	{
		m_width = width;
		m_height = height;
	}

	TSize 
	operator + (const TSize& size2)
	{
		return TSize (m_width + size2.m_width, m_height + size2.m_height);
	}

	TSize 
	operator - (const TSize& size2)
	{
		return TSize (m_width - size2.m_width, m_height - size2.m_height);
	}

	void
	setup (
		int width,
		int height
		)
	{
		m_width = width;
		m_height = height;
	}

	void
	clipNegative ()
	{
		if (m_width < 0)
			m_width = 0;

		if (m_height < 0)
			m_height = 0;
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
getFormFactor (
	int width,
	int height, 
	int stripThreshold = 32
	)
{
	return 
		height > width * stripThreshold ? EFormFactor_LineStrip :
		width > height * stripThreshold ? EFormFactor_ColumnStrip :
		height > width ? EFormFactor_Portrait : EFormFactor_Landscape;
}

inline
EFormFactor
getFormFactor (
	const TSize& size, 
	int stripThreshold = 32
	)
{
	return getFormFactor (size.m_width, size.m_height, stripThreshold);
}

//.............................................................................

struct TRect
{
	union 
	{
		struct 
		{
			int m_left;
			int m_top;
			int m_right;
			int m_bottom;
		};

		struct 
		{
			uint_t m_left_u;
			uint_t m_top_u;
			uint_t m_right_u;
			uint_t m_bottom_u;
		};
	};

	TRect ()
	{
		m_left = 0;
		m_top = 0;
		m_right = 0;
		m_bottom = 0;
	}

	TRect (
		int left,
		int top,
		int right,
		int bottom
		)
	{
		m_left = left;
		m_top = top;
		m_right = right;
		m_bottom = bottom;
	}

	TRect (
		int left,
		int top
		)
	{
		m_left = left;
		m_top = top;
		m_right = left;
		m_bottom = top;
	}

	TRect (
		const TPoint& leftTop,
		const TPoint& rightBottom
		)
	{
		m_left = leftTop.m_x;
		m_top = leftTop.m_y;
		m_right = rightBottom.m_x;
		m_bottom = rightBottom.m_y;
	}

	TRect (const TPoint& leftTop)
	{
		m_left = leftTop.m_x;
		m_top = leftTop.m_y;
		m_right = leftTop.m_x;
		m_bottom = leftTop.m_y;
	}

	TRect (
		const TPoint& leftTop,
		const TSize& size
		)
	{
		m_left = leftTop.m_x;
		m_top = leftTop.m_y;
		m_right = leftTop.m_x + size.m_width;
		m_bottom = leftTop.m_y + size.m_height;
	}

	bool
	isEmpty () const
	{
		return m_right == m_left || m_bottom == m_top;
	}

	int
	getWidth () const
	{
		return m_right - m_left;
	}

	int
	getHeight () const
	{
		return m_bottom - m_top;
	}

	TSize
	getSize () const
	{
		return TSize (getWidth (), getHeight ());
	}

	void
	setup (
		int left,
		int top,
		int right,
		int bottom
		)
	{
		m_left = left;
		m_top = top;
		m_right = right;
		m_bottom = bottom;
	}

	void
	clipNegative ()
	{
		if (m_left < 0)
			m_left = 0;

		if (m_top < 0)
			m_top = 0;

		if (m_right < 0)
			m_right = 0;

		if (m_bottom < 0)
			m_bottom = 0;
	}
};

//.............................................................................

struct TCursorPos
{
	union
	{
		struct
		{
			int m_line;
			int m_col;
		};

		struct
		{
			uint_t m_line_u;
			uint_t m_col_u;
		};
	};

	TCursorPos ()
	{
		m_line = 0;
		m_col = 0;
	}

	TCursorPos (
		int line,
		int col
		)
	{
		setup (line, col);
	}

	bool
	operator == (const TCursorPos& pos2) const
	{
		return cmp (pos2) == 0;
	}

	bool
	operator != (const TCursorPos& pos2) const
	{
		return cmp (pos2) != 0;
	}

	bool
	operator < (const TCursorPos& pos2) const
	{
		return cmp (pos2) < 0;
	}

	bool
	operator <= (const TCursorPos& pos2) const
	{
		return cmp (pos2) <= 0;
	}

	bool
	operator > (const TCursorPos& pos2) const
	{
		return cmp (pos2) > 0;
	}

	bool
	operator >= (const TCursorPos& pos2) const
	{
		return cmp (pos2) >= 0;
	}

	int
	cmp (const TCursorPos& pos2) const
	{
		return 
			m_line < pos2.m_line ? -1 : 
			m_line > pos2.m_line ?  1 : 
			m_col < pos2.m_col ? -1 : 
			m_col > pos2.m_col ?  1 : 0;
	}

	int
	cmp_u (const TCursorPos& pos2) const
	{
		return 
			m_line_u < pos2.m_line_u ? -1 : 
			m_line_u > pos2.m_line_u ?  1 : 
			m_col_u < pos2.m_col_u ? -1 : 
			m_col_u > pos2.m_col_u ?  1 : 0;
	}

	void
	setup (
		int line,
		int col
		)
	{
		m_line = line;
		m_col = col;
	}

	void
	clipNegative ()
	{
		if (m_line < 0)
			m_line = 0;

		if (m_col < 0)
			m_col = 0;
	}
};

//.............................................................................

class CGuiItem
{
protected:
	CEngine* m_engine;

public:
	CGuiItem ()
	{
		m_engine = NULL;
	}

	CEngine* 
	getEngine ()
	{
		return m_engine;
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

