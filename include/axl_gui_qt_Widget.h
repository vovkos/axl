// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QT_WIDGET_H

#include "axl_gui_Engine.h"
#include "axl_gui_Widget.h"
#include "axl_gui_qt_Painter.h"
#include "axl_gui_qt_Cursor.h"
#include "axl_err_Error.h"

namespace axl {
namespace gui {
namespace qt {

class CEngine;

CEngine*
GetEngineSingleton (); // thanks a lot gcc

//.............................................................................

struct IQtWidget: public IWidget
{
	// this class is needed to get access to protected members in IWidget 
	// also to put part of implementation into .cpp instead of having one huge CWidgetT <>
	
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
	GetKeyFromQtKey (int QtKey)
	{
		// TODO: implement table translation		
		return QtKey;
	}

	uint_t
	GetModifierKeysFromQtModifiers (int QtModifiers)
	{
		return (QtModifiers >> 1) & 0x7;
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

template <typename T>
class CWidgetT: public T
{
	friend class CEngine;

public:
	AXL_OBJ_CLASS_0 (CWidgetT, IWidget);

public:
	QAbstractScrollArea* m_pQtScrollArea;
	QWidget* m_pQtWidget;
	
public:
	CWidgetT (): T (GetEngineSingleton ())
	{
		m_pQtScrollArea = NULL;
		m_pQtWidget = NULL;
	}
	
	virtual
	ref::CPtrT <ICanvas>
	GetCanvas ()
	{
		ref::CPtrT <CPainter> Painter = AXL_REF_NEW (ref::CBoxT <CPainter>);
		Painter->Attach (new QPainter (m_pQtWidget));
		return Painter;
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
			m_pQtWidget->repaint ();
		else
			m_pQtWidget->repaint (Left, Top, Right - Left, Bottom - Top);

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
	SetCursor (ICursor* pCursor)
	{
		ASSERT (pCursor->GetEngine ()->GetEngineKind () == EEngine_Qt);
		m_pQtWidget->setCursor (((CCursor*) pCursor)->m_QtCursor);
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
	ShowCaret (	
		int x, 
		int y,
		int Width,
		int Height
		)
	{
		// TODO
		return true;
	}

	virtual
	void
	HideCaret ()
	{
		// TODO
	}

	virtual
	bool
	SetCaretPos (
		int x, 
		int y
		)
	{
		// TODO
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
		// TODO
		return 0;
	}
};

//.............................................................................

template <typename T>
class CQtWidgetT: public QAbstractScrollArea
{
protected:
	CWidgetT <T> m_Widget;
	
public:
	CQtWidgetT (QWidget* pQtParentWidget = NULL):
		QAbstractScrollArea (pQtParentWidget)
	{
		m_Widget.m_pQtScrollArea = this;
		m_Widget.m_pQtWidget = viewport ();
		m_Widget.m_pQtWidget->setMouseTracking (true);
	}
	
	T* 
	operator -> ()
	{
		return &m_Widget;
	}
	
	T*
	w ()
	{
		return &m_Widget;
	}

protected:	
	virtual 
	void 
	mousePressEvent (QMouseEvent* pEvent)
	{
		GetQtWidget ()->OnMouseEvent (pEvent, EWidgetMsg_MouseButtonDown);
	}

	virtual 
	void 
	mouseReleaseEvent (QMouseEvent* pEvent)
	{
		GetQtWidget ()->OnMouseEvent (pEvent, EWidgetMsg_MouseButtonUp);
	}
	
	virtual 
	void 
	mouseDoubleClickEvent (QMouseEvent* pEvent)
	{
		GetQtWidget ()->OnMouseEvent (pEvent, EWidgetMsg_MouseButtonDoubleClick);
	}

	virtual 
	void 
	mouseMoveEvent (QMouseEvent* pEvent)
	{
		GetQtWidget ()->OnMouseEvent (pEvent, EWidgetMsg_MouseMove);
	}
	
	virtual 
	void 
	wheelEvent (QWheelEvent* pEvent)
	{	
		GetQtWidget ()->OnMouseWheelEvent (pEvent);
	}
	
	virtual 
	void 
	keyPressEvent (QKeyEvent* pEvent)
	{
		GetQtWidget ()->OnKeyEvent (pEvent, EWidgetMsg_KeyDown);
	}
		
	virtual 
	void 
	keyReleaseEvent (QKeyEvent* pEvent)
	{
		GetQtWidget ()->OnKeyEvent (pEvent, EWidgetMsg_KeyUp);
	}
	
	virtual 
	void 
	focusInEvent (QFocusEvent* pEvent)
	{
		GetQtWidget ()->OnEvent (pEvent, EWidgetMsg_SetFocus);
	}
	
	virtual 
	void 
	focusOutEvent (QFocusEvent* pEvent)
	{
		GetQtWidget ()->OnEvent (pEvent, EWidgetMsg_KillFocus);
	}
	
	virtual 
	void 
	leaveEvent (QEvent* pEvent)
	{
		GetQtWidget ()->OnEvent (pEvent, EWidgetMsg_MouseLeave);
	}
	
	virtual 
	void 
	paintEvent (QPaintEvent* pEvent)
	{		
		QPainter Painter (viewport ());
		GetQtWidget ()->OnPaintEvent (pEvent, &Painter);
	}

	virtual 
	void 
	resizeEvent (QResizeEvent* pEvent)
	{
		GetQtWidget ()->OnResizeEvent (pEvent);
	}
	
	virtual 
	void 
	closeEvent (QCloseEvent* pEvent)
	{
		GetQtWidget ()->OnEvent (pEvent, EWidgetMsg_Close);
	}
	
	virtual void 
	scrollContentsBy (
		int dx, 
		int dy 
		)
	{
		GetQtWidget ()->OnScroll (
			dy ? verticalScrollBar () : NULL,
			dx ? horizontalScrollBar () : NULL
			);
	}
	
protected:
	IQtWidget*
	GetQtWidget ()
	{
		return (IQtWidget*) (IWidget*) &m_Widget;
	}
};

//.............................................................................

} // namespace qt
} // namespace gui
} // namespace axl
