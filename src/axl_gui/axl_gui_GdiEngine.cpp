#include "pch.h"
#include "axl_gui_GdiEngine.h"
#include "axl_ref_Factory.h"
#include "axl_err_Error.h"
#include "axl_gui_Widget.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiEngine::~CGdiEngine ()
{
	if (m_hWndClipboardOwner)
		::DestroyWindow (m_hWndClipboardOwner);
}

ref::CPtrT <CFont>
CGdiEngine::createStdFont (EStdFont fontKind)
{
	LOGFONTW logFont;
	HFONT hFont;

	switch (fontKind)
	{
	case EStdFont_Gui:
		return createStockFont (DEFAULT_GUI_FONT);

	case EStdFont_Monospace:
		buildLogFont (&logFont, L"Courier New", 10);
		logFont.lfPitchAndFamily = FIXED_PITCH;

		hFont = ::CreateFontIndirectW (&logFont);
		ASSERT (hFont);
		
		return createFont (hFont);

	default:
		return ref::EPtr_Null;
	}
}

ref::CPtrT <CFont>
CGdiEngine::createFont (
	const char* faceName,
	size_t pointSize,
	uint_t flags
	)
{
	LOGFONTW logFont;
	buildLogFont (&logFont, rtl::CString_w (faceName), pointSize, flags);

	HFONT hFont = ::CreateFontIndirect (&logFont);
	if (!hFont)
		return err::failWithLastSystemError (ref::EPtr_Null);

	return createFont (hFont);
}

ref::CPtrT <CFont>
CGdiEngine::createStockFont (int stockFontKind)
{
	HGDIOBJ h = ::GetStockObject (stockFontKind);
	dword_t gdiObjectType = ::GetObjectType (h);
	if (gdiObjectType != OBJ_FONT)
	{
		err::setError (err::EStatus_InvalidHandle);
		return ref::EPtr_Null;
	}

	return createFont ((HFONT) h);
}

ref::CPtrT <CFont>
CGdiEngine::createFont (HFONT hFont)
{
	CGdiFont* font = AXL_MEM_NEW (CGdiFont);

	LOGFONTW logFont;
	::GetObjectW (hFont, sizeof (logFont), &logFont);
	getFontDescFromLogFont (&logFont, &font->m_fontDesc);

	ref::CPtrT <CGdiFontTuple> fontTuple = AXL_REF_NEW (CGdiFontTuple);
	fontTuple->m_baseFont = font;
	fontTuple->m_fontModArray [font->m_fontDesc.m_flags] = font;

	font->m_h = hFont;
	font->m_tuple = fontTuple;

	return ref::CPtrT <CFont> (font, fontTuple);
}

CFont*
CGdiEngine::getFontMod (
	CFont* _pBaseFont,
	uint_t flags
	)
{
	ASSERT (_pBaseFont->getEngine () == this);

	CGdiFont* baseFont = (CGdiFont*) _pBaseFont;
	CGdiFontTuple* fontTuple = (CGdiFontTuple*) baseFont->m_tuple;

	LOGFONTW logFont;
	getLogFontFromFontDesc (*fontTuple->m_baseFont->getFontDesc (), &logFont);
	modifyLogFont (&logFont, flags);

	HFONT hFont = ::CreateFontIndirect (&logFont);
	if (!hFont)
		return err::failWithLastSystemError ((CFont*) NULL);

	CGdiFont* font = AXL_MEM_NEW (CGdiFont);
	font->m_fontDesc = baseFont->m_fontDesc;
	font->m_fontDesc.m_flags = flags;
	font->m_h = hFont;

	ASSERT (!(flags & EFontFlag_Transparent) && flags < countof (fontTuple->m_fontModArray));
	ASSERT (!fontTuple->m_fontModArray [flags]);

	fontTuple->m_fontModArray [flags] = font;
	return font;
}

ref::CPtrT <CCursor>
CGdiEngine::createStockCursor (LPCTSTR stockCursorRes)
{
	HCURSOR h = ::LoadCursor (NULL, stockCursorRes);
	if (!h)
		return err::failWithLastSystemError (ref::EPtr_Null);

	ref::CPtrT <CGdiCursor> cursor = AXL_REF_NEW (ref::CBoxT <CGdiCursor>);
	cursor->m_h = h;
	return cursor;
}

