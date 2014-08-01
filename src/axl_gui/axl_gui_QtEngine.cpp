#include "pch.h"
#include "axl_gui_QtEngine.h"
#include "axl_ref_Factory.h"
#include "axl_err_Error.h"

namespace axl {
namespace gui {

//.............................................................................

bool
CQtCaret::Show (
	CWidget* pWidget,
	const TRect& Rect
	)
{
	ASSERT (pWidget);

	if (m_pWidget && m_IsVisible)
		m_pWidget->Redraw (m_Rect);

	m_pWidget = pWidget;
	m_Rect = Rect;
	m_IsVisible = true;
	
	pWidget->Redraw (Rect);
	
	setSingleShot (false);
	start (500);
	return true;
}

void
CQtCaret::Hide ()
{
	if (!m_pWidget)
		return;

	if (m_IsVisible)
		m_pWidget->Redraw (m_Rect);

	m_pWidget = NULL;
	m_IsVisible = false;
	stop ();
}

void
CQtCaret::timerEvent  (QTimerEvent* e)
{
	if (!m_pWidget)
		return;

	m_IsVisible = !m_IsVisible;
	m_pWidget->Redraw (m_Rect);
}

//.............................................................................

CQtEngine*
GetQtEngineSingleton ()
{
	return CQtEngine::GetSingleton ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ref::CPtrT <CFont>
CQtEngine::CreateStdFont (EStdFont FontKind)
{
	switch (FontKind)
	{
	case EStdFont_Gui:
		return CreateFont (QApplication::font ());

	case EStdFont_Monospace:
		{
		QFont QtFont ("Monospace", 10);
		QtFont.setStyleHint (QFont::TypeWriter, QFont::NoFontMerging);
		return CreateFont (QtFont);
		}

	default:
		return ref::EPtr_Null;
	}
}

QFont
CQtEngine::CreateQtFont (
	const char* pFaceName,
	size_t PointSize,
	uint_t Flags
	)
{
	QString FamilyName = QString::fromUtf8 (pFaceName);

	int Weight = (Flags & EFontFlag_Bold) ? QFont::Bold : QFont::Normal;
	bool IsItalic = (Flags & EFontFlag_Italic) != 0;

	QFont QtFont (FamilyName, PointSize, Weight, IsItalic);

	if (Flags & EFontFlag_Underline)
		QtFont.setUnderline (true);

	if (Flags & EFontFlag_Strikeout)
		QtFont.setStrikeOut (true);

	return QtFont;
}

ref::CPtrT <CFont>
CQtEngine::CreateFont (const QFont& QtFont)
{
	CQtFont* pFont = AXL_MEM_NEW (CQtFont);
	pFont->m_QtFont = QtFont;

	GetFontDescFromFontInfo (QFontInfo (QtFont), &pFont->m_FontDesc);

	ref::CPtrT <CQtFontTuple> FontTuple = AXL_REF_NEW (CQtFontTuple);
	FontTuple->m_pBaseFont = pFont;
	FontTuple->m_FontModArray [pFont->m_FontDesc.m_Flags] = pFont;

	pFont->m_pTuple = FontTuple;

	return ref::CPtrT <CFont> (pFont, FontTuple);
}

CFont*
CQtEngine::GetFontMod (
	CFont* _pBaseFont,
	uint_t Flags
	)
{
	ASSERT (_pBaseFont->GetEngine () == this);

	CQtFont* pBaseFont = (CQtFont*) _pBaseFont;
	CQtFontTuple* pFontTuple = (CQtFontTuple*) pBaseFont->m_pTuple;

	TFontDesc FontDesc = *pBaseFont->GetFontDesc ();

	CQtFont* pFont = AXL_MEM_NEW (CQtFont);
	pFont->m_FontDesc = FontDesc;
	pFont->m_FontDesc.m_Flags = Flags;
	pFont->m_QtFont = CreateQtFont (FontDesc.m_FaceName, FontDesc.m_PointSize, Flags);

	ASSERT (!(Flags & EFontFlag_Transparent) && Flags < countof (pFontTuple->m_FontModArray));
	ASSERT (!pFontTuple->m_FontModArray [Flags]);

	pFontTuple->m_FontModArray [Flags] = pFont;
	return pFont;
}

ref::CPtrT <CCursor>
CQtEngine::CreateStdCursor (EStdCursor CursorKind)
{
	static Qt::CursorShape StdCursorShapeTable [EStdCursor__Count] =
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

	ASSERT (CursorKind < EStdCursor__Count);
	return CreateCursor (StdCursorShapeTable [CursorKind]);
}

ref::CPtrT <CCursor>
CQtEngine::CreateCursor (const QCursor& QtCursor)
{
	ref::CPtrT <CQtCursor> Cursor = AXL_REF_NEW (ref::CBoxT <CQtCursor>);
	Cursor->m_QtCursor = QtCursor;
	return Cursor;
}

ref::CPtrT <CImage>
CQtEngine::CreateImage ()
{
	ref::CPtrT <CQtImage> Image = AXL_REF_NEW (ref::CBoxT <CQtImage>);
	return Image;
}

ref::CPtrT <CImage>
CQtEngine::CreateImage (
	int Width,
	int Height,
	EPixelFormat PixelFormat,
	const void* pData,
	bool IsScreenCompatible
	)
{
	uint_t BitCount;

	switch (PixelFormat)
	{
	case EPixelFormat_Rgba:
		BitCount = 32;
		break;

	case EPixelFormat_Rgb:
		BitCount = 24;
		break;

	default:
		err::SetFormatStringError ("unsupported pixel format '%s'", GetPixelFormatString (PixelFormat));
	};

	QPixmap QtPixmap;

/*
	if (!IsScreenCompatible)
	{
		hBitmap = ::CreateBitmap (
			Width,
			Height,
			1,
			BitCount,
			pData
			);

		if (!hBitmap)
			return err::FailWithLastSystemError (ref::EPtr_Null);
	}
	else
	{
		BITMAPINFO BitmapInfo = { 0 };
		BitmapInfo.bmiHeader.biSize = sizeof (BitmapInfo.bmiHeader);
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biBitCount = BitCount;
		BitmapInfo.bmiHeader.biCompression = BI_RGB;
		BitmapInfo.bmiHeader.biWidth = Width;
		BitmapInfo.bmiHeader.biHeight = Height;

		CScreenDc ScreenDc;

		hBitmap = ::CreateCompatibleBitmap (
			ScreenDc,
			Width,
			Height
			);

		if (!hBitmap)
			return err::FailWithLastSystemError (ref::EPtr_Null);

		bool_t Result = SetDIBits (
			ScreenDc,
			hBitmap,
			0,
			Height,
			pData,
			&BitmapInfo,
			DIB_RGB_COLORS
			);

		if (!Result)
			return err::FailWithLastSystemError (ref::EPtr_Null);
	} */

	ref::CPtrT <CQtImage> Image = AXL_REF_NEW (ref::CBoxT <CQtImage>);
	Image->m_QtPixmap = QtPixmap;
	return Image;
}

ref::CPtrT <CCanvas>
CQtEngine::CreateOffscreenCanvas (
	int Width,
	int Height
	)
{
	return ref::EPtr_Null;
}

uintptr_t 
CQtEngine::RegisterClipboardFormat (const rtl::CString& FormatName)
{
	rtl::CStringHashTableMapIteratorT <uintptr_t> It = m_ClipboardFormatNameMap.Find (FormatName);
	if (It)
		return It->m_Value;

	size_t Count = m_ClipboardFormatTable.GetCount ();
	m_ClipboardFormatTable.Append (FormatName);
	m_ClipboardFormatNameMap [FormatName] = Count;
	return Count;
}

bool
CQtEngine::ReadClipboard (rtl::CString* pString)
{
	QClipboard* pQtClipboard = QApplication::clipboard ();
	QString QtString = pQtClipboard->text ();
	QByteArray Data = QtString.toUtf8 ();
	pString->Copy (Data.constData (), Data.size ());
	return true;
}

bool
CQtEngine::ReadClipboard (
	uintptr_t Format,
	rtl::CArrayT <char>* pData
	)
{
	size_t Count = m_ClipboardFormatTable.GetCount ();
	if (Format >= Count)
	{
		err::SetError (err::EStatus_InvalidParameter);
		return false;
	}

	const char* pFormat = m_ClipboardFormatTable [Format];

	QClipboard* pQtClipboard = QApplication::clipboard ();
	const QMimeData* mimeData = pQtClipboard->mimeData ();

	QByteArray data = mimeData->data (pFormat);
	size_t Size = data.size ();	
	pData->Copy (data.constData (), Size);
	return true;
}

bool
CQtEngine::WriteClipboard (
	const char* pString,
	size_t Length
	)
{
	QString String = QString::fromUtf8 (pString, Length);

	if (!m_pQtClipboardMimeData)
		m_pQtClipboardMimeData = new QMimeData;

	m_pQtClipboardMimeData->setText (String);
	return true;
}

bool
CQtEngine::WriteClipboard (
	uintptr_t Format,
	const void* pData,
	size_t Size
	)
{
	size_t Count = m_ClipboardFormatTable.GetCount ();
	if (Format >= Count)
	{
		err::SetError (err::EStatus_InvalidParameter);
		return false;
	}

	const char* pFormat = m_ClipboardFormatTable [Format];

	if (!m_pQtClipboardMimeData)
		m_pQtClipboardMimeData = new QMimeData;

	m_pQtClipboardMimeData->setData (pFormat, QByteArray ((const char*) pData, Size));
	return true;
}

bool
CQtEngine::CommitClipboard ()
{
	if (!m_pQtClipboardMimeData)
		return false;

	QClipboard* pQtClipboard = QApplication::clipboard ();
	pQtClipboard->setMimeData (m_pQtClipboardMimeData);

	m_pQtClipboardMimeData = NULL;
	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
