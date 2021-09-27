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

#define _AXL_GUI_DEF_H

#include "axl_gui_Pch.h"

namespace axl {
namespace gui {

class Engine;

//..............................................................................

enum MouseButton {
	MouseButton_None   = 0x00,
	MouseButton_Left   = 0x01,
	MouseButton_Right  = 0x02,
	MouseButton_Medium = 0x04,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
MouseButton
getFirstMouseButton(uint_t buttons) {
	return (MouseButton)sl::getLoBitIdx32(buttons);
}

//..............................................................................

enum ModifierKey {
	ModifierKey_None   = 0x00,
	ModifierKey_Shift  = 0x01,
	ModifierKey_Ctrl   = 0x02,
	ModifierKey_Alt    = 0x04,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ModifierKey
getFirstModifierKey(uint_t modifierKeys) {
	return (ModifierKey)sl::getLoBitIdx32(modifierKeys);
}

//..............................................................................

enum Key {
	Key_Backspace   = 0x08,
	Key_Tab         = 0x09,
	Key_Enter       = 0x0d,
	Key_Esc         = 0x1b,

	Key_Pause       = 0x10000013,
	Key_Print       = 0x1000002a,

	Key_CapsLock    = 0x10000014,
	Key_NumLock     = 0x10000090,
	Key_ScrollLock  = 0x10000091,

	Key_Shift       = 0x10000010,
	Key_Ctrl        = 0x10000011,
	Key_Alt         = 0x10000012,

	Key_PageUp      = 0x10000021,
	Key_PageDown    = 0x10000022,
	Key_End         = 0x10000023,
	Key_Home        = 0x10000024,

	Key_Left        = 0x10000025,
	Key_Up          = 0x10000026,
	Key_Right       = 0x10000027,
	Key_Down        = 0x10000028,

	Key_Insert      = 0x1000002d,
	Key_Delete      = 0x1000002e,

	Key_F1          = 0x10000070,
	Key_F2          = 0x10000071,
	Key_F3          = 0x10000072,
	Key_F4          = 0x10000073,
	Key_F5          = 0x10000074,
	Key_F6          = 0x10000075,
	Key_F7          = 0x10000076,
	Key_F8          = 0x10000077,
	Key_F9          = 0x10000078,
	Key_F10         = 0x10000079,
	Key_F11         = 0x1000007a,
	Key_F12         = 0x1000007b,
};

//..............................................................................

struct Point {
	union {
		struct {
			//! \unnamed{union/struct:2}
			int m_x;
			int m_y;
		};

		struct {
			//! \unnamed{struct:2}
			uint_t m_x_u;
			uint_t m_y_u;
		};
	};

	Point() {
		m_x = 0;
		m_y = 0;
	}

	Point(
		int x,
		int y
	) {
		m_x = x;
		m_y = y;
	}

	Point
	operator + (const Point& point2) {
		return Point(m_x + point2.m_x, m_y + point2.m_y);
	}

	Point
	operator - (const Point& point2) {
		return Point(m_x - point2.m_x, m_y - point2.m_y);
	}

	void
	setup(
		int x,
		int y
	) {
		m_x = x;
		m_y = y;
	}

	void
	clipNegative() {
		if (m_x < 0)
			m_x = 0;

		if (m_y < 0)
			m_y = 0;
	}
};

//..............................................................................

struct Size {
	union {
		struct {
			//! \unnamed{union/struct:2}
			int m_width;
			int m_height;
		};

		struct {
			//! \unnamed{struct:2}
			uint_t m_width_u;
			uint_t m_height_u;
		};
	};

	Size() {
		m_width = 0;
		m_height = 0;
	}

	Size(
		int width,
		int height
	) {
		m_width = width;
		m_height = height;
	}

	Size
	operator + (const Size& size2) {
		return Size(m_width + size2.m_width, m_height + size2.m_height);
	}

	Size
	operator - (const Size& size2) {
		return Size(m_width - size2.m_width, m_height - size2.m_height);
	}

	void
	setup(
		int width,
		int height
	) {
		m_width = width;
		m_height = height;
	}

	void
	clipNegative() {
		if (m_width < 0)
			m_width = 0;

		if (m_height < 0)
			m_height = 0;
	}
};

//..............................................................................

struct Rect {
	union {
		struct {
			//! \unnamed{union/struct:4}
			int m_left;
			int m_top;
			int m_right;
			int m_bottom;
		};

		struct {
			//! \unnamed{struct:4}
			uint_t m_left_u;
			uint_t m_top_u;
			uint_t m_right_u;
			uint_t m_bottom_u;
		};
	};

