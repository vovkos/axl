#include "pch.h"
#include "axl_io_ListenerSock.h"
#include "axl_io_ConnectionSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
CListenerSock::open (
	ESockProto protocol,
	const TSockAddr* addr
	)
{
	close ();

	SOCKADDR addr;

	bool result = 
		addr->toWinSockAddr (&addr) &&
		m_sock.open (addr->m_kind, SOCK_STREAM, protocol) &&
		m_sock.bind (&addr);

	if (!result)
		return false;

	exe::CFunctionT <exe::CArgT <CListenerSock*>, exe::CArgT <void> > onEvent (
		pvoid_cast (&CListenerSock::onSocketEvent_wt), 
		this
		);

	m_workerThread = exe::getWorkerThread (&m_event, &onEvent, &m_hWorkerThreadEvent);
	if (!m_workerThread)
		return false;

	return true;
}

void
CListenerSock::close ()
{
	if (!isOpen ())
		return;

	m_workerThread->syncSchedule <exe::CArgT <CListenerSock*> > (
		pvoid_cast (&CListenerSock::close_wt),
		this
		);
}

bool
CListenerSock::listen (
	size_t backLog,
	const exe::CFunction& onAccept
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_3 <CListenerSock*, size_t, exe::IFunction*> > (
		pvoid_cast (&CListenerSock::listen_wt),
		this,
		backLog,
		onAccept
		) != 0;
}

void
CListenerSock::close_wt ()
{
	m_sock.close ();
	m_onAccept.clear ();
	m_event.reset ();
	m_workerThread->removeEvent (m_hWorkerThreadEvent);
	m_workerThread = ref::EPtr_Null;
	m_hWorkerThreadEvent = NULL;
}

bool
CListenerSock::listen_wt (
	size_t backLog,
	const exe::CFunction& onAccept
	)
{
	bool result = 
		m_sock.select (m_event.m_event, FD_ACCEPT) &&
		m_sock.listen (backLog);	

	if (!result)
		return false;

	m_onAccept = ref::getPtrOrClone (onAccept);
	return true;
}

void
CListenerSock::onSocketEvent_wt ()
{
	WSANETWORKEVENTS events = { 0 };

	int result = WSAEnumNetworkEvents (m_sock, NULL, &events);
	if (result == SOCKET_ERROR)
		return ;

	if (!(events.lNetworkEvents & FD_ACCEPT))
		return;

	result = events.iErrorCode [FD_ACCEPT_BIT];
	if (result != ERROR_SUCCESS)
		return;

	SOCKET s = m_sock.accept ();
	if (s == INVALID_SOCKET)
		return;

	ref::CPtrT <CConnectionSock> socket = AXL_REF_NEW (ref::CBoxT <CConnectionSock>);
	socket->m_sock.attach (s);

	TSockAddrU addr;
	socket->getPeerAddress (&addr);

	m_onAccept->invoke (0, socket);
}

//.............................................................................

} // namespace io
} // namespace axl
