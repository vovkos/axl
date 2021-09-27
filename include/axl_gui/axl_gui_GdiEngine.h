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

#pragma once

#define _AXL_GUI_GDIENGINE_H

#include "axl_gui_GdiCanvas.h"
#include "axl_gui_GdiCursor.h"
#include "axl_gui_GdiFont.h"
#include "axl_gui_GdiImage.h"
#include "axl_gui_GdiWidget.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//..............................................................................

class GdiEngine: public Engine {
protected:
	HWND m_hWndClipboardOwner;

public:
	GdiEngine();
	~GdiEngine();

	static
	GdiEngine*
	getSingleton() {
		return sl::getSingleton<GdiEngine> ();
	}

	// fonts

	Font*
	getDefaultGuiFont();

	virtual
	Font*
	getDefaultMonospaceFont();

	virtual
	rc::Ptr<Font>
	createFont(
		const sl::StringRef& family,
		size_t pointSize = 0,
		uint_t flags = 0
	);

	rc::Ptr<Font>
	createStockFont(int stockFontKind);

	rc::Ptr<Font>
	createFont(HFONT hFont);

	// cursors

	rc::Ptr<Cursor>
	createStockCursor(LPCTSTR stockCursorRes);

	// images

	virtual
	rc::Ptr<Image>
	createImage();

	virtual
	rc::Ptr<Image>
	createImage(
		int width,
		int height,
		PixelFormat pixelFormat,
		const void* data,
		bool isScreenCompatible = true
	);

	virtual
	rc::Ptr<Canvas>
	createOffscreenCanvas(
		int width,
		int height
	);

	// clipboard

	virtual
	uintptr_t
	registerClipboardFormat(const sl::StringRef& formatName);

	virtual
	bool
	readClipboard(sl::String* string);

	virtual
	bool
	readClipboard(
		uintptr_t format,
		sl::Array<char>* data
	);

	virtual
	bool
	writeClipboard(const sl::StringRef& string);

	virtual
	bool
	writeClipboard(
		uintptr_t format,
		const void* data,
		size_t size
	);

	virtual
	bool
	commitClipboard() {
		return true;
	}

	// caret

	virtual
	bool
	showCaret(
		Widget* widget,
		const Rect& rect
	);

	virtual
	void
	hideCaret();

protected:
	bool
	openClipboard();

	virtual
	Font*
	getFontMod(
		Font* baseFont,
		uint_t flags
	);

	virtual
	rc::Ptr<Font>
	createStdFont(StdFontKind fontKind);

	virtual
	rc::Ptr<Cursor>
	createStdCursor(StdCursorKind cursorKind);
};

//..............................................................................

} // namespace gui
} // namespace axl
