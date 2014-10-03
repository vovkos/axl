// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_WIDGET_H

#include "axl_gui_Canvas.h"
#include "axl_gui_Cursor.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace gui {

class Engine;
class Canvas;

//.............................................................................

enum WidgetStyleFlag
{
	WidgetStyleFlag_Disabled      = 0x0001,	
	WidgetStyleFlag_HSizeRepaint  = 0x0010,
	WidgetStyleFlag_VSizeRepaint  = 0x0020,	
	WidgetStyleFlag_HScrollAlways = 0x0040,
	WidgetStyleFlag_VScrollAlways = 0x0080,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum WidgetOrientation
{
	WidgetOrientation_Vertical = 0,
	WidgetOrientation_Horizontal,
	WidgetOrientation__Count
};

//.............................................................................

// master message structure

enum WidgetMsgCode
{
	WidgetMsgCode_Undefined = 0,

	WidgetMsgCode_Close,                   // TWidgetMsg
	WidgetMsgCode_SetFocus,                // TWidgetMsg
	WidgetMsgCode_KillFocus,               // TWidgetMsg
	WidgetMsgCode_Size,                    // TWidgetMsgParamT <uint_t OrientationMask> 
	WidgetMsgCode_Scroll,                  // TWidgetMsgParamT <uint_t OrientationMask>
	WidgetMsgCode_Paint,                   // TWidgetPaintMsg

	WidgetMsgCode_MouseMove,               // TWidgetMouseMsg
	WidgetMsgCode_MouseLeave,              // TWidgetMsg
	WidgetMsgCode_MouseButtonDown,         // TWidgetMouseButtonMsg
	WidgetMsgCode_MouseButtonUp,           // TWidgetMouseButtonMsg
	WidgetMsgCode_MouseButtonDoubleClick,  // TWidgetMouseButtonMsg
	WidgetMsgCode_MouseWheel,              // TWidgetMouseWheelMsg
	WidgetMsgCode_MouseCaptureLost,        // TWidgetMsg

	WidgetMsgCode_KeyDown,                 // TWidgetKeyMsg
	WidgetMsgCode_KeyUp,                   // TWidgetKeyMsg

	WidgetMsgCode_ThreadMsg,               // TWidgetThreadMsg

	WidgetMsgCode_Gdi,                     // TWidgetGdiMsg
	WidgetMsgCode_Qt,                      // TWidgetQtMsg
	WidgetMsgCode_Gtk,                     // TWidgetGtkMsg
	WidgetMsgCode_User,                    // TWidgetMsg

	WidgetMsgCode__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMsg
{
	WidgetMsgCode m_msgCode;

	WidgetMsg ()
	{
		m_msgCode = WidgetMsgCode_Undefined;
	}
	
	WidgetMsg (WidgetMsgCode msgCode)
	{
		m_msgCode = msgCode;
	}
};

//.............................................................................

template <typename T>
struct WidgetMsgParam: WidgetMsg
{
	T m_param;

	WidgetMsgParam ()
	{
	}
	
