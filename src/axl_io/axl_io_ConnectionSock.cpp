#include "pch.h"
#include "axl_io_ConnectionSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
ConnectionSock::open (
	SockProto protocol,
	SockAddrKind addrKind,
	const SockAddr* addr
	)
{
	close ();

	bool result = m_sock.open (addrKind, SOCK_STREAM, protocol);
	if (!result)
		return false;

	if (addr)
	{
		SOCKADDR addr;

		result =
			addr->toWinSockAddr (&addr) &&
			m_sock.bind (&addr);

		if (!result)
			return false;
	}

	exe::Function <exe::Arg <ConnectionSock*>, exe::Arg <void> > onEvent (
		pvoid_cast (&ConnectionSock::onSocketEvent_wt),
		this
		);

	m_workerThread = exe::getWorkerThread (&m_event, &onEvent, &m_hWorkerThreadEvent);
	if (!m_workerThread)
		return false;

	return true;
}

void
ConnectionSock::close ()
{
	if (!isOpen ())
		return;

	m_workerThread->syncSchedule <exe::Arg <ConnectionSock*> > (
		pvoid_cast (&ConnectionSock::close_wt),
		this
		);
}

bool
ConnectionSock::connect (
	const SockAddr* addr,
	uint_t timeout,
	const exe::Function& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_4 <
		ConnectionSock*,
		SockAddr*,
		uint_t,
		exe::IFunction*
		> > (

		pvoid_cast (&ConnectionSock::connect_wt),
		this,
		addr,
		timeout,
		onComplete
		) != 0;
}

bool
ConnectionSock::disconnect (
	uint_t timeout,
	const exe::Function& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_3 <
		ConnectionSock*,
		uint_t,
		exe::IFunction*
		> > (

		pvoid_cast (&ConnectionSock::disconnect_wt),
		this,
		timeout,
		onComplete
		) != 0;
}

void
onSyncConnectDisconnectComplete (
	mt::Event* event,
	err::ErrorData* error,
	const err::Error& error
	)
{
	if (error)
		*error = error;

	event->signal ();
}

bool
ConnectionSock::syncConnect (
	const SockAddr* addr,
	uint_t timeout
	)
{
	mt::Event event;
	err::Error error;

	exe::Function <
		exe::ArgSeq_2 <mt::Event*, err::Error*>,
		OnConnectCompleteArg
		> onComplete (onSyncConnectDisconnectComplete, &event, &error);

	connect (addr, timeout, &onComplete);
	event.wait ();

	if (error)
	{
		err::setError (error);
		return false;
	}

	return true;
}

bool
ConnectionSock::syncDisconnect (uint_t timeout)
{
	mt::Event event;
	err::Error error;

	exe::Function <
		exe::ArgSeq_2 <mt::Event*, err::Error*>,
		OnDisconnectCompleteArg
		> onComplete (onSyncConnectDisconnectComplete, &event, &error);

	disconnect (timeout, &onComplete);
	event.wait ();

	if (error)
	{
		err::setError (error);
		return false;
	}

	return true;
}

bool
ConnectionSock::send (
	const void* p,
	size_t size,
	const exe::Function& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_4 <
		ConnectionSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (

		pvoid_cast (&ConnectionSock::send_wt),
		this,
		p,
		size,
		onComplete
		) != 0;
}

bool
ConnectionSock::recv (
	void* p,
	size_t size,
	const exe::Function& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_4 <
		ConnectionSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (

		pvoid_cast (&ConnectionSock::recv_wt),
		this,
		p,
		size,
		onComplete
		) != 0;
}

