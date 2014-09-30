#include "pch.h"
#include "axl_gui_GdiCanvas.h"
#include "axl_gui_GdiEngine.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiCanvas::CGdiCanvas ()
{
	m_engine = CGdiEngine::getSingleton ();
	m_destructKind = EDestruct_None;
	m_hCompatibleDc = NULL;
	m_hBitmap = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevFont = NULL;
}

void
CGdiCanvas::attach (
	HDC hdc,
	HWND hWnd,
	EDestruct destructKind
	)
{
	release ();

	m_h = hdc;
	m_hWnd = hWnd;
	m_destructKind = destructKind;
}

void
CGdiCanvas::release ()
{
	if (!m_h)
		return;

	if (m_hPrevBitmap)
		::SelectObject (m_h, m_hPrevBitmap);

	if (m_hPrevFont)
		::SelectObject (m_h, m_hPrevFont);

	if (m_hBitmap)
		::DeleteObject (m_hBitmap);

	if (m_hCompatibleDc)
		::DeleteDC (m_hCompatibleDc);

	switch (m_destructKind)
	{
	case EDestruct_DeleteDc:
		::DeleteDC (m_h);
		break;

	case EDestruct_ReleaseDc:
		::ReleaseDC (m_hWnd, m_h);
		break;
	}

	m_destructKind = EDestruct_None;
	m_h = NULL;
	m_hCompatibleDc = NULL;
	m_hBitmap = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevFont = NULL;
}

bool
CGdiCanvas::drawRect (
	int left,
	int top,
	int right,
	int bottom,
	uint_t color
	)
{
	color = overlayColor (m_baseTextAttr.m_backColor, color);

	if (m_colorAttr.m_backColor != color)
	{
		m_colorAttr.m_backColor = color;

		if (!(color & EColorFlag_Transparent))
			::SetBkColor (m_h, m_palette.getColorRgb (color));
	}

	RECT gdiRect = { left, top, right, bottom };
	::ExtTextOut (m_h, 0, 0, ETO_OPAQUE, &gdiRect, NULL, 0, NULL);
	return true;
}

bool
CGdiCanvas::drawText_utf8 (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const utf8_t* text,
	size_t length
	)
{
	char buffer [256];
	rtl::CString_w string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return drawText_utf16 (
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		string,
		string.getLength ()
	);
}

bool
CGdiCanvas::drawText_utf16 (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const utf16_t* text,
	size_t length
	)
{
	textColor = overlayColor (m_baseTextAttr.m_foreColor, textColor);
	backColor = overlayColor (m_baseTextAttr.m_backColor, backColor);
	fontFlags = overlayFontFlags (m_baseTextAttr.m_fontFlags, fontFlags);

	CFont* font = m_baseFont->getFontMod (fontFlags);

	if (m_font != font)
	{
		ASSERT (font->getEngine ()->getEngineKind () == EEngine_Gdi);
		CGdiFont* gdiFont = (CGdiFont*) font;

		m_font = font;
		HFONT hPrevFont = (HFONT) ::SelectObject (m_h, *gdiFont);

		if (!m_hPrevFont)
			m_hPrevFont = hPrevFont;
	}

	if (m_colorAttr.m_foreColor != textColor)
	{
		m_colorAttr.m_foreColor = textColor;

		if (!(textColor & EColorFlag_Transparent))
			::SetTextColor (m_h, m_palette.getColorRgb (textColor));
	}

	if (m_colorAttr.m_backColor != backColor)
	{
		m_colorAttr.m_backColor = backColor;

		if (!(backColor & EColorFlag_Transparent))
			::SetBkColor (m_h, m_palette.getColorRgb (backColor));
	}

	if (length == -1)
		length = wcslen (text);

	RECT gdiRect = { left, top, right, bottom };
	::ExtTextOutW (m_h, x, y, ETO_OPAQUE, &gdiRect, text, length, NULL);
	return true;
}

bool
CGdiCanvas::drawText_utf32 (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const utf32_t* text,
	size_t length
	)
{
	char buffer [256];
	rtl::CString_w string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return drawText_utf16 (
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		string,
		string.getLength ()
	);
}

bool
CGdiCanvas::drawImage (
	int x,
	int y,
	CImage* image,
	int left,
	int top,
	int right,
	int bottom
	)
{
	ASSERT (image->getEngine ()->getEngineKind () == EEngine_Gdi);
	CGdiImage* gdiImage = (CGdiImage*) image;

	if (!m_hCompatibleDc)
	{
		CScreenDc screenDc;
		m_hCompatibleDc = ::CreateCompatibleDC (screenDc);
	}

	HBITMAP hPrevBitmap = (HBITMAP) ::SelectObject (m_hCompatibleDc, *gdiImage);

	::BitBlt (
		m_h,
		x,
		y,
		right - left,
		bottom - top,
		m_hCompatibleDc,
		left,
		top,
		SRCCOPY
		);

	::SelectObject (m_hCompatibleDc, hPrevBitmap);
	return true;
}

bool
CGdiCanvas::copyRect (
	CCanvas* srcCanvas,
	int xDst,
	int yDst,
	int xSrc,
	int ySrc,
	int width,
	int height
	)
{
	ASSERT (srcCanvas->getEngine ()->getEngineKind () == EEngine_Gdi);
	CGdiCanvas* dc = (CGdiCanvas*) srcCanvas;

	::BitBlt (
		m_h,
		xDst,
		yDst,
		width,
		height,
		dc->m_h,
		xSrc,
		ySrc,
		SRCCOPY
		);

	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
