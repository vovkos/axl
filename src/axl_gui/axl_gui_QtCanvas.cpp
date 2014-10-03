#include "pch.h"
#include "axl_gui_QtCanvas.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

QtCanvas::QtCanvas ()
{
	m_engine = QtEngine::getSingleton ();
}

bool
QtCanvas::drawRect (
	int left,
	int top,
	int right,
	int bottom,
	uint_t color
	)
{
	color = overlayColor (m_baseTextAttr.m_backColor, color);

	if (!(color & ColorFlag_Transparent))
		m_h->fillRect (left, top, right - left, bottom - top, m_palette.getColorRgb (color));

	return true;
}

bool
QtCanvas::drawText_qt (
	int x,
	int y,
	int left,
	int top,
	int right,
	int bottom,
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const QString& string
	)
{
	textColor = overlayColor (m_baseTextAttr.m_foreColor, textColor);
	backColor = overlayColor (m_baseTextAttr.m_backColor, backColor);
	fontFlags = overlayFontFlags (m_baseTextAttr.m_fontFlags, fontFlags);

	Font* font = m_baseFont->getFontMod (fontFlags);

	if (m_font != font)
	{
		ASSERT (font->getEngine ()->getEngineKind () == EngineKind_Qt);
		QtFont* qtFont = (QtFont*) font;

		m_font = font;
		m_h->setFont (qtFont->m_qtFont);
	}

	if (m_colorAttr.m_foreColor != textColor)
	{
		m_colorAttr.m_foreColor = textColor;

		if (!(textColor & ColorFlag_Transparent))
			m_h->setPen (m_palette.getColorRgb (textColor));
	}

	drawRect (left, top, right, bottom, backColor);

	m_h->drawText (x, y, right - x, bottom - y, 0, string);
	return true;
}

bool
QtCanvas::drawText_utf8 (
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
	rtl::String_utf16 string (ref::BufKind_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return drawText_qt (
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		QString ((const QChar*) string.cc (), string.getLength ())
		);
}

bool
QtCanvas::drawText_utf32 (
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
	rtl::String_utf16 string (ref::BufKind_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return drawText_qt (
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		QString ((const QChar*) string.cc (), string.getLength ())
		);
}


bool
QtCanvas::drawImage (
	int x,
	int y,
	Image* image,
	int left,
	int top,
	int right,
	int bottom
	)
{
	ASSERT (image->getEngine ()->getEngineKind () == EngineKind_Qt);
	QtImage* qtImage = (QtImage*) image;
	m_h->drawPixmap (x, y, qtImage->m_qtPixmap);
	return true;
}

bool
QtCanvas::copyRect (
	Canvas* srcCanvas,
	int xDst,
	int yDst,
	int xSrc,
	int ySrc,
	int width,
	int height
	)
{
	ASSERT (srcCanvas->getEngine ()->getEngineKind () == EngineKind_Qt);
/*	Painter* dc = (Painter*) srcCanvas;

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
*/
	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
