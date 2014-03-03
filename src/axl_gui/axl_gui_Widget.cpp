#include "pch.h"
#include "axl_gui_Widget.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

CWidget::CWidget (CEngine* pEngine)
{
	ASSERT (pEngine);

	m_pEngine = pEngine;
	m_pCursor = pEngine->GetStdCursor (EStdCursor_Arrow);
	m_pBaseFont = pEngine->GetStdFont (EStdFont_Monospace);
	m_Style = 0;
	m_MsgMask = -1;	
	m_BaseTextAttr.m_ForeColor = EStdPalColor_WidgetText;
	m_BaseTextAttr.m_BackColor = EStdPalColor_WidgetBack;
	m_BaseTextAttr.m_FontFlags = 0;
	m_CaretSize.m_Width = 2;
	m_CaretSize.m_Height = 16;
	m_IsCaretVisible = false;
}

bool
CWidget::SetCaretVisible (bool IsVisible)
{
	if (m_IsCaretVisible == IsVisible)
		return true;

	m_IsCaretVisible = IsVisible;

	if (IsVisible)
		return m_pEngine->ShowCaret (this, TRect (m_CaretPos, m_CaretSize));

	m_pEngine->HideCaret ();
	return true;
}

bool
CWidget::SetCaretSize (
	uint_t Width,
	uint_t Height
	)
{
	if (m_CaretSize.m_Width == Width && m_CaretSize.m_Height == Height)
		return true; // nothing changed

	m_CaretSize.Setup (Width, Height);
	return m_IsCaretVisible ? m_pEngine->ShowCaret (this, TRect (m_CaretPos, m_CaretSize)) : true;
}

bool
CWidget::SetCaretPos (
	int x, 
	int y
	)
{
	if (m_CaretPos.m_x == x && m_CaretPos.m_y == y)
		return true; // nothing changed

	m_CaretPos.Setup (x, y);
	return m_IsCaretVisible ? m_pEngine->ShowCaret (this, TRect (m_CaretPos, m_CaretSize)) : true;
}

//.............................................................................

} // namespace gui
} // namespace axl
