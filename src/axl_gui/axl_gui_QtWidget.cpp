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
#include "axl_gui_QtWidget.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//..............................................................................

inline
MouseButton
getMouseButtonFromQtButton (Qt::MouseButton qtButton)
{
	return (MouseButton) (qtButton & 0x7);
}

inline
uint_t
getMouseButtonsFromQtButtons (int qtButtons)
{
	return qtButtons & 0x7;
}

inline
uint_t
getModifierKeysFromQtModifiers (int qtModifiers)
{
	return (qtModifiers >> 25) & 0x7;
}

uint_t
getKeyFromQtKey (int qtKey)
{
	ASSERT (qtKey & 0x01000000);

	size_t index = qtKey & ~0x01000000;

	static uint_t keyTable [] =
	{
		Key_Esc,         // 0x00
		Key_Tab,         // 0x01
		Key_Tab,         // 0x02
		Key_Backspace,   // 0x03
		Key_Enter,       // 0x04
		Key_Enter,       // 0x05
		Key_Insert,      // 0x06
		Key_Delete,      // 0x07
		Key_Pause,       // 0x08
		Key_Print,       // 0x09
		0,               // 0x0a
		0,               // 0x0b
		0,               // 0x0c
		0,               // 0x0d
		0,               // 0x0e
		0,               // 0x0f
		Key_Home,        // 0x10
		Key_End,         // 0x11
		Key_Left,        // 0x12
		Key_Up,          // 0x13
		Key_Right,       // 0x14
		Key_Down,        // 0x15
		Key_PageUp,      // 0x16
		Key_PageDown,    // 0x17
		0,               // 0x18
		0,               // 0x19
		0,               // 0x1a
		0,               // 0x1b
		0,               // 0x1c
		0,               // 0x1d
		0,               // 0x1e
		0,               // 0x1f
		Key_Shift,       // 0x20
		Key_Ctrl,        // 0x21
		0,               // 0x22
		Key_Alt,         // 0x23
		Key_CapsLock,    // 0x24
		Key_NumLock,     // 0x25
		Key_ScrollLock,  // 0x26
		0,               // 0x27
		0,               // 0x28
		0,               // 0x29
		0,               // 0x2a
		0,               // 0x2b
		0,               // 0x2c
		0,               // 0x2e
		0,               // 0x2f
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

//..............................................................................

QtWidgetBase::QtWidgetBase (
	WidgetDriver* widgetDriver,
	QWidget* parent
	):
	QAbstractScrollArea (parent)
{
	m_widgetDriver = widgetDriver;
	setMouseTracking (true);

	connect(
		this, &QtWidgetBase::threadMsgSignal,
		this, &QtWidgetBase::threadMsgSlot,
		Qt::QueuedConnection
		);
}

void
QtWidgetBase::postThreadMsg (
	uint_t code,
	const ref::Ptr <void>& params
	)
{
	WidgetThreadMsg* msg = AXL_MEM_NEW (WidgetThreadMsg);
	msg->m_msgCode = WidgetMsgCode_ThreadMsg;
	msg->m_code = code;
	msg->m_params = params;

	emit threadMsgSignal (msg);
}

void
QtWidgetBase::threadMsgSlot (WidgetThreadMsg* msg)
{
	bool isHandled = true;
	m_widgetDriver->processMsg (msg, &isHandled);
	AXL_MEM_DELETE (msg);
}

void
QtWidgetBase::wheelEvent (QWheelEvent* e)
{
	if (!m_widgetDriver->checkMsgMap (WidgetMsgCode_MouseWheel))
	{
		e->ignore ();
		return;
	}

	WidgetMouseWheelMsg msg;
	msg.m_point.setup (e->x (), e->y ());
	msg.m_buttons = getMouseButtonsFromQtButtons (e->buttons ());
	msg.m_modifierKeys = getModifierKeysFromQtModifiers (e->modifiers ());
	msg.m_wheelDelta = e->delta ();

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);

	if (!isHandled)
		e->ignore ();
}

void
QtWidgetBase::paintEvent (QPaintEvent* e)
{
	if (!m_widgetDriver->checkMsgMap (WidgetMsgCode_Paint))
	{
		e->ignore ();
		return;
	}

	QtCanvas canvas;
	canvas.m_qtPainter.begin (viewport ());
	canvas.m_font = m_widgetDriver->getFont ();
	canvas.m_colorAttr = m_widgetDriver->getColorAttr ();
	canvas.m_palette = m_widgetDriver->getPalette ();

	QRect rect = e->rect ();
	QRegion region = e->region ();

	WidgetPaintMsg msg (&canvas);
	msg.m_rect.m_left   = rect.x ();
	msg.m_rect.m_top    = rect.y ();
	msg.m_rect.m_right  = rect.x () + rect.width ();
	msg.m_rect.m_bottom = rect.y () + rect.height ();

	size_t rectCount = region.rectCount ();
	if (rectCount > countof (msg.m_region))
	{
		msg.m_regionRectCount = 1;
		msg.m_region [0] = msg.m_rect;
	}
	else
	{
		msg.m_regionRectCount = rectCount;

		QVector <QRect> rectVector = region.rects ();

		Rect* dst = msg.m_region;
		const QRect* src = rectVector.constBegin ();
		for (size_t i = 0; i < rectCount; i++, dst++, src++)
		{
			dst->m_left   = src->x ();
			dst->m_top    = src->y ();
			dst->m_right  = src->x () + src->width ();
			dst->m_bottom = src->y () + src->height ();
		}
	}

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);

	if (m_widgetDriver->isCaretVisible () &&
		((QtEngine*) m_widgetDriver->getEngine ())->isCaretVisible ())
	{
		Point caretPos = m_widgetDriver->getCaretPos ();
		Size caretSize = m_widgetDriver->getCaretSize ();

		canvas.m_qtPainter.setCompositionMode (QPainter::RasterOp_SourceXorDestination);
		canvas.m_qtPainter.fillRect (
			caretPos.m_x,
			caretPos.m_y,
			caretSize.m_width,
			caretSize.m_height,
			Qt::white
			);
	}

	canvas.m_qtPainter.end ();
}

