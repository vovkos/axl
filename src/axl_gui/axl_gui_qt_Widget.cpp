#include "pch.h"
#include "axl_gui_qt_Widget.h"
#include "axl_gui_qt_Engine.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

void 
IQtWidget::OnEvent (
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
IQtWidget::OnMouseEvent (
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
IQtWidget::OnMouseWheelEvent (QWheelEvent* pEvent)
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
IQtWidget::OnKeyEvent (
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
IQtWidget::OnPaintEvent (
	QPaintEvent* pEvent,
	QPainter* pQtPainter
	)		
{
	if (!CheckMsgMask (EWidgetMsg_Paint))
	{
		pEvent->ignore ();
		return;
	}
	
	CPainter Painter;
	Painter.Attach (pQtPainter);
	Painter.m_pBaseFont = m_pBaseFont;
	Painter.m_BaseTextAttr = m_BaseTextAttr;
	Painter.m_Palette = m_Palette;

	QRect QtRect = pEvent->rect ();
	TRect Rect (
		QtRect.x (), 
		QtRect.y (), 
		QtRect.x () + QtRect.width (), 
		QtRect.y () + QtRect.height ()
		);

	TWidgetPaintMsg Msg (&Painter, Rect);

	bool IsHandled = true;
	ProcessWidgetMsg (&Msg, &IsHandled);	

	Painter.Detach ();

	if (!IsHandled)
		pEvent->ignore ();				
}

void
IQtWidget::OnResizeEvent (QResizeEvent* pEvent)
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
IQtWidget::OnScroll (
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

} // namespace qt
} // namespace gui
} // namespace axl
