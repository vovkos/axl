#include "pch.h"
#include "axl_gui_qt_Painter.h"
#include "axl_gui_qt_Engine.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

CPainter::CPainter ()
{
	m_pEngine = CEngine::GetSingleton ();
}

bool
CPainter::DrawRect (
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t Color
	)
{
	Color = OverlayColor (m_BaseTextAttr.m_BackColor, Color);

	uint_t Rgb = m_Palette.GetColorRgb (Color);

	if (!(Rgb & EColorFlag_Transparent))
		m_h->fillRect (Left, Top, Right - Left, Bottom - Top, Rgb);
	
	return true;
}

bool
CPainter::DrawText (
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

	IFont* pFont = m_pBaseFont->GetFontMod (FontFlags);

	if (m_pFont != pFont)
	{
		ASSERT (pFont->GetEngine ()->GetEngineKind () == EEngine_Qt);
		CFont* pQtFont = (CFont*) pFont;

		m_pFont = pFont;
		m_h->setFont (pQtFont->m_QtFont);
	}

	if (m_ColorAttr.m_ForeColor != TextColor)
	{
		m_ColorAttr.m_ForeColor = TextColor;

		uint_t Rgb = m_Palette.GetColorRgb (TextColor);
		if (!(Rgb & EColorFlag_Transparent))
			m_h->setPen (Rgb);
	}

	DrawRect (Left, Top, Right, Bottom, BackColor);

	QString QtText = QString::fromUtf8 (pText, Length);
	m_h->drawText (x, y, Right - x, Bottom - y, 0, QtText);
	return true;
}

bool
CPainter::DrawImage (
	int x,
	int y,
	IImage* pImage,
	int Left,
	int Top,
	int Right,
	int Bottom
	)
{
	ASSERT (pImage->GetEngine ()->GetEngineKind () == EEngine_Qt);
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
CPainter::DrawImage (
	int x,
	int y,
	IImageList* pImageList,
	size_t Index
	)
{
	ASSERT (pImageList->GetEngine ()->GetEngineKind () == EEngine_Qt);
//	CImageList* pqtImageList = (CImageList*) pImageList;

	return true;
}

bool
CPainter::CopyRect (
	ICanvas* pSrcCanvas,
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

} // namespace qt
} // namespace gui
} // namespace axl
