#include "pch.h"
#include "axl_gui_QtWidget.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

uint_t
CQtWidgetImpl::GetKeyFromQtKey (int QtKey)
{
	if (!(QtKey & 0x01000000))
		return QtKey;

	size_t Index = QtKey & ~0x01000000;

	static uint_t KeyTable [] = 
	{
		EKey_Esc,         // 0x00
		EKey_Tab,         // 0x01
		0,                // 0x02
		EKey_Backspace,   // 0x03
		EKey_Enter,       // 0x04
		EKey_Enter,       // 0x05
		EKey_Insert,      // 0x06
		EKey_Delete,      // 0x07
		EKey_Pause,       // 0x08
		EKey_Print,       // 0x09
		0,                // 0x0a
		0,                // 0x0b
		0,                // 0x0c
		0,                // 0x0d
		0,                // 0x0e
		0,                // 0x0f
		EKey_Home,        // 0x10
		EKey_End,         // 0x11
		EKey_Left,        // 0x12
		EKey_Up,          // 0x13
		EKey_Right,       // 0x14
		EKey_Down,        // 0x15
		EKey_PageUp,      // 0x16
		EKey_PageDown,    // 0x17
		0,                // 0x18
		0,                // 0x19
		0,                // 0x1a
		0,                // 0x1b
		0,                // 0x1c
		0,                // 0x1d
		0,                // 0x1e
		0,                // 0x1f
		EKey_Shift,       // 0x20
		EKey_Ctrl,        // 0x21
		0,                // 0x22
		EKey_Alt,         // 0x23
		EKey_CapsLock,    // 0x24
		EKey_NumLock,     // 0x25
		EKey_ScrollLock,  // 0x26
		0,                // 0x27
		0,                // 0x28
		0,                // 0x29
		0,                // 0x2a
		0,                // 0x2b
		0,                // 0x2c
		0,                // 0x2e
		0,                // 0x2f
		EKey_F1,          // 0x30
		EKey_F2,          // 0x31
		EKey_F3,          // 0x32
		EKey_F4,          // 0x33
		EKey_F5,          // 0x34
		EKey_F6,          // 0x35
		EKey_F7,          // 0x36
		EKey_F8,          // 0x37
		EKey_F9,          // 0x38
		EKey_F10,         // 0x39
		EKey_F11,         // 0x3a
		EKey_F12,         // 0x3b
	};

	return Index < countof (KeyTable) ? KeyTable [Index] : 0;
}

void 
CQtWidgetImpl::OnEvent (
	QEvent* pEvent,
	EWidgetMsg MsgKind
	)
{
	if (!CheckMsgMask (MsgKind))
	{
		pEvent->ignore ();	
		return;
	}

	TWidgetMsg Msg (MsgKind); // thanks a lot gcc

	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);			

	if (!IsHandled)
		pEvent->ignore ();	
}

void 
CQtWidgetImpl::OnMouseEvent (
	QMouseEvent* pEvent,
	EWidgetMsg MsgKind
	)
{	
	if (!CheckMsgMask (MsgKind))
	{
		pEvent->ignore ();	
		return;
	}

	TWidgetMouseMsg Msg;
	Msg.m_MsgKind = MsgKind;
	Msg.m_Point.Setup (pEvent->x (), pEvent->y ());
	Msg.m_Buttons = GetMouseButtonsFromQtButtons (pEvent->buttons ());
	Msg.m_ModifierKeys = GetModifierKeysFromQtModifiers (pEvent->modifiers ());
	Msg.m_Button = GetMouseButtonFromQtButton (pEvent->button ());

	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);			

	if (!IsHandled)
		pEvent->ignore ();
}

void 
CQtWidgetImpl::OnMouseWheelEvent (QWheelEvent* pEvent)
{	
	if (!CheckMsgMask (EWidgetMsg_MouseWheel))
	{
		pEvent->ignore ();	
		return;
	}

	TWidgetMouseWheelMsg Msg;
	Msg.m_Point.Setup (pEvent->x (), pEvent->y ());
	Msg.m_Buttons = GetMouseButtonsFromQtButtons (pEvent->buttons ());
	Msg.m_ModifierKeys = GetModifierKeysFromQtModifiers (pEvent->modifiers ());
	Msg.m_WheelDelta = pEvent->delta () / 120;

	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);			

	if (!IsHandled)
		pEvent->ignore ();
}

