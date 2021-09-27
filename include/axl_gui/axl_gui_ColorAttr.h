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

#define _AXL_GUI_COLORATTR_H

#include "axl_gui_Color.h"

namespace axl {
namespace gui {

//..............................................................................

struct ColorAttr {
	uint_t m_foreColor;
	uint_t m_backColor;

	ColorAttr() {
		setup(-1, -1);
	}

	ColorAttr(
		uint_t foreColor,
		uint_t backColor = -1
	) {
		setup(foreColor, backColor);
	}

	int
	cmp(const ColorAttr& attr) {
		return memcmp(this, &attr, sizeof(ColorAttr));
	}

	void
	clear() {
		setup(-1, -1);
	}

	void
	setup(
		uint_t foreColor,
		uint_t backColor = -1
	) {
		m_foreColor = foreColor;
		m_backColor = backColor;
	}

	void
	overlay(
		const ColorAttr& baseAttr,
		const ColorAttr& overlayAttr
	) {
		m_foreColor = overlayColor(baseAttr.m_foreColor, overlayAttr.m_foreColor);
		m_backColor = overlayColor(baseAttr.m_backColor, overlayAttr.m_backColor);
	}

	void
	overlay(const ColorAttr& overlayAttr) {
		overlay(*this, overlayAttr);
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
