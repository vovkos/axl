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

#define _AXL_GUI_FONTDESC_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//..............................................................................

enum FontFlag {
	FontFlag_Undefined = 0x10,
	FontFlag_Bold      = 0x01,
	FontFlag_Italic    = 0x02,
	FontFlag_Underline = 0x04,
	FontFlag_Strikeout = 0x08,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
FontFlag
getFirstFontFlag(uint_t flags) {
	return (FontFlag)(1 << sl::getLoBitIdx(flags));
}

const char*
getFontFlagString(FontFlag flag);

inline
const char*
getFontFlagString(uint_t flags) {
	return getFontFlagString(getFirstFontFlag(flags));
}

inline
uint_t
getOverlayFontFlags(
	uint_t baseFontFlags,
	uint_t overlayFontFlags
) {
	return (overlayFontFlags & FontFlag_Undefined) ? baseFontFlags : overlayFontFlags;
}

//..............................................................................

struct FontDesc {
	char m_family[32];
	size_t m_pointSize;
	uint_t m_flags;

	FontDesc() {
		memset(this, 0, sizeof(FontDesc));
	}

	FontDesc(
		const sl::StringRef& family,
		size_t pointSize = 0,
		uint_t flags = 0
	) {
		setup(family, pointSize, flags);
	}

	void
	setup(
		const sl::StringRef& family,
		size_t pointSize = 0,
		uint_t flags = 0
	);
};

//..............................................................................

} // namespace gui
} // namespace axl
