// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTWIDGET_H

#include "axl_gui_Engine.h"
#include "axl_gui_Widget.h"
#include "axl_gui_QtCanvas.h"
#include "axl_gui_QtCursor.h"
#include "axl_err_Error.h"

namespace axl {
namespace gui {

class CQtEngine;
class QtWidgetBase;

CQtEngine*
GetQtEngineSingleton (); // thanks a lot gcc

//.............................................................................

class QtNotifyEvent: public QEvent
{
public:
	enum
	{
		QtType = QEvent::User + 0x200, // reserve 512 codes just in case
	};

public:
	QtNotifyEvent (
		uint_t code,
		const void* params
		): QEvent ((QEvent::Type) QtType)
	{
		m_code = code;
		m_params = params;
	}

	uint_t code ()
	{
		return m_code;
	}

	const void* params ()
	{
		return m_params;
	}

private:
	uint_t m_code;
	const void* m_params;
};

//.............................................................................

class CQtWidgetImpl: public CWidget
{
public:
	// this class is needed to get access to protected members in CWidget 
	// also to put part of implementation into .cpp instead of having one huge CQtWidgetT <>
	
	EMouseButton
	GetMouseButtonFromQtButton (Qt::MouseButton QtButton)
	{
		return (EMouseButton) (QtButton & 0x7);
	}

	uint_t
	GetMouseButtonsFromQtButtons (int QtButtons)
	{
		return QtButtons & 0x7;
	}

	uint_t
	GetModifierKeysFromQtModifiers (int QtModifiers)
	{
		return (QtModifiers >> 25) & 0x7;
	}
	
	void 
	OnEvent (
		QEvent* pEvent,
		EWidgetMsg MsgKind
		);
	
	void 
	OnMouseEvent (
		QMouseEvent* pEvent,
		EWidgetMsg MsgKind
		);

	void 
	OnMouseWheelEvent (QWheelEvent* pEvent);

	void 
	OnKeyEvent (
		QKeyEvent* pEvent,
		EWidgetMsg MsgKind
		);
	
	void
	OnPaintEvent (
		QPaintEvent* pEvent,
		QPainter* pQtPainter
		);
	
	void
	OnResizeEvent (QResizeEvent* pEvent);

	void
	OnScroll (
		QScrollBar* pVerticalScrollBar,
		QScrollBar* pHorizontalScrollBar
		);
};

//.............................................................................

// use QT naming conventions in this portion

class QtWidgetBase: public QAbstractScrollArea
{
	Q_OBJECT

protected:
	CQtWidgetImpl* m_qtWidget;
	bool m_mouseMoveEventFlag;

public:
	QtWidgetBase (
		CQtWidgetImpl* qtWidget,
		QWidget* parent
		):
		QAbstractScrollArea (parent)
	{
		m_qtWidget = qtWidget;
		m_mouseMoveEventFlag = false;

		connect(
			this, &QtWidgetBase::threadMsgSignal,
			this, &QtWidgetBase::threadMsgSlot, 
			Qt::QueuedConnection
			);
	}

	void
	postThreadMsg (
		uint_t code,
		const ref::CPtrT <void>& params
		)
	{
		TWidgetThreadMsg* msg = AXL_MEM_NEW (TWidgetThreadMsg);
		msg->m_MsgKind = EWidgetMsg_ThreadMsg;
		msg->m_Code = code;
		msg->m_Params = params;

		emit threadMsgSignal (msg);
	}

private slots:
	void threadMsgSlot (TWidgetThreadMsg* msg)
	{
		bool IsHandled = true;
		m_qtWidget->ProcessWidgetMsg (msg, &IsHandled);
		AXL_MEM_DELETE (msg);
	}

signals:
	void threadMsgSignal (TWidgetThreadMsg* msg);

public:
	virtual 
	bool 
	event (QEvent* e)
	{
		if (e->type () != QtNotifyEvent::QtType)
			return QAbstractScrollArea::event (e);
	
		notifyEvent ((QtNotifyEvent*) e);
		return e->isAccepted ();
	}

protected:
	virtual 
	void 
	notifyEvent (QtNotifyEvent* e)
	{
	}

	virtual 
	void 
	mousePressEvent (QMouseEvent* e)
	{
		m_qtWidget->OnMouseEvent (e, EWidgetMsg_MouseButtonDown);
	}

	virtual 
	void 
	mouseReleaseEvent (QMouseEvent* e)
	{
		m_qtWidget->OnMouseEvent (e, EWidgetMsg_MouseButtonUp);
	}
	
	virtual 
	void 
	mouseDoubleClickEvent (QMouseEvent* e)
	{
		m_qtWidget->OnMouseEvent (e, EWidgetMsg_MouseButtonDoubleClick);
	}

	virtual 
	void 
	mouseMoveEvent (QMouseEvent* e);
	
	virtual 
	void 
	wheelEvent (QWheelEvent* e)
	{	
		m_qtWidget->OnMouseWheelEvent (e);
	}
	
	virtual 
	void 
	keyPressEvent (QKeyEvent* e)
	{
		m_qtWidget->OnKeyEvent (e, EWidgetMsg_KeyDown);
	}
		
	virtual 
	void 
	keyReleaseEvent (QKeyEvent* e)
	{
		m_qtWidget->OnKeyEvent (e, EWidgetMsg_KeyUp);
	}
	
	virtual 
	void 
	focusInEvent (QFocusEvent* e)
	{
		m_qtWidget->OnEvent (e, EWidgetMsg_SetFocus);
	}
	
	virtual 
	void 
	focusOutEvent (QFocusEvent* e)
	{
		m_qtWidget->OnEvent (e, EWidgetMsg_KillFocus);
	}
	
	virtual 
	void 
	leaveEvent (QEvent* e)
	{
		m_qtWidget->OnEvent (e, EWidgetMsg_MouseLeave);
	}
	
	virtual 
	void 
	paintEvent (QPaintEvent* e)
	{		
		QPainter Painter (viewport ());
		m_qtWidget->OnPaintEvent (e, &Painter);
	}

	virtual 
	void 
	resizeEvent (QResizeEvent* e)
	{
		m_qtWidget->OnResizeEvent (e);
	}
	
	virtual 
	void 
	closeEvent (QCloseEvent* e)
	{
		m_qtWidget->OnEvent (e, EWidgetMsg_Close);
	}
	
	virtual 
	void 
	scrollContentsBy (
		int dx, 
		int dy 
		)
	{
		m_qtWidget->OnScroll (
			dy ? verticalScrollBar () : NULL,
			dx ? horizontalScrollBar () : NULL
			);
	}
};

//.............................................................................

template <typename T>
class CQtWidgetT: public T
{
	friend class CQtEngine;

public:
	QtWidgetBase* m_pQtScrollArea;
	QWidget* m_pQtWidget;
	bool m_MouseMoveFlag;
	
public:
	CQtWidgetT (): T (GetQtEngineSingleton ())
	{
		m_pQtScrollArea = NULL;
		m_pQtWidget = NULL;
		m_MouseMoveFlag = false;
	}
	
	virtual
	ref::CPtrT <CCanvas>
	GetCanvas ()
	{
		ref::CPtrT <CQtCanvas> Canvas = AXL_REF_NEW (ref::CBoxT <CQtCanvas>);
		Canvas->Attach (new QPainter (m_pQtScrollArea->viewport ()));
		return Canvas;
	}

	virtual
	bool
	Redraw (	
		int Left, 
		int Top, 
		int Right, 
		int Bottom		
		)
	{
		if (Left == Right || Top == Bottom)
			m_pQtWidget->update ();
		else
			m_pQtWidget->update (Left, Top, Right - Left, Bottom - Top);

		return true;
	}

	virtual
	bool
	IsFocused ()
	{
		return m_pQtWidget->hasFocus ();
	}

	virtual
	bool
	SetFocus ()
	{
		m_pQtWidget->setFocus ();
		return true;		
	}

	virtual
	bool
	SetCursor (CCursor* pCursor)
	{
		ASSERT (pCursor->GetEngine ()->GetEngineKind () == EEngine_Qt);
		m_pQtWidget->setCursor (((CQtCursor*) pCursor)->m_QtCursor);
		this->m_pCursor = pCursor; // thanks a lot gcc
		return true;		
	}

	virtual
	bool
	SetMouseCapture ()
	{
		m_pQtWidget->grabMouse ();
		return true;
	}

	virtual
	bool
	ReleaseMouseCapture ()
	{
		m_pQtWidget->releaseMouse ();
		return true;
	}

	virtual
	bool
	UpdateScrollBar (EWidgetOrientation Orientation)
	{
		ASSERT (Orientation < EWidgetOrientation__Count);
		
		const TWidgetScrollBar* pScrollBar = &this->m_ScrollBarArray [Orientation];
		
		QScrollBar* pQtScrollBar = Orientation == EWidgetOrientation_Horizontal ?
			m_pQtScrollArea->horizontalScrollBar () :
			m_pQtScrollArea->verticalScrollBar ();
		
		size_t Maximum = pScrollBar->m_Page < pScrollBar->m_End ? 
			pScrollBar->m_End - pScrollBar->m_Page : 
			0;
		
		pQtScrollBar->setPageStep (pScrollBar->m_Page);
		pQtScrollBar->setMaximum (Maximum);
		pQtScrollBar->setValue (pScrollBar->m_Pos);
		return true;
	}

	virtual
	intptr_t
	NotifyParent (
		intptr_t NotifyCode,
		void* pParam = NULL
		)
	{
		QtNotifyEvent e (NotifyCode, pParam);		
		return m_pQtScrollArea->event (&e);
	}

	virtual
	void
	PostThreadMsg (
		uint_t Code,
		const ref::CPtrT <void>& Params
		)
	{
		m_pQtScrollArea->postThreadMsg (Code, Params);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class QtWidget: public QtWidgetBase
{
protected:
	CQtWidgetT <T> m_widget; 
	
public:
	QtWidget (QWidget* parent = 0):
		QtWidgetBase ((CQtWidgetImpl*) (CWidget*) &m_widget, parent)
	{
		m_widget.m_pQtScrollArea = this;
		m_widget.m_pQtWidget = viewport ();
		m_widget.m_pQtWidget->setMouseTracking (true);
	}
	
	T* 
	operator -> ()
	{
		return &m_widget;
	}
	
	T*
	w ()
	{
		return &m_widget;
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
