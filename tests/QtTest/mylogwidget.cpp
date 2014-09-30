#include "pch.h"
#include "mylogwidget.h"
#include "mylogrepresenter.h"

//.............................................................................

void
myLogWidget::onKeyDown (
	gui::WidgetMsg* msg,
	bool* isHandled
	)
{
	char data [] = "for 'x' 123 while ";

	gui::WidgetKeyMsg* charMsg = (gui::WidgetKeyMsg*) msg;

	switch (charMsg->m_key)
	{
	case '1':
		m_serverPeer->writePacket (MyLogKind_DeviceOpened);
		break;

	case '2':
		m_serverPeer->writePacket (MyLogKind_DeviceClosed);
		break;

	case '3':
		m_serverPeer->writePacket (MyLogKind_Connect);
		break;

	case '4':
		m_serverPeer->writePacket (MyLogKind_ConnectComplete);
		break;

	case '5':
		m_serverPeer->writePacket (MyLogKind_ConnectError);
		break;

	case '0':
		m_serverPeer->writePacket (MyLogKind_Bin, data, sizeof (data));
		break;

	default:
		if (charMsg->m_key >= 'A' && charMsg->m_key <= 'Z')
		{
			uint_t char = 
				(charMsg->m_modifierKeys & gui::ModifierKeyKind_Ctrl) ? charMsg->m_key - 'A' + 1 :
				(charMsg->m_modifierKeys & gui::ModifierKeyKind_Shift) ? charMsg->m_key : 
				tolower (charMsg->m_key);

			m_serverPeer->writePacket (MyLogKind_Bin, &char, 1);
		}
		else if (isprint (charMsg->m_key))
		{
			m_serverPeer->writePacket (MyLogKind_Bin, &charMsg->m_key, 1);
		}
		else
		{
			*isHandled = false;
		}
	}
}

//.............................................................................

