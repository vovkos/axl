// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTWIDGET_H

#include "axl_gui_Widget.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

class QtWidgetBase: public QAbstractScrollArea
{
	Q_OBJECT

protected:
	WidgetDriver* m_widgetDriver;
	bool m_mouseMoveEventFlag;

public:
	QtWidgetBase (
		WidgetDriver* widgetDriver,
		QWidget* parent = NULL
		);

	~QtWidgetBase ()
	{
		m_widgetDriver->setCaretVisible (false); // important for QT
	}

	void
	postThreadMsg (
		uint_t code,
		const ref::Ptr <void>& params
		);

	virtual
	void
	emitNotificationSignal (
		uint_t code,
		const void* params
		)
	{
	}

private slots:
	void 
	threadMsgSlot (WidgetThreadMsg* msg);

signals:
	void 
	threadMsgSignal (WidgetThreadMsg* msg);
	
protected:
	virtual
	void
	mousePressEvent (QMouseEvent* e)
	{
		mouseEventImpl (e, WidgetMsgCode_MouseButtonDown);
	}

	virtual
	void
	mouseReleaseEvent (QMouseEvent* e)
	{
		mouseEventImpl (e, WidgetMsgCode_MouseButtonUp);
	}

	virtual
	void
	mouseDoubleClickEvent (QMouseEvent* e)
	{
		mouseEventImpl (e, WidgetMsgCode_MouseButtonDoubleClick);
	}

	virtual
	void
	mouseMoveEvent (QMouseEvent* e);

	virtual
	void
	wheelEvent (QWheelEvent* e);

	virtual
	void
	keyPressEvent (QKeyEvent* e)
	{
		keyEventImpl (e, WidgetMsgCode_KeyDown);
	}

	virtual
	void
	keyReleaseEvent (QKeyEvent* e)
	{
		keyEventImpl (e, WidgetMsgCode_KeyUp);
	}

	virtual
	void
	focusInEvent (QFocusEvent* e)
	{
		genericEventImpl (e, WidgetMsgCode_SetFocus);
	}

	virtual
	void
	focusOutEvent (QFocusEvent* e)
	{
		genericEventImpl (e, WidgetMsgCode_KillFocus);
	}

	virtual
	void
	leaveEvent (QEvent* e)
	{
		genericEventImpl (e, WidgetMsgCode_MouseLeave);
	}

	virtual
	void
	paintEvent (QPaintEvent* e);

	virtual
	void
	resizeEvent (QResizeEvent* e);

	virtual
	void
	closeEvent (QCloseEvent* e)
	{
		genericEventImpl (e, WidgetMsgCode_Close);
	}

	virtual
	void
	scrollContentsBy (
		int dx,
		int dy
		);

protected:
	void 
	genericEventImpl (
		QEvent* e,
		WidgetMsgCode msgCode
		);

	void 
	mouseEventImpl (
		QMouseEvent* e,
		WidgetMsgCode msgCode
		);

	void 
	keyEventImpl (
		QKeyEvent* e,
		WidgetMsgCode msgCode
		);
};

//.............................................................................

template <typename T>
class QtWidget: 
	public QtWidgetBase,
	public T
{
public:
	QtWidget (QWidget* parent = NULL):
		QtWidgetBase (&(T::m_widgetDriver), parent),
		T (WidgetConstructParam (getQtEngine (), (QtWidgetBase*) this))
	{
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
