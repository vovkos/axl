// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_FONT_H

#include "axl_gui_Font.h"
#include "axl_gui_gdi_ObjectHandle.h"
#include "axl_ref_RefCount.h"
#include "axl_rtl_String.h"

namespace axl {
namespace gui {
namespace gdi {

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

class CFont: 
	public IFont,
	public CObjectHandleT <HFONT>
{
	friend class CEngine;

public:
	CFont ();

	bool
	GetLogFont (LOGFONTW* pLogFont);

	virtual
	bool
	IsMonospace ();

	virtual
	TSize
	CalcTextSize (
		const char* pText,
		size_t Length = -1
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CFontTupleT <CFont> CFontTuple;

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl

