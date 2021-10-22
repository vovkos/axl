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
#include "axl_gui_GdiEngine.h"
#include "axl_rc_New.h"
#include "axl_err_Error.h"
#include "axl_gui_Widget.h"

namespace axl {
namespace gui {

//..............................................................................

GdiEngine::GdiEngine() {
	m_hWndClipboardOwner = NULL;
	updateStdPalette();
}

GdiEngine::~GdiEngine() {
	if (m_hWndClipboardOwner)
		::DestroyWindow(m_hWndClipboardOwner);
}

void
GdiEngine::updateStdPalette() {
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_WidgetText]    = invertRgb(::GetSysColor(COLOR_WINDOWTEXT));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_WidgetBack]    = invertRgb(::GetSysColor(COLOR_WINDOW));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_GrayText]      = invertRgb(::GetSysColor(COLOR_GRAYTEXT));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_SelectionText] = invertRgb(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_SelectionBack] = invertRgb(::GetSysColor(COLOR_HIGHLIGHT));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_3DFace]        = invertRgb(::GetSysColor(COLOR_3DFACE));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_3DShadow]      = invertRgb(::GetSysColor(COLOR_3DSHADOW));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_3DDarkShadow]  = invertRgb(::GetSysColor(COLOR_3DDKSHADOW));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_3DLight]       = invertRgb(::GetSysColor(COLOR_3DLIGHT));
	g_stdPalColorArray[~ColorFlag_Index & StdPalColor_3DHiLight]     = invertRgb(::GetSysColor(COLOR_3DHILIGHT));
}

rc::Ptr<Font>
GdiEngine::createStdFont(StdFontKind fontKind) {
	LOGFONTW logFont;
	HFONT hFont;

	switch (fontKind) {
	case StdFontKind_Gui:
		return createStockFont(DEFAULT_GUI_FONT);

	case StdFontKind_Monospace:
		buildLogFont(&logFont, L"Courier New", 10);
		logFont.lfPitchAndFamily = FIXED_PITCH;

		hFont = ::CreateFontIndirectW(&logFont);
		ASSERT(hFont);

		return createFont(hFont);

	default:
		return rc::g_nullPtr;
	}
}

rc::Ptr<Font>
GdiEngine::createFont(
	const sl::StringRef& family,
	size_t pointSize,
	uint_t flags
) {
	LOGFONTW logFont;
	buildLogFont(&logFont, family, pointSize, flags);

	HFONT hFont = ::CreateFontIndirect(&logFont);
	if (!hFont)
		return err::failWithLastSystemError(rc::g_nullPtr);

	return createFont(hFont);
}

rc::Ptr<Font>
GdiEngine::createStockFont(int stockFontKind) {
	HGDIOBJ h = ::GetStockObject(stockFontKind);
	dword_t gdiObjectType = ::GetObjectType(h);
	if (gdiObjectType != OBJ_FONT) {
		err::setError(err::SystemErrorCode_InvalidHandle);
		return rc::g_nullPtr;
	}

	return createFont((HFONT)h);
}

rc::Ptr<Font>
GdiEngine::createFont(HFONT hFont) {
	GdiFont* font = AXL_MEM_NEW(GdiFont);

	LOGFONTW logFont;
	::GetObjectW(hFont, sizeof(logFont), &logFont);
	getFontDescFromLogFont(&logFont, &font->m_fontDesc);

	rc::Ptr<GdiFontuple> fontTuple = AXL_RC_NEW(GdiFontuple);
	fontTuple->m_baseFont = font;
	fontTuple->m_fontModArray[font->m_fontDesc.m_flags] = font;

	font->m_h = hFont;
	font->m_tuple = fontTuple;

	return rc::Ptr<Font> (font, fontTuple);
}

Font*
GdiEngine::getFontMod(
	Font* _pBaseFont,
	uint_t flags
) {
	ASSERT(_pBaseFont->getEngine() == this);

	GdiFont* baseFont = (GdiFont*)_pBaseFont;
	GdiFontuple* fontTuple = (GdiFontuple*)baseFont->m_tuple;

	LOGFONTW logFont;
	getLogFontFromFontDesc(*fontTuple->m_baseFont->getFontDesc(), &logFont);
	modifyLogFont(&logFont, flags);

	HFONT hFont = ::CreateFontIndirect(&logFont);
	if (!hFont)
		return err::failWithLastSystemError((Font*)NULL);

	GdiFont* font = AXL_MEM_NEW(GdiFont);
	font->m_fontDesc = baseFont->m_fontDesc;
	font->m_fontDesc.m_flags = flags;
	font->m_h = hFont;

	ASSERT(!(flags & FontFlag_Transparent) && flags < countof(fontTuple->m_fontModArray));
	ASSERT(!fontTuple->m_fontModArray[flags]);

	fontTuple->m_fontModArray[flags] = font;
	return font;
}

rc::Ptr<Cursor>
GdiEngine::createStockCursor(LPCTSTR stockCursorRes) {
	HCURSOR h = ::LoadCursor(NULL, stockCursorRes);
	if (!h)
		return err::failWithLastSystemError(rc::g_nullPtr);

	rc::Ptr<GdiCursor> cursor = AXL_RC_NEW(rc::Box<GdiCursor>);
	cursor->m_h = h;
	return cursor;
}

