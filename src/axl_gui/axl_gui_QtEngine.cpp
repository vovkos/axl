#include "pch.h"
#include "axl_gui_QtEngine.h"
#include "axl_ref_Factory.h"
#include "axl_err_Error.h"

namespace axl {
namespace gui {

//.............................................................................

bool
QtCaret::show (
	Widget* widget,
	const Rect& rect
	)
{
	ASSERT (widget);

	if (m_widget && m_isVisible)
		m_widget->redraw (m_rect);

	m_widget = widget;
	m_rect = rect;
	m_isVisible = true;
	
	widget->redraw (rect);
	
	setSingleShot (false);
	start (500);
	return true;
}

void
QtCaret::hide ()
{
	if (!m_widget)
		return;

	if (m_isVisible)
		m_widget->redraw (m_rect);

	m_widget = NULL;
	m_isVisible = false;
	stop ();
}

void
QtCaret::timerEvent  (QTimerEvent* e)
{
	if (!m_widget)
		return;

	m_isVisible = !m_isVisible;
	m_widget->redraw (m_rect);
}

//.............................................................................

QtEngine*
getQtEngineSingleton ()
{
	return QtEngine::getSingleton ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ref::Ptr <Font>
QtEngine::createStdFont (StdFontKind fontKind)
{
	switch (fontKind)
	{
	case StdFontKind_Gui:
		return createFont (QApplication::font ());

	case StdFontKind_Monospace:
		{
		QFont qtFont ("Monospace", 10);
		qtFont.setStyleHint (QFont::TypeWriter, QFont::NoFontMerging);
		return createFont (qtFont);
		}

	default:
		return ref::PtrKind_Null;
	}
}

QFont
QtEngine::createQtFont (
	const char* faceName,
	size_t pointSize,
	uint_t flags
	)
{
	QString familyName = QString::fromUtf8 (faceName);

	int weight = (flags & FontFlag_Bold) ? QFont::Bold : QFont::Normal;
	bool isItalic = (flags & FontFlag_Italic) != 0;

	QFont qtFont (familyName, pointSize, weight, isItalic);

	if (flags & FontFlag_Underline)
		qtFont.setUnderline (true);

	if (flags & FontFlag_Strikeout)
		qtFont.setStrikeOut (true);

	return qtFont;
}

ref::Ptr <Font>
QtEngine::createFont (const QFont& qtFont)
{
	QtFont* font = AXL_MEM_NEW (QtFont);
	font->m_qtFont = qtFont;

	getFontDescFromFontInfo (QFontInfo (qtFont), &font->m_fontDesc);

	ref::Ptr <QtFontuple> fontTuple = AXL_REF_NEW (QtFontuple);
	fontTuple->m_baseFont = font;
	fontTuple->m_fontModArray [font->m_fontDesc.m_flags] = font;

	font->m_tuple = fontTuple;

	return ref::Ptr <Font> (font, fontTuple);
}

Font*
QtEngine::getFontMod (
	Font* _pBaseFont,
	uint_t flags
	)
{
	ASSERT (_pBaseFont->getEngine () == this);

	QtFont* baseFont = (QtFont*) _pBaseFont;
	QtFontuple* fontTuple = (QtFontuple*) baseFont->m_tuple;

	FontDesc fontDesc = *baseFont->getFontDesc ();

	QtFont* font = AXL_MEM_NEW (QtFont);
	font->m_fontDesc = fontDesc;
	font->m_fontDesc.m_flags = flags;
	font->m_qtFont = createQtFont (fontDesc.m_faceName, fontDesc.m_pointSize, flags);

	ASSERT (!(flags & FontFlag_Transparent) && flags < countof (fontTuple->m_fontModArray));
	ASSERT (!fontTuple->m_fontModArray [flags]);

	fontTuple->m_fontModArray [flags] = font;
	return font;
}

ref::Ptr <Cursor>
QtEngine::createStdCursor (StdCursorKind cursorKind)
{
	static Qt::CursorShape stdCursorShapeTable [StdCursorKind__Count] =
	{
		Qt::ArrowCursor,         // EStdCursor_Arrow = 0,
		Qt::WaitCursor,          // EStdCursor_Wait,
		Qt::IBeamCursor,         // EStdCursor_IBeam,
		Qt::PointingHandCursor,  // EStdCursor_Hyperlink,
		Qt::SizeVerCursor,       // EStdCursor_SizeNS,
		Qt::SizeHorCursor,       // EStdCursor_SizeWE,
		Qt::SizeFDiagCursor,     // EStdCursor_SizeNWSE,
		Qt::SizeBDiagCursor,     // EStdCursor_SizeNESW,
		Qt::SizeAllCursor,       // EStdCursor_SizeAll,
	};

	ASSERT (cursorKind < StdCursorKind__Count);
	return createCursor (stdCursorShapeTable [cursorKind]);
}

ref::Ptr <Cursor>
QtEngine::createCursor (const QCursor& qtCursor)
{
	ref::Ptr <QtCursor> cursor = AXL_REF_NEW (ref::Box <QtCursor>);
	cursor->m_qtCursor = qtCursor;
	return cursor;
}

ref::Ptr <Image>
QtEngine::createImage ()
{
	ref::Ptr <QtImage> image = AXL_REF_NEW (ref::Box <QtImage>);
	return image;
}

ref::Ptr <Image>
QtEngine::createImage (
	int width,
	int height,
	PixelFormat pixelFormat,
	const void* data,
	bool isScreenCompatible
	)
{
	uint_t bitCount;

	switch (pixelFormat)
	{
	case PixelFormat_Rgba:
		bitCount = 32;
		break;

	case PixelFormat_Rgb:
		bitCount = 24;
		break;

	default:
		err::setFormatStringError ("unsupported pixel format '%s'", getPixelFormatString (pixelFormat));
	};

	QPixmap qtPixmap;

/*
	if (!isScreenCompatible)
	{
		hBitmap = ::CreateBitmap (
			width,
			height,
			1,
			bitCount,
			data
			);

		if (!hBitmap)
			return err::failWithLastSystemError (ref::PtrKind_Null);
	}
	else
	{
		BITMAPINFO bitmapInfo = { 0 };
		bitmapInfo.bmiHeader.biSize = sizeof (bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = bitCount;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = width;
		bitmapInfo.bmiHeader.biHeight = height;

		ScreenDc screenDc;

		hBitmap = ::CreateCompatibleBitmap (
			screenDc,
			width,
			height
			);

		if (!hBitmap)
			return err::failWithLastSystemError (ref::PtrKind_Null);

		bool_t result = setDIBits (
			screenDc,
			hBitmap,
			0,
			height,
			data,
			&bitmapInfo,
			DIB_RGB_COLORS
			);

		if (!result)
			return err::failWithLastSystemError (ref::PtrKind_Null);
	} */

	ref::Ptr <QtImage> image = AXL_REF_NEW (ref::Box <QtImage>);
	image->m_qtPixmap = qtPixmap;
	return image;
}

ref::Ptr <Canvas>
QtEngine::createOffscreenCanvas (
	int width,
	int height
	)
{
	return ref::PtrKind_Null;
}

uintptr_t 
QtEngine::registerClipboardFormat (const rtl::String& formatName)
{
	rtl::StringHashTableMapIterator <uintptr_t> it = m_clipboardFormatNameMap.find (formatName);
	if (it)
		return it->m_value;

	size_t count = m_clipboardFormatNameTable.getCount ();
	m_clipboardFormatNameTable.append (formatName);
	m_clipboardFormatNameMap [formatName] = count;
	return count;
}

bool
QtEngine::readClipboard (rtl::String* string)
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
	rtl::Array <char>* data
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
QtEngine::writeClipboard (
	const char* string,
	size_t length
	)
{
	if (!m_qtClipboardMimeData)
		m_qtClipboardMimeData = new QMimeData;

	QString qtString = QString::fromUtf8 (string, length);
	m_qtClipboardMimeData->setText (qtString);
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

//.............................................................................

} // namespace gui
} // namespace axl
