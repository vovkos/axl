#pragma once

#define MYLOGWIDGET_H

//.............................................................................

class CMyLogWidget: public log::CWidget
{
public:
	CMyLogWidget (gui::IEngine* pEngine):
		log::CWidget (pEngine)
	{
	}
		
protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KeyDown, OnKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_CHAIN (log::CWidget)

	void
	OnKeyDown (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);
};

//.............................................................................
