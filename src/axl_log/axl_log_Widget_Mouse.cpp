#include "pch.h"
#include "axl_log_Widget.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace log {

//.............................................................................

void
CWidget::OnMouseMove (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetMouseMsg* pMouseMsg = (gui::TWidgetMouseMsg*) pMsg;

	gui::TCursorPos Pos;

	if (m_IsDragging)
	{
		gui::TCursorPos Pos = GetCursorPosFromMousePos (pMouseMsg->m_Point, true);
		SetCursorPos (Pos, true);
		return;
	}

	bool IsHyperlink = GetHyperlinkFromMousePos (pMouseMsg->m_Point, NULL, NULL);
	gui::EStdCursor CursorKind = IsHyperlink ? 
		gui::EStdCursor_Hyperlink :
		gui::EStdCursor_IBeam;

	SetCursor (m_pEngine->GetStdCursor (CursorKind));
}

void
CWidget::OnMouseButtonDown (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetMouseMsg* pMouseMsg = (gui::TWidgetMouseMsg*) pMsg;

	if (pMouseMsg->m_Button == gui::EMouseButton_Right)
	{
		if (!IsSelectionEmpty ())
			return;
	}
	
	CLine* pHyperlinkLine;
	rtl::CString Hyperlink;
	
	bool IsHyperlink = GetHyperlinkFromMousePos (pMouseMsg->m_Point, &pHyperlinkLine, &Hyperlink);

	if (IsHyperlink)
	{
		OnHyperlink (pHyperlinkLine, Hyperlink);
		return;
	}

	bool Shift = (pMouseMsg->m_ModifierKeys & gui::EModifierKey_Shift) != 0;

	gui::TCursorPos Pos = GetCursorPosFromMousePos (pMouseMsg->m_Point, true);
	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);

	m_IsDragging = true;

	SetFocus ();
	SetMouseCapture ();
}

void
CWidget::OnMouseButtonUp (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	m_IsDragging = false;
	ReleaseMouseCapture ();
}

void
CWidget::OnMouseCaptureLost (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	m_IsDragging = false;
}

void 
CWidget::OnMouseWheel (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetMouseWheelMsg* pMouseWheelMsg = (gui::TWidgetMouseWheelMsg*) pMsg;
	Scroll (-pMouseWheelMsg->m_WheelDelta);
}

//.............................................................................

} // namespace log {
} // namespace axl {
