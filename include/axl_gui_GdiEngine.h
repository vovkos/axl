// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIENGINE_H

#include "axl_gui_Engine.h"
#include "axl_gui_GdiCanvas.h"
#include "axl_gui_GdiCursor.h"
#include "axl_gui_GdiFont.h"
#include "axl_gui_GdiImage.h"
#include "axl_gui_GdiWidget.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace gui {

//.............................................................................

class CGdiEngine: public CEngine
{
protected:
	HWND m_hWndClipboardOwner;

public:
	CGdiEngine ()
	{
		m_EngineKind = EEngine_Gdi;
		m_hWndClipboardOwner = NULL;
	}

	~CGdiEngine ();

	static
	CGdiEngine*
	GetSingleton ()
	{
		return rtl::GetSingleton <CGdiEngine> ();
	}

	// fonts

	CFont*
	GetDefaultGuiFont ();

	virtual
	CFont*
	GetDefaultMonospaceFont ();

	virtual
	ref::CPtrT <CFont>
	CreateFont (
		const char* pFaceName,
		size_t PointSize = 0,
		uint_t Flags = 0
		);

	ref::CPtrT <CFont>
	CreateStockFont (int StockFontKind);

	ref::CPtrT <CFont>
	CreateFont (HFONT hFont);

	// cursors

	ref::CPtrT <CCursor>
	CreateStockCursor (LPCTSTR pStockCursorRes);

	// images

	virtual
	ref::CPtrT <CImage>
	CreateImage ();

	virtual
	ref::CPtrT <CImage>
	CreateImage (
		int Width,
		int Height,
		EPixelFormat PixelFormat,
		const void* pData,
		bool IsScreenCompatible = true
		);

	virtual
	ref::CPtrT <CCanvas>
	CreateOffscreenCanvas (
		int Width,
		int Height
		);

	// clipboard

	virtual
	uintptr_t 
	RegisterClipboardFormat (const rtl::CString& FormatName);

	virtual
	bool
	ReadClipboard (rtl::CString* pString);

	virtual
	bool
	ReadClipboard (
		uintptr_t Format,
		rtl::CArrayT <char>* pData
		);

	virtual
	bool
	WriteClipboard (
		const char* pString,
		size_t Length = -1
		);

	virtual
	bool
	WriteClipboard (
		uintptr_t Format,
		const void* pData,
		size_t Size
		);

	virtual
	bool
	CommitClipboard ()
	{
		return true;
	}

	// caret

	virtual
	bool
	ShowCaret (
		CWidget* pWidget,
		const TRect& Rect
		);

	virtual
	void
	HideCaret ();

protected:
	bool
	OpenClipboard ();

	virtual
	CFont*
	GetFontMod (
		CFont* pBaseFont,
		uint_t Flags
		);

	virtual
	ref::CPtrT <CFont>
	CreateStdFont (EStdFont FontKind);

	virtual
	ref::CPtrT <CCursor>
	CreateStdCursor (EStdCursor CursorKind);
};

//.............................................................................

} // namespace gui
} // namespace axl
