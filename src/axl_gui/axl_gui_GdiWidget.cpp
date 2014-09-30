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
		modifierKeys |= ModifierKeyKind_Shift;

	if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
		modifierKeys |= ModifierKeyKind_Ctrl;

	if (::GetAsyncKeyState (VK_MENU) & 0x8000)
		modifierKeys |= ModifierKeyKind_Alt;

	return modifierKeys;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t 
getMouseButtons ()
{
	uint_t buttons = 0;

	if (::GetAsyncKeyState (VK_LBUTTON) & 0x8000)
		buttons |= MouseButtonKind_Left;

	if (::GetAsyncKeyState (VK_RBUTTON) & 0x8000)
		buttons |= MouseButtonKind_Right;

	if (::GetAsyncKeyState (VK_MBUTTON) & 0x8000)
		buttons |= MouseButtonKind_Medium;

	return buttons;
}

//.............................................................................

ref::Ptr <Canvas>
GdiWidgetImpl::getCanvas (HWND hWnd)
{
	HDC hdc = ::GetDC (hWnd);

	ref::Ptr <GdiCanvas> dc = AXL_REF_NEW (ref::Box <GdiCanvas>);	
	dc->attach (hdc, hWnd, GdiCanvas::DestructKind_ReleaseDc);
	return dc;
}

