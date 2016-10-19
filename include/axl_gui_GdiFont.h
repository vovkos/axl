// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIFONT_H

#include "axl_gui_GdiObjectHandle.h"
#include "axl_gui_Font.h"

namespace axl {
namespace gui {

//..............................................................................

bool
buildLogFont (
	LOGFONTW* logFont,
	const sl::StringRef_w& family,
	size_t pointSize,
	uint_t flags = 0
	);

inline
bool
getLogFontFromFontDesc (
	const FontDesc& fontDesc,
	LOGFONTW* logFont
	)
{
	return buildLogFont (
		logFont,
		sl::String_w (fontDesc.m_family),
		fontDesc.m_pointSize,
		fontDesc.m_flags
		);
}

void
modifyLogFont (
	LOGFONTW* logFont,
	uint_t flags
	);

bool
getFontDescFromLogFont (
	const LOGFONTW* logFont,
	FontDesc* fontDesc
	);

//..............................................................................

class GdiFont:
	public Font,
	public GdiObjectHandle <HFONT>
{
	friend class GdiEngine;

public:
	GdiFont ();

	bool
	getLogFont (LOGFONTW* logFont);

	virtual
	bool
	isMonospace ();

	virtual
	Size
	calcTextSize_utf8 (const sl::StringRef_utf8& text);

	virtual
	Size
	calcTextSize_utf16 (const sl::StringRef_utf16& text);

	virtual
	Size
	calcTextSize_utf32 (const sl::StringRef_utf32& text);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef FontTuple <GdiFont> GdiFontuple;

//..............................................................................

} // namespace gui
} // namespace axl

