#include "pch.h"
#include "axl_gui_GdiFont.h"
#include "axl_gui_GdiEngine.h"
#include "axl_err_Error.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

//.............................................................................

bool
buildLogFont (
	LOGFONTW* logFont,
	const wchar_t* faceName,
	size_t pointSize,
	uint_t flags
	)
{
	memset (logFont, 0, sizeof (LOGFONT));

	size_t length = wcslen (faceName);

	memcpy (
		logFont->lfFaceName, 
		faceName, 
		AXL_MIN (countof (logFont->lfFaceName), length) * sizeof (wchar_t)
		);

	logFont->lfCharSet = DEFAULT_CHARSET;
	
	CScreenDc screenDc;
	::SetMapMode (screenDc, MM_TEXT);
	logFont->lfHeight = -::MulDiv (pointSize, ::GetDeviceCaps (screenDc, LOGPIXELSY), 72);
		
	modifyLogFont (logFont, flags);
	return true;
}

void
modifyLogFont (
	LOGFONTW* logFont,
	uint_t flags
	)
{
	logFont->lfWeight    = (flags & EFontFlag_Bold) ? FW_BOLD : FW_NORMAL;
	logFont->lfItalic    = (flags & EFontFlag_Italic) != 0;
	logFont->lfUnderline = (flags & EFontFlag_Underline) != 0;
	logFont->lfStrikeOut = (flags & EFontFlag_Strikeout) != 0;
}

bool
getFontDescFromLogFont (
	const LOGFONT* logFont,
	TFontDesc* fontDesc
	)
{
	memset (fontDesc, 0, sizeof (TFontDesc));

	memcpy (
		fontDesc->m_faceName, 
		logFont->lfFaceName, 
		(AXL_MIN (countof (logFont->lfFaceName), countof (fontDesc->m_faceName)) - 1) * sizeof (char)
		);

	CScreenDc screenDc;
	::SetMapMode (screenDc, MM_TEXT);
	
	fontDesc->m_pointSize = 
		logFont->lfHeight > 0 ? logFont->lfHeight :
		::MulDiv (-logFont->lfHeight, 72, ::GetDeviceCaps (screenDc, LOGPIXELSY));

	if (logFont->lfWeight >= FW_BOLD)
		fontDesc->m_flags |= EFontFlag_Bold;

	if (logFont->lfItalic)
		fontDesc->m_flags |= EFontFlag_Italic;

	if (logFont->lfUnderline)
		fontDesc->m_flags |= EFontFlag_Underline;

	if (logFont->lfStrikeOut)
		fontDesc->m_flags |= EFontFlag_Strikeout;

	return true;
}

//.............................................................................

CGdiFont::CGdiFont ()
{
	m_engine = CGdiEngine::getSingleton ();
}

bool
CGdiFont::getLogFont (LOGFONTW* logFont)
{
	bool_t result = ::GetObject (m_h, sizeof (LOGFONT), logFont);
	return err::complete (result);
}

bool
CGdiFont::isMonospace ()
{
	LOGFONT logFont;
	getLogFont (&logFont);
	return logFont.lfPitchAndFamily == FIXED_PITCH;
}

TSize
CGdiFont::calcTextSize_utf8 (
	const utf8_t* text,
	size_t length
	)
{
	char buffer [256];
	rtl::CString_w string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return calcTextSize_utf16 (string, string.getLength ());
}

TSize
CGdiFont::calcTextSize_utf16 (
	const utf16_t* text,
	size_t length
	)
{
	if (length == -1)
		length = wcslen (text);

	CScreenDc screenDc;
	HFONT hOldFont = (HFONT) ::SelectObject (screenDc, m_h);

	SIZE size;
	::GetTextExtentPoint32W (screenDc, text, length, &size);
	::SelectObject (screenDc, hOldFont);

	return TSize (size.cx, size.cy);
}

TSize
CGdiFont::calcTextSize_utf32 (
	const utf32_t* text,
	size_t length
	)
{
	char buffer [256];
	rtl::CString_w string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return calcTextSize_utf16 (string, string.getLength ());
}

//.............................................................................

} // namespace gui
} // namespace axl
