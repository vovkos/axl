// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_ANSIATTRPARSER_H

#include "axl_gui_TextAttr.h"

namespace axl {
namespace gui {

//.............................................................................

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

public:
	AnsiAttrParser ()
	{
		m_targetAttr = NULL;
	}
	
	size_t // returns attr count
	parse (
		TextAttr* targetAttr,
		const TextAttr& baseAttr,
		const char* p,
		size_t length = -1
		);

protected:
	void
	clear (uint_t)
	{
		*m_targetAttr = m_baseAttr;
	}

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
	setTextColor (uint_t color)
	{
		ASSERT (color >= 30 && color < 38);
		m_targetAttr->m_foreColor = StdPalColor_AnsiBlack + color - 30;
	}

	void
	setBrightTextColor (uint_t color)
	{
		ASSERT (color >= 90 && color < 98);
		m_targetAttr->m_foreColor = StdPalColor_AnsiBrightBlack + color - 90;
	}

	void
	setBaseTextColor (uint_t)
	{
		m_targetAttr->m_foreColor = m_baseAttr.m_foreColor;
	}

	void
	setBackColor (uint_t color)
	{
		ASSERT (color >= 40 && color < 48);
		m_targetAttr->m_backColor = StdPalColor_AnsiBlack + color - 40;
	}

	void
	setBrightBackColor (uint_t color)
	{
		ASSERT (color >= 100 && color < 108);
		m_targetAttr->m_backColor = StdPalColor_AnsiBrightBlack + color - 100;
	}

	void
	setBaseBackColor (uint_t)
	{
		m_targetAttr->m_backColor = m_baseAttr.m_backColor;
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