void 
CQtWidgetImpl::OnKeyEvent (
	QKeyEvent* pEvent,
	EWidgetMsg MsgKind
	)
{	
	if (!CheckMsgMask (MsgKind))
	{
		pEvent->ignore ();	
		return;
	}

	TWidgetKeyMsg Msg;
	Msg.m_MsgKind = MsgKind;
	Msg.m_Key = GetKeyFromQtKey (pEvent->key ());
	Msg.m_ModifierKeys = GetModifierKeysFromQtModifiers (pEvent->modifiers ());

	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);			

	if (!IsHandled)
		pEvent->ignore ();
}

void
CQtWidgetImpl::OnPaintEvent (
	QPaintEvent* pEvent,
	QPainter* pQtPainter
	)		
{
	if (!CheckMsgMask (EWidgetMsg_Paint))
	{
		pEvent->ignore ();
		return;
	}
	
	CQtCanvas Canvas;
	Canvas.Attach (pQtPainter);
	Canvas.m_pBaseFont = m_pBaseFont;
	Canvas.m_BaseTextAttr = m_BaseTextAttr;
	Canvas.m_Palette = m_Palette;

	QRect QtRect = pEvent->rect ();
	TRect Rect (
		QtRect.x (), 
		QtRect.y (), 
		QtRect.x () + QtRect.width (), 
		QtRect.y () + QtRect.height ()
		);

	TWidgetPaintMsg Msg (&Canvas, Rect);

	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);	

	Canvas.Detach ();

	if (m_IsCaretVisible && ((CQtEngine*) m_pEngine)->IsCaretVisible ())
	{
		pQtPainter->setCompositionMode (QPainter::RasterOp_SourceXorDestination);
		pQtPainter->fillRect (
			m_CaretPos.m_x, 
			m_CaretPos.m_y, 
			m_CaretSize.m_Width, 
			m_CaretSize.m_Height,
			Qt::white
			);
	}
}

void
CQtWidgetImpl::OnResizeEvent (QResizeEvent* pEvent)
{
	if (!CheckMsgMask (EWidgetMsg_Size))
	{
		pEvent->ignore ();
		return;
	}

	QSize QtSize = pEvent->size ();
	TSize Size (QtSize.width (), QtSize.height ());

	uint_t Mask = 0;
	if (m_Size.m_Width != Size.m_Width)
		Mask |= 1 << EWidgetOrientation_Horizontal;

	if (m_Size.m_Height != Size.m_Height)
		Mask |= 1 << EWidgetOrientation_Vertical;

	m_Size = Size;

	TWidgetMsgT <uint_t> Msg (EWidgetMsg_Size, Mask);
	
	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);

	if (!IsHandled)
		pEvent->ignore ();			
}		

void
CQtWidgetImpl::OnScroll (
	QScrollBar* pVerticalScrollBar,
	QScrollBar* pHorizontalScrollBar
	)
{
	uint_t Mask = 0;
	
	if (pVerticalScrollBar)
	{
		m_ScrollBarArray [EWidgetOrientation_Vertical].m_Pos = pVerticalScrollBar->value ();
		Mask |= 1 << EWidgetOrientation_Vertical;
	}

	if (pHorizontalScrollBar)
	{
		m_ScrollBarArray [EWidgetOrientation_Horizontal].m_Pos = pHorizontalScrollBar->value ();
		Mask |= 1 << EWidgetOrientation_Horizontal;
	}

	if (!CheckMsgMask (EWidgetMsg_Size))
		return;

	TWidgetMsgT <uint_t> Msg (EWidgetMsg_Scroll, Mask);
	
	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);	
}

//.............................................................................

void 
QtWidgetBase::mouseMoveEvent (QMouseEvent* e)
{
	if (!m_mouseMoveEventFlag)
	{	
		CCursor* cursor = m_qtWidget->GetCursor ();
		if (cursor)
		{
			setCursor (((CQtCursor*) cursor)->m_QtCursor);

			QCursor arrowCurosr (Qt::ArrowCursor);
			horizontalScrollBar ()->setCursor (arrowCurosr);
			verticalScrollBar ()->setCursor (arrowCurosr);
		}

		m_mouseMoveEventFlag = true;
	}

	m_qtWidget->OnMouseEvent (e, EWidgetMsg_MouseMove);
}

//.............................................................................

} // namespace gui
} // namespace axl
