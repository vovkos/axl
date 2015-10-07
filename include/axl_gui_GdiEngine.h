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
#include "axl_sl_Singleton.h"

namespace axl {
namespace gui {

//.............................................................................

class GdiEngine: public Engine
{
protected:
	HWND m_hWndClipboardOwner;

public:
	GdiEngine ();
	~GdiEngine ();

	static
	GdiEngine*
	getSingleton ()
	{
		return mt::getSingleton <GdiEngine> ();
	}

	// fonts

	Font*
	getDefaultGuiFont ();

	virtual
	Font*
	getDefaultMonospaceFont ();

	virtual
	ref::Ptr <Font>
	createFont (
		const char* family,
		size_t pointSize = 0,
		uint_t flags = 0
		);

	ref::Ptr <Font>
	createStockFont (int stockFontKind);

	ref::Ptr <Font>
	createFont (HFONT hFont);

	// cursors

	ref::Ptr <Cursor>
	createStockCursor (LPCTSTR stockCursorRes);

	// images

	virtual
	ref::Ptr <Image>
	createImage ();

	virtual
	ref::Ptr <Image>
	createImage (
		int width,
		int height,
		PixelFormat pixelFormat,
		const void* data,
		bool isScreenCompatible = true
		);

	virtual
	ref::Ptr <Canvas>
	createOffscreenCanvas (
		int width,
		int height
		);

	// clipboard

	virtual
	uintptr_t 
	registerClipboardFormat (const sl::String& formatName);

	virtual
	bool
	readClipboard (sl::String* string);

	virtual
	bool
	readClipboard (
		uintptr_t format,
		sl::Array <char>* data
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
		Widget* widget,
		const Rect& rect
		);

	virtual
	void
	hideCaret ();

protected:
	bool
	openClipboard ();

	virtual
	Font*
	getFontMod (
		Font* baseFont,
		uint_t flags
		);

	virtual
	ref::Ptr <Font>
	createStdFont (StdFontKind fontKind);

	virtual
	ref::Ptr <Cursor>
	createStdCursor (StdCursorKind cursorKind);
};

//.............................................................................

} // namespace gui
} // namespace axl
