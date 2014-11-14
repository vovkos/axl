#include "pch.h"
#include "axl_gui_Widget.h"
#include "axl_gui_Engine.h"
#include "axl_gui_Font.h"

namespace axl {
namespace gui {

//.............................................................................

WidgetDriver::WidgetDriver (const WidgetConstructParam& param):
	GuiItem (param.m_engine)
{
	m_engineWidget = param.m_engineWidget;
	m_cursor = m_engine->getStdCursor (StdCursorKind_Arrow);
	m_font = m_engine->getStdFont (StdFontKind_Monospace);
	m_msgMap = NULL;	
	m_colorAttr.m_foreColor = StdPalColor_WidgetText;
	m_colorAttr.m_backColor = StdPalColor_WidgetBack;
	m_caretSize.m_width = 2;
	m_caretSize.m_height = m_font->calcTextSize ('|').m_height;
	m_isCaretVisible = false;
}

bool
WidgetDriver::setFont (Font* font)
{
	if (m_font == font)
		return true;

	m_font = font;
	m_caretSize.m_height = m_font->calcTextSize ('|').m_height;
	return true;
}

bool
WidgetDriver::setCaretVisible (bool isVisible)
{
	if (m_isCaretVisible == isVisible)
		return true;

	m_isCaretVisible = isVisible;

	if (isVisible)
		return m_engine->showCaret (this, Rect (m_caretPos, m_caretSize));

	m_engine->hideCaret (this);
	return true;
}

bool
WidgetDriver::setCaretSize (
	uint_t width,
	uint_t height
	)
{
	if (m_caretSize.m_width == width && m_caretSize.m_height == height)
		return true; // nothing changed

	m_caretSize.setup (width, height);
	return m_isCaretVisible ? m_engine->showCaret (this, Rect (m_caretPos, m_caretSize)) : true;
}

bool
WidgetDriver::setCaretPos (
	int x, 
	int y
	)
{
	if (m_caretPos.m_x == x && m_caretPos.m_y == y)
		return true; // nothing changed

	m_caretPos.setup (x, y);
	return m_isCaretVisible ? m_engine->showCaret (this, Rect (m_caretPos, m_caretSize)) : true;
}

bool
WidgetDriver::updateScrollBars (uint_t mask)
{
	bool result = true;

	if (mask & Orientation_Vertical)
		result = updateScrollBar (Orientation_Vertical);

	if (mask & Orientation_Horizontal)
		result = updateScrollBar (Orientation_Horizontal) || result;

	return true;
}

void
WidgetDriver::processMsg (
	const WidgetMsg* msg,
	bool* isHandled
	)
{
	ASSERT (msg->m_msgCode < WidgetMsgCode__Count);

	Widget* widget = AXL_CONTAINING_RECORD (this, Widget, m_widgetDriver);
	if (!m_msgMap)
		m_msgMap = widget->getWidgetMsgMap ();

	bool result = false;

	WidgetMsgMap* msgMap = m_msgMap;
	for (; msgMap; msgMap = msgMap->m_baseMap)
	{
		WidgetMsgProc proc = msgMap->m_msgProcTable [msg->m_msgCode];
		if (!proc)
			break;

		result = true;
		(widget->*proc) (msg, &result);
		if (result)
			break;
	}

	*isHandled = result;
}

//.............................................................................

} // namespace gui
} // namespace axl
