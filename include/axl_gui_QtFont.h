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
	FontDesc* fontDesc
	);

//.............................................................................

class QtFont: public Font
{
	friend class QtEngine;

public:
	QFont m_qtFont;
	
public:
	QtFont ();

	virtual
	bool
	isMonospace ()
	{
		return QFontInfo (m_qtFont).fixedPitch ();
	}

	Size
	calcTextSize_qt (const QString& string);

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
		)
	{
		return calcTextSize_qt (QString ((const QChar*) text, length));
	}

	virtual
	Size
	calcTextSize_utf32 (
		const utf32_t* text,
		size_t length = -1
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef FontTuple <QtFont> QtFontuple;

//.............................................................................

} // namespace gui
} // namespace axl