ref::CPtrT <CCursor>
CGdiEngine::createStdCursor (EStdCursor cursorKind)
{
	static LPCTSTR stockCursorResTable [EStdCursor__Count] =
	{
		IDC_ARROW,    // EStdCursor_Arrow = 0,
		IDC_WAIT,     // EStdCursor_Wait,
		IDC_IBEAM,    // EStdCursor_IBeam,
		IDC_HAND,     // EStdCursor_Hyperlink,
		IDC_SIZENS,   // EStdCursor_SizeNS,
		IDC_SIZEWE,   // EStdCursor_SizeWE,
		IDC_SIZENWSE, // EStdCursor_SizeNWSE,
		IDC_SIZENESW, // EStdCursor_SizeNESW,
		IDC_SIZEALL,  // EStdCursor_SizeAll,
	};

	ASSERT (cursorKind < EStdCursor__Count);
	return createStockCursor (stockCursorResTable [cursorKind]);
}

ref::CPtrT <CImage>
CGdiEngine::createImage ()
{
	ref::CPtrT <CGdiImage> image = AXL_REF_NEW (ref::CBoxT <CGdiImage>);
	return image;
}

ref::CPtrT <CImage>
CGdiEngine::createImage (
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

	HBITMAP hBitmap;

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

		bool_t result = ::SetDIBits (
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
	}

	ref::CPtrT <CGdiImage> image = AXL_REF_NEW (ref::CBoxT <CGdiImage>);
	image->m_h = hBitmap;
	return image;
}

ref::CPtrT <CCanvas>
CGdiEngine::createOffscreenCanvas (
	int width,
	int height
	)
{
	CScreenDc screenDc;
	HBITMAP hBitmap = ::CreateCompatibleBitmap (screenDc, width, height);
	if (!hBitmap)
		return err::failWithLastSystemError (ref::EPtr_Null);

	HDC hdc = ::CreateCompatibleDC (screenDc);

	ref::CPtrT <CGdiCanvas> dc = AXL_REF_NEW (ref::CBoxT <CGdiCanvas>);
	dc->attach (hdc, NULL, CGdiCanvas::EDestruct_DeleteDc);
	dc->m_hBitmap = hBitmap;
	dc->m_hPrevBitmap = (HBITMAP) ::SelectObject (hdc, hBitmap);

	return dc;
}

uintptr_t 
CGdiEngine::registerClipboardFormat (const rtl::CString& formatName)
{
	err::setError (err::EStatus_NotImplemented);
	return -1;
}

bool
CGdiEngine::readClipboard (rtl::CString* string)
{
	bool result = openClipboard ();
	if (!result)
		return false;

	HANDLE hData = ::GetClipboardData (CF_TEXT);
	if (!hData)
	{
		::CloseClipboard();

		err::setError (err::EStatus_InvalidDeviceRequest);
		return false;
	}

	string->clear ();

	size_t size = ::GlobalSize (hData);
	if (!size)
	{
		::CloseClipboard();
		return true;
	}

	void* data = ::GlobalLock (hData);
	ASSERT (data);

	string->copy ((char*) data);

	::GlobalUnlock(data);

	::CloseClipboard ();
	return true;
}

bool
CGdiEngine::readClipboard (
	uintptr_t format,
	rtl::CArrayT <char>* data
	)
{
	err::setError (err::EStatus_NotImplemented);
	return false;
}

bool
CGdiEngine::writeClipboard (
	const char* string,
	size_t length
	)
{
	bool result = openClipboard ();
	if (!result)
		return false;

	HGLOBAL hData = ::GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, length + 1);
	void* p = ::GlobalLock (hData);
	memcpy (p, string, length);
	((char*) p) [length] = 0;
	::GlobalUnlock (hData);

	::SetClipboardData (CF_TEXT, hData);
	::CloseClipboard ();
	return true;
}

bool
CGdiEngine::writeClipboard (
	uintptr_t format,
	const void* data,
	size_t size
	)
{
	err::setError (err::EStatus_NotImplemented);
	return false;
}

bool
CGdiEngine::openClipboard ()
{
	bool_t result;

	if (!m_hWndClipboardOwner)
	{
		m_hWndClipboardOwner = ::CreateWindowExW (
			0,
			L"STATIC",
			NULL,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			g::getModule ()->getModuleHandle (),
			NULL
			);

		if (!m_hWndClipboardOwner)
			return err::failWithLastSystemError ();
	}

	result = ::OpenClipboard (m_hWndClipboardOwner);
	return err::complete (result);
}

bool
CGdiEngine::showCaret (
	CWidget* widget,
	const TRect& rect
	)
{
	return true;
}

void
CGdiEngine::hideCaret ()
{
}

//.............................................................................

} // namespace gui
} // namespace axl
