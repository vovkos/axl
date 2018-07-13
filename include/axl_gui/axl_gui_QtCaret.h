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

#define _AXL_GUI_QTCANVAS_H

#include "axl_gui_QtPch.h"
#include "axl_gui_Def.h"

namespace axl {
namespace gui {

class WidgetDriver;

//..............................................................................

class QtCaret: public QTimer
{
protected:
	WidgetDriver* m_widgetDriver;
	bool m_isVisible;
	Rect m_rect;

public:
	QtCaret ()
	{
		m_isVisible = false;
		m_widgetDriver = NULL;
	}

	bool
	isVisible ()
	{
		return m_isVisible;
	}

	bool
	show (
		WidgetDriver* widgetDriver,
		const Rect& rect,
		uint_t interval = 500
		);

	void
	hide (WidgetDriver* widgetDriver);

protected:
	virtual void timerEvent (QTimerEvent* e);
};

//..............................................................................

} // namespace gui
} // namespace axl
