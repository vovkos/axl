#include "pch.h"
#include "axl_gui_Widget.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

CWidget::CWidget (CEngine* engine)
{
	ASSERT (engine);

	m_engine = engine;
	m_cursor = engine->getStdCursor (EStdCursor_Arrow);
	m_baseFont = engine->getStdFont (EStdFont_Monospace);
	m_style = 0;
	m_msgMask = -1;	
	m_baseTextAttr.m_foreColor = EStdPalColor_WidgetText;
	m_baseTextAttr.m_backColor = EStdPalColor_WidgetBack;
	m_baseTextAttr.m_fontFlags = 0;
	m_caretSize.m_width = 2;
	m_caretSize.m_height = 16;
	m_isCaretVisible = false;
}

bool
CWidget::setCaretVisible (bool isVisible)
{
	if (m_isCaretVisible == isVisible)
		return true;

	m_isCaretVisible = isVisible;

	if (isVisible)
		return m_engine->showCaret (this, TRect (m_caretPos, m_caretSize));

	m_engine->hideCaret ();
	return true;
}

bool
CWidget::setCaretSize (
	uint_t width,
	uint_t height
	)
{
	if (m_caretSize.m_width == width && m_caretSize.m_height == height)
		return true; // nothing changed

	m_caretSize.setup (width, height);
	return m_isCaretVisible ? m_engine->showCaret (this, TRect (m_caretPos, m_caretSize)) : true;
}

bool
CWidget::setCaretPos (
	int x, 
	int y
	)
{
	if (m_caretPos.m_x == x && m_caretPos.m_y == y)
		return true; // nothing changed

	m_caretPos.setup (x, y);
	return m_isCaretVisible ? m_engine->showCaret (this, TRect (m_caretPos, m_caretSize)) : true;
}

//.............................................................................

} // namespace gui
} // namespace axl
