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

#define _AXL_GUI_ENGINE_H

#include "axl_gui_Color.h"
#include "axl_gui_ImageDesc.h"
#include "axl_gui_FontDesc.h"

namespace axl {
namespace gui {

class Font;
class FontTuple;
class Image;
class Cursor;
class Canvas;
class WidgetDriver;

//..............................................................................

enum StdFontKind
{
	StdFontKind_Gui = 0,
	StdFontKind_Monospace,

	StdFontKind__Count
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum StdCursorKind
{
	StdCursorKind_Arrow = 0,
	StdCursorKind_Wait,
	StdCursorKind_IBeam,
	StdCursorKind_Hyperlink,
	StdCursorKind_SizeNS,
	StdCursorKind_SizeWE,
	StdCursorKind_SizeNWSE,
	StdCursorKind_SizeNESW,
	StdCursorKind_SizeAll,

	StdCursorKind__Count
};

//..............................................................................

class Engine
{
protected:
	uint_t m_stdPalColorTable [StdPalColor__Count];

public:
	Engine ();

	Palette
	getStdPalette ()
	{
		return Palette (m_stdPalColorTable, countof (m_stdPalColorTable));
	}

	virtual
	void
	updateStdPalette () = 0;

	// canvas


	virtual
	bool
	createOffscreenCanvas (
		Canvas* canvas,
		uint_t width,
		uint_t height
		) = 0;

	bool
	createOffscreenCanvas (
		Canvas* canvas,
		const Size& size
		)
	{
		return createOffscreenCanvas (
			canvas,
			size.m_width,
			size.m_height
			);
	}

	virtual
	bool
	releaseOffscreenCanvas (Canvas* canvas) = 0;

	virtual
	Canvas*
	getSharedOffscreenCanvas (
		uint_t width,
		uint_t height
		) = 0;

	Canvas*
	getSharedOffscreenCanvas (const Size& size)
	{
		return getSharedOffscreenCanvas (
			size.m_width,
			size.m_height
			);
	}

	virtual
	void
	releaseAllSharedOffscreenCanvases () = 0;

	virtual
	bool
	drawRect (
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
		) = 0;

	virtual
	bool
	drawAlphaRect (
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		uint_t color,
		uint_t alpha
		)
	{
		return drawRect (canvas, left, top, right, bottom, color); // draw opaque rect by default
	}

	virtual
	bool
	drawText_utf8 (
		Canvas* canvas,
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf8& text
		) = 0;

	virtual
	bool
	drawText_utf16 (
		Canvas* canvas,
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf16& text
		) = 0;

	virtual
	bool
	drawText_utf32 (
		Canvas* canvas,
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf32& text
		) = 0;

	virtual
	bool
	drawImage (
		Canvas* canvas,
		int x,
		int y,
		Image* image,
		int left,
		int top,
		int right,
		int bottom
		) = 0;

	virtual
	bool
	copyRect (
		Canvas* canvas,
		int x,
		int y,
		Canvas* srcCanvas,
		int left,
		int top,
		int right,
		int bottom
		) = 0;

	// fonts

	virtual
	void
	clearFontTuple (FontTuple* fontTuple) = 0;

	virtual
	FontTuple*
	getStdFontTuple (StdFontKind fontKind) = 0;

	inline
	Font*
	getStdFont (
		StdFontKind fontKind,
		uint_t flags = 0
		);

	virtual
	Font*
	createFont (
		FontTuple* fontTuple,
		const sl::StringRef& family,
		size_t pointSize,
		uint_t flags = 0
		) = 0;

	Font*
	createFont (
		FontTuple* fontTuple,
		const FontDesc& fontDesc
		)
	{
		return createFont (
			fontTuple,
			fontDesc.m_family,
			fontDesc.m_pointSize,
			fontDesc.m_flags
			);
	}

	virtual
	Font*
	getFontMod (
		FontTuple* fontTuple,
		uint_t flags
		) = 0;

	virtual
	bool
	getFontDesc (
		Font* font,
		FontDesc* fontDesc
		) = 0;

