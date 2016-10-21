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
#include "axl_gui_QtEngine.h"
#include "axl_gui_QtWidget.h"

namespace axl {
namespace gui {

//..............................................................................

QtFont* QtFontTuple::attachFont (QFont qtFont)
{
	clear ();

	QtFont* font = AXL_MEM_NEW (QtFont);
	font->m_tuple = this;
	font->m_qtFont = qtFont;
	m_fontModArray [0] = font;

	return font;
}

//..............................................................................

QtEngine::QtEngine ():
	m_sharedOffscreenCanvasCache (this)
{
	m_qtClipboardMimeData = NULL;
	memset (m_stdFontTupleArray, 0, sizeof (m_stdFontTupleArray));
	memset (m_stdCursorArray, 0, sizeof (m_stdCursorArray));

	updateStdPalette ();
}

QtEngine::~QtEngine ()
{
	if (m_qtClipboardMimeData)
		delete m_qtClipboardMimeData;

	for (size_t i = 0; i < countof (m_stdFontTupleArray); i++)
		if (m_stdFontTupleArray [i])
			AXL_MEM_DELETE (m_stdFontTupleArray [i]);

	for (size_t i = 0; i < countof (m_stdCursorArray); i++)
		if (m_stdCursorArray [i])
			AXL_MEM_DELETE (m_stdCursorArray [i]);
}

void
QtEngine::updateStdPalette ()
{
	QPalette palette = QApplication::palette ();

	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_WidgetText]    = palette.color (QPalette::Text).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_WidgetBack]    = palette.color (QPalette::Base).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_GrayText]      = palette.color (QPalette::Disabled, QPalette::WindowText).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_SelectionText] = palette.color (QPalette::HighlightedText).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_SelectionBack] = palette.color (QPalette::Highlight).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DFace]        = palette.color (QPalette::Button).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DShadow]      = palette.color (QPalette::Dark).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DDarkShadow]  = palette.color (QPalette::Shadow).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DLight]       = palette.color (QPalette::Midlight).rgb() & ColorFlag_RgbMask;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DHiLight]     = palette.color (QPalette::Light).rgb() & ColorFlag_RgbMask;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// canvas

bool
QtEngine::createOffscreenCanvas (
	Canvas* canvas,
	uint_t width,
	uint_t height
	)
{
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;
	qtCanvas->m_qtPixmap = QPixmap (width, height);
	qtCanvas->m_qtPainter.begin (&qtCanvas->m_qtPixmap);
	return true;
}

bool
QtEngine::releaseOffscreenCanvas (Canvas* canvas)
{
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;
	qtCanvas->m_qtPainter.end ();
	qtCanvas->m_qtPixmap = QPixmap ();
	return true;
}

bool
QtEngine::drawRect (
	Canvas* canvas,
	int left,
	int top,
	int right,
	int bottom,
	uint_t color
	)
{
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;

	ASSERT (!(color & ColorFlag_Undefined));
	color = qtCanvas->m_palette.getColorRgb (color);
	qtCanvas->m_qtPainter.fillRect (left, top, right - left, bottom - top, color);
	return true;
}

bool
QtEngine::drawAlphaRect (
	Canvas* canvas,
	int left,
	int top,
	int right,
	int bottom,
	uint_t color,
	uint_t alpha
	)
{
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;

	ASSERT (!(color & ColorFlag_Undefined));
	QColor qtColor = qtCanvas->m_palette.getColorRgb (color);
	qtColor.setAlpha (alpha);

	qtCanvas->m_qtPainter.fillRect (left, top, right - left, bottom - top, qtColor);
	return true;
}

bool
QtEngine::drawText_qt (
	Canvas* canvas,
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
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;

	Font* font = qtCanvas->m_font->getFontMod (fontFlags);
	if (qtCanvas->m_driverFont != font)
	{
		ASSERT (font->getEngine () == this);
		QtFont* qtFont = (QtFont*) font;

		qtCanvas->m_driverFont = font;
		qtCanvas->m_qtPainter.setFont (qtFont->m_qtFont);
	}

	if (textColor & ColorFlag_Undefined)
		textColor = qtCanvas->m_colorAttr.m_foreColor;

	if (!(textColor & ColorFlag_Undefined))
	{
		textColor = qtCanvas->m_palette.getColorRgb (textColor);

		if (qtCanvas->m_driverColorAttr.m_foreColor != textColor)
		{
			qtCanvas->m_qtPainter.setPen (textColor);
			qtCanvas->m_driverColorAttr.m_foreColor = textColor;
		}
	}

	if (backColor & ColorFlag_Undefined)
		backColor = qtCanvas->m_colorAttr.m_backColor;

	if (!(backColor & ColorFlag_Undefined))
		drawRect (canvas, left, top, right, bottom, backColor);

	qtCanvas->m_qtPainter.drawText (x, y, right - x, bottom - y, 0, string);
	return true;
}

