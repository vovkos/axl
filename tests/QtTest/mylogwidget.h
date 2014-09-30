#pragma once

#define MYLOGWIDGET_H

//.............................................................................

class myLogWidget: public log::CWidget
{
public:
	myLogWidget (gui::CEngine* engine):
		log::CWidget (engine)
	{
	}
	
protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KeyDown, onKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_END_CHAIN (log::CWidget)

	void
	onKeyDown (
		gui::TWidgetMsg* msg,
		bool* isHandled
		);
};

//.............................................................................
