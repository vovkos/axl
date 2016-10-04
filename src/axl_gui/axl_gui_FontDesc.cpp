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

	size_t i = sl::getLoBitIdx32 (flag >> 8);

	return i < countof (stringTable) ? 
		stringTable [i] : 
		"undefined-font-flag";
}

//.............................................................................

void
FontDesc::setup (
	const sl::StringRef& family, 
	size_t pointSize,
	uint_t flags
	)
{
	size_t length = family.getLength ();

	if (length >= countof (m_family))
		length = countof (m_family) - 1;
	
	memcpy (m_family, family.cp (), length);
	m_family [length] = 0;
	m_pointSize = pointSize;
	m_flags = flags;
}

//.............................................................................

} // namespace gui
} // namespace axl
