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

#define _AXL_GUI_WIDGET_H

#include "axl_gui_Engine.h"
#include "axl_gui_TextAttr.h"

namespace axl {
namespace gui {

class Canvas;
class Widget;

//..............................................................................

// master message structure

enum WidgetMsgCode
{
	WidgetMsgCode_Undefined = 0,

	WidgetMsgCode_Close,                   // WidgetMsg
	WidgetMsgCode_SetFocus,                // WidgetMsg
	WidgetMsgCode_KillFocus,               // WidgetMsg
	WidgetMsgCode_Size,                    // WidgetMsgParam <uint_t OrientationMask>
	WidgetMsgCode_Scroll,                  // WidgetMsgParam <uint_t OrientationMask>
	WidgetMsgCode_Paint,                   // WidgetPaintMsg

	WidgetMsgCode_MouseMove,               // WidgetMouseMsg
	WidgetMsgCode_MouseLeave,              // WidgetMsg
	WidgetMsgCode_MouseButtonDown,         // WidgetMouseButtonMsg
	WidgetMsgCode_MouseButtonUp,           // WidgetMouseButtonMsg
	WidgetMsgCode_MouseButtonDoubleClick,  // WidgetMouseButtonMsg
	WidgetMsgCode_MouseWheel,              // WidgetMouseWheelMsg
	WidgetMsgCode_MouseCaptureLost,        // WidgetMsg

	WidgetMsgCode_KeyDown,                 // WidgetKeyMsg
	WidgetMsgCode_KeyUp,                   // WidgetKeyMsg

	WidgetMsgCode_ThreadMsg,               // WidgetThreadMsg
	WidgetMsgCode_Animate,                 // WidgetMsg
	WidgetMsgCode_ToolTip,                 // WidgetMouseMsg

	WidgetMsgCode__Count,
};

//..............................................................................

struct WidgetMsg
{
	WidgetMsgCode m_msgCode;

	WidgetMsg()
	{
		m_msgCode = WidgetMsgCode_Undefined;
	}