void
QtWidgetBase::resizeEvent (QResizeEvent* e)
{
	if (!m_widgetDriver->checkMsgMap (WidgetMsgCode_Size))
	{
		e->ignore ();
		return;
	}

	QSize qtSize = viewport ()->size ();
	Size size (qtSize.width (), qtSize.height ());

	uint_t mask = 0;
	if (m_widgetDriver->m_size.m_width != size.m_width)
		mask |= 1 << Orientation_Horizontal;

	if (m_widgetDriver->m_size.m_height != size.m_height)
		mask |= 1 << Orientation_Vertical;

	m_widgetDriver->m_size = size;

	WidgetMsgParam <uint_t> msg (WidgetMsgCode_Size, mask);

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);

	if (!isHandled)
		e->ignore ();
}

void
QtWidgetBase::scrollContentsBy (
	int dx,
	int dy
	)
{
	uint_t mask = 0;

	if (dy)
	{
		m_widgetDriver->m_scrollBarArray [Orientation_Vertical].m_pos = verticalScrollBar ()->value ();
		mask |= 1 << Orientation_Vertical;
	}

	if (dx)
	{
		m_widgetDriver->m_scrollBarArray [Orientation_Horizontal].m_pos = horizontalScrollBar ()->value ();
		mask |= 1 << Orientation_Horizontal;
	}

	if (!m_widgetDriver->checkMsgMap (WidgetMsgCode_Size))
		return;

	WidgetMsgParam <uint_t> msg (WidgetMsgCode_Scroll, mask);

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);
}

void
QtWidgetBase::genericEventImpl (
	QEvent* e,
	WidgetMsgCode msgCode
	)
{
	if (!m_widgetDriver->checkMsgMap (msgCode))
	{
		e->ignore ();
		return;
	}

	WidgetMsg msg (msgCode);

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);

	if (!isHandled)
		e->ignore ();
}

void
QtWidgetBase::mouseEventImpl (
	QMouseEvent* e,
	WidgetMsgCode msgCode
	)
{
	if (!m_widgetDriver->checkMsgMap (msgCode))
	{
		e->ignore ();
		return;
	}

	WidgetMouseMsg msg;
	msg.m_msgCode = msgCode;
	msg.m_point.setup (e->x (), e->y ());
	msg.m_buttons = getMouseButtonsFromQtButtons (e->buttons ());
	msg.m_modifierKeys = getModifierKeysFromQtModifiers (e->modifiers ());
	msg.m_button = getMouseButtonFromQtButton (e->button ());

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);

	if (!isHandled)
		e->ignore ();
}

void
QtWidgetBase::keyEventImpl (
	QKeyEvent* e,
	WidgetMsgCode msgCode
	)
{
	if (!m_widgetDriver->checkMsgMap (msgCode))
	{
		e->ignore ();
		return;
	}

	WidgetKeyMsg msg;
	msg.m_msgCode = msgCode;

	int qtKey = e->key ();
	if (qtKey & 0x01000000)
	{
		msg.m_key = getKeyFromQtKey (qtKey);
		if (qtKey <= Qt::Key_Enter)
			msg.m_char = msg.m_key;
	}
	else
	{
		msg.m_key = qtKey;
		msg.m_char = e->text ().at (0).unicode ();
	}

	msg.m_modifierKeys = getModifierKeysFromQtModifiers (e->modifiers ());

	bool isHandled = true;
	m_widgetDriver->processMsg (&msg, &isHandled);

	if (!isHandled)
		e->ignore ();
}

//..............................................................................

} // namespace gui
} // namespace axl
