#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

void
CWidget::OnScroll (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetMsgT <int>* pScrollMsg = (gui::TWidgetMsgT <int>*) pMsg;
	
	if (pScrollMsg->m_Param & (1 << gui::EWidgetOrientation_Vertical))
	{
		m_FirstVisibleLine = m_ScrollBarArray [gui::EWidgetOrientation_Vertical].m_Pos;
		FixupFirstVisibleLine ();
	}

	if (pScrollMsg->m_Param & (1 << gui::EWidgetOrientation_Horizontal))
	{
		m_FirstVisibleCol = m_ScrollBarArray [gui::EWidgetOrientation_Horizontal].m_Pos;
		FixupFirstVisibleCol ();
	}

	Redraw ();
	UpdateCaretPos ();
}

bool
CWidget::FixupFirstVisibleLine ()
{
	size_t NewFirstVisibleLine = m_FirstVisibleLine;

	if (m_FirstVisibleLine + m_VisibleLineCount > m_LineCount)
	{
		if (m_LineCount > m_VisibleLineCount)
			NewFirstVisibleLine = m_LineCount - m_VisibleLineCount;
		else
			NewFirstVisibleLine = 0;
	}

	if (NewFirstVisibleLine == m_FirstVisibleLine)
		return false;

	m_FirstVisibleLine = NewFirstVisibleLine;
	return true;
}

bool
CWidget::FixupFirstVisibleCol ()
{
	size_t NewFirstVisibleCol = m_FirstVisibleCol;
	size_t LastCol = m_BaseCol + m_ColCount;

	if (m_FirstVisibleCol + m_VisibleColCount > LastCol)
	{
		if (LastCol > m_VisibleColCount)
			NewFirstVisibleCol = LastCol - m_VisibleColCount;
		else
			NewFirstVisibleCol = 0;
	}

	if (NewFirstVisibleCol == m_FirstVisibleCol)
		return false;

	m_FirstVisibleCol = NewFirstVisibleCol;
	return true;	
}

void 
CWidget::RecalcHScroll ()
{
	gui::TWidgetScrollBar* pScrollBar = &m_ScrollBarArray [gui::EWidgetOrientation_Horizontal];
	pScrollBar->m_End = m_BaseCol + m_ColCount;
	pScrollBar->m_Page = m_VisibleColCount;
	pScrollBar->m_Pos  = m_FirstVisibleCol;

	UpdateScrollBar (gui::EWidgetOrientation_Horizontal);
}

void 
CWidget::RecalcVScroll ()
{
	gui::TWidgetScrollBar* pScrollBar = &m_ScrollBarArray [gui::EWidgetOrientation_Vertical];
	pScrollBar->m_End = m_LineCount;
	pScrollBar->m_Page = m_VisibleLineCount;
	pScrollBar->m_Pos  = m_FirstVisibleLine;

	UpdateScrollBar (gui::EWidgetOrientation_Vertical);
}

size_t
CWidget::ScrollToLine (size_t Line)
{
	gui::TWidgetScrollBar* pScrollBar = &m_ScrollBarArray [gui::EWidgetOrientation_Vertical];
	
	size_t MaxPos = pScrollBar->GetMaxPos ();
	if (Line > MaxPos)
		Line = MaxPos;

	if (m_FirstVisibleLine == Line)
		return Line;

	m_FirstVisibleLine = Line;
	pScrollBar->m_Pos = Line;

	Redraw ();
	UpdateScrollBar (gui::EWidgetOrientation_Vertical);
	UpdateCaretPos ();
	
	return Line;
}

//.............................................................................

} // namespace log {
} // namespace axl {
