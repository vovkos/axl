#include "pch.h"
#include "axl_gui_QtCanvas.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

CQtCanvas::CQtCanvas ()
{
	m_pEngine = CQtEngine::GetSingleton ();
}

bool
CQtCanvas::DrawRect (
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t Color
	)
{
	Color = OverlayColor (m_BaseTextAttr.m_BackColor, Color);

	if (!(Color & EColorFlag_Transparent))
		m_h->fillRect (Left, Top, Right - Left, Bottom - Top, m_Palette.GetColorRgb (Color));

	return true;
}

bool
CQtCanvas::DrawText (
	int x,
	int y,
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t TextColor,
	uint_t BackColor,
	uint_t FontFlags,
	const char* pText,
	size_t Length
	)
{
	TextColor = OverlayColor (m_BaseTextAttr.m_ForeColor, TextColor);
	BackColor = OverlayColor (m_BaseTextAttr.m_BackColor, BackColor);
	FontFlags = OverlayFontFlags (m_BaseTextAttr.m_FontFlags, FontFlags);

	CFont* pFont = m_pBaseFont->GetFontMod (FontFlags);

	if (m_pFont != pFont)
	{
		ASSERT (pFont->GetEngine ()->GetEngineKind () == EEngine_Qt);
		CQtFont* pQtFont = (CQtFont*) pFont;

		m_pFont = pFont;
		m_h->setFont (pQtFont->m_QtFont);
	}

	if (m_ColorAttr.m_ForeColor != TextColor)
	{
		m_ColorAttr.m_ForeColor = TextColor;

		if (!(TextColor & EColorFlag_Transparent))
			m_h->setPen (m_Palette.GetColorRgb (TextColor));
	}

	DrawRect (Left, Top, Right, Bottom, BackColor);

	QString QtText = QString::fromUtf8 (pText, Length);
	m_h->drawText (x, y, Right - x, Bottom - y, 0, QtText);
	return true;
}

bool
CQtCanvas::DrawImage (
	int x,
	int y,
	CImage* pImage,
	int Left,
	int Top,
	int Right,
	int Bottom
	)
{
	ASSERT (pImage->GetEngine ()->GetEngineKind () == EEngine_Qt);
	CQtImage* pQtImage = (CQtImage*) pImage;
	m_h->drawPixmap (x, y, pQtImage->m_QtPixmap);

/*	CBitmap* pBitmap = (CBitmap*) pImage;

	if (!m_hCompatibleDc)
	{
		CScreenDc ScreenDc;
		m_hCompatibleDc = ::CreateCompatibleDC (ScreenDc);
	}

	HBITMAP hPrevBitmap = (HBITMAP) ::SelectObject (m_hCompatibleDc, *pBitmap);

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

	::SelectObject (m_hCompatibleDc, hPrevBitmap); */
	return true;
}

bool
CQtCanvas::CopyRect (
	CCanvas* pSrcCanvas,
	int xDst,
	int yDst,
	int xSrc,
	int ySrc,
	int Width,
	int Height
	)
{
	ASSERT (pSrcCanvas->GetEngine ()->GetEngineKind () == EEngine_Qt);
/*	CPainter* pDc = (CPainter*) pSrcCanvas;

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
*/
	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
