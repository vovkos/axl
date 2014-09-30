#include "pch.h"
#include "axl_io_ListenerSock.h"
#include "axl_io_ConnectionSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
ListenerSock::open (
	SockProtoKind protocol,
	const SockAddr* addr
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

	exe::Function <exe::Arg <ListenerSock*>, exe::Arg <void> > onEvent (
		pvoid_cast (&ListenerSock::onSocketEvent_wt), 
		this
		);

	m_workerThread = exe::getWorkerThread (&m_event, &onEvent, &m_hWorkerThreadEvent);
	if (!m_workerThread)
		return false;

	return true;
}

void
ListenerSock::close ()
{
	if (!isOpen ())
		return;

	m_workerThread->syncSchedule <exe::Arg <ListenerSock*> > (
		pvoid_cast (&ListenerSock::close_wt),
		this
		);
}

bool
ListenerSock::listen (
	size_t backLog,
	const exe::Function& onAccept
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_3 <ListenerSock*, size_t, exe::IFunction*> > (
		pvoid_cast (&ListenerSock::listen_wt),
		this,
		backLog,
		onAccept
		) != 0;
}

void
ListenerSock::close_wt ()
{
	m_sock.close ();
	m_onAccept.clear ();
	m_event.reset ();
	m_workerThread->removeEvent (m_hWorkerThreadEvent);
	m_workerThread = ref::PtrKind_Null;
	m_hWorkerThreadEvent = NULL;
}

bool
ListenerSock::listen_wt (
	size_t backLog,
	const exe::Function& onAccept
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
ListenerSock::onSocketEvent_wt ()
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

	ref::Ptr <ConnectionSock> socket = AXL_REF_NEW (ref::Box <ConnectionSock>);
	socket->m_sock.attach (s);

	SockAddrU addr;
	socket->getPeerAddress (&addr);

	m_onAccept->invoke (0, socket);
}

//.............................................................................

} // namespace io
} // namespace axl
