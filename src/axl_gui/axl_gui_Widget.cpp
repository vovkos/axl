#include "pch.h"
#include "axl_gui_Widget.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

Widget::Widget (Engine* engine)
{
	ASSERT (engine);

	m_engine = engine;
	m_cursor = engine->getStdCursor (StdCursorKind_Arrow);
	m_baseFont = engine->getStdFont (StdFontKind_Monospace);
	m_style = 0;
	m_msgMask = -1;	
	m_baseTextAttr.m_foreColor = StdPalColorKind_WidgetText;
	m_baseTextAttr.m_backColor = StdPalColorKind_WidgetBack;
	m_baseTextAttr.m_fontFlags = 0;
	m_caretSize.m_width = 2;
	m_caretSize.m_height = 16;
	m_isCaretVisible = false;
}

bool
Widget::setCaretVisible (bool isVisible)
{
	if (m_isCaretVisible == isVisible)
		return true;

	m_isCaretVisible = isVisible;

	if (isVisible)
		return m_engine->showCaret (this, Rect (m_caretPos, m_caretSize));

	m_engine->hideCaret ();
	return true;
}

bool
Widget::setCaretSize (
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
Widget::setCaretPos (
	int x, 
	int y
	)
{
	if (m_caretPos.m_x == x && m_caretPos.m_y == y)
		return true; // nothing changed

	m_caretPos.setup (x, y);
	return m_isCaretVisible ? m_engine->showCaret (this, Rect (m_caretPos, m_caretSize)) : true;
}

//.............................................................................

} // namespace gui
} // namespace axl
