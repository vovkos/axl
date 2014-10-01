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

enum WidgetStyleKind
{
	WidgetStyleKind_Disabled      = 0x0001,	
	WidgetStyleKind_HSizeRepaint  = 0x0010,
	WidgetStyleKind_VSizeRepaint  = 0x0020,	
	WidgetStyleKind_HScrollAlways = 0x0040,
	WidgetStyleKind_VScrollAlways = 0x0080,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum WidgetOrientationKind
{
	WidgetOrientationKind_Vertical = 0,
	WidgetOrientationKind_Horizontal,
	WidgetOrientationKind__Count
};

//.............................................................................

// master message structure

enum WidgetMsgKind
{
	WidgetMsgKind_Undefined = 0,

	WidgetMsgKind_Close,                   // TWidgetMsg
	WidgetMsgKind_SetFocus,                // TWidgetMsg
	WidgetMsgKind_KillFocus,               // TWidgetMsg
	WidgetMsgKind_Size,                    // TWidgetMsgParamT <uint_t OrientationMask> 
	WidgetMsgKind_Scroll,                  // TWidgetMsgParamT <uint_t OrientationMask>
	WidgetMsgKind_Paint,                   // TWidgetPaintMsg

	WidgetMsgKind_MouseMove,               // TWidgetMouseMsg
	WidgetMsgKind_MouseLeave,              // TWidgetMsg
	WidgetMsgKind_MouseButtonDown,         // TWidgetMouseButtonMsg
	WidgetMsgKind_MouseButtonUp,           // TWidgetMouseButtonMsg
	WidgetMsgKind_MouseButtonDoubleClick,  // TWidgetMouseButtonMsg
	WidgetMsgKind_MouseWheel,              // TWidgetMouseWheelMsg
	WidgetMsgKind_MouseCaptureLost,        // TWidgetMsg

	WidgetMsgKind_KeyDown,                 // TWidgetKeyMsg
	WidgetMsgKind_KeyUp,                   // TWidgetKeyMsg

	WidgetMsgKind_ThreadMsg,               // TWidgetThreadMsg

	WidgetMsgKind_Gdi,                     // TWidgetGdiMsg
	WidgetMsgKind_Qt,                      // TWidgetQtMsg
	WidgetMsgKind_Gtk,                     // TWidgetGtkMsg
	WidgetMsgKind_User,                    // TWidgetMsg

	WidgetMsgKind__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMsg
{
	WidgetMsgKind m_msgKind;

	WidgetMsg ()
	{
		m_msgKind = WidgetMsgKind_Undefined;
	}
	
	WidgetMsg (WidgetMsgKind msgKind)
	{
		m_msgKind = msgKind;
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
		WidgetMsgKind msgKind,
		const T& param	
		)
	{
		m_msgKind = msgKind;
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
		m_msgKind = WidgetMsgKind_Paint;
		m_canvas = NULL;
	}

	WidgetPaintMsg (
		Canvas* canvas,
		const Rect& rect
		)
	{
		m_msgKind = WidgetMsgKind_Paint;
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
	MouseButtonKind m_button;

	WidgetMouseMsg ()
	{
		m_buttons = 0;
		m_modifierKeys = 0;		
		m_button = MouseButtonKind_None;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct WidgetMouseWheelMsg: WidgetMouseMsg
{
	int m_wheelDelta;	

	WidgetMouseWheelMsg ()
	{
		m_msgKind = WidgetMsgKind_MouseWheel;
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
		WidgetMsg (WidgetMsgKind_Gdi)
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
		WidgetMsg (WidgetMsgKind_Qt)
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
		WidgetMsg (WidgetMsgKind_Gtk)
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
	checkMsgMask (WidgetMsgKind msgKind)
	{
		return (m_msgMask & (1 << msgKind)) != 0;
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
	getScrollBar (WidgetOrientationKind orientation)
	{
		ASSERT (orientation < countof (m_scrollBarArray));
		return &m_scrollBarArray [orientation];
	}

	virtual
	bool
	updateScrollBar (WidgetOrientationKind orientation) = 0;

	bool
	updateScrollBars ()
	{
		return 
			updateScrollBar (WidgetOrientationKind_Vertical) &&
			updateScrollBar (WidgetOrientationKind_Horizontal);
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
	switch (msg->m_msgKind) \
	{

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_HANDLER(msgKind, handler) \
	case msgKind: \
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
