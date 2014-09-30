#include "pch.h"
#include "axl_gui_QtWidget.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

uint_t
getKeyFromQtKey (int qtKey)
{
	ASSERT (qtKey & 0x01000000);

	size_t index = qtKey & ~0x01000000;

	static uint_t keyTable [] = 
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

	return index < countof (keyTable) ? keyTable [index] : 0;
}

void 
CQtWidgetImpl::onEvent (
	QEvent* event,
	EWidgetMsg msgKind
	)
{
	if (!checkMsgMask (msgKind))
	{
		event->ignore ();	
		return;
	}

	TWidgetMsg msg (msgKind); // thanks a lot gcc

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();	
}

void 
CQtWidgetImpl::onMouseEvent (
	QMouseEvent* event,
	EWidgetMsg msgKind
	)
{	
	if (!checkMsgMask (msgKind))
	{
		event->ignore ();	
		return;
	}

	TWidgetMouseMsg msg;
	msg.m_msgKind = msgKind;
	msg.m_point.setup (event->x (), event->y ());
	msg.m_buttons = getMouseButtonsFromQtButtons (event->buttons ());
	msg.m_modifierKeys = getModifierKeysFromQtModifiers (event->modifiers ());
	msg.m_button = getMouseButtonFromQtButton (event->button ());

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();
}

void 
CQtWidgetImpl::onMouseWheelEvent (QWheelEvent* event)
{	
	if (!checkMsgMask (EWidgetMsg_MouseWheel))
	{
		event->ignore ();	
		return;
	}

	TWidgetMouseWheelMsg msg;
	msg.m_point.setup (event->x (), event->y ());
	msg.m_buttons = getMouseButtonsFromQtButtons (event->buttons ());
	msg.m_modifierKeys = getModifierKeysFromQtModifiers (event->modifiers ());
	msg.m_wheelDelta = event->delta () / 120;

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();
}

void 
CQtWidgetImpl::onKeyEvent (
	QKeyEvent* event,
	EWidgetMsg msgKind
	)
{	
	if (!checkMsgMask (msgKind))
	{
		event->ignore ();	
		return;
	}

	TWidgetKeyMsg msg;
	msg.m_msgKind = msgKind;

	int qtKey = event->key ();
	if (qtKey & 0x01000000)
	{
		msg.m_key = getKeyFromQtKey (qtKey);
		if (qtKey <= Qt::Key_Enter)
			msg.m_char = msg.m_key;
	}
	else 
	{
		msg.m_key = qtKey;
		msg.m_char = event->text ().at (0).unicode ();
	}

	msg.m_modifierKeys = getModifierKeysFromQtModifiers (event->modifiers ());
	
	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();
}

void
CQtWidgetImpl::onPaintEvent (
	QPaintEvent* event,
	QPainter* qtPainter
	)		
{
	if (!checkMsgMask (EWidgetMsg_Paint))
	{
		event->ignore ();
		return;
	}
	
	CQtCanvas canvas;
	canvas.attach (qtPainter);
	canvas.m_baseFont = m_baseFont;
	canvas.m_baseTextAttr = m_baseTextAttr;
	canvas.m_palette = m_palette;

	QRect qtRect = event->rect ();
	TRect rect (
		qtRect.x (), 
		qtRect.y (), 
		qtRect.x () + qtRect.width (), 
		qtRect.y () + qtRect.height ()
		);

	TWidgetPaintMsg msg (&canvas, rect);

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);	

	canvas.detach ();

	if (m_isCaretVisible && ((CQtEngine*) m_engine)->isCaretVisible ())
	{
		qtPainter->setCompositionMode (QPainter::RasterOp_SourceXorDestination);
		qtPainter->fillRect (
			m_caretPos.m_x, 
			m_caretPos.m_y, 
			m_caretSize.m_width, 
			m_caretSize.m_height,
			Qt::white
			);
	}
}

void
CQtWidgetImpl::onResizeEvent (QResizeEvent* event)
{
	if (!checkMsgMask (EWidgetMsg_Size))
	{
		event->ignore ();
		return;
	}

	QSize qtSize = event->size ();
	TSize size (qtSize.width (), qtSize.height ());

	uint_t mask = 0;
	if (m_size.m_width != size.m_width)
		mask |= 1 << EWidgetOrientation_Horizontal;

	if (m_size.m_height != size.m_height)
		mask |= 1 << EWidgetOrientation_Vertical;

	m_size = size;

	TWidgetMsgT <uint_t> msg (EWidgetMsg_Size, mask);
	
	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);

	if (!isHandled)
		event->ignore ();			
}		

void
CQtWidgetImpl::onScroll (
	QScrollBar* verticalScrollBar,
	QScrollBar* horizontalScrollBar
	)
{
	uint_t mask = 0;
	
	if (verticalScrollBar)
	{
		m_scrollBarArray [EWidgetOrientation_Vertical].m_pos = verticalScrollBar->value ();
		mask |= 1 << EWidgetOrientation_Vertical;
	}

	if (horizontalScrollBar)
	{
		m_scrollBarArray [EWidgetOrientation_Horizontal].m_pos = horizontalScrollBar->value ();
		mask |= 1 << EWidgetOrientation_Horizontal;
	}

	if (!checkMsgMask (EWidgetMsg_Size))
		return;

	TWidgetMsgT <uint_t> msg (EWidgetMsg_Scroll, mask);
	
	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);	
}

//.............................................................................

void 
qtWidgetBase::mouseMoveEvent (QMouseEvent* e)
{
	if (!m_mouseMoveEventFlag)
	{	
		CCursor* cursor = m_qtWidget->getCursor ();
		if (cursor)
		{
			setCursor (((CQtCursor*) cursor)->m_qtCursor);

			QCursor arrowCurosr (Qt::ArrowCursor);
			horizontalScrollBar ()->setCursor (arrowCurosr);
			verticalScrollBar ()->setCursor (arrowCurosr);
		}

		m_mouseMoveEventFlag = true;
	}

	m_qtWidget->onMouseEvent (e, EWidgetMsg_MouseMove);
}

//.............................................................................

} // namespace gui
} // namespace axl
