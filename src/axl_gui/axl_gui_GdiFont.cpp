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
#include "axl_gui_GdiFont.h"
#include "axl_gui_GdiEngine.h"
#include "axl_err_Error.h"
#include "axl_sl_Array.h"

namespace axl {
namespace gui {

//..............................................................................

bool
buildLogFont(
	LOGFONTW* logFont,
	const sl::StringRef_w& family,
	size_t pointSize,
	uint_t flags
	)
{
	memset(logFont, 0, sizeof(LOGFONT));

	size_t length = family.getLength();

	memcpy(
		logFont->lfFaceName,
		family,
		AXL_MIN(countof(logFont->lfFaceName), length) * sizeof(wchar_t)
		);

	logFont->lfCharSet = DEFAULT_CHARSET;

	ScreenDc screenDc;
	::SetMapMode(screenDc, MM_TEXT);
	logFont->lfHeight = -::MulDiv(pointSize, ::GetDeviceCaps(screenDc, LOGPIXELSY), 72);

	modifyLogFont(logFont, flags);
	return true;
}

void
modifyLogFont(
	LOGFONTW* logFont,
	uint_t flags
	)
{
	logFont->lfWeight    = (flags & FontFlag_Bold) ? FW_BOLD : FW_NORMAL;
	logFont->lfItalic    = (flags & FontFlag_Italic) != 0;
	logFont->lfUnderline = (flags & FontFlag_Underline) != 0;
	logFont->lfStrikeOut = (flags & FontFlag_Strikeout) != 0;
}

bool
getFontDescFromLogFont(
	const LOGFONT* logFont,
	FontDesc* fontDesc
	)
{
	memset(fontDesc, 0, sizeof(FontDesc));

	memcpy(
		fontDesc->m_family,
		logFont->lfFaceName,
		(AXL_MIN(countof(logFont->lfFaceName), countof(fontDesc->m_family)) - 1) * sizeof(char)
		);

	ScreenDc screenDc;
	::SetMapMode(screenDc, MM_TEXT);

	fontDesc->m_pointSize =
		logFont->lfHeight > 0 ? logFont->lfHeight :
		::MulDiv(-logFont->lfHeight, 72, ::GetDeviceCaps(screenDc, LOGPIXELSY));

	if (logFont->lfWeight >= FW_BOLD)
		fontDesc->m_flags |= FontFlag_Bold;

	if (logFont->lfItalic)
		fontDesc->m_flags |= FontFlag_Italic;

	if (logFont->lfUnderline)
		fontDesc->m_flags |= FontFlag_Underline;

	if (logFont->lfStrikeOut)
		fontDesc->m_flags |= FontFlag_Strikeout;

	return true;
}

//..............................................................................

GdiFont::GdiFont()
{
	m_engine = GdiEngine::getSingleton();
}

bool
GdiFont::getLogFont(LOGFONTW* logFont)
{
	bool_t result = ::GetObject(m_h, sizeof(LOGFONT), logFont);
	return err::complete(result);
}

bool
GdiFont::isMonospace()
{
	LOGFONT logFont;
	getLogFont(&logFont);
	return logFont.lfPitchAndFamily == FIXED_PITCH;
}

Size
GdiFont::calcTextSize_utf8(const sl::StringRef_utf8& text)
{
	char buffer[256];
	sl::String_w string(ref::BufKind_Stack, buffer, sizeof(buffer));
	string.copy(text, length);

	return calcTextSize_utf16(string, string.getLength());
}

Size
GdiFont::calcTextSize_utf16(const sl::StringRef_utf16& text)
{
	if (length == -1)
		length = wcslen_s(text);

	ScreenDc screenDc;
	HFONT hOldFont = (HFONT) ::SelectObject(screenDc, m_h);

	SIZE size;
	::GetTextExtentPoint32W(screenDc, text, length, &size);
	::SelectObject(screenDc, hOldFont);

	return Size(size.cx, size.cy);
}

Size
GdiFont::calcTextSize_utf32(const sl::StringRef_utf32& text)
{
	char buffer[256];
	sl::String_w string(ref::BufKind_Stack, buffer, sizeof(buffer));
	string.copy(text, length);

	return calcTextSize_utf16(string, string.getLength());
}

//..............................................................................

} // namespace gui
} // namespace axl
