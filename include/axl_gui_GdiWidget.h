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
	EWm_First = 0x0700, // WM_USER = 0x0400, reserve 512 codes just in case
	EWm_ThreadMsg,
};

struct TNotify: NMHDR
{
	void* pParam;
};

//.............................................................................

inline
int 
GetScrollBarFromOrientation (EWidgetOrientation Orientation)
{
	return Orientation == EWidgetOrientation_Horizontal ? SB_HORZ : SB_VERT;
}

void
BuildScrollInfo (
	SCROLLINFO* pScrollInfo,
	size_t Max,
	size_t Page,
	size_t Pos
	);

inline
void
GetScrollInfoFromScrollBar (
	const TWidgetScrollBar& ScrollBar,	
	SCROLLINFO* pScrollInfo
	)
{
	BuildScrollInfo (pScrollInfo, ScrollBar.m_End - 1, ScrollBar.m_Page, ScrollBar.m_Pos);
}

//.............................................................................

class CGdiWidgetImpl: public CWidget
{
public:
	// this class is needed to get access to protected members in CWidget 
	// also to put part of implementation into .cpp instead of having one huge CGdiWidgetT <>

	LRESULT 
	WindowProc (
		HWND hWnd,
		UINT WmMsg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* pIsHandled
		);

	static
	ref::CPtrT <CCanvas>
	GetCanvas (HWND hWnd);

protected:
	void
	ProcessWmMouse (
		EWidgetMsg MsgKind,
		int x,
		int y,
		EMouseButton Button,
		bool* pIsHandled
		);

	void
	ProcessWmMouseWheel (
		HWND hWnd,
		int WheelDelta,
		bool* pIsHandled
		);

	void
	ProcessWmKey (
		EWidgetMsg MsgKind,
		int Key,
		bool* pIsHandled
		);

	void
	ProcessWmSize (
		HWND hWnd,
		bool* pIsHandled
		);

	void
	ProcessWmScroll (
		HWND hWnd,
		EWidgetOrientation Orientation,
		int Code,
		bool* pIsHandled
		);

	void
	ProcessWmPaint (
		HWND hWnd,
		bool* pIsHandled
		);

	LRESULT
	ProcessWmSetCursor (
		HWND hWnd,
		bool* pIsHandled
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
	CGdiWidgetT (): T (CGdiEngine::GetSingleton ())
	{
		m_BaseTextAttr.m_ForeColor = ::GetSysColor (COLOR_WINDOWTEXT);
		m_BaseTextAttr.m_BackColor = ::GetSysColor (COLOR_WINDOW);
		m_BaseTextAttr.m_FontFlags = 0;
	}

	virtual
	ref::CPtrT <CCanvas>
	GetCanvas ()
	{
		return GetGdiWidget ()->GetCanvas (m_h);
	}

	virtual
	bool
	Redraw (	
		int Left, 
		int Top, 
		int Right, 
		int Bottom		
		)
	{
		if (Left == Right || Top == Bottom)
		{	
			::InvalidateRect (m_h, NULL, false);
		}
		else
		{
			RECT Rect = { Left, Top, Right, Bottom };
			::InvalidateRect (m_h, &Rect, false);
		}

		return true;
	}

	virtual
	bool
	IsFocused ()
	{
		return ::GetFocus () == m_h;
	}

	virtual
	bool
	SetFocus ()
	{
		::SetFocus (m_h);
		return true;		
	}

	virtual
	bool
	SetCursor (CCursor* pCursor)
	{
		ASSERT (pCursor->GetEngine ()->GetEngineKind () == EEngine_Gdi);
		::SetCursor (*(CCursor*) pCursor);
		m_pCursor = pCursor;
		return true;		
	}

	virtual
	bool
	SetMouseCapture ()
	{
		HWND hPrevWnd = ::SetCapture (m_h);
		return true;
	}

	virtual
	bool
	ReleaseMouseCapture ()
	{
		bool_t Result = ::ReleaseCapture ();
		return err::Complete (Result);
	}

	virtual
	bool
	SetCaretVisible (bool IsVisible)
	{
		return true;
	}

	virtual
	bool
	SetCaretPos (
		int x, 
		int y
		)
	{
		return true;
	}

	virtual
	bool
	SetCaretSize (
		uint_t Width, 
		uint_t Height
		)
	{
		return true;
	}

	virtual
	bool
	UpdateScrollBar (EWidgetOrientation Orientation)
	{
		ASSERT (Orientation < countof (m_ScrollBarArray));

		int Bar = GetScrollBarFromOrientation (Orientation);

		SCROLLINFO ScrollInfo;
		GetScrollInfoFromScrollBar (m_ScrollBarArray [Orientation], &ScrollInfo);
		bool_t Result = ::SetScrollInfo (m_h, Bar, &ScrollInfo, TRUE); 
		return err::Complete (Result);
	}

	virtual
	intptr_t
	NotifyParent (
		intptr_t NotifyCode,
		void* pParam = NULL
		)
	{
		HWND hWndParent = ::GetParent (m_h);

		TNotify Notify;
		Notify.code = NotifyCode;
		Notify.hwndFrom = m_h;
		Notify.idFrom = ::GetDlgCtrlID (m_h);
		Notify.pParam = pParam;

		return ::SendMessageW (hWndParent, WM_NOTIFY, Notify.idFrom, (LPARAM) &Notify);
	}

	virtual
	void
	PostThreadMsg (
		uint_t Code,
		const ref::CPtrT <void>& Params
		)
	{
		TWidgetThreadMsg* pMsg = AXL_MEM_NEW (TWidgetThreadMsg);
		pMsg->m_MsgKind = EWidgetMsg_ThreadMsg;
		pMsg->m_Code = Code;
		pMsg->m_Params = Params;

		bool_t Result = ::PostMessageW (m_h, EWm_ThreadMsg, Code, (LPARAM) pMsg);
		if (!Result)
			AXL_MEM_DELETE (pMsg);
	}
	
protected:
	CGdiWidgetImpl*
	GetGdiWidget ()
	{
		return (CGdiWidgetImpl*) (CWidget*) this;
	}
	
	LRESULT 
	WindowProc (
		UINT WmMsg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* pIsHandled
		)
	{
		return GetGdiWidget ()->WindowProc (m_h, WmMsg, wParam, lParam, pIsHandled);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

