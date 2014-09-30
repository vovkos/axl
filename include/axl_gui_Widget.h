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

class CEngine;
class CCanvas;

//.............................................................................

enum EWidgetStyle
{
	EWidgetStyle_Disabled      = 0x0001,	
	EWidgetStyle_HSizeRepaint  = 0x0010,
	EWidgetStyle_VSizeRepaint  = 0x0020,	
	EWidgetStyle_HScrollAlways = 0x0040,
	EWidgetStyle_VScrollAlways = 0x0080,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EWidgetOrientation
{
	EWidgetOrientation_Vertical = 0,
	EWidgetOrientation_Horizontal,
	EWidgetOrientation__Count
};

//.............................................................................

// master message structure

enum EWidgetMsg
{
	EWidgetMsg_Undefined = 0,

	EWidgetMsg_Close,                   // TWidgetMsg
	EWidgetMsg_SetFocus,                // TWidgetMsg
	EWidgetMsg_KillFocus,               // TWidgetMsg
	EWidgetMsg_Size,                    // TWidgetMsgT <uint_t OrientationMask> 
	EWidgetMsg_Scroll,                  // TWidgetMsgT <uint_t OrientationMask>
	EWidgetMsg_Paint,                   // TWidgetPaintMsg

	EWidgetMsg_MouseMove,               // TWidgetMouseMsg
	EWidgetMsg_MouseLeave,              // TWidgetMsg
	EWidgetMsg_MouseButtonDown,         // TWidgetMouseButtonMsg
	EWidgetMsg_MouseButtonUp,           // TWidgetMouseButtonMsg
	EWidgetMsg_MouseButtonDoubleClick,  // TWidgetMouseButtonMsg
	EWidgetMsg_MouseWheel,              // TWidgetMouseWheelMsg
	EWidgetMsg_MouseCaptureLost,        // TWidgetMsg

	EWidgetMsg_KeyDown,                 // TWidgetKeyMsg
	EWidgetMsg_KeyUp,                   // TWidgetKeyMsg

	EWidgetMsg_ThreadMsg,               // TWidgetThreadMsg

	EWidgetMsg_Gdi,                     // TWidgetGdiMsg
	EWidgetMsg_Qt,                      // TWidgetQtMsg
	EWidgetMsg_Gtk,                     // TWidgetGtkMsg
	EWidgetMsg_User,                    // TWidgetMsg

	EWidgetMsg__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetMsg
{
	EWidgetMsg m_msgKind;

	TWidgetMsg ()
	{
		m_msgKind = EWidgetMsg_Undefined;
	}
	
	TWidgetMsg (EWidgetMsg msgKind)
	{
		m_msgKind = msgKind;
	}
};

//.............................................................................

template <typename T>
struct TWidgetMsgT: TWidgetMsg
{
	T m_param;

	TWidgetMsgT ()
	{
	}
	
