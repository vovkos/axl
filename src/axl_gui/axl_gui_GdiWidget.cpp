#include "pch.h"
#include "axl_gui_GdiWidget.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//.............................................................................

void
buildScrollInfo (
	SCROLLINFO* scrollInfo,
	size_t max,
	size_t page,
	size_t pos
	)
{
	memset (scrollInfo, 0, sizeof (SCROLLINFO));
	scrollInfo->cbSize = sizeof (SCROLLINFO);
	scrollInfo->fMask = SIF_ALL;
	scrollInfo->nMin  = 0;
	scrollInfo->nMax  = max;
	scrollInfo->nPage = page;
	scrollInfo->nPos  = pos;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t 
getModifierKeys ()
{
	uint_t modifierKeys = 0;

	if (::GetAsyncKeyState (VK_SHIFT) & 0x8000)
		modifierKeys |= EModifierKey_Shift;

	if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
		modifierKeys |= EModifierKey_Ctrl;

	if (::GetAsyncKeyState (VK_MENU) & 0x8000)
		modifierKeys |= EModifierKey_Alt;

	return modifierKeys;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t 
getMouseButtons ()
{
	uint_t buttons = 0;

	if (::GetAsyncKeyState (VK_LBUTTON) & 0x8000)
		buttons |= EMouseButton_Left;

	if (::GetAsyncKeyState (VK_RBUTTON) & 0x8000)
		buttons |= EMouseButton_Right;

	if (::GetAsyncKeyState (VK_MBUTTON) & 0x8000)
		buttons |= EMouseButton_Medium;

	return buttons;
}

//.............................................................................

ref::CPtrT <CCanvas>
CGdiWidgetImpl::getCanvas (HWND hWnd)
{
	HDC hdc = ::GetDC (hWnd);

	ref::CPtrT <CGdiCanvas> dc = AXL_REF_NEW (ref::CBoxT <CGdiCanvas>);	
	dc->attach (hdc, hWnd, CGdiCanvas::EDestruct_ReleaseDc);
	return dc;
}

LRESULT 
CGdiWidgetImpl::windowProc (
	HWND hWnd,
	UINT wmMsg, 
	WPARAM wParam, 
	LPARAM lParam, 
	bool* isHandled_o
	)
{
	ASSERT (m_engine->getEngineKind () == EEngine_Gdi);

	if (checkMsgMask (EWidgetMsg_Gdi))
	{
		TWidgetGdiMsg msg (wmMsg, wParam, lParam);
		
		bool isHandled = true;
		processWidgetMsg (&msg, &isHandled);
		if (isHandled)
			return msg.m_lResult;
	}

	LRESULT lResult = 0;
	switch (wmMsg)
	{
	case WM_DESTROY:
		if (checkMsgMask (EWidgetMsg_Close))
			processWidgetMsg (&TWidgetMsg (EWidgetMsg_Close), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_SETFOCUS:
		if (checkMsgMask (EWidgetMsg_SetFocus))
			processWidgetMsg (&TWidgetMsg (EWidgetMsg_SetFocus), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_KILLFOCUS:
		if (checkMsgMask (EWidgetMsg_KillFocus))
			processWidgetMsg (&TWidgetMsg (EWidgetMsg_KillFocus), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_SIZE:
		if (checkMsgMask (EWidgetMsg_Size))
			processWmSize (hWnd, isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_HSCROLL:
		if (checkMsgMask (EWidgetMsg_Scroll))
			processWmScroll (hWnd, EWidgetOrientation_Horizontal, LOWORD (wParam), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_VSCROLL:
		if (checkMsgMask (EWidgetMsg_Scroll))
			processWmScroll (hWnd, EWidgetOrientation_Vertical, LOWORD (wParam), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_PAINT:
		if (checkMsgMask (EWidgetMsg_Paint))
			processWmPaint (hWnd, isHandled_o);
		else
			*isHandled_o = false;

		break;

	// mouse messages

	case WM_SETCURSOR:
		lResult = processWmSetCursor (hWnd, isHandled_o);
		break;

	case WM_MOUSEMOVE:
		if (checkMsgMask (EWidgetMsg_MouseMove))
			processWmMouse (
				EWidgetMsg_MouseMove, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_None,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_MOUSELEAVE:
		if (checkMsgMask (EWidgetMsg_MouseLeave))
			processWidgetMsg (&TWidgetMsg (EWidgetMsg_MouseLeave), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_CAPTURECHANGED:
		if (checkMsgMask (EWidgetMsg_MouseCaptureLost))
			processWidgetMsg (&TWidgetMsg (EWidgetMsg_MouseCaptureLost), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONDOWN:
		if (checkMsgMask (EWidgetMsg_MouseButtonDown))
			processWmMouse (
				EWidgetMsg_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Left,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONUP:
		if (checkMsgMask (EWidgetMsg_MouseButtonUp))
			processWmMouse (
				EWidgetMsg_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Left,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONDBLCLK:
		if (checkMsgMask (EWidgetMsg_MouseButtonDoubleClick))
			processWmMouse (
				EWidgetMsg_MouseButtonDoubleClick, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Left,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;		

	case WM_RBUTTONDOWN:
		if (checkMsgMask (EWidgetMsg_MouseButtonDown))
			processWmMouse (
				EWidgetMsg_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Right,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONUP:
		if (checkMsgMask (EWidgetMsg_MouseButtonUp))
			processWmMouse (
				EWidgetMsg_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Right,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONDBLCLK:
		if (checkMsgMask (EWidgetMsg_MouseButtonDoubleClick))
			processWmMouse (
				EWidgetMsg_MouseButtonDoubleClick,
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Right,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;		

	case WM_MBUTTONDOWN:
		if (checkMsgMask (EWidgetMsg_MouseButtonDown))
			processWmMouse (
				EWidgetMsg_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Medium,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONUP:
		if (checkMsgMask (EWidgetMsg_MouseButtonUp))
			processWmMouse (
				EWidgetMsg_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Medium,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONDBLCLK:
		if (checkMsgMask (EWidgetMsg_MouseButtonDoubleClick))
			processWmMouse (
				EWidgetMsg_MouseButtonDoubleClick, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Medium,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;		

	case WM_MOUSEWHEEL:
		if (checkMsgMask (EWidgetMsg_MouseWheel))
			processWmMouseWheel (
				hWnd,
				(short) HIWORD(wParam) / WHEEL_DELTA,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	// keyboard
		
	case WM_KEYDOWN:
		if (checkMsgMask (EWidgetMsg_KeyDown))
			processWmKey (EWidgetMsg_KeyDown, (uint_t) wParam, isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_KEYUP:
		if (checkMsgMask (EWidgetMsg_KeyUp))
			processWmKey (EWidgetMsg_KeyUp, (uint_t) wParam, isHandled_o);
		else
			*isHandled_o = false;

		break;

/*	case WM_MOUSEACTIVATE:
		lResult = axl_win_TPropertyCtrl_IsMenu (propertyCtrl) ? MA_NOACTIVATE : defWindowProc (hWnd, msg, wParam, lParam);
		break;

	case WM_GETDLGCODE:
		lResult = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_THEMECHANGED:
		TStockCtrlPaint_CloseThemes (&propertyCtrl->m_stockCtrlPaint);
		break;

	case WM_NCPAINT:
		TStockCtrlPaint_NcPaintEdge (&propertyCtrl->m_stockCtrlPaint, hWnd, (HRGN) wParam);
		break;

	case WM_TIMER:
		lResult = axl_win_TPropertyCtrl_OnTimer (propertyCtrl, (int) wParam);
		break;
*/

	default:
		*isHandled_o = false;
	}

	return lResult;
}

void
CGdiWidgetImpl::processWmMouse (
	EWidgetMsg msgKind,
	int x,
	int y,
	EMouseButton button,
	bool* isHandled_o
	)
{
	TWidgetMouseMsg msg;
	msg.m_msgKind = msgKind;
	msg.m_point.m_x = x;
	msg.m_point.m_y = y;
	msg.m_modifierKeys = getModifierKeys ();
	msg.m_button = button;
	msg.m_buttons = getMouseButtons ();

	processWidgetMsg (&msg, isHandled_o);
};

void
CGdiWidgetImpl::processWmKey (
	EWidgetMsg msgKind,
	int key,
	bool* isHandled_o
	)
{
	TWidgetKeyMsg msg;
	msg.m_msgKind = msgKind;
	msg.m_key = key;
	msg.m_modifierKeys = getModifierKeys ();

	processWidgetMsg (&msg, isHandled_o);
}

void
CGdiWidgetImpl::processWmMouseWheel (
	HWND hWnd,
	int wheelDelta,
	bool* isHandled_o
	)
{
	POINT point;
	::GetCursorPos (&point);
	::ScreenToClient (hWnd, &point);

	TWidgetMouseWheelMsg msg;
	msg.m_msgKind = EWidgetMsg_MouseWheel;
	msg.m_point.m_x = point.x;
	msg.m_point.m_y = point.y;
	msg.m_modifierKeys = getModifierKeys ();
	msg.m_button = EMouseButton_Medium;
	msg.m_buttons = getMouseButtons ();
	msg.m_wheelDelta = wheelDelta;

	processWidgetMsg (&msg, isHandled_o);
};

void
CGdiWidgetImpl::processWmSize (
	HWND hWnd,
	bool* isHandled_o
	)
{
	RECT rect;
	::GetClientRect (hWnd, &rect);

	TSize size (rect.right - rect.left, rect.bottom - rect.top);

	uint_t mask = 0;
	if (m_size.m_width != size.m_width)
		mask |= 1 << EWidgetOrientation_Horizontal;

	if (m_size.m_height != size.m_height)
		mask |= 1 << EWidgetOrientation_Vertical;

	m_size = size;

	processWidgetMsg (&TWidgetMsgT <uint_t> (EWidgetMsg_Size, mask), isHandled_o);
}

void
CGdiWidgetImpl::processWmScroll (
	HWND hWnd,
	EWidgetOrientation orientation,
	int code,
	bool* isHandled_o
	)
{
	ASSERT ((size_t) orientation < 2);

	TWidgetScrollBar* scrollBar = &m_scrollBarArray [orientation];

	int bar = getScrollBarFromOrientation (orientation);

	SCROLLINFO scrollInfo = { 0 };
	scrollInfo.cbSize = sizeof (scrollInfo);
	scrollInfo.fMask = SIF_POS | SIF_TRACKPOS;
	
	::GetScrollInfo (hWnd, bar, &scrollInfo);

	intptr_t newPos = scrollInfo.nPos;
	intptr_t maxPos = scrollBar->getMaxPos ();
	
	switch (code)
	{
	case SB_TOP:
		newPos = 0;
		break;

	case SB_LINEUP:
		newPos--;
		break;

	case SB_LINEDOWN:
		newPos++;
		break;

	case SB_PAGEUP:
		newPos -= scrollBar->m_page; 
		break;

	case SB_PAGEDOWN:
		newPos += scrollBar->m_page; 
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		newPos = scrollInfo.nTrackPos; 
		break;
	}

	if (newPos < 0)
		newPos = 0;
	
	if (newPos > maxPos)
		newPos = maxPos;

	if (newPos != scrollInfo.nPos)
	{
		scrollInfo.fMask = SIF_POS;
		scrollInfo.nPos = (int) newPos;
		::SetScrollInfo (hWnd, bar, &scrollInfo, TRUE);
	}

	scrollBar->m_pos = newPos;

	processWidgetMsg (&TWidgetMsgT <uint_t> (EWidgetMsg_Scroll, 1 << orientation), isHandled_o);
}

void
CGdiWidgetImpl::processWmPaint (
	HWND hWnd,
	bool* isHandled_o
	)
{
	PAINTSTRUCT paintStruct;
	HDC hdc = ::BeginPaint (hWnd, &paintStruct);

	CGdiCanvas dc;
	dc.attach (hdc, NULL, CGdiCanvas::EDestruct_None);
	dc.m_baseFont = m_baseFont;
	dc.m_baseTextAttr = m_baseTextAttr;
	dc.m_palette = m_palette;

	TRect rect (
		paintStruct.rcPaint.left, 
		paintStruct.rcPaint.top, 
		paintStruct.rcPaint.right, 
		paintStruct.rcPaint.bottom
		);

	bool isHandled = true; // ignore it
	processWidgetMsg (&TWidgetPaintMsg (&dc, rect), &isHandled);

	::EndPaint (hWnd, &paintStruct);
}

LRESULT
CGdiWidgetImpl::processWmSetCursor (
	HWND hWnd,
	bool* isHandled_o
	)
{
	POINT mousePos;
	RECT clientRect;
	
	::GetCursorPos (&mousePos);
	::ScreenToClient (hWnd, &mousePos);
	::GetClientRect (hWnd, &clientRect);
	
	bool_t isOnClientRect = ::PtInRect (&clientRect, mousePos);	
	if (!isOnClientRect)
	{
		*isHandled_o = false; 
		return 0;
	}

	if (!m_cursor)
	{
		::SetCursor (NULL);
	}
	else
	{
		ASSERT (m_cursor->getEngine ()->getEngineKind () == EEngine_Gdi);
		::SetCursor (*(CGdiCursor*) m_cursor);
	}

	return TRUE;
}

//.............................................................................

} // namespace gui
} // namespace axl
