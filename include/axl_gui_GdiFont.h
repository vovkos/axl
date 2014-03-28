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
BuildLogFont (
	LOGFONTW* pLogFont,
	const wchar_t* pFaceName,
	size_t PointSize,
	uint_t Flags = 0
	);

inline
bool
GetLogFontFromFontDesc (
	const TFontDesc& FontDesc,
	LOGFONTW* pLogFont
	)
{
	return BuildLogFont (
		pLogFont,
		rtl::CString_w (FontDesc.m_FaceName), 
		FontDesc.m_PointSize, 
		FontDesc.m_Flags
		);
}

void
ModifyLogFont (
	LOGFONTW* pLogFont,
	uint_t Flags
	);

bool
GetFontDescFromLogFont (
	const LOGFONTW* pLogFont,
	TFontDesc* pFontDesc
	);

//.............................................................................

class CGdiFont: 
	public CFont,
	public CGdiObjectHandleT <HFONT>
{
	friend class CGdiEngine;

public:
	CGdiFont ();

	bool
	GetLogFont (LOGFONTW* pLogFont);

	virtual
	bool
	IsMonospace ();

	virtual
	TSize
	CalcTextSize_utf8 (
		const utf8_t* pText,
		size_t Length = -1
		);

	virtual
	TSize
	CalcTextSize_utf16 (
		const utf16_t* pText,
		size_t Length = -1
		);

	virtual
	TSize
	CalcTextSize_utf32 (
		const utf32_t* pText,
		size_t Length = -1
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CFontTupleT <CGdiFont> CGdiFontTuple;

//.............................................................................

} // namespace gui
} // namespace axl

