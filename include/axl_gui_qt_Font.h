// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QT_FONT_H

#include "axl_gui_Font.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

bool
GetFontDescFromFontInfo (
	const QFontInfo& FontInfo,
	TFontDesc* pFontDesc
	);

//.............................................................................

class CFont: public IFont
{
	friend class CEngine;

public:
	QFont m_QtFont;
	
public:
	CFont ();

	virtual
	bool
	IsMonospace ()
	{
		return QFontInfo (m_QtFont).fixedPitch ();
	}

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

} // namespace qt
} // namespace gui
} // namespace axl

