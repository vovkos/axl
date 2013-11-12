#pragma once

#define MYLOGWIDGET_H

//.............................................................................

class MyLogWidget: public log::CWidget
{
public:
	MyLogWidget (gui::CEngine* pEngine):
		log::CWidget (pEngine)
	{
	}
	
protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KeyDown, OnKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_END_CHAIN (log::CWidget)

	void
	OnKeyDown (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);
};

//.............................................................................