rc::Ptr<Cursor>
GdiEngine::createStdCursor(StdCursorKind cursorKind) {
	static LPCTSTR stockCursorResTable[StdCursorKind__Count] = {
		IDC_ARROW,    // StdCursorKind_Arrow = 0,
		IDC_WAIT,     // StdCursorKind_Wait,
		IDC_IBEAM,    // StdCursorKind_IBeam,
		IDC_HAND,     // StdCursorKind_Hyperlink,
		IDC_SIZENS,   // StdCursorKind_SizeNS,
		IDC_SIZEWE,   // StdCursorKind_SizeWE,
		IDC_SIZENWSE, // StdCursorKind_SizeNWSE,
		IDC_SIZENESW, // StdCursorKind_SizeNESW,
		IDC_SIZEALL,  // StdCursorKind_SizeAll,
	};

	ASSERT(cursorKind < StdCursorKind__Count);
	return createStockCursor(stockCursorResTable[cursorKind]);
}

rc::Ptr<Image>
GdiEngine::createImage() {
	rc::Ptr<GdiImage> image = AXL_RC_NEW(rc::Box<GdiImage>);
	return image;
}

rc::Ptr<Image>
GdiEngine::createImage(
	int width,
	int height,
	PixelFormat pixelFormat,
	const void* data,
	bool isScreenCompatible
) {
	uint_t bitCount;

	switch (pixelFormat) {
	case PixelFormat_Rgba:
		bitCount = 32;
		break;

	case PixelFormat_Rgb:
		bitCount = 24;
		break;

	default:
		err::setFormatStringError("unsupported pixel format '%s'", getPixelFormatString(pixelFormat));
	};

	HBITMAP hBitmap;

	if (!isScreenCompatible) {
		hBitmap = ::CreateBitmap(
			width,
			height,
			1,
			bitCount,
			data
		);

		if (!hBitmap)
			return err::failWithLastSystemError(rc::g_nullPtr);
	} else {
		BITMAPINFO bitmapInfo = { 0 };
		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = bitCount;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = width;
		bitmapInfo.bmiHeader.biHeight = height;

		ScreenDc screenDc;

		hBitmap = ::CreateCompatibleBitmap(
			screenDc,
			width,
			height
		);

		if (!hBitmap)
			return err::failWithLastSystemError(rc::g_nullPtr);

		bool_t result = ::SetDIBits(
			screenDc,
			hBitmap,
			0,
			height,
			data,
			&bitmapInfo,
			DIB_RGB_COLORS
		);

		if (!result)
			return err::failWithLastSystemError(rc::g_nullPtr);
	}

	rc::Ptr<GdiImage> image = AXL_RC_NEW(rc::Box<GdiImage>);
	image->m_h = hBitmap;
	return image;
}

rc::Ptr<Canvas>
GdiEngine::createOffscreenCanvas(
	int width,
	int height
) {
	ScreenDc screenDc;
	HBITMAP hBitmap = ::CreateCompatibleBitmap(screenDc, width, height);
	if (!hBitmap)
		return err::failWithLastSystemError(rc::g_nullPtr);

	HDC hdc = ::CreateCompatibleDC(screenDc);

	rc::Ptr<GdiCanvas> dc = AXL_RC_NEW(rc::Box<GdiCanvas>);
	dc->attach(hdc, NULL, GdiCanvas::DestructKind_DeleteDc);
	dc->m_hBitmap = hBitmap;
	dc->m_hPrevBitmap = (HBITMAP) ::SelectObject(hdc, hBitmap);

	return dc;
}

uintptr_t
GdiEngine::registerClipboardFormat(const sl::StringRef& formatName) {
	err::setError(err::SystemErrorCode_NotImplemented);
	return -1;
}

bool
GdiEngine::readClipboard(sl::String* string) {
	bool result = openClipboard();
	if (!result)
		return false;

	HANDLE hData = ::GetClipboardData(CF_TEXT);
	if (!hData) {
		::CloseClipboard();

		err::setError(err::SystemErrorCode_InvalidDeviceRequest);
		return false;
	}

	string->clear();

	size_t size = ::GlobalSize(hData);
	if (!size) {
		::CloseClipboard();
		return true;
	}

	void* data = ::GlobalLock(hData);
	ASSERT(data);

	string->copy((char*)data);

	::GlobalUnlock(data);

	::CloseClipboard();
	return true;
}

bool
GdiEngine::readClipboard(
	uintptr_t format,
	sl::Array<char>* data
) {
	err::setError(err::SystemErrorCode_NotImplemented);
	return false;
}

bool
GdiEngine::writeClipboard(const sl::StringRef& string) {
	bool result = openClipboard();
	if (!result)
		return false;

	HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, length + 1);
	void* p = ::GlobalLock(hData);
	memcpy(p, string, length);
	((char*)p) [length] = 0;
	::GlobalUnlock(hData);

	::SetClipboardData(CF_TEXT, hData);
	::CloseClipboard();
	return true;
}

bool
GdiEngine::writeClipboard(
	uintptr_t format,
	const void* data,
	size_t size
) {
	err::setError(err::SystemErrorCode_NotImplemented);
	return false;
}

bool
GdiEngine::openClipboard() {
	bool_t result;

	if (!m_hWndClipboardOwner) {
		m_hWndClipboardOwner = ::CreateWindowExW(
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
			g::getModule()->getModuleHandle(),
			NULL
		);

		if (!m_hWndClipboardOwner)
			return err::failWithLastSystemError();
	}

	result = ::OpenClipboard(m_hWndClipboardOwner);
	return err::complete(result);
}

bool
GdiEngine::showCaret(
	Widget* widget,
	const Rect& rect
) {
	return true;
}

void
GdiEngine::hideCaret() {}

//..............................................................................

} // namespace gui
} // namespace axl
