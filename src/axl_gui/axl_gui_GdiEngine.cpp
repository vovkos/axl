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

CFont* 
CGdiEngine::GetDefaultGuiFont ()
{
	if (m_DefaultGuiFont)
		return m_DefaultGuiFont;

	m_DefaultGuiFont = CreateStockFont (DEFAULT_GUI_FONT);
	return m_DefaultGuiFont;
}

CFont*
CGdiEngine::GetDefaultMonospaceFont ()
{
	if (m_DefaultMonospaceFont)
		return m_DefaultMonospaceFont;

	LOGFONTW LogFont;
	BuildLogFont (&LogFont, L"Courier New", 10);
	LogFont.lfPitchAndFamily = FIXED_PITCH;

	HFONT hFont = ::CreateFontIndirectW (&LogFont);
	ASSERT (hFont);

	m_DefaultMonospaceFont = CreateFont (hFont);
	return m_DefaultMonospaceFont;
}

ref::CPtrT <CFont>
CGdiEngine::CreateFont (
	const char* pFaceName,
	size_t PointSize,
	uint_t Flags
	)
{
	LOGFONTW LogFont;
	BuildLogFont (&LogFont, rtl::CString_w (pFaceName), PointSize, Flags);

	HFONT hFont = ::CreateFontIndirect (&LogFont);
	if (!hFont)
		return err::FailWithLastSystemError (ref::EPtr_Null);

	return CreateFont (hFont);
}

ref::CPtrT <CFont>
CGdiEngine::CreateStockFont (int StockFontKind)
{
	HGDIOBJ h = ::GetStockObject (StockFontKind);
	dword_t GdiObjectType = ::GetObjectType (h);
	if (GdiObjectType != OBJ_FONT)
	{
		err::SetError (err::EStatus_InvalidHandle);
		return ref::EPtr_Null;
	}

	return CreateFont ((HFONT) h);
}

ref::CPtrT <CFont>
CGdiEngine::CreateFont (HFONT hFont)
{
	CGdiFont* pFont = AXL_MEM_NEW (CGdiFont);

	LOGFONTW LogFont;
	::GetObjectW (hFont, sizeof (LogFont), &LogFont);
	GetFontDescFromLogFont (&LogFont, &pFont->m_FontDesc);

	ref::CPtrT <CGdiFontTuple> FontTuple = AXL_REF_NEW (CGdiFontTuple);
	FontTuple->m_pBaseFont = pFont;
	FontTuple->m_FontModArray [pFont->m_FontDesc.m_Flags] = pFont;

	pFont->m_h = hFont;
	pFont->m_pTuple = FontTuple;

	return ref::CPtrT <CFont> (pFont, FontTuple);
}

CFont*
CGdiEngine::GetFontMod (
	CFont* _pBaseFont,
	uint_t Flags
	)
{
	ASSERT (_pBaseFont->GetEngine () == this);

	CGdiFont* pBaseFont = (CGdiFont*) _pBaseFont;
	CGdiFontTuple* pFontTuple = (CGdiFontTuple*) pBaseFont->m_pTuple;

	LOGFONTW LogFont;
	GetLogFontFromFontDesc (*pFontTuple->m_pBaseFont->GetFontDesc (), &LogFont);
	ModifyLogFont (&LogFont, Flags);
	
	HFONT hFont = ::CreateFontIndirect (&LogFont);
	if (!hFont)
		return err::FailWithLastSystemError ((CFont*) NULL);
	
	CGdiFont* pFont = AXL_MEM_NEW (CGdiFont);
	pFont->m_FontDesc = pBaseFont->m_FontDesc;
	pFont->m_FontDesc.m_Flags = Flags;
	pFont->m_h = hFont;

	ASSERT (!(Flags & EFontFlag_Transparent) && Flags < countof (pFontTuple->m_FontModArray));
	ASSERT (!pFontTuple->m_FontModArray [Flags]);

	pFontTuple->m_FontModArray [Flags] = pFont;
	return pFont;
}

ref::CPtrT <CCursor>
CGdiEngine::CreateStockCursor (LPCTSTR pStockCursorRes)
{
	HCURSOR h = ::LoadCursor (NULL, pStockCursorRes);
	if (!h)
		return err::FailWithLastSystemError (ref::EPtr_Null);

	ref::CPtrT <CGdiCursor> Cursor = AXL_REF_NEW (ref::CBoxT <CGdiCursor>);
	Cursor->m_h = h;
	return Cursor;
}

