#include "pch.h"
#include "mylogproxy.h"

//.............................................................................

MyLogProxy::MyLogProxy ()
{
	m_listenSocket = INVALID_SOCKET;
	m_connectSocket = INVALID_SOCKET;
}

void MyLogProxy::close ()
{
	if (m_listenSocket != INVALID_SOCKET)
		closesocket (m_listenSocket);

	if (m_connectSocket != INVALID_SOCKET)
			closesocket (m_connectSocket);
	
	m_socketThread.WaitAndClose ();
	m_packetizer.Reset ();
	m_target = NULL;
	m_listenSocket = INVALID_SOCKET;
	m_connectSocket = INVALID_SOCKET;
}

bool MyLogProxy::listen (log::IPeer* target, int port)
{
	int result;

	close ();

	m_listenSocket = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_listenSocket == INVALID_SOCKET)
		return false;

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons ((uint16_t) port);

	result = ::bind (m_listenSocket, (sockaddr*) &addr, sizeof (addr));
	if (result == SOCKET_ERROR)
		return false;

	result = ::listen (m_listenSocket, 0);
	if (result == SOCKET_ERROR)
		return false;

	m_target = target;
	return m_socketThread.Start ();
}

bool MyLogProxy::connect (log::IPeer* target, int ip, int port)
{
	int result;

	close ();

	m_connectSocket = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_connectSocket == INVALID_SOCKET)
		return false;

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl (ip);
	addr.sin_port = htons ((uint16_t) port);

	result = ::connect (m_connectSocket, (sockaddr*) &addr, sizeof (addr));
	if (result == SOCKET_ERROR)
		return false;

	m_target = target;
	return m_socketThread.Start ();
}

void MyLogProxy::SendMsg (const log::TMsg* msg)
{
	ASSERT (m_connectSocket != INVALID_SOCKET);

	uint64_t hdr = m_packetizer.CreateHdr (msg->m_MsgSize);
	::send (m_connectSocket, (char*) &hdr, sizeof (hdr), 0);
	::send (m_connectSocket, (char*) msg, msg->m_MsgSize, 0);
}

void MyLogProxy::onPacket (const void* p, size_t size)
{
	log::TMsg* msg = (log::TMsg*) p;
	ASSERT (size >= sizeof (log::TMsg) && size >= msg->m_MsgSize);	

	m_target->SendMsg (msg);
}

void MyLogProxy::socketThreadProc ()
{
	int result;
	char buffer [256] = { 0 };

	if (m_listenSocket != INVALID_SOCKET)
	{
		sockaddr_in addr;
		int addrSize = sizeof (addr);
		m_connectSocket = ::accept (m_listenSocket, (sockaddr*) &addr, &addrSize);
		if (m_connectSocket == INVALID_SOCKET)
			return;
	}

	for (;;)
	{
		result = ::recv (m_connectSocket, buffer, sizeof (buffer), 0);
		if (result <= 0)
			break;

		m_packetizer.Write (buffer, result);				
	}
}

//.............................................................................
