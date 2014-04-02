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
CQtCanvas::DrawText_qt (
	int x,
	int y,
	int Left,
	int Top,
	int Right,
	int Bottom,
	uint_t TextColor,
	uint_t BackColor,
	uint_t FontFlags,
	const QString& String
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

	m_h->drawText (x, y, Right - x, Bottom - y, 0, String);
	return true;
}

bool
CQtCanvas::DrawText_utf8 (
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
	rtl::CString_utf16 String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return DrawText_qt (
		x,
		y,
		Left,
		Top,
		Right,
		Bottom,
		TextColor,
		BackColor,
		FontFlags,
		QString ((const QChar*) String.cc (), String.GetLength ())
		);
}

bool
CQtCanvas::DrawText_utf32 (
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
	rtl::CString_utf16 String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return DrawText_qt (
		x,
		y,
		Left,
		Top,
		Right,
		Bottom,
		TextColor,
		BackColor,
		FontFlags,
		QString ((const QChar*) String.cc (), String.GetLength ())
		);
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
