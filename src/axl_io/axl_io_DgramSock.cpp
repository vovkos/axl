#include "pch.h"
#include "axl_io_DgramSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
CDgramSock::open (
	ESockProto protocol,
	ESockAddr addrKind,
	const TSockAddr* addr
	)
{
	close ();

	bool result = m_sock.open (addrKind, SOCK_DGRAM, protocol);
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

	m_workerThread = exe::getWorkerThread ();
	if (!m_workerThread)
		return false;

	return true;
}

void
CDgramSock::close ()
{
	if (!isOpen ())
		return;

	m_workerThread->syncSchedule <exe::CArgT <CDgramSock*> > (
		pvoid_cast (&CDgramSock::close_wt),
		this
		);
}

bool 
CDgramSock::sendTo (
	const void* p,
	size_t size,
	const TSockAddr* addr,
	const exe::CFunction& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_5 <
		CDgramSock*,
		void*,
		size_t,
		TSockAddr*,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CDgramSock::sendTo_wt),
		this,
		p,
		size,
		addr,
		onComplete
		) != 0;
}

bool 
CDgramSock::recvFrom (
	void* p,
	size_t size,
	const exe::CFunction& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::CArgSeqT_4 <
		CDgramSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CDgramSock::recvFrom_wt),
		this,
		p,
		size,
		onComplete
		) != 0;
}

void 
onSyncSendRecvComplete (
	mt::CEvent* event,
	err::CError* error,
	TSockAddrU* addrFromBuf,
	size_t* actualSize,
	
	const err::CError& error,
	const TSockAddrU* addrFrom,
	size_t actualSize
	)
{
	if (error)
		*error = error;
	
	if (addrFromBuf)
		*addrFromBuf = *addrFrom;
	
	*actualSize = actualSize;
	event->signal ();
}

size_t 
CDgramSock::syncSendTo (
	const void* p,
	size_t size,
	const TSockAddr* addr
	)
{
	mt::CEvent event;
	err::CError error;
	size_t actualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_4 <mt::CEvent*, err::CError*, TSockAddrU*, size_t*>,
		COnSendRecvCompleteArg
		> onComplete (onSyncSendRecvComplete, &event, &error, NULL, &actualSize);

	sendTo (p, size, addr, &onComplete);
	event.wait ();

	if (error)
	{
		err::setError (error);
		return -1;
	}

	return actualSize;
}

size_t
CDgramSock::syncRecvFrom (
	void* p,
	size_t size,
	TSockAddrU* from
	)
{
	mt::CEvent event;
	err::CError error;
	size_t actualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_4 <mt::CEvent*, err::CError*, TSockAddrU*, size_t*>,
		COnSendRecvCompleteArg
		> onComplete (onSyncSendRecvComplete, &event, &error, from, &actualSize);

	recvFrom (p, size, &onComplete);
	event.wait ();

	if (error)
	{
		err::setError (error);
		return -1;
	}

	return actualSize;
}

void
CDgramSock::close_wt ()
{
	m_sock.close ();
	
	while (!m_sendRecvList.isEmpty ())
		::SleepEx (0, true);

	m_workerThread = ref::EPtr_Null;
}

bool
CDgramSock::sendTo_wt (
	const void* p,
	size_t size,
	const TSockAddr* addr,
	const exe::CFunction& onComplete
	)
{
	TSendRecv* send = AXL_MEM_NEW (TSendRecv);
	send->m_sock = this;
	send->m_onComplete = ref::getPtrOrClone (onComplete);
	send->m_overlapped.hEvent = send;
	m_sendRecvList.insertTail (send);

	bool result = 
		addr->toWinSockAddr (&send->m_address) &&
		m_sock.sendTo (p, size, &send->m_address, &send->m_overlapped, onSendRecvComplete_wt);
	
	if (!result)
		completeSendRecv_wt (send, err::getError (), 0);

	return result;
}

bool
CDgramSock::recvFrom_wt (
	void* p,
	size_t size,
	const exe::CFunction& onComplete
	)
{
	TSendRecv* recv = AXL_MEM_NEW (TSendRecv);
	recv->m_sock = this;
	recv->m_onComplete = ref::getPtrOrClone (onComplete);
	recv->m_overlapped.hEvent = recv;
	recv->m_addressSize = sizeof (SOCKADDR);
	m_sendRecvList.insertTail (recv);

	bool result = m_sock.recvFrom (
		p, size, 
		&recv->m_address, 
		&recv->m_addressSize, 
		&recv->m_overlapped, 
		onSendRecvComplete_wt
		);

	if (!result)	
		completeSendRecv_wt (recv, err::getError (), 0); 

	return result;
}

void
CDgramSock::completeSendRecv_wt (
	TSendRecv* sendRecv,
	const err::CError& error,
	size_t actualSize
	)
{
	if (error)
	{
		sendRecv->m_onComplete->invoke (0, &error, NULL, 0);
	}
	else
	{
		TSockAddrU address;
		address.fromWinSockAddr (&sendRecv->m_address);
		sendRecv->m_onComplete->invoke (0, &error, &address, actualSize);
	}

	m_sendRecvList.remove (sendRecv);
	AXL_MEM_DELETE (sendRecv);
}

//.............................................................................

} // namespace io
} // namespace axl
