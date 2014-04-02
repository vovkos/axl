#include "pch.h"
#include "axl_gui_GdiCanvas.h"
#include "axl_gui_GdiEngine.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiCanvas::CGdiCanvas ()
{
	m_pEngine = CGdiEngine::GetSingleton ();
	m_DestructKind = EDestruct_None;
	m_hCompatibleDc = NULL;
	m_hBitmap = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevFont = NULL;
}

void
CGdiCanvas::Attach (
	HDC hdc,
	HWND hWnd,
	EDestruct DestructKind
	)
{
	Release ();

	m_h = hdc;
	m_hWnd = hWnd;
	m_DestructKind = DestructKind;
}

void
CGdiCanvas::Release ()
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

	switch (m_DestructKind)
	{
	case EDestruct_DeleteDc:
		::DeleteDC (m_h);
		break;

	case EDestruct_ReleaseDc:
		::ReleaseDC (m_hWnd, m_h);
		break;
	}

	m_DestructKind = EDestruct_None;
	m_h = NULL;
	m_hCompatibleDc = NULL;
	m_hBitmap = NULL;
	m_hPrevBitmap = NULL;
	m_hPrevFont = NULL;
}

bool
CGdiCanvas::DrawRect (
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t Color
	)
{
	Color = OverlayColor (m_BaseTextAttr.m_BackColor, Color);

	if (m_ColorAttr.m_BackColor != Color)
	{
		m_ColorAttr.m_BackColor = Color;

		if (!(Color & EColorFlag_Transparent))
			::SetBkColor (m_h, m_Palette.GetColorRgb (Color));
	}

	RECT GdiRect = { Left, Top, Right, Bottom };
	ExtTextOut (m_h, 0, 0, ETO_OPAQUE, &GdiRect, NULL, 0, NULL);
	return true;
}

bool
CGdiCanvas::DrawText_utf8 (
	int x,
	int y,
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t TextColor,
	uint_t BackColor,
	uint_t FontFlags,
	const utf8_t* pText,
	size_t Length
	)
{
	char Buffer [256];
	rtl::CString_w String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return DrawText_utf16 (
		x,
		y,
		Left,
		Top,
		Right,
		Bottom,
		TextColor,
		BackColor,
		FontFlags,
		String,
		String.GetLength ()
	);
}

bool
CGdiCanvas::DrawText_utf16 (
	int x,
	int y,
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t TextColor,
	uint_t BackColor,
	uint_t FontFlags,
	const utf16_t* pText,
	size_t Length
	)
{
	TextColor = OverlayColor (m_BaseTextAttr.m_ForeColor, TextColor);
	BackColor = OverlayColor (m_BaseTextAttr.m_BackColor, BackColor);
	FontFlags = OverlayFontFlags (m_BaseTextAttr.m_FontFlags, FontFlags);

	CFont* pFont = m_pBaseFont->GetFontMod (FontFlags);

	if (m_pFont != pFont)
	{
		ASSERT (pFont->GetEngine ()->GetEngineKind () == EEngine_Gdi);
		CGdiFont* pGdiFont = (CGdiFont*) pFont;

		m_pFont = pFont;
		HFONT hPrevFont = (HFONT) ::SelectObject (m_h, *pGdiFont);

		if (!m_hPrevFont)
			m_hPrevFont = hPrevFont;
	}

	if (m_ColorAttr.m_ForeColor != TextColor)
	{
		m_ColorAttr.m_ForeColor = TextColor;

		if (!(TextColor & EColorFlag_Transparent))
			::SetTextColor (m_h, m_Palette.GetColorRgb (TextColor));
	}

	if (m_ColorAttr.m_BackColor != BackColor)
	{
		m_ColorAttr.m_BackColor = BackColor;

		if (!(BackColor & EColorFlag_Transparent))
			::SetBkColor (m_h, m_Palette.GetColorRgb (BackColor));
	}

	if (Length == -1)
		Length = wcslen (pText);

	RECT GdiRect = { Left, Top, Right, Bottom };
	::ExtTextOutW (m_h, x, y, ETO_OPAQUE, &GdiRect, pText, Length, NULL);
	return true;
}

bool
CGdiCanvas::DrawText_utf32 (
	int x,
	int y,
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t TextColor,
	uint_t BackColor,
	uint_t FontFlags,
	const utf32_t* pText,
	size_t Length
	)
{
	char Buffer [256];
	rtl::CString_w String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return DrawText_utf16 (
		x,
		y,
		Left,
		Top,
		Right,
		Bottom,
		TextColor,
		BackColor,
		FontFlags,
		String,
		String.GetLength ()
	);
}

bool
CGdiCanvas::DrawImage (
	int x,
	int y,
	CImage* pImage,
	int Left,
	int Top,
	int Right,
	int Bottom
	)
{
	ASSERT (pImage->GetEngine ()->GetEngineKind () == EEngine_Gdi);
	CGdiImage* pGdiImage = (CGdiImage*) pImage;

	if (!m_hCompatibleDc)
	{
		CScreenDc ScreenDc;
		m_hCompatibleDc = ::CreateCompatibleDC (ScreenDc);
	}

	HBITMAP hPrevBitmap = (HBITMAP) ::SelectObject (m_hCompatibleDc, *pGdiImage);

	::BitBlt (
		m_h,
		x,
		y,
		Right - Left,
		Bottom - Top,
		m_hCompatibleDc,
		Left,
		Top,
		SRCCOPY
		);

	::SelectObject (m_hCompatibleDc, hPrevBitmap);
	return true;
}

bool
CGdiCanvas::CopyRect (
	CCanvas* pSrcCanvas,
	int xDst,
	int yDst,
	int xSrc,
	int ySrc,
	int Width,
	int Height
	)
{
	ASSERT (pSrcCanvas->GetEngine ()->GetEngineKind () == EEngine_Gdi);
	CGdiCanvas* pDc = (CGdiCanvas*) pSrcCanvas;

	::BitBlt (
		m_h,
		xDst,
		yDst,
		Width,
		Height,
		pDc->m_h,
		xSrc,
		ySrc,
		SRCCOPY
		);

	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