bool
QtEngine::drawText_utf8 (
	Canvas* canvas,
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
	return drawText_qt (
		canvas,
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		QString::fromUtf8 (text.cp (), text.getLength ())
		);
}

bool
QtEngine::drawText_utf16 (
	Canvas* canvas,
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
	return drawText_qt (
		canvas,
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		QString ((const QChar*) text.cp (), text.getLength ())
		);
}

bool
QtEngine::drawText_utf32 (
	Canvas* canvas,
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
	return drawText_qt (
		canvas,
		x,
		y,
		left,
		top,
		right,
		bottom,
		textColor,
		backColor,
		fontFlags,
		QString::fromUcs4 ((const uint*) text.cp (), text.getLength ())
		);
}

bool
QtEngine::drawImage (
	Canvas* canvas,
	int x,
	int y,
	Image* image,
	int left,
	int top,
	int right,
	int bottom
	)
{
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;

	ASSERT (image->getEngine () == this);
	QtImage* qtImage = (QtImage*) image;

	qtCanvas->m_qtPainter.drawImage (
		x,
		y,
		qtImage->m_qtImage,
		left,
		top,
		right - left,
		bottom - top
		);

	return true;
}

bool
QtEngine::copyRect (
	Canvas* canvas,
	int x,
	int y,
	Canvas* srcCanvas,
	int left,
	int top,
	int right,
	int bottom
	)
{
	ASSERT (canvas->getEngine () == this);
	QtCanvas* qtCanvas = (QtCanvas*) canvas;

	ASSERT (srcCanvas->getEngine () == this);
	QtCanvas* qtSrcCanvas = (QtCanvas*) srcCanvas;

	if (!qtSrcCanvas->m_qtPixmap.isNull ())
		return true;

	qtCanvas->m_qtPainter.drawPixmap (
		x,
		y,
		qtSrcCanvas->m_qtPixmap,
		left,
		top,
		right - left,
		bottom - top
		);

	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// font

void
QtEngine::clearFontTuple (FontTuple* fontTuple)
{
	ASSERT (fontTuple->getEngine () == this);
	QtFontTuple* qtFontTuple = (QtFontTuple*) fontTuple;

	for (size_t i = 0; i < countof (qtFontTuple->m_fontModArray); i++)
	{
		Font* font = qtFontTuple->m_fontModArray [i];
		if (!font)
			continue;

		ASSERT (font->getEngine () == this);
		AXL_MEM_DELETE ((QtFont*) font);
	}

	memset (qtFontTuple->m_fontModArray, 0, sizeof (qtFontTuple->m_fontModArray));
}

FontTuple*
QtEngine::getStdFontTuple (StdFontKind fontKind)
{
	ASSERT (fontKind < countof (m_stdFontTupleArray));
	if (m_stdFontTupleArray [fontKind])
		return m_stdFontTupleArray [fontKind];

	QtFontTuple* fontTuple = AXL_MEM_NEW (QtFontTuple);
	QtFont* font = AXL_MEM_NEW (QtFont);
	font->m_tuple = fontTuple;
	m_stdFontTupleArray [fontKind] = fontTuple;
	fontTuple->m_fontModArray [0] = font;

	switch (fontKind)
	{
	case StdFontKind_Gui:
		font->m_qtFont = QApplication::font ();
		break;

	case StdFontKind_Monospace:
#if (_AXL_OS_DARWIN)
		font->m_qtFont = QFont ("Menlo", 11);
#else
		font->m_qtFont = QFont ("Monospace", 9);
#endif
		font->m_qtFont.setFixedPitch (true);
		font->m_qtFont.setKerning (false);
		font->m_qtFont.setStyleHint (
			QFont::Monospace,
			(QFont::StyleStrategy) (QFont::NoFontMerging | QFont::ForceIntegerMetrics)
			);

		break;
	}

	return fontTuple;
}

Font*
QtEngine::createFont (
	FontTuple* fontTuple,
	const sl::StringRef& family,
	size_t pointSize,
	uint_t flags
	)
{
	ASSERT (fontTuple->getEngine () == this);
	QtFontTuple* qtFontTuple = (QtFontTuple*) fontTuple;

	flags &= 0x0f;

	clearFontTuple (qtFontTuple);

	QtFont* qtBaseFont = AXL_MEM_NEW (QtFont);
	qtBaseFont->m_tuple = fontTuple;
	qtBaseFont->m_qtFont = QFont (QString::fromUtf8 (family.cp (), family.getLength ()), pointSize, QFont::Normal);
	qtFontTuple->m_fontModArray [0] = qtBaseFont;

	if (!flags)
		return qtBaseFont;

	QtFont* qtFont = AXL_MEM_NEW (QtFont);
	qtFont->m_tuple = fontTuple;
	qtFont->m_qtFont = qtBaseFont->m_qtFont;
	qtFont->m_qtFont.setBold ((flags & FontFlag_Bold) != 0);
	qtFont->m_qtFont.setItalic ((flags & FontFlag_Italic) != 0);
	qtFont->m_qtFont.setUnderline ((flags & FontFlag_Underline) != 0);
	qtFont->m_qtFont.setStrikeOut ((flags & FontFlag_Strikeout) != 0);

	qtFontTuple->m_fontModArray [flags] = qtFont;
	return qtFont;
}

Font*
QtEngine::getFontMod (
	FontTuple* fontTuple,
	uint_t flags
	)
{
	ASSERT (fontTuple->getEngine () == this);
	QtFontTuple* qtFontTuple = (QtFontTuple*) fontTuple;

	flags &= 0x0f;

	QtFont* qtBaseFont = (QtFont*) qtFontTuple->m_fontModArray [0];
	ASSERT (qtBaseFont);

	QtFont* qtFont = AXL_MEM_NEW (QtFont);
	qtFont->m_tuple = fontTuple;
	qtFont->m_qtFont = qtBaseFont->m_qtFont;
	qtFont->m_qtFont.setBold ((flags & FontFlag_Bold) != 0);
	qtFont->m_qtFont.setItalic ((flags & FontFlag_Italic) != 0);
	qtFont->m_qtFont.setUnderline ((flags & FontFlag_Underline) != 0);
	qtFont->m_qtFont.setStrikeOut ((flags & FontFlag_Strikeout) != 0);

	ASSERT (!qtFontTuple->m_fontModArray [flags]);
	qtFontTuple->m_fontModArray [flags] = qtFont;
	return qtFont;
}

bool
QtEngine::getFontDesc (
	Font* font,
	FontDesc* fontDesc
	)
{
	ASSERT (font->getEngine () == this);
	QtFont* qtFont = (QtFont*) font;
	QFontInfo qtFontInfo (qtFont->m_qtFont);

	QString family = qtFontInfo.family ();
	QByteArray familyUtf = family.toUtf8 ();
	size_t length = familyUtf.size ();

	if (length >= countof (fontDesc->m_family))
		length = countof (fontDesc->m_family) - 1;

	memcpy (fontDesc->m_family, familyUtf, length);
	fontDesc->m_family [length] = 0;
	fontDesc->m_pointSize = qtFontInfo.pointSize ();

	if (qtFontInfo.weight () >= QFont::Bold)
		fontDesc->m_flags |= FontFlag_Bold;

	if (qtFontInfo.italic ())
		fontDesc->m_flags |= FontFlag_Italic;

	if (qtFontInfo.underline ())
		fontDesc->m_flags |= FontFlag_Underline;

	if (qtFontInfo.strikeOut ())
		fontDesc->m_flags |= FontFlag_Strikeout;

	return true;
}

bool
QtEngine::isMonospaceFont (Font* font)
{
	ASSERT (font->getEngine () == this);
	QtFont* qtFont = (QtFont*) font;
	QFontInfo qtFontInfo (qtFont->m_qtFont);

	return qtFontInfo.fixedPitch ();
}

Size
QtEngine::calcTextSize_qt (
	Font* font,
	const QString& string
	)
{
	ASSERT (font->getEngine () == this);
	QtFont* qtFont = (QtFont*) font;
	QFontMetrics qtFontMetrics (qtFont->m_qtFont);

	Size size;
	size.m_width = qtFontMetrics.width (string);
	size.m_height = qtFontMetrics.height ();
	return size;
}

Size
QtEngine::calcTextSize_utf8 (
	Font* font,
	const sl::StringRef_utf8& text
	)
{
	return calcTextSize_qt (font, QString::fromUtf8 (text.cp (), text.getLength ()));
}

Size
QtEngine::calcTextSize_utf16 (
	Font* font,
	const sl::StringRef_utf16& text
	)
{
	return calcTextSize_qt (font, QString ((const QChar*) text.cp (), text.getLength ()));
}

Size
QtEngine::calcTextSize_utf32 (
	Font* font,
	const sl::StringRef_utf32& text
	)
{
	return calcTextSize_qt (font, QString::fromUcs4 ((const uint*) text.cp (), text.getLength ()));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// image

bool
QtEngine::createImage (
	Image* image,
	uint_t width,
	uint_t height,
	PixelFormat pixelFormat
	)
{
	ASSERT (image->getEngine () == this);
	QtImage* qtImage = (QtImage*) image;
	qtImage->m_qtImage = QImage (width, height, QImage::Format_ARGB32_Premultiplied);
	return true;
}

bool
QtEngine::getImageDesc (
	Image* image,
	ImageDesc* imageDesc
	)
{
	ASSERT (image->getEngine () == this);
	QtImage* qtImage = (QtImage*) image;
	imageDesc->m_size.m_width = qtImage->m_qtImage.width ();
	imageDesc->m_size.m_height = qtImage->m_qtImage.height ();
	imageDesc->m_pixelFormat = PixelFormat_Rgba;
	return true;
}

Cursor*
QtEngine::getStdCursor (StdCursorKind cursorKind)
{
	ASSERT (cursorKind < countof (m_stdCursorArray));
	if (m_stdCursorArray [cursorKind])
		return m_stdCursorArray [cursorKind];

	static Qt::CursorShape stdCursorShapeTable [StdCursorKind__Count] =
	{
		Qt::ArrowCursor,         // StdCursorKind_Arrow = 0,
		Qt::WaitCursor,          // StdCursorKind_Wait,
		Qt::IBeamCursor,         // StdCursorKind_IBeam,
		Qt::PointingHandCursor,  // StdCursorKind_Hyperlink,
		Qt::SizeVerCursor,       // StdCursorKind_SizeNS,
		Qt::SizeHorCursor,       // StdCursorKind_SizeWE,
		Qt::SizeFDiagCursor,     // StdCursorKind_SizeNWSE,
		Qt::SizeBDiagCursor,     // StdCursorKind_SizeNESW,
		Qt::SizeAllCursor,       // StdCursorKind_SizeAll,
	};

	ASSERT (cursorKind < countof (stdCursorShapeTable));
	Qt::CursorShape cursorShape = stdCursorShapeTable [cursorKind];

	QtCursor* cursor = AXL_MEM_NEW (QtCursor);
	cursor->m_qtCursor = QCursor (cursorShape);
	m_stdCursorArray [cursorKind] = cursor;
	return cursor;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// clipboard

uintptr_t
QtEngine::registerClipboardFormat (const sl::StringRef& formatName)
{
	sl::StringHashTableMapIterator <uintptr_t> it = m_clipboardFormatNameMap.find (formatName);
	if (it)
		return it->m_value;

	size_t count = m_clipboardFormatNameTable.getCount ();
	m_clipboardFormatNameTable.append (formatName);
	m_clipboardFormatNameMap [formatName] = count;
	return count;
}

bool
QtEngine::readClipboard (sl::String* string)
{
	QClipboard* qtClipboard = QApplication::clipboard ();
	QString qtString = qtClipboard->text ();
	QByteArray data = qtString.toUtf8 ();
	string->copy (data.constData (), data.size ());
	return true;
}

bool
QtEngine::readClipboard (
	uintptr_t format,
	sl::Array <char>* data
	)
{
	size_t count = m_clipboardFormatNameTable.getCount ();
	if (format >= count)
	{
		err::setError (err::SystemErrorCode_InvalidParameter);
		return false;
	}

	const char* formatName = m_clipboardFormatNameTable [format];

	QClipboard* qtClipboard = QApplication::clipboard ();
	const QMimeData* mimeData = qtClipboard->mimeData ();

	QByteArray qtData = mimeData->data (formatName);
	data->copy (qtData.constData (), qtData.size ());
	return true;
}

bool
QtEngine::writeClipboard (const sl::StringRef& string)
{
	if (!m_qtClipboardMimeData)
		m_qtClipboardMimeData = new QMimeData;

	m_qtClipboardMimeData->setText (QString::fromUtf8 (string.cp (), string.getLength ()));
	return true;
}

bool
QtEngine::writeClipboard (
	uintptr_t format,
	const void* data,
	size_t size
	)
{
	size_t count = m_clipboardFormatNameTable.getCount ();
	if (format >= count)
	{
		err::setError (err::SystemErrorCode_InvalidParameter);
		return false;
	}

	const char* formatName = m_clipboardFormatNameTable [format];

	if (!m_qtClipboardMimeData)
		m_qtClipboardMimeData = new QMimeData;

	m_qtClipboardMimeData->setData (formatName, QByteArray ((const char*) data, size));
	return true;
}

bool
QtEngine::commitClipboard ()
{
	if (!m_qtClipboardMimeData)
		return false;

	QClipboard* qtClipboard = QApplication::clipboard ();
	qtClipboard->setMimeData (m_qtClipboardMimeData);

	m_qtClipboardMimeData = NULL;
	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// widget

bool
QtEngine::isWidgetFocused (WidgetDriver* widgetDriver)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	return qtWidget->viewport ()->hasFocus ();
}

bool
QtEngine::setWidgetFocus (WidgetDriver* widgetDriver)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->viewport ()->setFocus ();
	return true;
}

bool
QtEngine::redrawWidget (
	WidgetDriver* widgetDriver,
	int left,
	int top,
	int right,
	int bottom
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();

	if (left == right || top == bottom)
		qtWidget->viewport ()->update ();
	else
		qtWidget->viewport ()->update (left, top, right - left, bottom - top);

	return true;
}

bool
QtEngine::scrollWidget (
	WidgetDriver* widgetDriver,
	int dx,
	int dy
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();

	qtWidget->viewport ()->scroll (dx, dy);
	return true;
}

bool
QtEngine::scrollWidgetRect (
	WidgetDriver* widgetDriver,
	int left,
	int top,
	int right,
	int bottom,
	int dx,
	int dy
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();

	qtWidget->viewport ()->scroll (dx, dy, QRect (left, top, right - left, bottom - top));
	return true;
}

bool
QtEngine::setWidgetCursor (
	WidgetDriver* widgetDriver,
	Cursor* cursor
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();

	ASSERT (cursor->getEngine () == this);
	QtCursor* qtCursor = (QtCursor*) cursor;

	qtWidget->viewport ()->setCursor (qtCursor->m_qtCursor);
	return true;
}

bool
QtEngine::setMouseCapture (WidgetDriver* widgetDriver)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->viewport ()->grabMouse ();
	return true;
}

bool
QtEngine::releaseMouse (WidgetDriver* widgetDriver)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->viewport ()->releaseMouse ();
	return true;
}

