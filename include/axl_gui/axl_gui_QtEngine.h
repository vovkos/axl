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

#define _AXL_GUI_QTENGINE_H

#include "axl_gui_QtCaret.h"
#include "axl_gui_Canvas.h"
#include "axl_gui_Font.h"
#include "axl_gui_Image.h"

namespace axl {
namespace gui {

class QtEngine;
class QtFont;

inline
Engine*
getQtEngine();

//..............................................................................

class QtCanvas: public Canvas
{
	friend class QtEngine;

public:
	QPainter m_qtPainter;
	QPixmap m_qtPixmap;

public:
	QtCanvas():
		Canvas(getQtEngine())
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtFontTuple: public FontTuple
{
	friend class QtEngine;

public:
	QtFontTuple():
		FontTuple(getQtEngine())
	{
	}

	QtFont* attachFont(QFont qtFont);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtFont: public Font
{
	friend class QtEngine;
	friend class QtFontTuple;

public:
	QFont m_qtFont;

protected:
	QtFont():
		Font(getQtEngine())
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtImage: public Image
{
	friend class QtEngine;

public:
	QImage m_qtImage;

public:
	QtImage():
		Image(getQtEngine())
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class QtCursor: public Cursor
{
	friend class QtEngine;

public:
	QCursor m_qtCursor;

public:
	QtCursor():
		Cursor(getQtEngine())
	{
	}
};

//..............................................................................

class QtEngine: public Engine
{
protected:
	enum Def
	{
		Def_ToolTipTimeout = 1000,
	};

protected:
	OffscreenCanvasCache<QtCanvas> m_sharedOffscreenCanvasCache;
	QtFontTuple* m_stdFontTupleArray[StdFontKind__Count];
	QtCursor* m_stdCursorArray[StdCursorKind__Count];
	QtCaret m_qtCaret;
	QTimer m_toolTipTimer;

	sl::StringHashTable<uintptr_t> m_clipboardFormatNameMap;
	sl::Array<sl::String> m_clipboardFormatNameTable;
	QMimeData* m_qtClipboardMimeData;

public:
	QtEngine();
	~QtEngine();

	// canvas

	virtual
	void
	updateStdPalette();

	virtual
	bool
	createOffscreenCanvas(
		Canvas* canvas,
		uint_t width,
		uint_t height
		);

	virtual
	bool
	releaseOffscreenCanvas(Canvas* canvas);

	virtual
	Canvas*
	getSharedOffscreenCanvas(
		uint_t width,
		uint_t height
		)
	{
		return m_sharedOffscreenCanvasCache.getCanvas(width, height);
	}

	virtual
	void
	releaseAllSharedOffscreenCanvases()
	{
		return m_sharedOffscreenCanvasCache.clear();
	}

	virtual
	bool
	drawRect(
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
		);

	virtual
	bool
	drawAlphaRect(
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		uint_t color,
		uint_t alpha
		);

	virtual
	bool
	drawGradientRect(
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		int x1,
		int y1,
		uint_t color1,
		int x2,
		int y2,
		uint_t color2
		);

	virtual
	bool
	drawAlphaGradientRect(
		Canvas* canvas,
		int left,
		int top,
		int right,
		int bottom,
		int x1,
		int y1,
		uint_t color1,
		uint_t alpha1,
		int x2,
		int y2,
		uint_t color2,
		uint_t alpha2
		);

	bool
	drawText_qt(
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
	drawText_utf8(
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
		);

	virtual
	bool
	drawText_utf16(
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
		);

	virtual
	bool
	drawText_utf32(
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
		);

	virtual
	bool
	drawImage(
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
	copyRect(
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
	clearFontTuple(FontTuple* fontTuple);

	virtual
	FontTuple*
	getStdFontTuple(StdFontKind fontKind);

	virtual
	Font*
	createFont(
		FontTuple* fontTuple,
		const sl::StringRef& family,
		size_t pointSize,
		uint_t flags = 0
		);

	virtual
	Font*
	getFontMod(
		FontTuple* fontTuple,
		uint_t flags
		);

	virtual
	bool
	getFontDesc(
		Font* font,
		FontDesc* fontDesc
		);

	virtual
	bool
	isMonospaceFont(Font* font);

	Size
	calcCharSize_qt(
		const QFont& font,
		QPaintDevice* device,
		QChar c
		);

	Size
	calcTextSize_qt(
		const QFont& font,
		QPaintDevice* device,
		const QString& string
		);

	virtual
	Size
	calcCharSize(
		Font* font,
		Canvas* canvas,
		utf32_t c
		);

	virtual
	Size
	calcCharSize(
		Font* font,
		Widget* widget,
		utf32_t c
		);

	virtual
	Size
	calcTextSize_utf8(
		Font* font,
		Canvas* canvas,
		const sl::StringRef_utf8& text
		);

	virtual
	Size
	calcTextSize_utf8(
		Font* font,
		Widget* widget,
		const sl::StringRef_utf8& text
		);

	virtual
	Size
	calcTextSize_utf16(
		Font* font,
		Canvas* canvas,
		const sl::StringRef_utf16& text
		);

	virtual
	Size
	calcTextSize_utf16(
		Font* font,
		Widget* widget,
		const sl::StringRef_utf16& text
		);

	virtual
	Size
	calcTextSize_utf32(
		Font* font,
		Canvas* canvas,
		const sl::StringRef_utf32& text
		);

	virtual
	Size
	calcTextSize_utf32(
		Font* font,
		Widget* widget,
		const sl::StringRef_utf32& text
		);

	// images

	virtual
	bool
	createImage(
		Image* image,
		uint_t width,
		uint_t height,
		PixelFormat pixelFormat
		);

	virtual
	bool
	getImageDesc(
		Image* image,
		ImageDesc* imageDesc
		);

	virtual
	Cursor*
	getStdCursor(StdCursorKind cursorKind);

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
	commitClipboard();

	// widget

	virtual
	bool
	isWidgetFocused(WidgetDriver* widgetDriver);

	virtual
	bool
	setWidgetFocus(WidgetDriver* widgetDriver);

	virtual
	bool
	redrawWidget(
		WidgetDriver* widgetDriver,
		int left,
		int top,
		int right,
		int bottom
		);

	virtual
	bool
	redrawWidgetImmediate(
		WidgetDriver* widgetDriver,
		int left,
		int top,
		int right,
		int bottom
		);

	virtual
	bool
	scrollWidget(
		WidgetDriver* widgetDriver,
		int dx,
		int dy
		);

	virtual
	bool
	scrollWidgetRect(
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
	setWidgetCursor(
		WidgetDriver* widgetDriver,
		Cursor* cursor
		);

	virtual
	bool
	setMouseCapture(WidgetDriver* widgetDriver);

	virtual
	bool
	releaseMouse(WidgetDriver* widgetDriver);

	virtual
	bool
	updateWidgetScrollBar(
		WidgetDriver* widgetDriver,
		Orientation orientation
		);

	virtual
	void
	sendWidgetNotification(
		WidgetDriver* widgetDriver,
		uint_t code,
		const void* param = NULL
		);

	virtual
	bool
	postWidgetThreadMsg(
		WidgetDriver* widgetDriver,
		uint_t code,
		const ref::Ptr<void>& params
		);

	virtual
	bool
	startWidgetAnimation(WidgetDriver* widgetDriver);

	virtual
	void
	stopWidgetAnimation(WidgetDriver* widgetDriver);

	virtual
	bool
	showCaret(
		WidgetDriver* widgetDriver,
		const Rect& rect
		)
	{
		return m_qtCaret.show(widgetDriver, rect);
	}

	virtual
	void
	hideCaret(WidgetDriver* widgetDriver)
	{
		m_qtCaret.hide(widgetDriver);
	}

	bool
	isCaretVisible()
	{
		return m_qtCaret.isVisible();
	}

	virtual
	bool
	scheduleToolTipMsg(
		WidgetDriver* widgetDriver,
		uint_t timeout
		);

	virtual
	bool
	cancelToolTipMsg(WidgetDriver* widgetDriver);

	virtual
	bool
	showToolTip(
		WidgetDriver* widgetDriver,
		int x,
		int y,
		const sl::StringRef& toolTip
		);

	virtual
	bool
	hideToolTip(WidgetDriver* widgetDriver);

	virtual
	void
	processUiEvents(uint32_t timeLimit);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Engine*
getQtEngine()
{
	return sl::getSingleton<QtEngine> ();
}

//..............................................................................

} // namespace gui
} // namespace axl
