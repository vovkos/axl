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
		Key_Esc,         // 0x00
		Key_Tab,         // 0x01
		0,                // 0x02
		Key_Backspace,   // 0x03
		Key_Enter,       // 0x04
		Key_Enter,       // 0x05
		Key_Insert,      // 0x06
		Key_Delete,      // 0x07
		Key_Pause,       // 0x08
		Key_Print,       // 0x09
		0,                // 0x0a
		0,                // 0x0b
		0,                // 0x0c
		0,                // 0x0d
		0,                // 0x0e
		0,                // 0x0f
		Key_Home,        // 0x10
		Key_End,         // 0x11
		Key_Left,        // 0x12
		Key_Up,          // 0x13
		Key_Right,       // 0x14
		Key_Down,        // 0x15
		Key_PageUp,      // 0x16
		Key_PageDown,    // 0x17
		0,                // 0x18
		0,                // 0x19
		0,                // 0x1a
		0,                // 0x1b
		0,                // 0x1c
		0,                // 0x1d
		0,                // 0x1e
		0,                // 0x1f
		Key_Shift,       // 0x20
		Key_Ctrl,        // 0x21
		0,                // 0x22
		Key_Alt,         // 0x23
		Key_CapsLock,    // 0x24
		Key_NumLock,     // 0x25
		Key_ScrollLock,  // 0x26
		0,                // 0x27
		0,                // 0x28
		0,                // 0x29
		0,                // 0x2a
		0,                // 0x2b
		0,                // 0x2c
		0,                // 0x2e
		0,                // 0x2f
		Key_F1,          // 0x30
		Key_F2,          // 0x31
		Key_F3,          // 0x32
		Key_F4,          // 0x33
		Key_F5,          // 0x34
		Key_F6,          // 0x35
		Key_F7,          // 0x36
		Key_F8,          // 0x37
		Key_F9,          // 0x38
		Key_F10,         // 0x39
		Key_F11,         // 0x3a
		Key_F12,         // 0x3b
	};

	return index < countof (keyTable) ? keyTable [index] : 0;
}

void 
QtWidgetEventHandler::onEvent (
	QEvent* event,
	WidgetMsgCode msgCode
	)
{
	if (!checkMsgMask (msgCode))
	{
		event->ignore ();	
		return;
	}

	WidgetMsg msg (msgCode);

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();	
}

void 
QtWidgetEventHandler::onMouseEvent (
	QMouseEvent* event,
	WidgetMsgCode msgCode
	)
{	
	if (!checkMsgMask (msgCode))
	{
		event->ignore ();	
		return;
	}

	WidgetMouseMsg msg;
	msg.m_msgCode = msgCode;
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
QtWidgetEventHandler::onMouseWheelEvent (QWheelEvent* event)
{	
	if (!checkMsgMask (WidgetMsgCode_MouseWheel))
	{
		event->ignore ();	
		return;
	}

	WidgetMouseWheelMsg msg;
	msg.m_point.setup (event->x (), event->y ());
	msg.m_buttons = getMouseButtonsFromQtButtons (event->buttons ());
	msg.m_modifierKeys = getModifierKeysFromQtModifiers (event->modifiers ());
	msg.m_wheelDelta = event->delta ();

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();
}

void 
QtWidgetEventHandler::onKeyEvent (
	QKeyEvent* event,
	WidgetMsgCode msgCode
	)
{	
	if (!checkMsgMask (msgCode))
	{
		event->ignore ();	
		return;
	}

	WidgetKeyMsg msg;
	msg.m_msgCode = msgCode;

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
QtWidgetEventHandler::onPaintEvent (
	QPaintEvent* event,
	QPainter* qtPainter
	)		
{
	if (!checkMsgMask (WidgetMsgCode_Paint))
	{
		event->ignore ();
		return;
	}
	
	QtCanvas canvas;
	canvas.attach (qtPainter);
	canvas.m_baseFont = m_baseFont;
	canvas.m_baseTextAttr = m_baseTextAttr;
	canvas.m_palette = m_palette;

	QRect qtRect = event->rect ();
	Rect rect (
		qtRect.x (), 
		qtRect.y (), 
		qtRect.x () + qtRect.width (), 
		qtRect.y () + qtRect.height ()
		);

	WidgetPaintMsg msg (&canvas, rect);

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);	

	canvas.detach ();

	if (m_isCaretVisible && ((QtEngine*) m_engine)->isCaretVisible ())
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
QtWidgetEventHandler::onResizeEvent (QResizeEvent* event)
{
	if (!checkMsgMask (WidgetMsgCode_Size))
	{
		event->ignore ();
		return;
	}

	QSize qtSize = event->size ();
	Size size (qtSize.width (), qtSize.height ());

	uint_t mask = 0;
	if (m_size.m_width != size.m_width)
		mask |= 1 << WidgetOrientation_Horizontal;

	if (m_size.m_height != size.m_height)
		mask |= 1 << WidgetOrientation_Vertical;

	m_size = size;

	WidgetMsgParam <uint_t> msg (WidgetMsgCode_Size, mask);
	
	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);

	if (!isHandled)
		event->ignore ();			
}		

void
QtWidgetEventHandler::onScroll (
	QScrollBar* verticalScrollBar,
	QScrollBar* horizontalScrollBar
	)
{
	uint_t mask = 0;
	
	if (verticalScrollBar)
	{
		m_scrollBarArray [WidgetOrientation_Vertical].m_pos = verticalScrollBar->value ();
		mask |= 1 << WidgetOrientation_Vertical;
	}

	if (horizontalScrollBar)
	{
		m_scrollBarArray [WidgetOrientation_Horizontal].m_pos = horizontalScrollBar->value ();
		mask |= 1 << WidgetOrientation_Horizontal;
	}

	if (!checkMsgMask (WidgetMsgCode_Size))
		return;

	WidgetMsgParam <uint_t> msg (WidgetMsgCode_Scroll, mask);
	
	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);	
}

//.............................................................................

void 
QtWidgetBase::mouseMoveEvent (QMouseEvent* e)
{
	if (!m_mouseMoveEventFlag)
	{	
		Cursor* cursor = m_qtWidget->getCursor ();
		if (cursor)
		{
			setCursor (((QtCursor*) cursor)->m_qtCursor);

			QCursor arrowCurosr (Qt::ArrowCursor);
			horizontalScrollBar ()->setCursor (arrowCurosr);
			verticalScrollBar ()->setCursor (arrowCurosr);
		}

		m_mouseMoveEventFlag = true;
	}

	m_qtWidget->onMouseEvent (e, WidgetMsgCode_MouseMove);
}

//.............................................................................

} // namespace gui
} // namespace axl