void
ConnectionSock::close_wt ()
{
	m_sock.close ();

	if (m_onConnectComplete)
		m_onConnectComplete->invoke (0, &err::Error (err::SystemErrorCode_Cancelled));

	if (m_onDisconnectComplete)
		m_onDisconnectComplete->invoke (0, &err::Error (err::SystemErrorCode_Cancelled));

	while (!m_sendRecvList.isEmpty ())
		::SleepEx (0, true);

	m_onConnectComplete = ref::PtrKind_Null;
	m_onDisconnectComplete = ref::PtrKind_Null;
	m_event.reset ();
	m_workerThread->removeEvent (m_hWorkerThreadEvent);
	m_workerThread = ref::PtrKind_Null;
	m_hWorkerThreadEvent = NULL;
}

bool
ConnectionSock::connect_wt (
	const SockAddr* addr,
	uint_t timeout,
	const exe::Function& onComplete
	)
{
	SOCKADDR addr;

	bool result =
		addr->toWinSockAddr (&addr) &&
		m_sock.select (m_event.m_event, FD_CONNECT) &&
		m_sock.connect (&addr);

	if (!result)
	{
		onComplete->invoke (0, err::getError ());
		return false;
	}

	m_onConnectComplete = ref::getPtrOrClone (onComplete);
	return true;
}

bool
ConnectionSock::disconnect_wt (
	uint_t timeout,
	const exe::Function& onComplete
	)
{
	bool result =
		m_sock.select (m_event.m_event, FD_CLOSE) &&
		m_sock.shutdown (SD_BOTH);

	if (!result)
	{
		onComplete->invoke (0, false, err::getError ());
		return false;
	}

	m_onDisconnectComplete = ref::getPtrOrClone (onComplete);
	return true;
}

void
ConnectionSock::onSocketEvent_wt ()
{
	WSANETWORKEVENTS events = { 0 };

	int result = WSAEnumNetworkEvents (m_sock, NULL, &events);
	if (result == SOCKET_ERROR)
		return ;

	if (events.lNetworkEvents & FD_CONNECT)
	{
		result = events.iErrorCode [FD_CONNECT_BIT];

		err::Error e (result);

		m_onConnectComplete->invoke (0, result ? &e : NULL);

		memset (&e, -1, sizeof (e));

		m_onConnectComplete = ref::PtrKind_Null;
	}

	if (events.lNetworkEvents & FD_CLOSE)
	{
		result = events.iErrorCode [FD_CLOSE_BIT];

		m_onDisconnectComplete->invoke (0, result ? &err::Error (result) : NULL);
		m_onDisconnectComplete = ref::PtrKind_Null;
	}
}

bool
ConnectionSock::send_wt (
	const void* p,
	size_t size,
	const exe::Function& onComplete
	)
{
	SendRecv* send = AXL_MEM_NEW (SendRecv);
	send->m_sock = this;
	send->m_onComplete = ref::getPtrOrClone (onComplete);
	send->m_overlapped.hEvent = send;
	m_sendRecvList.insertTail (send);

	bool result = m_sock.send (p, size, &send->m_overlapped, onSendRecvComplete_wt);
	if (!result)
		completeSendRecv_wt (send, err::getError (), 0);

	return result;
}

bool
ConnectionSock::recv_wt (
	void* p,
	size_t size,
	const exe::Function& onComplete
	)
{
	SendRecv* recv = AXL_MEM_NEW (SendRecv);
	recv->m_sock = this;
	recv->m_onComplete = ref::getPtrOrClone (onComplete);
	recv->m_overlapped.hEvent = recv;
	m_sendRecvList.insertTail (recv);

	bool result = m_sock.recv (p, size, &recv->m_overlapped, onSendRecvComplete_wt);
	if (!result)
		completeSendRecv_wt (recv, err::getError (), 0);

	return result;
}

void
ConnectionSock::completeSendRecv_wt (
	SendRecv* sendRecv,
	const err::Error& error,
	size_t actualSize
	)
{
	sendRecv->m_onComplete->invoke (0, &error, actualSize);
	m_sendRecvList.remove (sendRecv);
	AXL_MEM_DELETE (sendRecv);
}

//.............................................................................

} // namespace io
} // namespace axl
