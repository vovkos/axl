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
#include "axl_gui_QtCaret.h"
#include "axl_gui_Widget.h"

namespace axl {
namespace gui {

//..............................................................................

bool
QtCaret::show(
	WidgetDriver* widgetDriver,
	const Rect& rect,
	uint_t interval
) {
	ASSERT(widgetDriver);

	if (m_widgetDriver && m_isVisible) {
		m_isVisible = false;
		m_widgetDriver->redraw(m_rect);
	}

	m_widgetDriver = widgetDriver;
	m_rect = rect;
	m_isVisible = true;

	widgetDriver->redraw(rect);

	setSingleShot(false);
	start(interval);
	return true;
}

void
QtCaret::hide(WidgetDriver* widgetDriver) {
	if (widgetDriver != m_widgetDriver)
		return;

	if (m_isVisible)
		m_widgetDriver->redraw(m_rect);

	m_widgetDriver = NULL;
	m_isVisible = false;
	stop();
}

void
QtCaret::timerEvent  (QTimerEvent* e) {
	if (!m_widgetDriver)
		return;

	m_isVisible = !m_isVisible;
	m_widgetDriver->redraw(m_rect);
}

//..............................................................................

} // namespace gui
} // namespace axl
