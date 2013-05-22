#include "pch.h"
#include "mylogwidget.h"
#include "mylogrepresentor.h"

//.............................................................................

void
CMyLogWidget::OnKeyDown (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetKeyMsg* pCharMsg = (gui::TWidgetKeyMsg*) pMsg;

	switch (pCharMsg->m_Key)
	{
	case '1':
		GetPacketFile ()->Write (EMyLogPacket_DeviceOpened);
		break;

	case '2':
		GetPacketFile ()->Write (EMyLogPacket_DeviceClosed);
		break;

	case '3':
		GetPacketFile ()->Write (EMyLogPacket_Connect);
		break;

	case '4':
		GetPacketFile ()->Write (EMyLogPacket_ConnectComplete);
		break;

	case '5':
		GetPacketFile ()->Write (EMyLogPacket_ConnectError);
		break;

	default:
		GetPacketFile ()->Write (EMyLogPacket_Bin, &pCharMsg->m_Key, 1);
	}

	UpdateLog ();
}

//.............................................................................
