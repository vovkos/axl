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
	m_pBaseFont = pEngine->GetDefaultFont ();
	m_Style = 0;
	m_MsgMask = -1;	
	m_BaseTextAttr.m_ForeColor = EStdPalColor_WidgetText;
	m_BaseTextAttr.m_BackColor = EStdPalColor_WidgetBack;
	m_BaseTextAttr.m_FontFlags = 0;
}

//.............................................................................

} // namespace gui
} // namespace axl
