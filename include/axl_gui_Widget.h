// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_WIDGET_H

#include "axl_gui_Canvas.h"
#include "axl_gui_Cursor.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace gui {

struct IEngine;
struct ICanvas;
struct IWidget;

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
	EWidgetMsg m_MsgKind;

	TWidgetMsg ()
	{
		m_MsgKind = EWidgetMsg_Undefined;
	}
	
	TWidgetMsg (EWidgetMsg MsgKind)
	{
		m_MsgKind = MsgKind;
	}
};

//.............................................................................

template <typename T>
struct TWidgetMsgT: TWidgetMsg
{
	T m_Param;

	TWidgetMsgT ()
	{
	}
	
	TWidgetMsgT (
		EWidgetMsg MsgKind,
		const T& Param	
		)
	{
		m_MsgKind = MsgKind;
		m_Param = Param;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetPaintMsg: TWidgetMsg
{
	ICanvas* m_pCanvas;
	TRect m_Rect;

	TWidgetPaintMsg ()
	{		
		m_MsgKind = EWidgetMsg_Paint;
		m_pCanvas = NULL;
	}

	TWidgetPaintMsg (
		ICanvas* pCanvas,
		const TRect& Rect
		)
	{
		m_MsgKind = EWidgetMsg_Paint;
		m_pCanvas = pCanvas;
		m_Rect = Rect;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetMouseMsg: TWidgetMsg
{
	TPoint m_Point;
	uint_t m_Buttons;
	uint_t m_ModifierKeys;
	EMouseButton m_Button;

	TWidgetMouseMsg ()
	{
		m_Buttons = 0;
		m_ModifierKeys = 0;		
		m_Button = EMouseButton_None;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetMouseWheelMsg: TWidgetMouseMsg
{
	int m_WheelDelta;	

	TWidgetMouseWheelMsg ()
	{
		m_WheelDelta = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetKeyMsg: TWidgetMsg
{
	uint_t m_Key;
	uint_t m_ModifierKeys;

	TWidgetKeyMsg ()
	{
		m_Key = 0;
		m_ModifierKeys = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWidgetThreadMsg: TWidgetMsg
{
	uint_t m_Code;
	ref::CPtrT <void> m_Params;
};

//.............................................................................

// engine-specific messages 

#if (_AXL_ENV == AXL_ENV_WIN)

struct TWidgetGdiMsg: TWidgetMsg
{
	intptr_t m_WmMsg;
	intptr_t m_wParam;
	intptr_t m_lParam;
	intptr_t m_lResult;

	TWidgetGdiMsg (
		intptr_t WmMsg,
		intptr_t wParam = 0,
		intptr_t lParam = 0
		):
		TWidgetMsg (EWidgetMsg_Gdi)
	{
		m_WmMsg = WmMsg;
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
	QEvent* m_pEvent;

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
	size_t m_End;
	size_t m_Page;
	size_t m_Pos;

	TWidgetScrollBar ()
	{
		m_End = 0;
		m_Page = 0;
		m_Pos = 0;
	}

	TWidgetScrollBar (
		size_t End,
		size_t Page,
		size_t Pos = 0
		)
	{
		m_End = End;
		m_Page = Page;
		m_Pos = 0;
	}

	bool
	IsRelevant () const
	{
		return m_Page >= m_End;
	}

	bool
	IsMaxed () const
	{
		return m_Pos + m_Page >= m_End;
	}

	size_t 
	GetMaxPos ()
	{
		return m_Page < m_End ? m_End - m_Page : 0;
	}
};

//.............................................................................

struct IWidget: obj::IRoot
{
	// {69997BBD-04F2-46CC-BC2D-8CB2074E0A6F}
	AXL_OBJ_INTERFACE (
		IWidget,
		0x69997bbd, 0x04f2, 0x46cc, 0xbc, 0x2d, 0x8c, 0xb2, 0x07, 0x4e, 0x0a, 0x6f
		)

protected:
	friend struct IEngine;

	IEngine* m_pEngine;
	ICursor* m_pCursor;
	IFont* m_pBaseFont;
	TTextAttr m_BaseTextAttr;
	TPalette m_Palette;
	TSize m_Size;
	TSize m_CaretSize;
	uint_t m_Style;
	uint_t m_MsgMask;	
	TWidgetScrollBar m_ScrollBarArray [2];
	
public:
	IWidget (IEngine* pEngine);

	IEngine* 
	GetEngine ()
	{
		return m_pEngine;
	}

	TSize
	GetSize ()
	{
		return m_Size;
	}

	uint_t
	GetStyle ()
	{
		return m_Style;
	}

	uint_t
	GetMsgMask ()
	{
		return m_MsgMask;
	}
	
	bool
	CheckMsgMask (EWidgetMsg MsgKind)
	{
		return (m_MsgMask & (1 << MsgKind)) != 0;
	}

	virtual
	ref::CPtrT <ICanvas>
	GetCanvas () = 0;

	virtual
	bool
	Redraw (
		int Left, 
		int Top, 
		int Right, 
		int Bottom		
		) = 0;

	bool
	Redraw (const TRect& Rect)
	{
		return Redraw (Rect.m_Left, Rect.m_Top, Rect.m_Right, Rect.m_Bottom);
	}

	bool
	Redraw ()
	{
		return Redraw (0, 0, 0, 0);
	}

	virtual
	bool
	IsFocused () = 0;

	virtual
	bool
	SetFocus () = 0;

	virtual
	bool
	SetCursor (ICursor* pCursor) = 0;

	virtual
	bool
	SetMouseCapture () = 0;

	virtual
	bool
	ReleaseMouseCapture () = 0;

	virtual
	bool
	ShowCaret (	
		int x, 
		int y,
		int Width,
		int Height
		) = 0;

	bool
	ShowCaret (
		const gui::TPoint& Point,
		const gui::TSize& Size
		)
	{
		return ShowCaret (Point.m_x, Point.m_y, Size.m_Width, Size.m_Height);
	}

	virtual
	void
	HideCaret () = 0;

	virtual
	bool
	SetCaretPos (
		int x, 
		int y
		) = 0;

	bool
	SetCaretPos (const gui::TPoint& Point)
	{
		return SetCaretPos (Point.m_x, Point.m_y);
	}
		
	virtual
	bool
	UpdateScrollBar (EWidgetOrientation Orientation) = 0;

	bool
	UpdateScrollBars ()
	{
		return 
			UpdateScrollBar (EWidgetOrientation_Vertical) &&
			UpdateScrollBar (EWidgetOrientation_Horizontal);
	}

	virtual
	intptr_t
	NotifyParent (
		intptr_t NotifyCode,
		void* pParam = NULL
		) = 0;

	virtual
	void
	PostThreadMsg (
		uint_t Code,
		const ref::CPtrT <void>& Params
		) = 0;

	virtual
	void
	ProcessWidgetMsg (
		TWidgetMsg* pMsg,
		bool* pIsHandled
		)
	{
	}
};

//.............................................................................

#define AXL_GUI_WIDGET_MSG_MAP_BEGIN() \
virtual \
void \
ProcessWidgetMsg ( \
	axl::gui::TWidgetMsg* pMsg, \
	bool* pIsHandled \
	) \
{ \
	switch (pMsg->m_MsgKind) \
	{

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_HANDLER(MsgKind, Handler) \
	case MsgKind: \
		Handler (pMsg, pIsHandled); \
		break; \

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_MAP_END() \
	default: \
		*pIsHandled = false; \
	} \
} 

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_GUI_WIDGET_MSG_MAP_CHAIN(Class) \
	default: \
		Class::ProcessWidgetMsg (pMsg, pIsHandled); \
	} \
} 

//.............................................................................

} // namespace gui
} // namespace axl