	WidgetMsgParam (
		WidgetMsgCode msgCode,
		const T& param	
		)
	{
		m_msgCode = msgCode;
		m_param = param;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetPaintMsg: WidgetMsg
{
	Canvas* m_canvas;
	Rect m_rect;

	WidgetPaintMsg ()
	{		
		m_msgCode = WidgetMsgCode_Paint;
		m_canvas = NULL;
	}

	WidgetPaintMsg (
		Canvas* canvas,
		const Rect& rect
		)
	{
		m_msgCode = WidgetMsgCode_Paint;
		m_canvas = canvas;
		m_rect = rect;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMouseMsg: WidgetMsg
{
	Point m_point;
	uint_t m_buttons;
	uint_t m_modifierKeys;
	MouseButton m_button;

	WidgetMouseMsg ()
	{
		m_buttons = 0;
		m_modifierKeys = 0;		
		m_button = MouseButton_None;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMouseWheelMsg: WidgetMouseMsg
{
	int m_wheelDelta;	

	WidgetMouseWheelMsg ()
	{
		m_msgCode = WidgetMsgCode_MouseWheel;
		m_wheelDelta = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetKeyMsg: WidgetMsg
{
	uint_t m_key;
	uint_t m_modifierKeys;
	utf32_t m_char;

	WidgetKeyMsg ()
	{
		m_key = 0;
		m_modifierKeys = 0;
		m_char = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetThreadMsg: WidgetMsg
{
	uint_t m_code;
	ref::Ptr <void> m_params;
};

//.............................................................................

// engine-specific messages 

#if (_AXL_ENV == AXL_ENV_WIN)

struct WidgetGdiMsg: WidgetMsg
{
	intptr_t m_wmMsg;
	intptr_t m_wParam;
	intptr_t m_lParam;
	intptr_t m_lResult;

	WidgetGdiMsg (
		intptr_t wmMsg,
		intptr_t wParam = 0,
		intptr_t lParam = 0
		):
		WidgetMsg (WidgetMsgCode_Gdi)
	{
		m_wmMsg = wmMsg;
		m_wParam = wParam;
		m_lParam = lParam;
		m_lResult = 0;
	}
};

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_GUI_QT)

struct WidgetQtMsg: WidgetMsg
{
	QEvent* m_event;

	WidgetQtMsg ():
		WidgetMsg (WidgetMsgCode_Qt)
	{
	}
};

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_GTK)

struct WidgetGtkMsg: WidgetMsg
{
	// not yet

	WidgetGtkMsg ():
		WidgetMsg (WidgetMsgCode_Gtk)
	{
	}
};

#endif

//.............................................................................

// scroll bars

struct WidgetScrollBar
{
	size_t m_end;
	size_t m_page;
	size_t m_pos;

	WidgetScrollBar ()
	{
		m_end = 0;
		m_page = 0;
		m_pos = 0;
	}

	WidgetScrollBar (
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
	isRelevant () const
	{
		return m_page >= m_end;
	}

	bool
	isMaxed () const
	{
		return m_pos + m_page + 1 >= m_end;
	}

	size_t 
	getMaxPos ()
	{
		return m_end > m_page ? m_end - m_page : 0;
	}
};

//.............................................................................

class Widget
{
protected:
	friend class Engine;

	Engine* m_engine;
	Cursor* m_cursor;
	Font* m_baseFont;
	TextAttr m_baseTextAttr;
	Palette m_palette;
	Size m_size;
	Point m_caretPos;
	Size m_caretSize;
	bool m_isCaretVisible;
	uint_t m_style;
	uint_t m_msgMask;	
	WidgetScrollBar m_scrollBarArray [2];	
	
public:
	Widget (Engine* engine);

	Engine* 
	getEngine ()
	{
		return m_engine;
	}

	Cursor* 
	getCursor ()
	{
		return m_cursor;
	}

	Font* 
	getBaseFont ()
	{
		return m_baseFont;
	}

	Size
	getSize ()
	{
		return m_size;
	}

	uint_t
	getStyle ()
	{
		return m_style;
	}

	uint_t
	getMsgMask ()
	{
		return m_msgMask;
	}
	
	bool
	checkMsgMask (WidgetMsgCode msgCode)
	{
		return (m_msgMask & (1 << msgCode)) != 0;
	}

	virtual
	ref::Ptr <Canvas>
	getCanvas () = 0;

	virtual
	bool
	redraw (
		int left, 
		int top, 
		int right, 
		int bottom		
		) = 0;

	bool
	redraw (const Rect& rect)
	{
		return redraw (rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);
	}

	bool
	redraw ()
	{
		return redraw (0, 0, 0, 0);
	}

	virtual
	bool
	isFocused () = 0;

	virtual
	bool
	setFocus () = 0;

	virtual
	bool
	setCursor (Cursor* cursor) = 0;

	virtual
	bool
	setMouseCapture () = 0;

	virtual
	bool
	releaseMouseCapture () = 0;

	bool
	isCaretVisible ()
	{
		return m_isCaretVisible;
	}

	bool
	setCaretVisible (bool isVisible);

	bool
	setCaretWidth (uint_t width)
	{
		return setCaretSize (width, m_caretSize.m_height);
	}

	bool
	setCaretHeight (uint_t height)
	{
		return setCaretSize (m_caretSize.m_width, height);
	}

	virtual
	bool
	setCaretSize (
		uint_t width,
		uint_t height
		);

	bool
	setCaretSize (const Size& size)
	{
		return setCaretSize (size.m_width, size.m_height);
	}

	bool
	setCaretPos (
		int x, 
		int y
		);

	bool
	setCaretPos (const gui::Point& point)
	{
		return setCaretPos (point.m_x, point.m_y);
	}
		
	WidgetScrollBar*
	getScrollBar (WidgetOrientation orientation)
	{
		ASSERT (orientation < countof (m_scrollBarArray));
		return &m_scrollBarArray [orientation];
	}

	virtual
	bool
	updateScrollBar (WidgetOrientation orientation) = 0;

	bool
	updateScrollBars ()
	{
		return 
			updateScrollBar (WidgetOrientation_Vertical) &&
			updateScrollBar (WidgetOrientation_Horizontal);
	}

	virtual
	intptr_t
	notifyParent (
		intptr_t notifyCode,
		void* param = NULL
		) = 0;

	virtual
	void
	postThreadMsg (
		uint_t code,
		const ref::Ptr <void>& params
		) = 0;

	virtual
	void
	processWidgetMsg (
		WidgetMsg* msg,
		bool* isHandled_o
		)
	{
	}
};

//.............................................................................

#define AXL_GUI_WIDGET_MSG_MAP_BEGIN() \
virtual \
void \
processWidgetMsg ( \
	axl::gui::WidgetMsg* msg, \
	bool* isHandled_o \
	) \
{ \
	switch (msg->m_msgCode) \
	{

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_HANDLER(msgCode, handler) \
	case msgCode: \
		handler (msg, isHandled_o); \
		break; \

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_MAP_END() \
	default: \
		*isHandled_o = false; \
	} \
} 

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_MAP_END_CHAIN(class) \
	default: \
		*isHandled_o = false; \
	} \
	if (!*isHandled_o) \
	{ \
		*isHandled_o = true; \
		class::processWidgetMsg (msg, isHandled_o); \
	} \
} 

//.............................................................................

} // namespace gui
} // namespace axl
