#include "pch.h"
#include "axl_gui_GdiFont.h"
#include "axl_gui_GdiEngine.h"
#include "axl_err_Error.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

//.............................................................................

bool
BuildLogFont (
	LOGFONTW* pLogFont,
	const wchar_t* pFaceName,
	size_t PointSize,
	uint_t Flags
	)
{
	memset (pLogFont, 0, sizeof (LOGFONT));

	size_t Length = wcslen (pFaceName);

	memcpy (
		pLogFont->lfFaceName, 
		pFaceName, 
		AXL_MIN (countof (pLogFont->lfFaceName), Length) * sizeof (wchar_t)
		);

	pLogFont->lfCharSet = DEFAULT_CHARSET;
	
	CScreenDc ScreenDc;
	::SetMapMode (ScreenDc, MM_TEXT);
	pLogFont->lfHeight = -MulDiv (PointSize, GetDeviceCaps (ScreenDc, LOGPIXELSY), 72);
		
	ModifyLogFont (pLogFont, Flags);
	return true;
}

void
ModifyLogFont (
	LOGFONTW* pLogFont,
	uint_t Flags
	)
{
	pLogFont->lfWeight    = (Flags & EFontFlag_Bold) ? FW_BOLD : FW_NORMAL;
	pLogFont->lfItalic    = (Flags & EFontFlag_Italic) != 0;
	pLogFont->lfUnderline = (Flags & EFontFlag_Underline) != 0;
	pLogFont->lfStrikeOut = (Flags & EFontFlag_Strikeout) != 0;
}

bool
GetFontDescFromLogFont (
	const LOGFONT* pLogFont,
	TFontDesc* pFontDesc
	)
{
	memset (pFontDesc, 0, sizeof (TFontDesc));

	memcpy (
		pFontDesc->m_FaceName, 
		pLogFont->lfFaceName, 
		(AXL_MIN (countof (pLogFont->lfFaceName), countof (pFontDesc->m_FaceName)) - 1) * sizeof (char)
		);

	CScreenDc ScreenDc;
	::SetMapMode (ScreenDc, MM_TEXT);
	
	pFontDesc->m_PointSize = 
		pLogFont->lfHeight > 0 ? pLogFont->lfHeight :
		MulDiv (-pLogFont->lfHeight, 72, GetDeviceCaps (ScreenDc, LOGPIXELSY));

	if (pLogFont->lfWeight >= FW_BOLD)
		pFontDesc->m_Flags |= EFontFlag_Bold;

	if (pLogFont->lfItalic)
		pFontDesc->m_Flags |= EFontFlag_Italic;

	if (pLogFont->lfUnderline)
		pFontDesc->m_Flags |= EFontFlag_Underline;

	if (pLogFont->lfStrikeOut)
		pFontDesc->m_Flags |= EFontFlag_Strikeout;

	return true;
}

//.............................................................................

CGdiFont::CGdiFont ()
{
	m_pEngine = CGdiEngine::GetSingleton ();
}

bool
CGdiFont::GetLogFont (LOGFONTW* pLogFont)
{
	bool_t Result = ::GetObject (m_h, sizeof (LOGFONT), pLogFont);
	return err::Complete (Result);
}

bool
CGdiFont::IsMonospace ()
{
	LOGFONT LogFont;
	GetLogFont (&LogFont);
	return LogFont.lfPitchAndFamily == FIXED_PITCH;
}

TSize
CGdiFont::CalcTextSize_utf8 (
	const utf8_t* pText,
	size_t Length
	)
{
	wchar_t Buffer [256];
	rtl::CString_w String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return CalcTextSize_utf16 (String, String.GetLength ());
}

TSize
CGdiFont::CalcTextSize_utf16 (
	const utf16_t* pText,
	size_t Length
	)
{
	if (Length == -1)
		Length = wcslen (pText);

	CScreenDc ScreenDc;
	HFONT hOldFont = (HFONT) ::SelectObject (ScreenDc, m_h);

	SIZE Size;
	::GetTextExtentPoint32W (ScreenDc, pText, Length, &Size);
	::SelectObject (ScreenDc, hOldFont);

	return TSize (Size.cx, Size.cy);
}

TSize
CGdiFont::CalcTextSize_utf32 (
	const utf32_t* pText,
	size_t Length
	)
{
	typedef rtl::CUtfConvertT <rtl::CUtf16, rtl::CUtf32> CConvert;

	if (Length == -1)
		Length = strlen_utf32 (pText);

	size_t Length_utf16 = CConvert::CalcRequiredLength (pText, Length);
	
	utf16_t Buffer [256];
	rtl::CArrayT <utf16_t> String_utf16 (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String_utf16.SetCount (Length_utf16);

	CConvert::Convert (String_utf16, Length_utf16, pText, Length);

	return CalcTextSize_utf16 (String_utf16, Length_utf16);
}

//.............................................................................

} // namespace gui
} // namespace axl
