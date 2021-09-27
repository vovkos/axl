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

#include "pch.h"
#include "axl_gui_GdiWidget.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//..............................................................................

void
buildScrollInfo(
	SCROLLINFO* scrollInfo,
	size_t max,
	size_t page,
	size_t pos
) {
	memset(scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo->cbSize = sizeof(SCROLLINFO);
	scrollInfo->fMask = SIF_ALL;
	scrollInfo->nMin  = 0;
	scrollInfo->nMax  = max;
	scrollInfo->nPage = page;
	scrollInfo->nPos  = pos;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t
getModifierKeys() {
	uint_t modifierKeys = 0;

	if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
		modifierKeys |= ModifierKey_Shift;

	if (::GetAsyncKeyState(VK_CONTROL) & 0x8000)
		modifierKeys |= ModifierKey_Ctrl;

	if (::GetAsyncKeyState(VK_MENU) & 0x8000)
		modifierKeys |= ModifierKey_Alt;

	return modifierKeys;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t
getMouseButtons() {
	uint_t buttons = 0;

	if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		buttons |= MouseButton_Left;

	if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		buttons |= MouseButton_Right;

	if (::GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		buttons |= MouseButton_Medium;

	return buttons;
}

//..............................................................................

rc::Ptr<Canvas>
GdiWidgetImpl::getCanvas(HWND hWnd) {
	HDC hdc = ::GetDC(hWnd);

	rc::Ptr<GdiCanvas> dc = AXL_RC_NEW(rc::Box<GdiCanvas>);
	dc->attach(hdc, hWnd, GdiCanvas::DestructKind_ReleaseDc);
	return dc;
}

LRESULT
GdiWidgetImpl::windowProc(
	HWND hWnd,
	UINT wmMsg,
	WPARAM wParam,
	LPARAM lParam,
	bool* isHandled_o
) {
	ASSERT(m_engine->getEngineKind() == EngineKind_Gdi);

	if (checkMsgMap(WidgetMsgCode_Gdi)) {
		WidgetGdiMsg msg(wmMsg, wParam, lParam);

		bool isHandled = true;
		processWidgetMsg(&msg, &isHandled);
		if (isHandled)
			return msg.m_lResult;
	}

	LRESULT lResult = 0;
	switch (wmMsg) {
	case WM_DESTROY:
		if (checkMsgMap(WidgetMsgCode_Close))
			processWidgetMsg(&WidgetMsg(WidgetMsgCode_Close), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_SETFOCUS:
		if (checkMsgMap(WidgetMsgCode_SetFocus))
			processWidgetMsg(&WidgetMsg(WidgetMsgCode_SetFocus), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_KILLFOCUS:
		if (checkMsgMap(WidgetMsgCode_KillFocus))
			processWidgetMsg(&WidgetMsg(WidgetMsgCode_KillFocus), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_SIZE:
		if (checkMsgMap(WidgetMsgCode_Size))
			processWmSize(hWnd, isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_HSCROLL:
		if (checkMsgMap(WidgetMsgCode_Scroll))
			processWmScroll(hWnd, Orientation_Horizontal, LOWORD(wParam), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_VSCROLL:
		if (checkMsgMap(WidgetMsgCode_Scroll))
			processWmScroll(hWnd, Orientation_Vertical, LOWORD(wParam), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_PAINT:
		if (checkMsgMap(WidgetMsgCode_Paint))
			processWmPaint(hWnd, isHandled_o);
		else
			*isHandled_o = false;

		break;

	// mouse messages

	case WM_SETCURSOR:
		lResult = processWmSetCursor(hWnd, isHandled_o);
		break;

	case WM_MOUSEMOVE:
		if (checkMsgMap(WidgetMsgCode_MouseMove))
			processWmMouse(
				WidgetMsgCode_MouseMove,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_None,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_MOUSELEAVE:
		if (checkMsgMap(WidgetMsgCode_MouseLeave))
			processWidgetMsg(&WidgetMsg(WidgetMsgCode_MouseLeave), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_CAPTURECHANGED:
		if (checkMsgMap(WidgetMsgCode_MouseCaptureLost))
			processWidgetMsg(&WidgetMsg(WidgetMsgCode_MouseCaptureLost), isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONDOWN:
		if (checkMsgMap(WidgetMsgCode_MouseButtonDown))
			processWmMouse(
				WidgetMsgCode_MouseButtonDown,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Left,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONUP:
		if (checkMsgMap(WidgetMsgCode_MouseButtonUp))
			processWmMouse(
				WidgetMsgCode_MouseButtonUp,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Left,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_LBUTTONDBLCLK:
		if (checkMsgMap(WidgetMsgCode_MouseButtonDoubleClick))
			processWmMouse(
				WidgetMsgCode_MouseButtonDoubleClick,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Left,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONDOWN:
		if (checkMsgMap(WidgetMsgCode_MouseButtonDown))
			processWmMouse(
				WidgetMsgCode_MouseButtonDown,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Right,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONUP:
		if (checkMsgMap(WidgetMsgCode_MouseButtonUp))
			processWmMouse(
				WidgetMsgCode_MouseButtonUp,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Right,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_RBUTTONDBLCLK:
		if (checkMsgMap(WidgetMsgCode_MouseButtonDoubleClick))
			processWmMouse(
				WidgetMsgCode_MouseButtonDoubleClick,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Right,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONDOWN:
		if (checkMsgMap(WidgetMsgCode_MouseButtonDown))
			processWmMouse(
				WidgetMsgCode_MouseButtonDown,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Medium,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONUP:
		if (checkMsgMap(WidgetMsgCode_MouseButtonUp))
			processWmMouse(
				WidgetMsgCode_MouseButtonUp,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Medium,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_MBUTTONDBLCLK:
		if (checkMsgMap(WidgetMsgCode_MouseButtonDoubleClick))
			processWmMouse(
				WidgetMsgCode_MouseButtonDoubleClick,
				(short)LOWORD(lParam),
				(short)HIWORD(lParam),
				MouseButton_Medium,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	case WM_MOUSEWHEEL:
		if (checkMsgMap(WidgetMsgCode_MouseWheel))
			processWmMouseWheel(
				hWnd,
				(short)HIWORD(wParam) / WHEEL_DELTA,
				isHandled_o
			);
		else
			*isHandled_o = false;

		break;

	// keyboard

	case WM_KEYDOWN:
		if (checkMsgMap(WidgetMsgCode_KeyDown))
			processWmKey(WidgetMsgCode_KeyDown, (uint_t)wParam, isHandled_o);
		else
			*isHandled_o = false;

		break;

	case WM_KEYUP:
		if (checkMsgMap(WidgetMsgCode_KeyUp))
			processWmKey(WidgetMsgCode_KeyUp, (uint_t)wParam, isHandled_o);
		else
			*isHandled_o = false;

		break;

/*	case WM_MOUSEACTIVATE:
		lResult = axl_win_TPropertyCtrl_IsMenu(propertyCtrl) ? MA_NOACTIVATE : defWindowProc(hWnd, msg, wParam, lParam);
		break;

	case WM_GETDLGCODE:
		lResult = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_THEMECHANGED:
		StockCtrlPaint_Closehemes(&propertyCtrl->m_stockCtrlPaint);
		break;

	case WM_NCPAINT:
		StockCtrlPaint_NcPaintEdge(&propertyCtrl->m_stockCtrlPaint, hWnd, (HRGN)wParam);
		break;

	case WM_TIMER:
		lResult = axl_win_TPropertyCtrl_OnTimer(propertyCtrl, (int)wParam);
		break;
*/

	default:
		*isHandled_o = false;
	}

	return lResult;
}

void
GdiWidgetImpl::processWmMouse(
	WidgetMsgCode msgCode,
	int x,
	int y,
	MouseButton button,
	bool* isHandled_o
) {
	WidgetMouseMsg msg;
	msg.m_msgCode = msgCode;
	msg.m_point.m_x = x;
	msg.m_point.m_y = y;
	msg.m_modifierKeys = getModifierKeys();
	msg.m_button = button;
	msg.m_buttons = getMouseButtons();

	processWidgetMsg(&msg, isHandled_o);
};

void
GdiWidgetImpl::processWmKey(
	WidgetMsgCode msgCode,
	int key,
	bool* isHandled_o
) {
	WidgetKeyMsg msg;
	msg.m_msgCode = msgCode;
	msg.m_key = key;
	msg.m_modifierKeys = getModifierKeys();

	processWidgetMsg(&msg, isHandled_o);
}

void
GdiWidgetImpl::processWmMouseWheel(
	HWND hWnd,
	int wheelDelta,
	bool* isHandled_o
) {
	POINT point;
	::GetCursorPos(&point);
	::ScreenToClient(hWnd, &point);

	WidgetMouseWheelMsg msg;
	msg.m_msgCode = WidgetMsgCode_MouseWheel;
	msg.m_point.m_x = point.x;
	msg.m_point.m_y = point.y;
	msg.m_modifierKeys = getModifierKeys();
	msg.m_button = MouseButton_Medium;
	msg.m_buttons = getMouseButtons();
	msg.m_wheelDelta = wheelDelta;

	processWidgetMsg(&msg, isHandled_o);
};

void
GdiWidgetImpl::processWmSize(
	HWND hWnd,
	bool* isHandled_o
) {
	RECT rect;
	::GetClientRect(hWnd, &rect);

	Size size(rect.right - rect.left, rect.bottom - rect.top);

	uint_t mask = 0;
	if (m_size.m_width != size.m_width)
		mask |= 1 << Orientation_Horizontal;

	if (m_size.m_height != size.m_height)
		mask |= 1 << Orientation_Vertical;

	m_size = size;

	processWidgetMsg(&WidgetMsgParam<uint_t> (WidgetMsgCode_Size, mask), isHandled_o);
}

void
GdiWidgetImpl::processWmScroll(
	HWND hWnd,
	Orientation orientation,
	int code,
	bool* isHandled_o
) {
	ASSERT((size_t)orientation < 2);

	WidgetScrollBar* scrollBar = &m_scrollBarArray[orientation];

	int bar = getScrollBarFromOrientation(orientation);

	SCROLLINFO scrollInfo = { 0 };
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.fMask = SIF_POS | SIF_TRACKPOS;

	::GetScrollInfo(hWnd, bar, &scrollInfo);

	intptr_t newPos = scrollInfo.nPos;
	intptr_t maxPos = scrollBar->getMaxPos();

	switch (code) {
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

	if (newPos != scrollInfo.nPos) {
		scrollInfo.fMask = SIF_POS;
		scrollInfo.nPos = (int)newPos;
		::SetScrollInfo(hWnd, bar, &scrollInfo, TRUE);
	}

	scrollBar->m_pos = newPos;

	processWidgetMsg(&WidgetMsgParam<uint_t> (WidgetMsgCode_Scroll, 1 << orientation), isHandled_o);
}

void
GdiWidgetImpl::processWmPaint(
	HWND hWnd,
	bool* isHandled_o
) {
	PAINTSTRUCT paintStruct;
	HDC hdc = ::BeginPaint(hWnd, &paintStruct);

	GdiCanvas dc;
	dc.attach(hdc, NULL, GdiCanvas::DestructKind_None);
	dc.m_baseFont = m_baseFont;
	dc.m_baseTextAttr = m_baseTextAttr;
	dc.m_palette = m_palette;

	Rect rect(
		paintStruct.rcPaint.left,
		paintStruct.rcPaint.top,
		paintStruct.rcPaint.right,
		paintStruct.rcPaint.bottom
	);

	bool isHandled = true; // ignore it
	processWidgetMsg(&WidgetPaintMsg(&dc, rect), &isHandled);

	::EndPaint(hWnd, &paintStruct);
}

LRESULT
GdiWidgetImpl::processWmSetCursor(
	HWND hWnd,
	bool* isHandled_o
) {
	POINT mousePos;
	RECT clientRect;

	::GetCursorPos(&mousePos);
	::ScreenToClient(hWnd, &mousePos);
	::GetClientRect(hWnd, &clientRect);

	bool_t isOnClientRect = ::PtInRect(&clientRect, mousePos);
	if (!isOnClientRect) {
		*isHandled_o = false;
		return 0;
	}

	if (!m_cursor) {
		::SetCursor(NULL);
	} else {
		ASSERT(m_cursor->getEngine()->getEngineKind() == EngineKind_Gdi);
		::SetCursor(*(GdiCursor*)m_cursor);
	}

	return TRUE;
}

//..............................................................................

} // namespace gui
} // namespace axl