	Rect() {
		m_left = 0;
		m_top = 0;
		m_right = 0;
		m_bottom = 0;
	}

	Rect(
		int left,
		int top,
		int right,
		int bottom
	) {
		m_left = left;
		m_top = top;
		m_right = right;
		m_bottom = bottom;
	}

	Rect(
		int left,
		int top
	) {
		m_left = left;
		m_top = top;
		m_right = left;
		m_bottom = top;
	}

	Rect(
		const Point& leftTop,
		const Point& rightBottom
	) {
		m_left = leftTop.m_x;
		m_top = leftTop.m_y;
		m_right = rightBottom.m_x;
		m_bottom = rightBottom.m_y;
	}

	Rect(const Point& leftTop) {
		m_left = leftTop.m_x;
		m_top = leftTop.m_y;
		m_right = leftTop.m_x;
		m_bottom = leftTop.m_y;
	}

	Rect(
		const Point& leftTop,
		const Size& size
	) {
		m_left = leftTop.m_x;
		m_top = leftTop.m_y;
		m_right = leftTop.m_x + size.m_width;
		m_bottom = leftTop.m_y + size.m_height;
	}

	bool
	isEmpty() const {
		return m_right == m_left || m_bottom == m_top;
	}

	int
	getWidth() const {
		return m_right - m_left;
	}

	int
	getHeight() const {
		return m_bottom - m_top;
	}

	Size
	getSize() const {
		return Size(getWidth(), getHeight());
	}

	void
	setup(
		int left,
		int top,
		int right,
		int bottom
	) {
		m_left = left;
		m_top = top;
		m_right = right;
		m_bottom = bottom;
	}

	void
	clipNegative() {
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

//..............................................................................

struct CaretPos {
	union {
		struct {
			//! \unnamed{union/struct:2}
			int m_line;
			int m_col;
		};

		struct {
			//! \unnamed{struct:2}
			uint_t m_line_u;
			uint_t m_col_u;
		};
	};

	CaretPos() {
		m_line = 0;
		m_col = 0;
	}

	CaretPos(
		int line,
		int col
	) {
		setup(line, col);
	}

	bool
	operator == (const CaretPos& pos2) const {
		return cmp(pos2) == 0;
	}

	bool
	operator != (const CaretPos& pos2) const {
		return cmp(pos2) != 0;
	}

	bool
	operator < (const CaretPos& pos2) const {
		return cmp(pos2) < 0;
	}

	bool
	operator <= (const CaretPos& pos2) const {
		return cmp(pos2) <= 0;
	}

	bool
	operator > (const CaretPos& pos2) const {
		return cmp(pos2) > 0;
	}

	bool
	operator >= (const CaretPos& pos2) const {
		return cmp(pos2) >= 0;
	}

	int
	cmp(const CaretPos& pos2) const {
		return
			m_line < pos2.m_line ? -1 :
			m_line > pos2.m_line ?  1 :
			m_col < pos2.m_col ? -1 :
			m_col > pos2.m_col ?  1 : 0;
	}

	int
	cmp_u(const CaretPos& pos2) const {
		return
			m_line_u < pos2.m_line_u ? -1 :
			m_line_u > pos2.m_line_u ?  1 :
			m_col_u < pos2.m_col_u ? -1 :
			m_col_u > pos2.m_col_u ?  1 : 0;
	}

	void
	setup(
		int line,
		int col
	) {
		m_line = line;
		m_col = col;
	}

	void
	clipNegative() {
		if (m_line < 0)
			m_line = 0;

		if (m_col < 0)
			m_col = 0;
	}
};

//..............................................................................

enum Orientation {
	Orientation_Vertical = 0,
	Orientation_Horizontal,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum FormFactor {
	FormFactor_Landscape,
	FormFactor_Portrait,
	FormFactor_LineStrip,
	FormFactor_ColumnStrip,
	FormFactor__Count,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
FormFactor
getFormFactor(
	int width,
	int height,
	int stripThreshold = 8
) {
	return
		height > width * stripThreshold ? FormFactor_LineStrip :
		width > height * stripThreshold ? FormFactor_ColumnStrip :
		height > width ? FormFactor_Portrait : FormFactor_Landscape;
}

inline
FormFactor
getFormFactor(
	const Size& size,
	int stripThreshold = 8
) {
	return getFormFactor(size.m_width, size.m_height, stripThreshold);
}

//..............................................................................

class GuiItem {
protected:
	Engine* m_engine;

public:
	GuiItem(Engine* engine) {
		m_engine = engine;
	}

	Engine*
	getEngine() {
		return m_engine;
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
