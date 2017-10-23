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

#include "pch.h"
#include "axl_gui_AnsiAttrParser.h"

namespace axl {
namespace gui {

//..............................................................................

AnsiAttrParser::AnsiAttrParser ()
{
	m_targetAttr = NULL;
	m_textColor = NULL;
	m_backColor = NULL;
}

size_t
AnsiAttrParser::parse (
	TextAttr* targetAttr,
	const TextAttr& baseAttr,
	const sl::StringRef& string
	)
{
	static SetAttrFuncPtr setAttrFuncTable [108] = { 0 };
	static bool once = false;

	if (!once) // no need to use sl::callOnce here
	{
		setAttrFuncTable [0] = &AnsiAttrParser::clear;
		setAttrFuncTable [1] = &AnsiAttrParser::setBoldOn;
		setAttrFuncTable [3] = &AnsiAttrParser::setItalicOn;
		setAttrFuncTable [4] = &AnsiAttrParser::setUnderlineOn;
		setAttrFuncTable [7] = &AnsiAttrParser::setInverseOn;

		for (size_t i = 10; i < 20; i++)
			setAttrFuncTable [i] = &AnsiAttrParser::setFont;

		setAttrFuncTable [22] = &AnsiAttrParser::setBoldOff;
		setAttrFuncTable [23] = &AnsiAttrParser::setItalicOff;
		setAttrFuncTable [24] = &AnsiAttrParser::setUnderlineOff;
		setAttrFuncTable [27] = &AnsiAttrParser::setInverseOff;

		for (size_t i = 30; i < 38; i++)
			setAttrFuncTable [i] = &AnsiAttrParser::setTextColor;

		setAttrFuncTable [39] = &AnsiAttrParser::setBaseTextColor;

		for (size_t i = 40; i < 48; i++)
			setAttrFuncTable [i] = &AnsiAttrParser::setBackColor;

		setAttrFuncTable [49] = &AnsiAttrParser::setBaseBackColor;

		for (size_t i = 90; i < 98; i++)
			setAttrFuncTable [i] = &AnsiAttrParser::setBrightTextColor;

		for (size_t i = 100; i < 108; i++)
			setAttrFuncTable [i] = &AnsiAttrParser::setBrightBackColor;

		once = true;
	}

	m_targetAttr = targetAttr;
	m_baseAttr = baseAttr;
	m_textColor = &m_targetAttr->m_foreColor;
	m_backColor = &m_targetAttr->m_backColor;

	size_t attrCount = 0;

	const char* p = string.cp ();
	const char* end = string.getEnd ();

	while (p < end)
	{
		char* attrEnd;
		uint_t attr = strtoul (p, &attrEnd, 10);
		if (attrEnd == p)
			break;

		attrCount++;

		if (attr < countof (setAttrFuncTable))
		{
			SetAttrFuncPtr setAttrFunc = setAttrFuncTable [attr];
			if (setAttrFunc)
				(this->*setAttrFunc) (attr);
		}

		p = attrEnd;
		while (p < end)
		{
			if (*p == ';')
			{
				p++;
				break;
			}

			p++;
		}
	}

	if (!attrCount)
		*targetAttr = baseAttr;

	return attrCount;
}

void
AnsiAttrParser::clear (uint_t)
{
	*m_targetAttr = m_baseAttr;
	m_textColor = &m_targetAttr->m_foreColor;
	m_backColor = &m_targetAttr->m_backColor;
}

void
AnsiAttrParser::setInverse (bool isInversed)
{
	if (isInversed == (m_textColor == &m_targetAttr->m_backColor))
		return;
	
	uint_t c = m_targetAttr->m_foreColor;
	m_targetAttr->m_foreColor = m_targetAttr->m_backColor;
	m_targetAttr->m_backColor = c;

	uint_t* p = m_textColor;
	m_textColor = m_backColor;
	m_backColor = p;
}

void
AnsiAttrParser::setFontFlag (uint_t flag)
{
	if (m_targetAttr->m_fontFlags & FontFlag_Undefined)
		m_targetAttr->m_fontFlags = flag;
	else
		m_targetAttr->m_fontFlags |= flag;
}

void
AnsiAttrParser::clearFontFlag (uint_t flag)
{
	if (m_targetAttr->m_fontFlags & FontFlag_Undefined)
		return;

	if (m_baseAttr.m_fontFlags & FontFlag_Undefined)
		m_targetAttr->m_fontFlags &= ~flag;
	else
		m_targetAttr->m_fontFlags |= (m_baseAttr.m_fontFlags & flag);
}

//..............................................................................

} // namespace gui
} // namespace axl