	virtual
	bool
	isMonospaceFont (Font* font) = 0;

	virtual
	Size
	calcTextSize_utf8 (
		Font* font,
		const sl::StringRef_utf8& text
		) = 0;

	virtual
	Size
	calcTextSize_utf16 (
		Font* font,
		const sl::StringRef_utf16& text
		) = 0;

	virtual
	Size
	calcTextSize_utf32 (
		Font* font,
		const sl::StringRef_utf32& text
		) = 0;

	// images

	virtual
	bool
	createImage (
		Image* image,
		uint_t width,
		uint_t height,
		PixelFormat pixelFormat
		) = 0;

	bool
	createImage (
		Image* image,
		const ImageDesc& imageDesc
		)
	{
		return createImage (
			image,
			imageDesc.m_size.m_width,
			imageDesc.m_size.m_height,
			imageDesc.m_pixelFormat
			);
	}

	virtual
	bool
	getImageDesc (
		Image* image,
		ImageDesc* imageDesc
		) = 0;

	virtual
	Cursor*
	getStdCursor (StdCursorKind cursorKind) = 0;

	// clipboard

	virtual
	uintptr_t
	registerClipboardFormat (const sl::StringRef& formatName) = 0;

	virtual
	bool
	readClipboard (sl::String* string) = 0;

	virtual
	bool
	readClipboard (
		uintptr_t format,
		sl::Array <char>* data
		) = 0;

	sl::String
	readClipboard ()
	{
		sl::String string;
		readClipboard (&string);
		return string;
	}

	sl::Array <char>
	readClipboard (uintptr_t format)
	{
		sl::Array <char> data;
		readClipboard (format, &data);
		return data;
	}

	virtual
	bool
	writeClipboard (const sl::StringRef& string) = 0;

	virtual
	bool
	writeClipboard (
		uintptr_t format,
		const void* data,
		size_t size
		) = 0;

	bool
	writeClipboard (
		uintptr_t format,
		const sl::Array <char>& data
		)
	{
		return writeClipboard (format, data, data.getCount ());
	}

	virtual
	bool
	commitClipboard () = 0;

	// widget

	virtual
	bool
	isWidgetFocused (WidgetDriver* widgetDriver) = 0;

	virtual
	bool
	setWidgetFocus (WidgetDriver* widgetDriver) = 0;

	virtual
	bool
	redrawWidget (
		WidgetDriver* widgetDriver,
		int left,
		int top,
		int right,
		int bottom
		) = 0;

	virtual
	bool
	scrollWidget (
		WidgetDriver* widgetDriver,
		int dx,
		int dy
		) = 0;

	virtual
	bool
	scrollWidgetRect (
		WidgetDriver* widgetDriver,
		int left,
		int top,
		int right,
		int bottom,
		int dx,
		int dy
		) = 0;

	virtual
	bool
	setWidgetCursor (
		WidgetDriver* widgetDriver,
		Cursor* cursor
		) = 0;

	virtual
	bool
	setMouseCapture (WidgetDriver* widgetDriver) = 0;

	virtual
	bool
	releaseMouse (WidgetDriver* widgetDriver) = 0;

	virtual
	bool
	updateWidgetScrollBar (
		WidgetDriver* widgetDriver,
		Orientation orientation
		) = 0;

	virtual
	void
	sendWidgetNotification (
		WidgetDriver* widgetDriver,
		uint_t code,
		const void* params = NULL
		) = 0;

	virtual
	bool
	postWidgetThreadMsg (
		WidgetDriver* widgetDriver,
		uint_t code,
		const ref::Ptr <void>& params
		) = 0;

	virtual
	bool
	startWidgetAnimation (WidgetDriver* widgetDriver) = 0;

	virtual
	void
	stopWidgetAnimation (WidgetDriver* widgetDriver) = 0;

	virtual
	bool
	showCaret (
		WidgetDriver* widgetDriver,
		const Rect& rect
		) = 0;

	virtual
	void
	hideCaret (WidgetDriver* widgetDriver) = 0;
};

//..............................................................................

} // namespace gui
} // namespace axl
