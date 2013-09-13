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
		m_pServer->WritePacket (EMyLogPacket_DeviceOpened);
		break;

	case '2':
		m_pServer->WritePacket (EMyLogPacket_DeviceClosed);
		break;

	case '3':
		m_pServer->WritePacket (EMyLogPacket_Connect);
		break;

	case '4':
		m_pServer->WritePacket (EMyLogPacket_ConnectComplete);
		break;

	case '5':
		m_pServer->WritePacket (EMyLogPacket_ConnectError);
		break;

	default:
		m_pServer->WritePacket (EMyLogPacket_Bin, &pCharMsg->m_Key, 1);
	}
}

//.............................................................................