bool
QtEngine::updateWidgetScrollBar (
	WidgetDriver* widgetDriver,
	Orientation orientation
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();

	ASSERT ((size_t) orientation < 2);
	const WidgetScrollBar* scrollBar = &widgetDriver->m_scrollBarArray [orientation];

	QScrollBar* qtScrollBar = orientation == Orientation_Horizontal ?
		qtWidget->horizontalScrollBar () :
		qtWidget->verticalScrollBar ();

	size_t maximum = scrollBar->m_page < scrollBar->m_end ?
		scrollBar->m_end - scrollBar->m_page :
		0;

	qtScrollBar->setPageStep (scrollBar->m_page);
	qtScrollBar->setMaximum (maximum);
	qtScrollBar->setValue (scrollBar->m_pos);
	return true;
}

void
QtEngine::sendWidgetNotification (
	WidgetDriver* widgetDriver,
	uint_t code,
	const void* params
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->emitNotificationSignal (code, params);
}

bool
QtEngine::postWidgetThreadMsg (
	WidgetDriver* widgetDriver,
	uint_t code,
	const ref::Ptr <void>& params
	)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->postThreadMsg (code, params);
	return true;
}

bool
QtEngine::startWidgetAnimation (WidgetDriver* widgetDriver)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->m_animationTimer.start (10, qtWidget);
	return true;
}

void
QtEngine::stopWidgetAnimation (WidgetDriver* widgetDriver)
{
	ASSERT (widgetDriver->getEngine () == this);
	QtWidgetBase* qtWidget = (QtWidgetBase*) widgetDriver->getEngineWidget ();
	qtWidget->m_animationTimer.stop ();
}

//..............................................................................

} // namespace gui
} // namespace axl
