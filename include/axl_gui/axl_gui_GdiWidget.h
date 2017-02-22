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

#define _AXL_GUI_GDIWIDGET_H

#include "axl_gui_GdiPch.h"
#include "axl_gui_Widget.h"

namespace axl {
namespace gui {

//..............................................................................

enum WmKind
{
	WmKind_First = 0x0600, // WM_USER = 0x0400, reserve 512 codes just in case
	WmKind_ThreadMsg,
};

struct Notify: NMHDR
{
	void* param;
};

//..............................................................................

inline
int
getScrollBarFromOrientation (Orientation orientation)
{
	return orientation == Orientation_Horizontal ? SB_HORZ : SB_VERT;
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
	const WidgetScrollBar& scrollBar,
	SCROLLINFO* scrollInfo
	)
{
	buildScrollInfo (scrollInfo, scrollBar.m_end - 1, scrollBar.m_page, scrollBar.m_pos);
}

//..............................................................................

class GdiWidgetImpl: public Widget
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
	ref::Ptr <Canvas>
	getCanvas (HWND hWnd);

protected:
	void
	processWmMouse (
		WidgetMsgCode msgCode,
		int x,
		int y,
		MouseButton button,
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
		WidgetMsgCode msgCode,
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
		Orientation orientation,
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

//..............................................................................

template <typename T>
class GdiWidget:
	public T,
	public g::win::WindowImpl <GdiWidget <T> >
{
	friend class GdiEngine;
	friend class g::win::WindowImpl <GdiWidget <T> >;

public:
	GdiWidget (): T (GdiEngine::getSingleton ())
	{
		m_baseTextAttr.m_foreColor = ::GetSysColor (COLOR_WINDOWTEXT);
		m_baseTextAttr.m_backColor = ::GetSysColor (COLOR_WINDOW);
		m_baseTextAttr.m_fontFlags = 0;
	}

	virtual
	ref::Ptr <Canvas>
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
	setCursor (Cursor* cursor)
	{
		ASSERT (cursor->getEngine ()->getEngineKind () == EngineKind_Gdi);
		::SetCursor (*(Cursor*) cursor);
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
	updateScrollBar (Orientation orientation)
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

		Notify notify;
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
		const ref::Ptr <void>& params
		)
	{
		WidgetThreadMsg* msg = AXL_MEM_NEW (WidgetThreadMsg);
		msg->m_msgCode = WidgetMsgCode_ThreadMsg;
		msg->m_code = code;
		msg->m_params = params;

		bool_t result = ::PostMessageW (m_h, WmKind_ThreadMsg, code, (LPARAM) msg);
		if (!result)
			AXL_MEM_DELETE (msg);
	}

protected:
	GdiWidgetImpl*
	getGdiWidget ()
	{
		return (GdiWidgetImpl*) (Widget*) this;
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

//..............................................................................

} // namespace gui
} // namespace axl
