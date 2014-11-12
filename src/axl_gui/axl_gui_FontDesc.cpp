#include "pch.h"
#include "axl_gui_FontDesc.h"

namespace axl {
namespace gui {

//.............................................................................
	
const char* 
getFontFlagString (FontFlag flag)
{
	static const char* stringTable [] = 
	{
		"bold",            // EFontFlag_Bold      = 0x01,
		"italic",          // EFontFlag_Italic    = 0x02,
		"underline",       // EFontFlag_Underline = 0x04,
		"strikeout",       // EFontFlag_Strikeout = 0x08,
		"clear-bold",      // EFontFlag_Bold      = 0x01,
		"clear-italic",    // EFontFlag_Italic    = 0x02,
		"clear-underline", // EFontFlag_Underline = 0x04,
		"clear-strikeout", // EFontFlag_Strikeout = 0x08,
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

		return 0;
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
			mod = FontFlag_Bold;
			break;

		case 'i': 
		case 'I':
			mod = FontFlag_Italic;
			break;

		case 'u': 
		case 'U':
			mod = FontFlag_Underline;
			break;

		case 's': 
		case 'S':
			mod = FontFlag_Strikeout;
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
FontDesc::setup (
	const char* family, 
	size_t pointSize,
	uint_t flags
	)
{
	size_t length = strlen (family);
	if (length >= countof (m_family))
		length = countof (m_family) - 1;
	
	memcpy (m_family, family, length * sizeof (char));
	m_family [length] = 0;
	m_pointSize = pointSize;
	m_flags = flags;
}

//.............................................................................

} // namespace gui
} // namespace axl