	TWidgetMsgT (
		EWidgetMsg msgKind,
		const T& param	
		)
	{
		m_msgKind = msgKind;
		m_param = param;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetPaintMsg: TWidgetMsg
{
	CCanvas* m_canvas;
	TRect m_rect;

	TWidgetPaintMsg ()
	{		
		m_msgKind = EWidgetMsg_Paint;
		m_canvas = NULL;
	}

	TWidgetPaintMsg (
		CCanvas* canvas,
		const TRect& rect
		)
	{
		m_msgKind = EWidgetMsg_Paint;
		m_canvas = canvas;
		m_rect = rect;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetMouseMsg: TWidgetMsg
{
	TPoint m_point;
	uint_t m_buttons;
	uint_t m_modifierKeys;
	EMouseButton m_button;

	TWidgetMouseMsg ()
	{
		m_buttons = 0;
		m_modifierKeys = 0;		
		m_button = EMouseButton_None;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetMouseWheelMsg: TWidgetMouseMsg
{
	int m_wheelDelta;	

	TWidgetMouseWheelMsg ()
	{
		m_msgKind = EWidgetMsg_MouseWheel;
		m_wheelDelta = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetKeyMsg: TWidgetMsg
{
	uint_t m_key;
	uint_t m_modifierKeys;
	utf32_t m_char;

	TWidgetKeyMsg ()
	{
		m_key = 0;
		m_modifierKeys = 0;
		m_char = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetThreadMsg: TWidgetMsg
{
	uint_t m_code;
	ref::CPtrT <void> m_params;
};

//.............................................................................

// engine-specific messages 

#if (_AXL_ENV == AXL_ENV_WIN)

struct TWidgetGdiMsg: TWidgetMsg
{
	intptr_t m_wmMsg;
	intptr_t m_wParam;
	intptr_t m_lParam;
	intptr_t m_lResult;

	TWidgetGdiMsg (
		intptr_t wmMsg,
		intptr_t wParam = 0,
		intptr_t lParam = 0
		):
		TWidgetMsg (EWidgetMsg_Gdi)
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

struct TWidgetQtMsg: TWidgetMsg
{
	QEvent* m_event;

	TWidgetQtMsg ():
		TWidgetMsg (EWidgetMsg_Qt)
	{
	}
};

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_GTK)

struct TWidgetGtkMsg: TWidgetMsg
{
	// not yet

	TWidgetGtkMsg ():
		TWidgetMsg (EWidgetMsg_Gtk)
	{
	}
};

#endif

//.............................................................................

// scroll bars

struct TWidgetScrollBar
{
	size_t m_end;
	size_t m_page;
	size_t m_pos;

	TWidgetScrollBar ()
	{
		m_end = 0;
		m_page = 0;
		m_pos = 0;
	}

	TWidgetScrollBar (
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

class CWidget
{
protected:
	friend class CEngine;

	CEngine* m_engine;
	CCursor* m_cursor;
	CFont* m_baseFont;
	TTextAttr m_baseTextAttr;
	TPalette m_palette;
	TSize m_size;
	TPoint m_caretPos;
	TSize m_caretSize;
	bool m_isCaretVisible;
	uint_t m_style;
	uint_t m_msgMask;	
	TWidgetScrollBar m_scrollBarArray [2];	
	
public:
	CWidget (CEngine* engine);

	CEngine* 
	getEngine ()
	{
		return m_engine;
	}

	CCursor* 
	getCursor ()
	{
		return m_cursor;
	}

	CFont* 
	getBaseFont ()
	{
		return m_baseFont;
	}

	TSize
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
	checkMsgMask (EWidgetMsg msgKind)
	{
		return (m_msgMask & (1 << msgKind)) != 0;
	}

	virtual
	ref::CPtrT <CCanvas>
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
	redraw (const TRect& rect)
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
	setCursor (CCursor* cursor) = 0;

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
	setCaretSize (const TSize& size)
	{
		return setCaretSize (size.m_width, size.m_height);
	}

	bool
	setCaretPos (
		int x, 
		int y
		);

	bool
	setCaretPos (const gui::TPoint& point)
	{
		return setCaretPos (point.m_x, point.m_y);
	}
		
	TWidgetScrollBar*
	getScrollBar (EWidgetOrientation orientation)
	{
		ASSERT (orientation < countof (m_scrollBarArray));
		return &m_scrollBarArray [orientation];
	}

	virtual
	bool
	updateScrollBar (EWidgetOrientation orientation) = 0;

	bool
	updateScrollBars ()
	{
		return 
			updateScrollBar (EWidgetOrientation_Vertical) &&
			updateScrollBar (EWidgetOrientation_Horizontal);
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
		const ref::CPtrT <void>& params
		) = 0;

	virtual
	void
	processWidgetMsg (
		TWidgetMsg* msg,
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
	axl::gui::TWidgetMsg* msg, \
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