	WidgetMsg(WidgetMsgCode msgCode)
	{
		m_msgCode = msgCode;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
struct WidgetMsgParam: WidgetMsg
{
	T m_param;

	WidgetMsgParam()
	{
	}

	WidgetMsgParam(
		WidgetMsgCode msgCode,
		const T& param
		)
	{
		m_msgCode = msgCode;
		m_param = param;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetPaintMsg: WidgetMsg
{
	Canvas* m_canvas;
	Rect m_rect;
	Rect m_region[4];
	size_t m_regionRectCount;

	WidgetPaintMsg(Canvas* canvas)
	{
		m_msgCode = WidgetMsgCode_Paint;
		m_canvas = canvas;
		m_regionRectCount = 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMouseMsg: WidgetMsg
{
	Point m_point;
	uint_t m_buttons;
	uint_t m_modifierKeys;
	MouseButton m_button;

	WidgetMouseMsg()
	{
		m_buttons = 0;
		m_modifierKeys = 0;
		m_button = MouseButton_None;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMouseWheelMsg: WidgetMouseMsg
{
	int m_wheelDelta;

	WidgetMouseWheelMsg()
	{
		m_msgCode = WidgetMsgCode_MouseWheel;
		m_wheelDelta = 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetKeyMsg: WidgetMsg
{
	uint_t m_key;
	uint_t m_modifierKeys;
	utf32_t m_char;

	WidgetKeyMsg()
	{
		m_key = 0;
		m_modifierKeys = 0;
		m_char = 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetThreadMsg: WidgetMsg
{
	uint_t m_code;
	rc::Ptr<void> m_params;
};

//..............................................................................

// scroll bars

struct WidgetScrollBar
{
	size_t m_end;
	size_t m_page;
	size_t m_pos;

	WidgetScrollBar()
	{
		m_end = 0;
		m_page = 0;
		m_pos = 0;
	}

	WidgetScrollBar(
		size_t end,
		size_t page,
		size_t pos = 0
		)
	{
		m_end = end;
		m_page = page;
		m_pos = 0;
	}

	bool
	isRelevant() const
	{
		return m_page >= m_end;
	}

	bool
	isMaxed() const
	{
		return m_pos + m_page >= m_end;
	}

	size_t
	getMaxPos()
	{
		return m_end > m_page ? m_end - m_page : 0;
	}
};

//..............................................................................

typedef
void
(Widget::*WidgetMsgProc) (
	const WidgetMsg* msg,
	bool* isHandled
	);

struct WidgetMsgMap
{
	WidgetMsgProc m_msgProcTable[WidgetMsgCode__Count];
	WidgetMsgMap* m_baseMap;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetConstructParam
{
	Engine* m_engine;
	void* m_engineWidget;

	WidgetConstructParam(
		Engine* engine,
		void* engineWidget
		)
	{
		m_engine = engine;
		m_engineWidget = engineWidget;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class WidgetDriver: public GuiItem
{
	friend class Engine;
	friend class Widget;

protected:
	void* m_engineWidget;
	Cursor* m_cursor;
	Font* m_font;
	ColorAttr m_colorAttr;
	Palette m_palette;
	Point m_caretPos;
	Size m_caretSize;
	bool m_isCaretVisible;
	WidgetMsgMap* m_msgMap;

public:
	Size m_size;
	WidgetScrollBar m_scrollBarArray[2];

protected:
	WidgetDriver(const WidgetConstructParam& param);

public:
	void*
	getEngineWidget()
	{
		return m_engineWidget;
	}

	Font*
	getFont()
	{
		return m_font;
	}

	bool
	setFont(Font* font);

	ColorAttr
	getColorAttr()
	{
		return m_colorAttr;
	}

	bool
	setColorAttr(
		uint_t foreColor,
		uint_t backColor
		)
	{
		m_colorAttr.setup(foreColor, backColor);
		return redraw();
	}

	bool
	setColorAttr(const ColorAttr& attr)
	{
		m_colorAttr = attr;
		return redraw();
	}

	Palette
	getPalette()
	{
		return m_palette;
	}

	bool
	setPalette(
		const uint_t* colorArray,
		size_t count
		)
	{
		m_palette.setup(colorArray, count);
		return redraw();
	}

	bool
	setPalette(const Palette& palette)
	{
		m_palette = palette;
		return redraw();
	}

	// redraw

	bool
	redraw(
		int left,
		int top,
		int right,
		int bottom
		)
	{
		return m_engine->redrawWidget(this, left, top, right, bottom);
	}

	bool
	redraw(const Rect& rect)
	{
		return redraw(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);
	}

	bool
	redraw()
	{
		return redraw(0, 0, 0, 0);
	}

	bool
	redrawImmediate(
		int left,
		int top,
		int right,
		int bottom
		)
	{
		return m_engine->redrawWidgetImmediate(this, left, top, right, bottom);
	}

	bool
	redrawImmediate(const Rect& rect)
	{
		return redrawImmediate(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);
	}

	bool
	redrawImmediate()
	{
		return redrawImmediate(0, 0, 0, 0);
	}

	bool
	scroll(
		int dx,
		int dy
		)
	{
		return m_engine->scrollWidget(this, dx, dy);
	}

	bool
	scrollRect(
		int left,
		int top,
		int right,
		int bottom,
		int dx,
		int dy
		)
	{
		return m_engine->scrollWidgetRect(this, left, top, right, bottom, dx, dy);
	}

	bool
	scrollRect(
		const Rect& rect,
		int dx,
		int dy
		)
	{
		return scrollRect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, dx, dy);
	}

	bool
	scrollRect(
		int dx,
		int dy
		)
	{
		return scrollRect(0, 0, m_size.m_width, m_size.m_height, dx, dy);
	}

	// focus

	bool
	isFocused()
	{
		return m_engine->isWidgetFocused(this);
	}

	bool
	setFocus()
	{
		return m_engine->setWidgetFocus(this);
	}

	// mouse

	Cursor*
	getCursor()
	{
		return m_cursor;
	}

	bool
	setCursor(Cursor* cursor)
	{
		m_cursor = cursor;
		return m_engine->setWidgetCursor(this, cursor);
	}

	bool
	setMouseCapture()
	{
		return m_engine->setMouseCapture(this);
	}

	bool
	releaseMouseCapture()
	{
		return m_engine->releaseMouse(this);
	}

	// caret

	bool
	isCaretVisible()
	{
		return m_isCaretVisible;
	}

	bool
	setCaretVisible(bool isVisible);

	Size
	getCaretSize()
	{
		return m_caretSize;
	}

	bool
	setCaretWidth(uint_t width)
	{
		return setCaretSize(width, m_caretSize.m_height);
	}

	bool
	setCaretHeight(uint_t height)
	{
		return setCaretSize(m_caretSize.m_width, height);
	}

	bool
	setCaretSize(
		uint_t width,
		uint_t height
		);

	bool
	setCaretSize(const Size& size)
	{
		return setCaretSize(size.m_width, size.m_height);
	}

	Point
	getCaretPos()
	{
		return m_caretPos;
	}

	bool
	setCaretPos(
		int x,
		int y
		);

	bool
	setCaretPos(const Point& point)
	{
		return setCaretPos(point.m_x, point.m_y);
	}

	// tooltips

	bool
	scheduleToolTipMsg(uint_t timeout = 0) // 0 -- use engine default
	{
		return m_engine->scheduleToolTipMsg(this, timeout);
	}

	bool
	cancelToolTipMsg()
	{
		return m_engine->cancelToolTipMsg(this);
	}

	bool
	showToolTip(
		int x,
		int y,
		const sl::StringRef& toolTip
		)
	{
		return m_engine->showToolTip(this, x, y, toolTip);
	}

	bool
	showToolTip(
		const Point& point,
		const sl::StringRef& toolTip
		)
	{
		return m_engine->showToolTip(this, point.m_x, point.m_y, toolTip);
	}

	bool
	hideToolTip()
	{
		return m_engine->hideToolTip(this);
	}

	// scroll bars

	bool
	updateScrollBar(Orientation orientation)
	{
		return m_engine->updateWidgetScrollBar(this, orientation);
	}

	bool
	updateScrollBars()
	{
		return
			updateScrollBar(Orientation_Vertical) &&
			updateScrollBar(Orientation_Horizontal);
	}

	bool
	updateScrollBars(uint_t mask);

	// misc

	void
	notify(
		uint_t code,
		const void* params = NULL
		)
	{
		m_engine->sendWidgetNotification(this, code, params);
	}

	bool
	postThreadMsg(
		uint_t code,
		const rc::Ptr<void>& params
		)
	{
		return m_engine->postWidgetThreadMsg(this, code, params);
	}

	bool
	postThreadMsg(
		uint_t code,
		const void* params,
		size_t size
		);

	bool
	postThreadMsg(
		uint_t code,
		uintptr_t param
		)
	{
		return m_engine->postWidgetThreadMsg(this, code, rc::Ptr<void> ((void*)param, NULL));
	}

	bool
	postThreadMsg(uint_t code)
	{
		return m_engine->postWidgetThreadMsg(this, code, rc::Ptr<void> (NULL, NULL));
	}

	bool
	startAnimation()
	{
		return m_engine->startWidgetAnimation(this);
	}

	void
	stopAnimation()
	{
		m_engine->stopWidgetAnimation(this);
	}

	bool
	checkMsgMap(WidgetMsgCode msgCode)
	{
		ASSERT(msgCode < WidgetMsgCode__Count);
		return m_msgMap ? m_msgMap->m_msgProcTable[msgCode] != NULL : true;
	}

	void
	processMsg(
		const WidgetMsg* msg,
		bool* isHandled
		);
};

//..............................................................................

class Widget
{
	friend class WidgetDriver;

public:
	WidgetDriver m_widgetDriver;

protected:
	Widget(const WidgetConstructParam& param):
		m_widgetDriver(param)
	{
	}

protected:
	virtual
	WidgetMsgMap*
	getWidgetMsgMap()
	{
		return NULL;
	}
};

//..............................................................................

#define AXL_GUI_WIDGET_BEGIN_MSG_MAP(BaseClass) \
virtual \
axl::gui::WidgetMsgMap* \
getWidgetMsgMap() \
{ \
	static axl::gui::WidgetMsgMap map = { 0 }; \
	static bool once = false; \
	if (once) \
		return &map; \
	map.m_baseMap = BaseClass::getWidgetMsgMap(); \
	if (map.m_baseMap) \
		memcpy(map.m_msgProcTable, map.m_baseMap->m_msgProcTable, sizeof(map.m_msgProcTable));

#define AXL_GUI_WIDGET_MSG_HANDLER(msgCode, handler) \
	map.m_msgProcTable[msgCode] = (axl::gui::WidgetMsgProc)handler;

#define AXL_GUI_WIDGET_END_MSG_MAP() \
	once = true; \
	return &map; \
}

//..............................................................................

} // namespace gui
} // namespace axl
