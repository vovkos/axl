// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIWIDGET_H

#include "axl_gui_Widget.h"
#include "axl_g_win_Window.h"

namespace axl {
namespace gui {

//.............................................................................

enum EWm
{
	EWm_First = 0x0600, // WM_USER = 0x0400, reserve 512 codes just in case
	EWm_ThreadMsg,
};

struct TNotify: NMHDR
{
	void* param;
};

//.............................................................................

inline
int 
getScrollBarFromOrientation (EWidgetOrientation orientation)
{
	return orientation == EWidgetOrientation_Horizontal ? SB_HORZ : SB_VERT;
}

void
buildScrollInfo (
	SCROLLINFO* scrollInfo,
	size_t max,
	size_t page,
	size_t pos
	);

inline
void
getScrollInfoFromScrollBar (
	const TWidgetScrollBar& scrollBar,	
	SCROLLINFO* scrollInfo
	)
{
	buildScrollInfo (scrollInfo, scrollBar.m_end - 1, scrollBar.m_page, scrollBar.m_pos);
}

//.............................................................................

class CGdiWidgetImpl: public CWidget
{
public:
	// this class is needed to get access to protected members in CWidget 
	// also to put part of implementation into .cpp instead of having one huge CGdiWidgetT <>

	LRESULT 
	windowProc (
		HWND hWnd,
		UINT wmMsg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* isHandled_o
		);

	static
	ref::CPtrT <CCanvas>
	getCanvas (HWND hWnd);

protected:
	void
	processWmMouse (
		EWidgetMsg msgKind,
		int x,
		int y,
		EMouseButton button,
		bool* isHandled_o
		);

	void
	processWmMouseWheel (
		HWND hWnd,
		int wheelDelta,
		bool* isHandled_o
		);

	void
	processWmKey (
		EWidgetMsg msgKind,
		int key,
		bool* isHandled_o
		);

	void
	processWmSize (
		HWND hWnd,
		bool* isHandled_o
		);

	void
	processWmScroll (
		HWND hWnd,
		EWidgetOrientation orientation,
		int code,
		bool* isHandled_o
		);

	void
	processWmPaint (
		HWND hWnd,
		bool* isHandled_o
		);

	LRESULT
	processWmSetCursor (
		HWND hWnd,
		bool* isHandled_o
		);
};

//.............................................................................

template <typename T>
class CGdiWidgetT: 
	public T,
	public g::win::CWindowImplT <CGdiWidgetT <T> >
{
	friend class CGdiEngine;
	friend class g::win::CWindowImplT <CGdiWidgetT <T> >;

public:
	CGdiWidgetT (): T (CGdiEngine::getSingleton ())
	{
		m_baseTextAttr.m_foreColor = ::GetSysColor (COLOR_WINDOWTEXT);
		m_baseTextAttr.m_backColor = ::GetSysColor (COLOR_WINDOW);
		m_baseTextAttr.m_fontFlags = 0;
	}

	virtual
	ref::CPtrT <CCanvas>
	getCanvas ()
	{
		return getGdiWidget ()->getCanvas (m_h);
	}

	virtual
	bool
	redraw (	
		int left, 
		int top, 
		int right, 
		int bottom		
		)
	{
		if (left == right || top == bottom)
		{	
			::InvalidateRect (m_h, NULL, false);
		}
		else
		{
			RECT rect = { left, top, right, bottom };
			::InvalidateRect (m_h, &rect, false);
		}

		return true;
	}

	virtual
	bool
	isFocused ()
	{
		return ::GetFocus () == m_h;
	}

	virtual
	bool
	setFocus ()
	{
		::SetFocus (m_h);
		return true;		
	}

	virtual
	bool
	setCursor (CCursor* cursor)
	{
		ASSERT (cursor->getEngine ()->getEngineKind () == EEngine_Gdi);
		::SetCursor (*(CCursor*) cursor);
		m_cursor = cursor;
		return true;		
	}

	virtual
	bool
	setMouseCapture ()
	{
		HWND hPrevWnd = ::SetCapture (m_h);
		return true;
	}

	virtual
	bool
	releaseMouseCapture ()
	{
		bool_t result = ::ReleaseCapture ();
		return err::complete (result);
	}

	virtual
	bool
	setCaretVisible (bool isVisible)
	{
		return true;
	}

	virtual
	bool
	setCaretPos (
		int x, 
		int y
		)
	{
		return true;
	}

	virtual
	bool
	setCaretSize (
		uint_t width, 
		uint_t height
		)
	{
		return true;
	}

	virtual
	bool
	updateScrollBar (EWidgetOrientation orientation)
	{
		ASSERT (orientation < countof (m_scrollBarArray));

		int bar = getScrollBarFromOrientation (orientation);

		SCROLLINFO scrollInfo;
		getScrollInfoFromScrollBar (m_scrollBarArray [orientation], &scrollInfo);
		bool_t result = ::SetScrollInfo (m_h, bar, &scrollInfo, TRUE); 
		return err::complete (result);
	}

	virtual
	intptr_t
	notifyParent (
		intptr_t notifyCode,
		void* param = NULL
		)
	{
		HWND hWndParent = ::GetParent (m_h);

		TNotify notify;
		notify.code = notifyCode;
		notify.hwndFrom = m_h;
		notify.idFrom = ::GetDlgCtrlID (m_h);
		notify.param = param;

		return ::SendMessageW (hWndParent, WM_NOTIFY, notify.idFrom, (LPARAM) &notify);
	}

	virtual
	void
	postThreadMsg (
		uint_t code,
		const ref::CPtrT <void>& params
		)
	{
		TWidgetThreadMsg* msg = AXL_MEM_NEW (TWidgetThreadMsg);
		msg->m_msgKind = EWidgetMsg_ThreadMsg;
		msg->m_code = code;
		msg->m_params = params;

		bool_t result = ::PostMessageW (m_h, EWm_ThreadMsg, code, (LPARAM) msg);
		if (!result)
			AXL_MEM_DELETE (msg);
	}
	
protected:
	CGdiWidgetImpl*
	getGdiWidget ()
	{
		return (CGdiWidgetImpl*) (CWidget*) this;
	}
	
	LRESULT 
	windowProc (
		UINT wmMsg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* isHandled_o
		)
	{
		return getGdiWidget ()->windowProc (m_h, wmMsg, wParam, lParam, isHandled_o);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

