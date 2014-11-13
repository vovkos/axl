// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTENGINE_H

#include "axl_gui_Canvas.h"
#include "axl_gui_Font.h"
#include "axl_gui_Image.h"
#include "axl_gui_QtCaret.h"
#include "axl_rtl_Singleton.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace gui {

class QtEngine;

inline 
Engine* 
getQtEngine ();

//.............................................................................

class QtCanvas: public Canvas
{
	friend class QtEngine;

public:
	QPainter m_qtPainter;
	QPixmap m_qtPixmap;

public:
	QtCanvas ():
		Canvas (getQtEngine ()) 
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtFontTuple: public FontTuple
{
	friend class QtEngine;

public:
	QtFontTuple ():
		FontTuple (getQtEngine ()) 
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtFont: public Font
{
	friend class QtEngine;

public:
	QFont m_qtFont;

public:
	QtFont ():
		Font (getQtEngine ()) 
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtImage: public Image
{
	friend class QtEngine;

public:
	QImage m_qtImage;

public:
	QtImage ():
		Image (getQtEngine ()) 
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtCursor: public Cursor
{
	friend class QtEngine;

public:
	QCursor m_qtCursor;

public:
	QtCursor ():
		Cursor (getQtEngine ()) 
	{
	}
};

//.............................................................................

class QtEngine: public Engine
{
protected:
	OffscreenCanvasCache <QtCanvas> m_sharedOffscreenCanvasCache;
	QtFontTuple* m_stdFontTupleArray [StdFontKind__Count];
	QtCursor* m_stdCursorArray [StdCursorKind__Count];
	QtCaret m_qtCaret;

	rtl::StringHashTableMap <uintptr_t> m_clipboardFormatNameMap;
	rtl::Array <rtl::String> m_clipboardFormatNameTable;
	QMimeData* m_qtClipboardMimeData;

public:
	QtEngine ();
	~QtEngine ();

	// canvas

	virtual
	void
	updateStdPalette ();

	virtual
	bool
	createOffscreenCanvas (
		Canvas* canvas,
		uint_t width,
		uint_t height
		);

	virtual
	bool
	releaseOffscreenCanvas (Canvas* canvas);

	virtual
	Canvas*
	getSharedOffscreenCanvas (
		uint_t width,
		uint_t height
		)
	{
		return m_sharedOffscreenCanvasCache.getCanvas (width, height);
	}

	virtual
	void
	releaseAllSharedOffscreenCanvases ()
	{
		return m_sharedOffscreenCanvasCache.clear ();
	}

	virtual
	bool
	drawRect (
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
		);

	bool
	drawText_qt (
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
		const QString& string
		);

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
		const utf8_t* text,
		size_t length = -1
		);

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
		const utf16_t* text,
		size_t length = -1
		);

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
		const utf32_t* text,
		size_t length = -1
		);

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
		);

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
		);

	// fonts

	virtual
	void
	clearFontTuple (FontTuple* fontTuple);

	virtual
	FontTuple*
	getStdFontTuple (StdFontKind fontKind);

	virtual
	Font*
	createFont (
		FontTuple* fontTuple,
		const char* family,
		size_t pointSize,
		uint_t flags = 0
		);

	virtual
	Font*
	getFontMod (
		FontTuple* fontTuple,
		uint_t flags
		);

	virtual
	bool
	getFontDesc (
		Font* font,
		FontDesc* fontDesc
		);

	virtual
	bool
	isMonospaceFont (Font* font);

	Size
	calcTextSize_qt (
		Font* font,
		const QString& string
		);

	virtual
	Size
	calcTextSize_utf8 (
		Font* font,
		const utf8_t* text,
		size_t length = -1
		);

	virtual
	Size
	calcTextSize_utf16 (
		Font* font,
		const utf16_t* text,
		size_t length = -1
		);

	virtual
	Size
	calcTextSize_utf32 (
		Font* font,
		const utf32_t* text,
		size_t length = -1
		);

	// images

	virtual
	bool
	createImage (
		Image* image,
		uint_t width,
		uint_t height,
		PixelFormat pixelFormat
		);

	virtual
	bool
	getImageDesc (
		Image* image,
		ImageDesc* imageDesc
		);

	virtual
	Cursor*
	getStdCursor (StdCursorKind cursorKind);
	
	// clipboard

	virtual
	uintptr_t 
	registerClipboardFormat (const rtl::String& formatName);

	virtual
	bool
	readClipboard (rtl::String* string);

	virtual
	bool
	readClipboard (
		uintptr_t format,
		rtl::Array <char>* data
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
	commitClipboard ();

	// widget

	virtual
	bool
	isWidgetFocused (WidgetDriver* widgetDriver);

	virtual
	bool
	setWidgetFocus (WidgetDriver* widgetDriver);

	virtual
	bool
	redrawWidget (
		WidgetDriver* widgetDriver,
		int left, 
		int top, 
		int right, 
		int bottom
		);

	virtual
	bool
	scrollWidget (
		WidgetDriver* widgetDriver,
		int dx, 
		int dy
		);

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
		);

	virtual
	bool
	setWidgetCursor (
		WidgetDriver* widgetDriver,
		Cursor* cursor
		);

	virtual
	bool
	setMouseCapture (WidgetDriver* widgetDriver);

	virtual
	bool
	releaseMouse (WidgetDriver* widgetDriver);

	virtual
	bool
	updateWidgetScrollBar (
		WidgetDriver* widgetDriver,
		Orientation orientation
		);

	virtual
	void
	sendWidgetNotification (
		WidgetDriver* widgetDriver,
		uint_t code,
		void* param = NULL
		);

	virtual
	bool
	postWidgetThreadMsg (
		WidgetDriver* widgetDriver,
		uint_t code,
		const ref::Ptr <void>& params
		);

	virtual
	bool
	showCaret (
		WidgetDriver* widgetDriver,
		const Rect& rect
		)
	{
		return m_qtCaret.show (widgetDriver, rect);
	}

	virtual
	void
	hideCaret (WidgetDriver* widgetDriver)
	{
		m_qtCaret.hide ();
	}

	bool
	isCaretVisible ()
	{
		return m_qtCaret.isVisible ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline 
Engine* 
getQtEngine ()
{
	return rtl::getSingleton <QtEngine> ();
}

//.............................................................................

} // namespace gui
} // namespace axl
