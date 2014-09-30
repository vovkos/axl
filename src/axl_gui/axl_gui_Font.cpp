#include "pch.h"
#include "axl_gui_Font.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

const char* 
getFontFlagString (EFontFlag flag)
{
	static const char* stringTable [] = 
	{
		"bold",      // EFontFlag_Bold      = 0x01,
		"italic",    // EFontFlag_Italic    = 0x02,
		"underline", // EFontFlag_Underline = 0x04,
		"strikeout", // EFontFlag_Strikeout = 0x08,
	};

	size_t i = rtl::getLoBitIdx32 (flag >> 8);

	return i < countof (stringTable) ? 
		stringTable [i] : 
		"undefined-font-flag";
}

uint_t
parseFontFlagString (
	const char* string,
	const char** end
	)
{
	if (!string)
	{
		if (end)
			*end = NULL;

		return EFontFlag_Transparent;
	}

	uint_t flags = 0;

	const char* p = string;

	for (;;)
	{
		int mod = 0;

		switch (*p)
		{
		case 'b': 
		case 'B':
			mod = EFontFlag_Bold;
			break;

		case 'i': 
		case 'I':
			mod = EFontFlag_Italic;
			break;

		case 'u': 
		case 'U':
			mod = EFontFlag_Underline;
			break;

		case 's': 
		case 'S':
			mod = EFontFlag_Strikeout;
			break;
		}

		if (!mod)
			break;

		flags |= mod;
		p++;
	}

	if (end)
		*end = p;

	return flags;
}

//.............................................................................

void
TFontDesc::setup (
	const char* faceName, 
	size_t pointSize,
	uint_t flags
	)
{
	size_t length = strlen (faceName);
	if (length >= countof (m_faceName))
		length = countof (m_faceName) - 1;
	
	memcpy (m_faceName, faceName, length * sizeof (char));
	m_faceName [length] = 0;
	m_pointSize = pointSize;
	m_flags = flags;
}

//.............................................................................

CFont*
CFont::getFontMod (uint_t flags)
{
	if (flags & EFontFlag_Transparent)
		return this;

	flags &= (countof (m_tuple->m_fontModArray) - 1);
	
	return m_tuple->m_fontModArray [flags] ?
		m_tuple->m_fontModArray [flags] :
		m_engine->getFontMod (this, flags);
}

//.............................................................................

void
TTextAttr::parse (
	const char* string,
	const char** end
	)
{
	TColorAttr::parse (string, &string);

	while (isspace (*string))
		string++;

	if (*string == '+')
		m_fontFlags = parseFontFlagString (string + 1, &string);

	if (end)
		*end = string;
}

void
TTextAttr::parseOverlay (
	const TTextAttr& baseAttr,
	const char* string,
	const char** end
	)
{
	TTextAttr overlayAttr;
	overlayAttr.parse (string, end);
	overlay (baseAttr, overlayAttr);
}

//.............................................................................

} // namespace gui
} // namespace axl
