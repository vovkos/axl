#include "pch.h"
#include "axl_io_ConnectionSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
CConnectionSock::open (
	ESockProto protocol,
	ESockAddr addrKind,
	const TSockAddr* addr
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

	exe::CFunctionT <exe::CArgT <CConnectionSock*>, exe::CArgT <void> > onEvent (
		pvoid_cast (&CConnectionSock::onSocketEvent_wt), 
		this
		);

	m_workerThread = exe::getWorkerThread (&m_event, &onEvent, &m_hWorkerThreadEvent);
	if (!m_workerThread)
		return false;

	return true;
}

void
CConnectionSock::close ()
{
	if (!isOpen ())
		return;

	m_workerThread->syncSchedule <exe::CArgT <CConnectionSock*> > (
		pvoid_cast (&CConnectionSock::close_wt),
		this
		);
}

bool 
CConnectionSock::connect (
	const TSockAddr* addr,
	uint_t timeout,
	const exe::CFunction& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_4 <
		CConnectionSock*,
		TSockAddr*,
		uint_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::connect_wt),
		this,
		addr,
		timeout,
		onComplete
		) != 0;
}

bool 
CConnectionSock::disconnect (
	uint_t timeout,
	const exe::CFunction& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_3 <
		CConnectionSock*,
		uint_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::disconnect_wt),
		this,
		timeout,
		onComplete
		) != 0;
}

void 
onSyncConnectDisconnectComplete (
	mt::CEvent* event,
	err::CError* error,
	const err::CError& error
	)
{
	if (error)
		*error = error;

	event->signal ();
}

bool 
CConnectionSock::syncConnect (
	const TSockAddr* addr,
	uint_t timeout
	)
{
	mt::CEvent event;
	err::CError error;
	
	exe::CFunctionT <
		exe::CArgSeqT_2 <mt::CEvent*, err::CError*>,
		COnConnectCompleteArg
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
CConnectionSock::syncDisconnect (uint_t timeout)
{
	mt::CEvent event;
	err::CError error;

	exe::CFunctionT <
		exe::CArgSeqT_2 <mt::CEvent*, err::CError*>,
		COnDisconnectCompleteArg
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
CConnectionSock::send (
	const void* p,
	size_t size,
	const exe::CFunction& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_4 <
		CConnectionSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::send_wt),
		this,
		p,
		size,
		onComplete
		) != 0;
}

bool
CConnectionSock::recv (
	void* p,
	size_t size,
	const exe::CFunction& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_4 <
		CConnectionSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::recv_wt),
		this,
		p,
		size,
		onComplete
		) != 0;
}

void
CConnectionSock::close_wt ()
{
	m_sock.close ();
	
	if (m_onConnectComplete)
		m_onConnectComplete->invoke (0, &err::CError (err::EStatus_Cancelled));

	if (m_onDisconnectComplete)
		m_onDisconnectComplete->invoke (0, &err::CError (err::EStatus_Cancelled));

	while (!m_sendRecvList.isEmpty ())
		::SleepEx (0, true);

	m_onConnectComplete = ref::EPtr_Null;
	m_onDisconnectComplete = ref::EPtr_Null;
	m_event.reset ();
	m_workerThread->removeEvent (m_hWorkerThreadEvent);
	m_workerThread = ref::EPtr_Null;
	m_hWorkerThreadEvent = NULL;
}

bool 
CConnectionSock::connect_wt (
	const TSockAddr* addr,
	uint_t timeout,
	const exe::CFunction& onComplete
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
CConnectionSock::disconnect_wt (
	uint_t timeout,
	const exe::CFunction& onComplete
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
CConnectionSock::onSocketEvent_wt ()
{
	WSANETWORKEVENTS events = { 0 };

	int result = WSAEnumNetworkEvents (m_sock, NULL, &events);
	if (result == SOCKET_ERROR)
		return ;

	if (events.lNetworkEvents & FD_CONNECT)
	{
		result = events.iErrorCode [FD_CONNECT_BIT];

		err::CError e (result);

		m_onConnectComplete->invoke (0, result ? &e : NULL);

		memset (&e, -1, sizeof (e));

		m_onConnectComplete = ref::EPtr_Null;
	}

	if (events.lNetworkEvents & FD_CLOSE)
	{
		result = events.iErrorCode [FD_CLOSE_BIT];

		m_onDisconnectComplete->invoke (0, result ? &err::CError (result) : NULL);
		m_onDisconnectComplete = ref::EPtr_Null;
	}
}

bool
CConnectionSock::send_wt (
	const void* p,
	size_t size,
	const exe::CFunction& onComplete
	)
{
	TSendRecv* send = AXL_MEM_NEW (TSendRecv);
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
CConnectionSock::recv_wt (
	void* p,
	size_t size,
	const exe::CFunction& onComplete
	)
{
	TSendRecv* recv = AXL_MEM_NEW (TSendRecv);
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
CConnectionSock::completeSendRecv_wt (
	TSendRecv* sendRecv,
	const err::CError& error,
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
