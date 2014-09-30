#include "pch.h"
#include "axl_gui_QtEngine.h"
#include "axl_ref_Factory.h"
#include "axl_err_Error.h"

namespace axl {
namespace gui {

//.............................................................................

bool
CQtCaret::show (
	CWidget* widget,
	const TRect& rect
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
CQtCaret::hide ()
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
CQtCaret::timerEvent  (QTimerEvent* e)
{
	if (!m_widget)
		return;

	m_isVisible = !m_isVisible;
	m_widget->redraw (m_rect);
}

//.............................................................................

CQtEngine*
getQtEngineSingleton ()
{
	return CQtEngine::getSingleton ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ref::CPtrT <CFont>
CQtEngine::createStdFont (EStdFont fontKind)
{
	switch (fontKind)
	{
	case EStdFont_Gui:
		return createFont (QApplication::font ());

	case EStdFont_Monospace:
		{
		QFont qtFont ("Monospace", 10);
		qtFont.setStyleHint (QFont::TypeWriter, QFont::NoFontMerging);
		return createFont (qtFont);
		}

	default:
		return ref::EPtr_Null;
	}
}

QFont
CQtEngine::createQtFont (
	const char* faceName,
	size_t pointSize,
	uint_t flags
	)
{
	QString familyName = QString::fromUtf8 (faceName);

	int weight = (flags & EFontFlag_Bold) ? QFont::Bold : QFont::Normal;
	bool isItalic = (flags & EFontFlag_Italic) != 0;

	QFont qtFont (familyName, pointSize, weight, isItalic);

	if (flags & EFontFlag_Underline)
		qtFont.setUnderline (true);

	if (flags & EFontFlag_Strikeout)
		qtFont.setStrikeOut (true);

	return qtFont;
}

ref::CPtrT <CFont>
CQtEngine::createFont (const QFont& qtFont)
{
	CQtFont* font = AXL_MEM_NEW (CQtFont);
	font->m_qtFont = qtFont;

	getFontDescFromFontInfo (QFontInfo (qtFont), &font->m_fontDesc);

	ref::CPtrT <CQtFontTuple> fontTuple = AXL_REF_NEW (CQtFontTuple);
	fontTuple->m_baseFont = font;
	fontTuple->m_fontModArray [font->m_fontDesc.m_flags] = font;

	font->m_tuple = fontTuple;

	return ref::CPtrT <CFont> (font, fontTuple);
}

CFont*
CQtEngine::getFontMod (
	CFont* _pBaseFont,
	uint_t flags
	)
{
	ASSERT (_pBaseFont->getEngine () == this);

	CQtFont* baseFont = (CQtFont*) _pBaseFont;
	CQtFontTuple* fontTuple = (CQtFontTuple*) baseFont->m_tuple;

	TFontDesc fontDesc = *baseFont->getFontDesc ();

	CQtFont* font = AXL_MEM_NEW (CQtFont);
	font->m_fontDesc = fontDesc;
	font->m_fontDesc.m_flags = flags;
	font->m_qtFont = createQtFont (fontDesc.m_faceName, fontDesc.m_pointSize, flags);

	ASSERT (!(flags & EFontFlag_Transparent) && flags < countof (fontTuple->m_fontModArray));
	ASSERT (!fontTuple->m_fontModArray [flags]);

	fontTuple->m_fontModArray [flags] = font;
	return font;
}

ref::CPtrT <CCursor>
CQtEngine::createStdCursor (EStdCursor cursorKind)
{
	static Qt::CursorShape stdCursorShapeTable [EStdCursor__Count] =
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

	ASSERT (cursorKind < EStdCursor__Count);
	return createCursor (stdCursorShapeTable [cursorKind]);
}

ref::CPtrT <CCursor>
CQtEngine::createCursor (const QCursor& qtCursor)
{
	ref::CPtrT <CQtCursor> cursor = AXL_REF_NEW (ref::CBoxT <CQtCursor>);
	cursor->m_qtCursor = qtCursor;
	return cursor;
}

ref::CPtrT <CImage>
CQtEngine::createImage ()
{
	ref::CPtrT <CQtImage> image = AXL_REF_NEW (ref::CBoxT <CQtImage>);
	return image;
}

ref::CPtrT <CImage>
CQtEngine::createImage (
	int width,
	int height,
	EPixelFormat pixelFormat,
	const void* data,
	bool isScreenCompatible
	)
{
	uint_t bitCount;

	switch (pixelFormat)
	{
	case EPixelFormat_Rgba:
		bitCount = 32;
		break;

	case EPixelFormat_Rgb:
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
			return err::failWithLastSystemError (ref::EPtr_Null);
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

		CScreenDc screenDc;

		hBitmap = ::CreateCompatibleBitmap (
			screenDc,
			width,
			height
			);

		if (!hBitmap)
			return err::failWithLastSystemError (ref::EPtr_Null);

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
			return err::failWithLastSystemError (ref::EPtr_Null);
	} */

	ref::CPtrT <CQtImage> image = AXL_REF_NEW (ref::CBoxT <CQtImage>);
	image->m_qtPixmap = qtPixmap;
	return image;
}

ref::CPtrT <CCanvas>
CQtEngine::createOffscreenCanvas (
	int width,
	int height
	)
{
	return ref::EPtr_Null;
}

uintptr_t 
CQtEngine::registerClipboardFormat (const rtl::CString& formatName)
{
	rtl::CStringHashTableMapIteratorT <uintptr_t> it = m_clipboardFormatNameMap.find (formatName);
	if (it)
		return it->m_value;

	size_t count = m_clipboardFormatNameTable.getCount ();
	m_clipboardFormatNameTable.append (formatName);
	m_clipboardFormatNameMap [formatName] = count;
	return count;
}

bool
CQtEngine::readClipboard (rtl::CString* string)
{
	QClipboard* qtClipboard = QApplication::clipboard ();
	QString qtString = qtClipboard->text ();
	QByteArray data = qtString.toUtf8 ();
	string->copy (data.constData (), data.size ());
	return true;
}

bool
CQtEngine::readClipboard (
	uintptr_t format,
	rtl::CArrayT <char>* data
	)
{
	size_t count = m_clipboardFormatNameTable.getCount ();
	if (format >= count)
	{
		err::setError (err::EStatus_InvalidParameter);
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
CQtEngine::writeClipboard (
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
CQtEngine::writeClipboard (
	uintptr_t format,
	const void* data,
	size_t size
	)
{
	size_t count = m_clipboardFormatNameTable.getCount ();
	if (format >= count)
	{
		err::setError (err::EStatus_InvalidParameter);
		return false;
	}

	const char* formatName = m_clipboardFormatNameTable [format];

	if (!m_qtClipboardMimeData)
		m_qtClipboardMimeData = new QMimeData;

	m_qtClipboardMimeData->setData (formatName, QByteArray ((const char*) data, size));
	return true;
}

bool
CQtEngine::commitClipboard ()
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
