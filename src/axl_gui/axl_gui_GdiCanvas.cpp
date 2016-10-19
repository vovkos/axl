#include "pch.h"
#include "axl_gui_GdiCanvas.h"
#include "axl_gui_GdiEngine.h"
#include "axl_sl_Array.h"

namespace axl {
namespace gui {

//..............................................................................

GdiCanvas::GdiCanvas ()
{
	m_engine = GdiEngine::getSingleton ();
	m_destructKind = DestructKind_None;
	m_hCompatibleDc = NULL;
	m_hBitmap = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevFont = NULL;
}

void
GdiCanvas::attach (
	HDC hdc,
	HWND hWnd,
	DestructKind destructKind
	)
{
	release ();

	m_h = hdc;
	m_hWnd = hWnd;
	m_destructKind = destructKind;
}

void
GdiCanvas::release ()
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
	case DestructKind_DeleteDc:
		::DeleteDC (m_h);
		break;

	case DestructKind_ReleaseDc:
		::ReleaseDC (m_hWnd, m_h);
		break;
	}

	m_destructKind = DestructKind_None;
	m_h = NULL;
	m_hCompatibleDc = NULL;
	m_hBitmap = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevFont = NULL;
}

bool
GdiCanvas::drawRect (
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

		if (!(color & ColorFlag_Transparent))
			::SetBkColor (m_h, m_palette.getColorRgb (color));
	}

	RECT gdiRect = { left, top, right, bottom };
	::ExtTextOut (m_h, 0, 0, ETO_OPAQUE, &gdiRect, NULL, 0, NULL);
	return true;
}

bool
GdiCanvas::drawText_utf8 (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf8& text
	)
{
	char buffer [256];
	sl::String_w text_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	text_w.copy (text);

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
		text_w
		);
}

bool
GdiCanvas::drawText_utf16 (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf16& text
	)
{
	textColor = overlayColor (m_baseTextAttr.m_foreColor, textColor);
	backColor = overlayColor (m_baseTextAttr.m_backColor, backColor);
	fontFlags = overlayFontFlags (m_baseTextAttr.m_fontFlags, fontFlags);

	Font* font = m_baseFont->getFontMod (fontFlags);

	if (m_font != font)
	{
		ASSERT (font->getEngine ()->getEngineKind () == EngineKind_Gdi);
		GdiFont* gdiFont = (GdiFont*) font;

		m_font = font;
		HFONT hPrevFont = (HFONT) ::SelectObject (m_h, *gdiFont);

		if (!m_hPrevFont)
			m_hPrevFont = hPrevFont;
	}

	if (m_colorAttr.m_foreColor != textColor)
	{
		m_colorAttr.m_foreColor = textColor;

		if (!(textColor & ColorFlag_Transparent))
			::SetTextColor (m_h, m_palette.getColorRgb (textColor));
	}

	if (m_colorAttr.m_backColor != backColor)
	{
		m_colorAttr.m_backColor = backColor;

		if (!(backColor & ColorFlag_Transparent))
			::SetBkColor (m_h, m_palette.getColorRgb (backColor));
	}

	if (length == -1)
		length = wcslen_s (text);

	RECT gdiRect = { left, top, right, bottom };
	::ExtTextOutW (m_h, x, y, ETO_OPAQUE, &gdiRect, text, length, NULL);
	return true;
}

bool
GdiCanvas::drawText_utf32 (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf32& text
	)
{
	char buffer [256];
	sl::String_w text_w (ref::BufKind_Stack, buffer, sizeof (buffer));
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
		text_w
		);
}

bool
GdiCanvas::drawImage (
	int x,
	int y,
	Image* image,
	int left,
	int top,
	int right,
	int bottom
	)
{
	ASSERT (image->getEngine ()->getEngineKind () == EngineKind_Gdi);
	GdiImage* gdiImage = (GdiImage*) image;

	if (!m_hCompatibleDc)
	{
		ScreenDc screenDc;
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
GdiCanvas::copyRect (
	Canvas* srcCanvas,
	int xDst,
	int yDst,
	int xSrc,
	int ySrc,
	int width,
	int height
	)
{
	ASSERT (srcCanvas->getEngine ()->getEngineKind () == EngineKind_Gdi);
	GdiCanvas* dc = (GdiCanvas*) srcCanvas;

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

//..............................................................................

} // namespace gui
} // namespace axl
