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

#pragma once

#define _AXL_GUI_QTWIDGET_H

#include "axl_gui_QtEngine.h"
#include "axl_gui_Widget.h"

namespace axl {
namespace gui {

//..............................................................................

class QtWidgetBase: public QAbstractScrollArea
{
	Q_OBJECT

	friend class QtEngine;

protected:
	WidgetDriver* m_widgetDriver;
	QBasicTimer m_animationTimer;

public:
	QtWidgetBase(
		WidgetDriver* widgetDriver,
		QWidget* parent = NULL
		);

	~QtWidgetBase()
	{
		m_widgetDriver->setCaretVisible(false); // important for QT
	}

	void
	postThreadMsg(
		uint_t code,
		const rc::Ptr<void>& params
		);

	virtual
	void
	emitNotificationSignal(
		uint_t code,
		const void* param
		)
	{
	}

private slots:
	void
	threadMsgSlot(WidgetThreadMsg* msg);

	void
	toolTipTimerSlot();

signals:
	void
	threadMsgSignal(WidgetThreadMsg* msg);

protected:
	virtual
	void
	mousePressEvent(QMouseEvent* e)
	{
		mouseEventImpl(e, WidgetMsgCode_MouseButtonDown);
	}

	virtual
	void
	mouseReleaseEvent(QMouseEvent* e)
	{
		mouseEventImpl(e, WidgetMsgCode_MouseButtonUp);
	}

	virtual
	void
	mouseDoubleClickEvent(QMouseEvent* e)
	{
		mouseEventImpl(e, WidgetMsgCode_MouseButtonDoubleClick);
	}

	virtual
	void
	mouseMoveEvent(QMouseEvent* e)
	{
		mouseEventImpl(e, WidgetMsgCode_MouseMove);
	}

	virtual
	void
	wheelEvent(QWheelEvent* e);

	virtual
	void
	keyPressEvent(QKeyEvent* e)
	{
		keyEventImpl(e, WidgetMsgCode_KeyDown);
	}

	virtual
	void
	keyReleaseEvent(QKeyEvent* e)
	{
		keyEventImpl(e, WidgetMsgCode_KeyUp);
	}

	virtual
	void
	focusInEvent(QFocusEvent* e)
	{
		genericEventImpl(e, WidgetMsgCode_SetFocus);
	}

	virtual
	void
	focusOutEvent(QFocusEvent* e)
	{
		genericEventImpl(e, WidgetMsgCode_KillFocus);
	}

	virtual
	void
	leaveEvent(QEvent* e)
	{
		genericEventImpl(e, WidgetMsgCode_MouseLeave);
	}

	virtual
	void
	paintEvent(QPaintEvent* e);

	virtual
	void
	resizeEvent(QResizeEvent* e);

	virtual
	void
	closeEvent(QCloseEvent* e)
	{
		genericEventImpl(e, WidgetMsgCode_Close);
	}

	virtual
	void
	timerEvent(QTimerEvent* e)
	{
		if (e->timerId() == m_animationTimer.timerId())
			genericEventImpl(e, WidgetMsgCode_Animate);
	}

	virtual
	void
	scrollContentsBy(
		int dx,
		int dy
		);

protected:
	void
	genericEventImpl(
		QEvent* e,
		WidgetMsgCode msgCode
		);

	void
	mouseEventImpl(
		QMouseEvent* e,
		WidgetMsgCode msgCode
		);

	void
	keyEventImpl(
		QKeyEvent* e,
		WidgetMsgCode msgCode
		);
};

//..............................................................................

template <typename T>
class QtWidget:
	public QtWidgetBase,
	public T
{
public:
	QtWidget(QWidget* parent = NULL):
		QtWidgetBase(&(T::m_widgetDriver), parent),
		T(WidgetConstructParam(getQtEngine(), (QtWidgetBase*)this))
	{
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
