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

	bool
	operator == (const ColorAttr& attr) const {
		return isEqual(attr);
	}

	bool
	operator != (const ColorAttr& attr) const {
		return !isEqual(attr);
	}

	bool
	isEqual(const ColorAttr& attr) const {
		return m_foreColor == attr.m_foreColor && m_backColor == attr.m_backColor;
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
	overlay(const ColorAttr& overlayAttr) {
		*this = getOverlayAttr(*this, overlayAttr);
	}

	void
	overlay(
		const ColorAttr& baseAttr,
		const ColorAttr& overlayAttr
	) {
		*this = getOverlayAttr(baseAttr, overlayAttr);
	}

	static
	ColorAttr
	getOverlayAttr(
		const ColorAttr& baseAttr,
		const ColorAttr& overlayAttr
	) {
		return ColorAttr(
			getOverlayColor(baseAttr.m_foreColor, overlayAttr.m_foreColor),
			getOverlayColor(baseAttr.m_backColor, overlayAttr.m_backColor)
		);
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