CCursor*
CGdiEngine::GetStdCursor (EStdCursor CursorKind)
{
	static LPCTSTR StockCursorResTable [EStdCursor__Count] = 
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

	ASSERT (CursorKind < EStdCursor__Count);
	if (m_StdCursorArray [CursorKind])
		return m_StdCursorArray [CursorKind];
	
	ref::CPtrT <CCursor> Cursor = CreateStockCursor (StockCursorResTable [CursorKind]);
	m_StdCursorArray [CursorKind] = Cursor;
	return Cursor;
}

ref::CPtrT <CImage>
CGdiEngine::CreateImage (
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

	HBITMAP hBitmap;

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
	}
	
	ref::CPtrT <CGdiImage> Image = AXL_REF_NEW (ref::CBoxT <CGdiImage>);
	Image->m_h = hBitmap;
	return Image;
}

ref::CPtrT <CImageList>
CGdiEngine::CreateImageList (
	int Width,
	int Height
	)
{
	HIMAGELIST hImageList = NULL;

	ref::CPtrT <CGdiImageList> ImageList = AXL_REF_NEW (ref::CBoxT <CGdiImageList>);
	ImageList->m_h = hImageList;
	return ImageList;
}

ref::CPtrT <CImageList>
CGdiEngine::CreateImageList (
	CImage* pStipImage,
	int Width
	)
{
	HIMAGELIST hImageList = NULL;

	ref::CPtrT <CGdiImageList> ImageList = AXL_REF_NEW (ref::CBoxT <CGdiImageList>);
	ImageList->m_h = hImageList;
	return ImageList;
}

ref::CPtrT <CCanvas>
CGdiEngine::CreateOffscreenCanvas (
	int Width,
	int Height
	)	
{
	CScreenDc ScreenDc;
	HBITMAP hBitmap = ::CreateCompatibleBitmap (ScreenDc, Width, Height);
	if (!hBitmap)
		return err::FailWithLastSystemError (ref::EPtr_Null);

	HDC hdc = ::CreateCompatibleDC (ScreenDc);

	ref::CPtrT <CGdiCanvas> Dc = AXL_REF_NEW (ref::CBoxT <CGdiCanvas>);	
	Dc->Attach (hdc, NULL, CGdiCanvas::EDestruct_DeleteDc);
	Dc->m_hBitmap = hBitmap;
	Dc->m_hPrevBitmap = (HBITMAP) ::SelectObject (hdc, hBitmap);

	return Dc;
}

bool
CGdiEngine::ReadClipboard (rtl::CString* pString)
{
	bool Result = OpenClipboard ();
	if (!Result)		
		return false;
	
	HANDLE hData = ::GetClipboardData (CF_TEXT);
	if (!hData)
	{
		::CloseClipboard();

		err::SetError (err::EStatus_InvalidDeviceRequest);
		return false;
	}

	pString->Clear ();

	size_t Size = GlobalSize (hData);
	if (!Size)
	{
		::CloseClipboard();
		return true;
	}

	void* pData = GlobalLock (hData);
	ASSERT (pData);

	pString->Copy ((char*) pData);

	GlobalUnlock(pData);

	::CloseClipboard (); 
	return true;
}

bool
CGdiEngine::WriteClipboard (
	const char* pString,
	size_t Length
	)
{
	bool Result = OpenClipboard ();
	if (!Result)		
		return false;

	::EmptyClipboard ();

	size_t Size = (Length + 1) * sizeof (char);
	HGLOBAL hData = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, Size);
	void* p = GlobalLock (hData);
	memcpy (p, pString, Length * sizeof (char));
	
	::SetClipboardData (CF_TEXT, hData);
	::CloseClipboard (); 
	return true;
}

bool
CGdiEngine::OpenClipboard ()
{
	bool_t Result;

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
			g::GetModule ()->GetModuleHandle (),
			NULL
			);
		
		if (!m_hWndClipboardOwner)
			return err::FailWithLastSystemError ();
	}

	Result = ::OpenClipboard (m_hWndClipboardOwner);
	return err::Complete (Result);
}

//.............................................................................

} // namespace gui
} // namespace axl
