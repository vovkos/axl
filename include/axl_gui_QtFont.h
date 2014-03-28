// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTFONT_H

#include "axl_gui_Font.h"

namespace axl {
namespace gui {

//.............................................................................

bool
GetFontDescFromFontInfo (
	const QFontInfo& FontInfo,
	TFontDesc* pFontDesc
	);

//.............................................................................

class CQtFont: public CFont
{
	friend class CQtEngine;

public:
	QFont m_QtFont;
	
public:
	CQtFont ();

	virtual
	bool
	IsMonospace ()
	{
		return QFontInfo (m_QtFont).fixedPitch ();
	}

	TSize
	CalcTextSize_qt (const QString& String);

	virtual
	TSize
	CalcTextSize_utf8 (
		const utf8_t* pText,
		size_t Length = -1
		)
	{
		return CalcTextSize_qt (QString::fromUtf8 (pText, Length));
	}

	virtual
	TSize
	CalcTextSize_utf16 (
		const utf16_t* pText,
		size_t Length = -1
		)
	{
		return CalcTextSize_qt (QString::fromUtf16 ((ushort*) pText, Length));
	}

	virtual
	TSize
	CalcTextSize_utf32 (
		const utf32_t* pText,
		size_t Length = -1
		)
	{
		return CalcTextSize_qt (QString::fromUcs4 ((const uint*) pText, Length));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CFontTupleT <CQtFont> CQtFontTuple;

//.............................................................................

} // namespace gui
} // namespace axl

