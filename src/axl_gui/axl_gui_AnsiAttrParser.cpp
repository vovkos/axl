#include "pch.h"
#include "axl_gui_AnsiAttrParser.h"

namespace axl {
namespace gui {

//.............................................................................

size_t
AnsiAttrParser::parse (
	TextAttr* targetAttr,
	const TextAttr& baseAttr,
	const char* p,
	size_t length
	)
{
	static SetAttrProc setAttrProcTable [108] = { 0 };
	static bool once = false;

	if (!once) // no need to use mt::callOnce here
	{
		setAttrProcTable [0] = &AnsiAttrParser::clear;
		setAttrProcTable [1] = &AnsiAttrParser::setBoldOn;
		setAttrProcTable [3] = &AnsiAttrParser::setItalicOn;
		setAttrProcTable [4] = &AnsiAttrParser::setUnderlineOn;

		for (size_t i = 10; i < 20; i++)
			setAttrProcTable [i] = &AnsiAttrParser::setFont;

		setAttrProcTable [22] = &AnsiAttrParser::setBoldOff;
		setAttrProcTable [23] = &AnsiAttrParser::setItalicOff;
		setAttrProcTable [24] = &AnsiAttrParser::setUnderlineOff;

		for (size_t i = 30; i < 38; i++)
			setAttrProcTable [i] = &AnsiAttrParser::setTextColor;

		setAttrProcTable [39] = &AnsiAttrParser::setBaseTextColor;

		for (size_t i = 40; i < 48; i++)
			setAttrProcTable [i] = &AnsiAttrParser::setBackColor;

		setAttrProcTable [49] = &AnsiAttrParser::setBaseBackColor;

		for (size_t i = 90; i < 98; i++)
			setAttrProcTable [i] = &AnsiAttrParser::setBrightTextColor;

		for (size_t i = 100; i < 108; i++)
			setAttrProcTable [i] = &AnsiAttrParser::setBrightBackColor;

		once = true;
	}

	m_targetAttr = targetAttr;
	m_baseAttr = baseAttr;

	if (length == -1)
		length = strlen (p);

	size_t attrCount = 0;

	const char* end = p + length;

	while (p < end)
	{
		char* attrEnd;
		uint_t attr = strtoul (p, &attrEnd, 10);
		if (attrEnd == p)
			break;

		attrCount++;

		if (attr < countof (setAttrProcTable))
		{
			SetAttrProc setAttrProc = setAttrProcTable [attr];
			if (setAttrProc)
				(this->*setAttrProc) (attr);
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

//.............................................................................

} // namespace gui
} // namespace axl