LRESULT 
GdiWidgetImpl::windowProc (
	HWND hWnd,
	UINT wmMsg, 
	WPARAM wParam, 
	LPARAM lParam, 
	bool* isHandled_o
	)
{
	ASSERT (m_engine->getEngineKind () == EngineKind_Gdi);

	if (checkMsgMask (WidgetMsgKind_Gdi))
	{
		WidgetGdiMsg msg (wmMsg, wParam, lParam);
		
		bool isHandled = true;
		processWidgetMsg (&msg, &isHandled);
		if (isHandled)
			return msg.m_lResult;
	}

	LRESULT lResult = 0;
	switch (wmMsg)
	{
	case WM_DESTROY:
		if (checkMsgMask (WidgetMsgKind_Close))
			processWidgetMsg (&WidgetMsg (WidgetMsgKind_Close), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_SETFOCUS:
		if (checkMsgMask (WidgetMsgKind_SetFocus))
			processWidgetMsg (&WidgetMsg (WidgetMsgKind_SetFocus), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_KILLFOCUS:
		if (checkMsgMask (WidgetMsgKind_KillFocus))
			processWidgetMsg (&WidgetMsg (WidgetMsgKind_KillFocus), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_SIZE:
		if (checkMsgMask (WidgetMsgKind_Size))
			processWmSize (hWnd, isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_HSCROLL:
		if (checkMsgMask (WidgetMsgKind_Scroll))
			processWmScroll (hWnd, WidgetOrientationKind_Horizontal, LOWORD (wParam), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_VSCROLL:
		if (checkMsgMask (WidgetMsgKind_Scroll))
			processWmScroll (hWnd, WidgetOrientationKind_Vertical, LOWORD (wParam), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_PAINT:
		if (checkMsgMask (WidgetMsgKind_Paint))
			processWmPaint (hWnd, isHandled_o);
		else
			*isHandled_o = false;

		break;

	// mouse messages

	case WM_SETCURSOR:
		lResult = processWmSetCursor (hWnd, isHandled_o);
		break;

	case WM_MOUSEMOVE:
		if (checkMsgMask (WidgetMsgKind_MouseMove))
			processWmMouse (
				WidgetMsgKind_MouseMove, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_None,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_MOUSELEAVE:
		if (checkMsgMask (WidgetMsgKind_MouseLeave))
			processWidgetMsg (&WidgetMsg (WidgetMsgKind_MouseLeave), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_CAPTURECHANGED:
		if (checkMsgMask (WidgetMsgKind_MouseCaptureLost))
			processWidgetMsg (&WidgetMsg (WidgetMsgKind_MouseCaptureLost), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONDOWN:
		if (checkMsgMask (WidgetMsgKind_MouseButtonDown))
			processWmMouse (
				WidgetMsgKind_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Left,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONUP:
		if (checkMsgMask (WidgetMsgKind_MouseButtonUp))
			processWmMouse (
				WidgetMsgKind_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Left,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONDBLCLK:
		if (checkMsgMask (WidgetMsgKind_MouseButtonDoubleClick))
			processWmMouse (
				WidgetMsgKind_MouseButtonDoubleClick, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Left,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;		

	case WM_RBUTTONDOWN:
		if (checkMsgMask (WidgetMsgKind_MouseButtonDown))
			processWmMouse (
				WidgetMsgKind_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Right,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONUP:
		if (checkMsgMask (WidgetMsgKind_MouseButtonUp))
			processWmMouse (
				WidgetMsgKind_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Right,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONDBLCLK:
		if (checkMsgMask (WidgetMsgKind_MouseButtonDoubleClick))
			processWmMouse (
				WidgetMsgKind_MouseButtonDoubleClick,
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Right,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;		

	case WM_MBUTTONDOWN:
		if (checkMsgMask (WidgetMsgKind_MouseButtonDown))
			processWmMouse (
				WidgetMsgKind_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Medium,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONUP:
		if (checkMsgMask (WidgetMsgKind_MouseButtonUp))
			processWmMouse (
				WidgetMsgKind_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Medium,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONDBLCLK:
		if (checkMsgMask (WidgetMsgKind_MouseButtonDoubleClick))
			processWmMouse (
				WidgetMsgKind_MouseButtonDoubleClick, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				MouseButtonKind_Medium,
				isHandled_o
				);
		else
			*isHandled_o = false;

		break;		

	case WM_MOUSEWHEEL:
		if (checkMsgMask (WidgetMsgKind_MouseWheel))
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
		if (checkMsgMask (WidgetMsgKind_KeyDown))
			processWmKey (WidgetMsgKind_KeyDown, (uint_t) wParam, isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_KEYUP:
		if (checkMsgMask (WidgetMsgKind_KeyUp))
			processWmKey (WidgetMsgKind_KeyUp, (uint_t) wParam, isHandled_o);
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
		StockCtrlPaint_Closehemes (&propertyCtrl->m_stockCtrlPaint);
		break;

	case WM_NCPAINT:
		StockCtrlPaint_NcPaintEdge (&propertyCtrl->m_stockCtrlPaint, hWnd, (HRGN) wParam);
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
GdiWidgetImpl::processWmMouse (
	WidgetMsgKind msgKind,
	int x,
	int y,
	MouseButtonKind button,
	bool* isHandled_o
	)
{
	WidgetMouseMsg msg;
	msg.m_msgKind = msgKind;
	msg.m_point.m_x = x;
	msg.m_point.m_y = y;
	msg.m_modifierKeys = getModifierKeys ();
	msg.m_button = button;
	msg.m_buttons = getMouseButtons ();

	processWidgetMsg (&msg, isHandled_o);
};

void
GdiWidgetImpl::processWmKey (
	WidgetMsgKind msgKind,
	int key,
	bool* isHandled_o
	)
{
	WidgetKeyMsg msg;
	msg.m_msgKind = msgKind;
	msg.m_key = key;
	msg.m_modifierKeys = getModifierKeys ();

	processWidgetMsg (&msg, isHandled_o);
}

void
GdiWidgetImpl::processWmMouseWheel (
	HWND hWnd,
	int wheelDelta,
	bool* isHandled_o
	)
{
	POINT point;
	::GetCursorPos (&point);
	::ScreenToClient (hWnd, &point);

	WidgetMouseWheelMsg msg;
	msg.m_msgKind = WidgetMsgKind_MouseWheel;
	msg.m_point.m_x = point.x;
	msg.m_point.m_y = point.y;
	msg.m_modifierKeys = getModifierKeys ();
	msg.m_button = MouseButtonKind_Medium;
	msg.m_buttons = getMouseButtons ();
	msg.m_wheelDelta = wheelDelta;

	processWidgetMsg (&msg, isHandled_o);
};

void
GdiWidgetImpl::processWmSize (
	HWND hWnd,
	bool* isHandled_o
	)
{
	RECT rect;
	::GetClientRect (hWnd, &rect);

	Size size (rect.right - rect.left, rect.bottom - rect.top);

	uint_t mask = 0;
	if (m_size.m_width != size.m_width)
		mask |= 1 << WidgetOrientationKind_Horizontal;

	if (m_size.m_height != size.m_height)
		mask |= 1 << WidgetOrientationKind_Vertical;

	m_size = size;

	processWidgetMsg (&WidgetMsgParam <uint_t> (WidgetMsgKind_Size, mask), isHandled_o);
}

void
GdiWidgetImpl::processWmScroll (
	HWND hWnd,
	WidgetOrientationKind orientation,
	int code,
	bool* isHandled_o
	)
{
	ASSERT ((size_t) orientation < 2);

	WidgetScrollBar* scrollBar = &m_scrollBarArray [orientation];

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

	processWidgetMsg (&WidgetMsgParam <uint_t> (WidgetMsgKind_Scroll, 1 << orientation), isHandled_o);
}

void
GdiWidgetImpl::processWmPaint (
	HWND hWnd,
	bool* isHandled_o
	)
{
	PAINTSTRUCT paintStruct;
	HDC hdc = ::BeginPaint (hWnd, &paintStruct);

	GdiCanvas dc;
	dc.attach (hdc, NULL, GdiCanvas::DestructKind_None);
	dc.m_baseFont = m_baseFont;
	dc.m_baseTextAttr = m_baseTextAttr;
	dc.m_palette = m_palette;

	Rect rect (
		paintStruct.rcPaint.left, 
		paintStruct.rcPaint.top, 
		paintStruct.rcPaint.right, 
		paintStruct.rcPaint.bottom
		);

	bool isHandled = true; // ignore it
	processWidgetMsg (&WidgetPaintMsg (&dc, rect), &isHandled);

	::EndPaint (hWnd, &paintStruct);
}

LRESULT
GdiWidgetImpl::processWmSetCursor (
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
		ASSERT (m_cursor->getEngine ()->getEngineKind () == EngineKind_Gdi);
		::SetCursor (*(GdiCursor*) m_cursor);
	}

	return TRUE;
}

//.............................................................................

} // namespace gui
} // namespace axl
