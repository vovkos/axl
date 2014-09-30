// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIFONT_H

#include "axl_gui_Font.h"
#include "axl_gui_GdiObjectHandle.h"
#include "axl_ref_RefCount.h"
#include "axl_rtl_String.h"

namespace axl {
namespace gui {

//.............................................................................

bool
buildLogFont (
	LOGFONTW* logFont,
	const wchar_t* faceName,
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
		rtl::String_w (fontDesc.m_faceName), 
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

//.............................................................................

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
	calcTextSize_utf8 (
		const utf8_t* text,
		size_t length = -1
		);

	virtual
	Size
	calcTextSize_utf16 (
		const utf16_t* text,
		size_t length = -1
		);

	virtual
	Size
	calcTextSize_utf32 (
		const utf32_t* text,
		size_t length = -1
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef FontTuple <GdiFont> GdiFontuple;

//.............................................................................

} // namespace gui
} // namespace axl

