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

#define _AXL_GUI_TEXTATTR_H

#include "axl_gui_ColorAttr.h"
#include "axl_gui_FontDesc.h"

namespace axl {
namespace gui {

//..............................................................................

struct TextAttr: public ColorAttr {
	uint_t m_fontFlags;

	TextAttr() {
		m_fontFlags = -1;
	}

	TextAttr(
		uint_t foreColor,
		uint_t backColor = -1,
		uint_t fontFlags = -1
	) {
		setup(foreColor, backColor, fontFlags);
	}

	TextAttr(
		const ColorAttr& colorAttr,
		uint_t fontFlags = -1
	) {
		setup(colorAttr, fontFlags);
	}

	bool
	operator == (const TextAttr& attr) const {
		return isEqual(attr);
	}

	bool
	operator != (const TextAttr& attr) const {
		return !isEqual(attr);
	}

	bool
	isEqual(const TextAttr& attr) const {
		return ColorAttr::isEqual(attr) && m_fontFlags == attr.m_fontFlags;
	}

	void
	clear() {
		ColorAttr::clear();
		m_fontFlags = 0;
	}

	void
	setup(
		uint_t foreColor,
		uint_t backColor,
		uint_t fontFlags
	) {
		m_foreColor = foreColor;
		m_backColor = backColor;
		m_fontFlags = fontFlags;
	}

	void
	setup(
		const ColorAttr& colorAttr,
		uint_t fontFlags
	) {

		*(ColorAttr*)this = colorAttr;
		m_fontFlags = fontFlags;
	}

	void
	overlay(
		const TextAttr& baseAttr,
		const TextAttr& overlayAttr
	) {
		ColorAttr::overlay(baseAttr, overlayAttr);
		m_fontFlags = overlayFontFlags(baseAttr.m_fontFlags, overlayAttr.m_fontFlags);
	}

	void
	overlay(const TextAttr& overlayAttr) {
		overlay(*this, overlayAttr);
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
