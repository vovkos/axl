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
		KeyKind_Esc,         // 0x00
		KeyKind_Tab,         // 0x01
		0,                // 0x02
		KeyKind_Backspace,   // 0x03
		KeyKind_Enter,       // 0x04
		KeyKind_Enter,       // 0x05
		KeyKind_Insert,      // 0x06
		KeyKind_Delete,      // 0x07
		KeyKind_Pause,       // 0x08
		KeyKind_Print,       // 0x09
		0,                // 0x0a
		0,                // 0x0b
		0,                // 0x0c
		0,                // 0x0d
		0,                // 0x0e
		0,                // 0x0f
		KeyKind_Home,        // 0x10
		KeyKind_End,         // 0x11
		KeyKind_Left,        // 0x12
		KeyKind_Up,          // 0x13
		KeyKind_Right,       // 0x14
		KeyKind_Down,        // 0x15
		KeyKind_PageUp,      // 0x16
		KeyKind_PageDown,    // 0x17
		0,                // 0x18
		0,                // 0x19
		0,                // 0x1a
		0,                // 0x1b
		0,                // 0x1c
		0,                // 0x1d
		0,                // 0x1e
		0,                // 0x1f
		KeyKind_Shift,       // 0x20
		KeyKind_Ctrl,        // 0x21
		0,                // 0x22
		KeyKind_Alt,         // 0x23
		KeyKind_CapsLock,    // 0x24
		KeyKind_NumLock,     // 0x25
		KeyKind_ScrollLock,  // 0x26
		0,                // 0x27
		0,                // 0x28
		0,                // 0x29
		0,                // 0x2a
		0,                // 0x2b
		0,                // 0x2c
		0,                // 0x2e
		0,                // 0x2f
		KeyKind_F1,          // 0x30
		KeyKind_F2,          // 0x31
		KeyKind_F3,          // 0x32
		KeyKind_F4,          // 0x33
		KeyKind_F5,          // 0x34
		KeyKind_F6,          // 0x35
		KeyKind_F7,          // 0x36
		KeyKind_F8,          // 0x37
		KeyKind_F9,          // 0x38
		KeyKind_F10,         // 0x39
		KeyKind_F11,         // 0x3a
		KeyKind_F12,         // 0x3b
	};

	return index < countof (keyTable) ? keyTable [index] : 0;
}

void 
QtWidgetImpl::onEvent (
	QEvent* event,
	WidgetMsgKind msgKind
	)
{
	if (!checkMsgMask (msgKind))
	{
		event->ignore ();	
		return;
	}

	WidgetMsg msg (msgKind);

	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);			

	if (!isHandled)
		event->ignore ();	
}

void 
QtWidgetImpl::onMouseEvent (
	QMouseEvent* event,
	WidgetMsgKind msgKind
	)
{	
	if (!checkMsgMask (msgKind))
	{
		event->ignore ();	
		return;
	}

	WidgetMouseMsg msg;
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
QtWidgetImpl::onMouseWheelEvent (QWheelEvent* event)
{	
	if (!checkMsgMask (WidgetMsgKind_MouseWheel))
	{
		event->ignore ();	
		return;
	}

	WidgetMouseWheelMsg msg;
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
QtWidgetImpl::onKeyEvent (
	QKeyEvent* event,
	WidgetMsgKind msgKind
	)
{	
	if (!checkMsgMask (msgKind))
	{
		event->ignore ();	
		return;
	}

	WidgetKeyMsg msg;
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
QtWidgetImpl::onPaintEvent (
	QPaintEvent* event,
	QPainter* qtPainter
	)		
{
	if (!checkMsgMask (WidgetMsgKind_Paint))
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
QtWidgetImpl::onResizeEvent (QResizeEvent* event)
{
	if (!checkMsgMask (WidgetMsgKind_Size))
	{
		event->ignore ();
		return;
	}

	QSize qtSize = event->size ();
	Size size (qtSize.width (), qtSize.height ());

	uint_t mask = 0;
	if (m_size.m_width != size.m_width)
		mask |= 1 << WidgetOrientationKind_Horizontal;

	if (m_size.m_height != size.m_height)
		mask |= 1 << WidgetOrientationKind_Vertical;

	m_size = size;

	WidgetMsgParam <uint_t> msg (WidgetMsgKind_Size, mask);
	
	bool isHandled = true;
	processWidgetMsg (&msg, &isHandled);

	if (!isHandled)
		event->ignore ();			
}		

void
QtWidgetImpl::onScroll (
	QScrollBar* verticalScrollBar,
	QScrollBar* horizontalScrollBar
	)
{
	uint_t mask = 0;
	
	if (verticalScrollBar)
	{
		m_scrollBarArray [WidgetOrientationKind_Vertical].m_pos = verticalScrollBar->value ();
		mask |= 1 << WidgetOrientationKind_Vertical;
	}

	if (horizontalScrollBar)
	{
		m_scrollBarArray [WidgetOrientationKind_Horizontal].m_pos = horizontalScrollBar->value ();
		mask |= 1 << WidgetOrientationKind_Horizontal;
	}

	if (!checkMsgMask (WidgetMsgKind_Size))
		return;

	WidgetMsgParam <uint_t> msg (WidgetMsgKind_Scroll, mask);
	
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

	m_qtWidget->onMouseEvent (e, WidgetMsgKind_MouseMove);
}

//.............................................................................

} // namespace gui
} // namespace axl
