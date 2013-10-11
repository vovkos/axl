#include "pch.h"
#include "axl_gui_gdi_Widget.h"
#include "axl_gui_gdi_Engine.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

void
BuildScrollInfo (
	SCROLLINFO* pScrollInfo,
	size_t Max,
	size_t Page,
	size_t Pos
	)
{
	memset (pScrollInfo, 0, sizeof (SCROLLINFO));
	pScrollInfo->cbSize = sizeof (SCROLLINFO);
	pScrollInfo->fMask = SIF_ALL;
	pScrollInfo->nMin  = 0;
	pScrollInfo->nMax  = Max;
	pScrollInfo->nPage = Page;
	pScrollInfo->nPos  = Pos;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t 
GetModifierKeys ()
{
	uint_t ModifierKeys = 0;

	if (GetAsyncKeyState (VK_SHIFT) & 0x8000)
		ModifierKeys |= EModifierKey_Shift;

	if (GetAsyncKeyState (VK_CONTROL) & 0x8000)
		ModifierKeys |= EModifierKey_Ctrl;

	if (GetAsyncKeyState (VK_MENU) & 0x8000)
		ModifierKeys |= EModifierKey_Alt;

	return ModifierKeys;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t 
GetMouseButtons ()
{
	uint_t Buttons = 0;

	if (GetAsyncKeyState (VK_LBUTTON) & 0x8000)
		Buttons |= EMouseButton_Left;

	if (GetAsyncKeyState (VK_RBUTTON) & 0x8000)
		Buttons |= EMouseButton_Right;

	if (GetAsyncKeyState (VK_MBUTTON) & 0x8000)
		Buttons |= EMouseButton_Medium;

	return Buttons;
}

//.............................................................................

ref::CPtrT <ICanvas>
IGdiWidget::GetCanvas (HWND hWnd)
{
	HDC hdc = ::GetDC (hWnd);

	ref::CPtrT <CDc> Dc = AXL_REF_NEW (ref::CBoxT <CDc>);	
	Dc->Attach (hdc, hWnd, CDc::EDestruct_ReleaseDc);
	return Dc;
}

LRESULT 
IGdiWidget::WindowProc (
	HWND hWnd,
	UINT WmMsg, 
	WPARAM wParam, 
	LPARAM lParam, 
	bool* pIsHandled
	)
{
	ASSERT (m_pEngine->GetEngineKind () == EEngine_Gdi);

	if (CheckMsgMask (EWidgetMsg_Gdi))
	{
		TWidgetGdiMsg Msg (WmMsg, wParam, lParam);
		
		bool IsHandled = true;
		ProcessWidgetMsg (&Msg, &IsHandled);
		if (IsHandled)
			return Msg.m_lResult;
	}

	LRESULT lResult = 0;
	switch (WmMsg)
	{
	case WM_DESTROY:
		if (CheckMsgMask (EWidgetMsg_Close))
			ProcessWidgetMsg (&TWidgetMsg (EWidgetMsg_Close), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_SETFOCUS:
		if (CheckMsgMask (EWidgetMsg_SetFocus))
			ProcessWidgetMsg (&TWidgetMsg (EWidgetMsg_SetFocus), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_KILLFOCUS:
		if (CheckMsgMask (EWidgetMsg_KillFocus))
			ProcessWidgetMsg (&TWidgetMsg (EWidgetMsg_KillFocus), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_SIZE:
		if (CheckMsgMask (EWidgetMsg_Size))
			ProcessWmSize (hWnd, pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_HSCROLL:
		if (CheckMsgMask (EWidgetMsg_Scroll))
			ProcessWmScroll (hWnd, EWidgetOrientation_Horizontal, LOWORD (wParam), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_VSCROLL:
		if (CheckMsgMask (EWidgetMsg_Scroll))
			ProcessWmScroll (hWnd, EWidgetOrientation_Vertical, LOWORD (wParam), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_PAINT:
		if (CheckMsgMask (EWidgetMsg_Paint))
			ProcessWmPaint (hWnd, pIsHandled);
		else
			*pIsHandled = false;

		break;

	// mouse messages

	case WM_SETCURSOR:
		lResult = ProcessWmSetCursor (hWnd, pIsHandled);
		break;

	case WM_MOUSEMOVE:
		if (CheckMsgMask (EWidgetMsg_MouseMove))
			ProcessWmMouse (
				EWidgetMsg_MouseMove, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_None,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_MOUSELEAVE:
		if (CheckMsgMask (EWidgetMsg_MouseLeave))
			ProcessWidgetMsg (&TWidgetMsg (EWidgetMsg_MouseLeave), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_CAPTURECHANGED:
		if (CheckMsgMask (EWidgetMsg_MouseCaptureLost))
			ProcessWidgetMsg (&TWidgetMsg (EWidgetMsg_MouseCaptureLost), pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_LBUTTONDOWN:
		if (CheckMsgMask (EWidgetMsg_MouseButtonDown))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Left,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_LBUTTONUP:
		if (CheckMsgMask (EWidgetMsg_MouseButtonUp))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Left,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_LBUTTONDBLCLK:
		if (CheckMsgMask (EWidgetMsg_MouseButtonDoubleClick))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonDoubleClick, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Left,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;		

	case WM_RBUTTONDOWN:
		if (CheckMsgMask (EWidgetMsg_MouseButtonDown))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Right,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_RBUTTONUP:
		if (CheckMsgMask (EWidgetMsg_MouseButtonUp))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Right,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_RBUTTONDBLCLK:
		if (CheckMsgMask (EWidgetMsg_MouseButtonDoubleClick))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonDoubleClick,
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Right,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;		

	case WM_MBUTTONDOWN:
		if (CheckMsgMask (EWidgetMsg_MouseButtonDown))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonDown, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Medium,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_MBUTTONUP:
		if (CheckMsgMask (EWidgetMsg_MouseButtonUp))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonUp, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Medium,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	case WM_MBUTTONDBLCLK:
		if (CheckMsgMask (EWidgetMsg_MouseButtonDoubleClick))
			ProcessWmMouse (
				EWidgetMsg_MouseButtonDoubleClick, 
				(short) LOWORD(lParam), 
				(short) HIWORD(lParam), 
				EMouseButton_Medium,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;		

	case WM_MOUSEWHEEL:
		if (CheckMsgMask (EWidgetMsg_MouseWheel))
			ProcessWmMouseWheel (
				hWnd,
				(short) HIWORD(wParam) / WHEEL_DELTA,
				pIsHandled
				);
		else
			*pIsHandled = false;

		break;

	// keyboard
		
	case WM_KEYDOWN:
		if (CheckMsgMask (EWidgetMsg_KeyDown))
			ProcessWmKey (EWidgetMsg_KeyDown, (uint_t) wParam, pIsHandled);
		else
			*pIsHandled = false;

		break;

	case WM_KEYUP:
		if (CheckMsgMask (EWidgetMsg_KeyUp))
			ProcessWmKey (EWidgetMsg_KeyUp, (uint_t) wParam, pIsHandled);
		else
			*pIsHandled = false;

		break;

/*	case WM_MOUSEACTIVATE:
		lResult = axl_win_TPropertyCtrl_IsMenu (pPropertyCtrl) ? MA_NOACTIVATE : DefWindowProc (hWnd, Msg, wParam, lParam);
		break;

	case WM_GETDLGCODE:
		lResult = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_THEMECHANGED:
		TStockCtrlPaint_CloseThemes (&pPropertyCtrl->m_StockCtrlPaint);
		break;

	case WM_NCPAINT:
		TStockCtrlPaint_NcPaintEdge (&pPropertyCtrl->m_StockCtrlPaint, hWnd, (HRGN) wParam);
		break;

	case WM_TIMER:
		lResult = axl_win_TPropertyCtrl_OnTimer (pPropertyCtrl, (int) wParam);
		break;
*/

	default:
		*pIsHandled = false;
	}

	return lResult;
}

void
IGdiWidget::ProcessWmMouse (
	EWidgetMsg MsgKind,
	int x,
	int y,
	EMouseButton Button,
	bool* pIsHandled
	)
{
	TWidgetMouseMsg Msg;
	Msg.m_MsgKind = MsgKind;
	Msg.m_Point.m_x = x;
	Msg.m_Point.m_y = y;
	Msg.m_ModifierKeys = GetModifierKeys ();
	Msg.m_Button = Button;
	Msg.m_Buttons = GetMouseButtons ();

	ProcessWidgetMsg (&Msg, pIsHandled);
};

void
IGdiWidget::ProcessWmKey (
	EWidgetMsg MsgKind,
	int Key,
	bool* pIsHandled
	)
{
	TWidgetKeyMsg Msg;
	Msg.m_MsgKind = MsgKind;
	Msg.m_Key = Key;
	Msg.m_ModifierKeys = GetModifierKeys ();

	ProcessWidgetMsg (&Msg, pIsHandled);
}

void
IGdiWidget::ProcessWmMouseWheel (
	HWND hWnd,
	int WheelDelta,
	bool* pIsHandled
	)
{
	POINT Point;
	::GetCursorPos (&Point);
	::ScreenToClient (hWnd, &Point);

	TWidgetMouseWheelMsg Msg;
	Msg.m_MsgKind = EWidgetMsg_MouseWheel;
	Msg.m_Point.m_x = Point.x;
	Msg.m_Point.m_y = Point.y;
	Msg.m_ModifierKeys = GetModifierKeys ();
	Msg.m_Button = EMouseButton_Medium;
	Msg.m_Buttons = GetMouseButtons ();
	Msg.m_WheelDelta = WheelDelta;

	ProcessWidgetMsg (&Msg, pIsHandled);
};

void
IGdiWidget::ProcessWmSize (
	HWND hWnd,
	bool* pIsHandled
	)
{
	RECT Rect;
	GetClientRect (hWnd, &Rect);

	TSize Size (Rect.right - Rect.left, Rect.bottom - Rect.top);

	uint_t Mask = 0;
	if (m_Size.m_Width != Size.m_Width)
		Mask |= 1 << EWidgetOrientation_Horizontal;

	if (m_Size.m_Height != Size.m_Height)
		Mask |= 1 << EWidgetOrientation_Vertical;

	m_Size = Size;

	ProcessWidgetMsg (&TWidgetMsgT <uint_t> (EWidgetMsg_Size, Mask), pIsHandled);
}

void
IGdiWidget::ProcessWmScroll (
	HWND hWnd,
	EWidgetOrientation Orientation,
	int Code,
	bool* pIsHandled
	)
{
	ASSERT ((size_t) Orientation < 2);

	TWidgetScrollBar* pScrollBar = &m_ScrollBarArray [Orientation];

	int Bar = GetScrollBarFromOrientation (Orientation);

	SCROLLINFO ScrollInfo = { 0 };
	ScrollInfo.cbSize = sizeof (ScrollInfo);
	ScrollInfo.fMask = SIF_POS | SIF_TRACKPOS;
	
	GetScrollInfo (hWnd, Bar, &ScrollInfo);

	intptr_t NewPos = ScrollInfo.nPos;
	intptr_t MaxPos = pScrollBar->GetMaxPos ();
	
	switch (Code)
	{
	case SB_TOP:
		NewPos = 0;
		break;

	case SB_LINEUP:
		NewPos--;
		break;

	case SB_LINEDOWN:
		NewPos++;
		break;

	case SB_PAGEUP:
		NewPos -= pScrollBar->m_Page; 
		break;

	case SB_PAGEDOWN:
		NewPos += pScrollBar->m_Page; 
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		NewPos = ScrollInfo.nTrackPos; 
		break;
	}

	if (NewPos < 0)
		NewPos = 0;
	
	if (NewPos > MaxPos)
		NewPos = MaxPos;

	if (NewPos != ScrollInfo.nPos)
	{
		ScrollInfo.fMask = SIF_POS;
		ScrollInfo.nPos = (int) NewPos;
		SetScrollInfo (hWnd, Bar, &ScrollInfo, TRUE);
	}

	pScrollBar->m_Pos = NewPos;

	ProcessWidgetMsg (&TWidgetMsgT <uint_t> (EWidgetMsg_Scroll, 1 << Orientation), pIsHandled);
}

void
IGdiWidget::ProcessWmPaint (
	HWND hWnd,
	bool* pIsHandled
	)
{
	PAINTSTRUCT PaintStruct;
	HDC hdc = BeginPaint (hWnd, &PaintStruct);

	CDc Dc;
	Dc.Attach (hdc, NULL, CDc::EDestruct_None);
	Dc.m_pBaseFont = m_pBaseFont;
	Dc.m_BaseTextAttr = m_BaseTextAttr;
	Dc.m_Palette = m_Palette;

	TRect Rect (
		PaintStruct.rcPaint.left, 
		PaintStruct.rcPaint.top, 
		PaintStruct.rcPaint.right, 
		PaintStruct.rcPaint.bottom
		);

	bool IsHandled = true; // ignore it
	ProcessWidgetMsg (&TWidgetPaintMsg (&Dc, Rect), &IsHandled);

	EndPaint (hWnd, &PaintStruct);
}

LRESULT
IGdiWidget::ProcessWmSetCursor (
	HWND hWnd,
	bool* pIsHandled
	)
{
	POINT MousePos;
	RECT ClientRect;
	
	::GetCursorPos (&MousePos);
	::ScreenToClient (hWnd, &MousePos);
	::GetClientRect (hWnd, &ClientRect);
	
	bool_t IsOnClientRect = ::PtInRect (&ClientRect, MousePos);	
	if (!IsOnClientRect)
	{
		*pIsHandled = false; 
		return 0;
	}

	if (!m_pCursor)
	{
		::SetCursor (NULL);
	}
	else
	{
		ASSERT (m_pCursor->GetEngine ()->GetEngineKind () == EEngine_Gdi);
		::SetCursor (*(CCursor*) m_pCursor);
	}

	return TRUE;
}

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl
