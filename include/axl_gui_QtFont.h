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
getFontDescFromFontInfo (
	const QFontInfo& fontInfo,
	TFontDesc* fontDesc
	);

//.............................................................................

class CQtFont: public CFont
{
	friend class CQtEngine;

public:
	QFont m_qtFont;
	
public:
	CQtFont ();

	virtual
	bool
	isMonospace ()
	{
		return QFontInfo (m_qtFont).fixedPitch ();
	}

	TSize
	calcTextSize_qt (const QString& string);

	virtual
	TSize
	calcTextSize_utf8 (
		const utf8_t* text,
		size_t length = -1
		);

	virtual
	TSize
	calcTextSize_utf16 (
		const utf16_t* text,
		size_t length = -1
		)
	{
		return calcTextSize_qt (QString ((const QChar*) text, length));
	}

	virtual
	TSize
	calcTextSize_utf32 (
		const utf32_t* text,
		size_t length = -1
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CFontTupleT <CQtFont> CQtFontTuple;

//.............................................................................

} // namespace gui
} // namespace axl

