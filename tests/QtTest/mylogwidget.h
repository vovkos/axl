#pragma once

#define MYLOGWIDGET_H

//.............................................................................

class myLogWidget: public log::Widget
{
public:
	myLogWidget (gui::Engine* engine):
		log::Widget (engine)
	{
	}
	
protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::WidgetMsgKind_KeyDown, onKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_END_CHAIN (log::Widget)

	void
	onKeyDown (
		gui::WidgetMsg* msg,
		bool* isHandled
		);
};

//.............................................................................
