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

#define _AXL_GUI_ANSIATTRPARSER_H

#include "axl_gui_TextAttr.h"

namespace axl {
namespace gui {

//..............................................................................

class AnsiAttrParser
{
	friend class InitSetAttrFuncTable;

protected:
	typedef
	void
	(AnsiAttrParser::*SetAttrFuncPtr) (uint_t);

protected:
	TextAttr* m_targetAttr;
	TextAttr m_baseAttr;
	uint_t* m_textColor;
	uint_t* m_backColor;

public:
	AnsiAttrParser ();

	size_t // returns attr count
	parse (
		TextAttr* targetAttr,
		const TextAttr& baseAttr,
		const sl::StringRef& string
		);

protected:
	void
	clear (uint_t);

	void
	setFontFlag (uint_t flag);

	void
	clearFontFlag (uint_t flag);

	void
	setBoldOn (uint_t)
	{
		setFontFlag (FontFlag_Bold);
	}

	void
	setBoldOff (uint_t)
	{
		clearFontFlag (FontFlag_Bold);
	}

	void
	setItalicOn (uint_t)
	{
		setFontFlag (FontFlag_Italic);
	}

	void
	setItalicOff (uint_t)
	{
		clearFontFlag (FontFlag_Italic);
	}

	void
	setUnderlineOn (uint_t)
	{
		setFontFlag (FontFlag_Underline);
	}

	void
	setUnderlineOff (uint_t)
	{
		clearFontFlag (FontFlag_Underline);
	}

	void
	setFont (uint_t font)
	{
		ASSERT (font >= 10 && font < 20);
		m_targetAttr->m_fontFlags = m_baseAttr.m_fontFlags;
	}

	void
	setInverse (bool isInversed);

	void
	setInverseOn (uint_t)
	{
		setInverse (true);
	}

	void
	setInverseOff (uint_t)
	{
		setInverse (false);
	}

	void
	setTextColor (uint_t color)
	{
		ASSERT (color >= 30 && color < 38);
		*m_textColor = StdPalColor_AnsiBlack + color - 30;
	}

	void
	setBrightTextColor (uint_t color)
	{
		ASSERT (color >= 90 && color < 98);
		*m_textColor = StdPalColor_AnsiBrightBlack + color - 90;
	}

	void
	setBaseTextColor (uint_t)
	{
		*m_textColor = m_baseAttr.m_foreColor;
	}

	void
	setBackColor (uint_t color)
	{
		ASSERT (color >= 40 && color < 48);
		*m_backColor = StdPalColor_AnsiBlack + color - 40;
	}

	void
	setBrightBackColor (uint_t color)
	{
		ASSERT (color >= 100 && color < 108);
		*m_backColor = StdPalColor_AnsiBrightBlack + color - 100;
	}

	void
	setBaseBackColor (uint_t)
	{
		*m_backColor = m_baseAttr.m_backColor;
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
