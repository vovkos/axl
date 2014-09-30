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
		m_engineKind = EEngine_Gdi;
		m_hWndClipboardOwner = NULL;
	}

	~CGdiEngine ();

	static
	CGdiEngine*
	getSingleton ()
	{
		return rtl::getSingleton <CGdiEngine> ();
	}

	// fonts

	CFont*
	getDefaultGuiFont ();

	virtual
	CFont*
	getDefaultMonospaceFont ();

	virtual
	ref::CPtrT <CFont>
	createFont (
		const char* faceName,
		size_t pointSize = 0,
		uint_t flags = 0
		);

	ref::CPtrT <CFont>
	createStockFont (int stockFontKind);

	ref::CPtrT <CFont>
	createFont (HFONT hFont);

	// cursors

	ref::CPtrT <CCursor>
	createStockCursor (LPCTSTR stockCursorRes);

	// images

	virtual
	ref::CPtrT <CImage>
	createImage ();

	virtual
	ref::CPtrT <CImage>
	createImage (
		int width,
		int height,
		EPixelFormat pixelFormat,
		const void* data,
		bool isScreenCompatible = true
		);

	virtual
	ref::CPtrT <CCanvas>
	createOffscreenCanvas (
		int width,
		int height
		);

	// clipboard

	virtual
	uintptr_t 
	registerClipboardFormat (const rtl::CString& formatName);

	virtual
	bool
	readClipboard (rtl::CString* string);

	virtual
	bool
	readClipboard (
		uintptr_t format,
		rtl::CArrayT <char>* data
		);

	virtual
	bool
	writeClipboard (
		const char* string,
		size_t length = -1
		);

	virtual
	bool
	writeClipboard (
		uintptr_t format,
		const void* data,
		size_t size
		);

	virtual
	bool
	commitClipboard ()
	{
		return true;
	}

	// caret

	virtual
	bool
	showCaret (
		CWidget* widget,
		const TRect& rect
		);

	virtual
	void
	hideCaret ();

protected:
	bool
	openClipboard ();

	virtual
	CFont*
	getFontMod (
		CFont* baseFont,
		uint_t flags
		);

	virtual
	ref::CPtrT <CFont>
	createStdFont (EStdFont fontKind);

	virtual
	ref::CPtrT <CCursor>
	createStdCursor (EStdCursor cursorKind);
};

//.............................................................................

} // namespace gui
} // namespace axl
