#include "pch.h"
#include "axl_io_DgramSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
DgramSock::open (
	SockProtoKind protocol,
	SockAddrKind addrKind,
	const SockAddr* addr
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
DgramSock::close ()
{
	if (!isOpen ())
		return;

	m_workerThread->syncSchedule <exe::Arg <DgramSock*> > (
		pvoid_cast (&DgramSock::close_wt),
		this
		);
}

bool
DgramSock::sendTo (
	const void* p,
	size_t size,
	const SockAddr* addr,
	const exe::Function& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_5 <
		DgramSock*,
		void*,
		size_t,
		SockAddr*,
		exe::IFunction*
		> > (

		pvoid_cast (&DgramSock::sendTo_wt),
		this,
		p,
		size,
		addr,
		onComplete
		) != 0;
}

bool
DgramSock::recvFrom (
	void* p,
	size_t size,
	const exe::Function& onComplete
	)
{
	ASSERT (isOpen ());

	return m_workerThread->syncSchedule <exe::ArgSeq_4 <
		DgramSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (

		pvoid_cast (&DgramSock::recvFrom_wt),
		this,
		p,
		size,
		onComplete
		) != 0;
}

void
onSyncSendRecvComplete (
	mt::Event* event,
	err::ErrorData* error,
	SockAddrU* addrFromBuf,
	size_t* actualSize,

	const err::Error& error,
	const SockAddrU* addrFrom,
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
DgramSock::syncSendTo (
	const void* p,
	size_t size,
	const SockAddr* addr
	)
{
	mt::Event event;
	err::Error error;
	size_t actualSize;

	exe::Function <
		exe::ArgSeq_4 <mt::Event*, err::Error*, SockAddrU*, size_t*>,
		OnSendRecvCompleteArg
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
DgramSock::syncRecvFrom (
	void* p,
	size_t size,
	SockAddrU* from
	)
{
	mt::Event event;
	err::Error error;
	size_t actualSize;

	exe::Function <
		exe::ArgSeq_4 <mt::Event*, err::Error*, SockAddrU*, size_t*>,
		OnSendRecvCompleteArg
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
DgramSock::close_wt ()
{
	m_sock.close ();

	while (!m_sendRecvList.isEmpty ())
		::SleepEx (0, true);

	m_workerThread = ref::PtrKind_Null;
}

bool
DgramSock::sendTo_wt (
	const void* p,
	size_t size,
	const SockAddr* addr,
	const exe::Function& onComplete
	)
{
	SendRecv* send = AXL_MEM_NEW (SendRecv);
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
DgramSock::recvFrom_wt (
	void* p,
	size_t size,
	const exe::Function& onComplete
	)
{
	SendRecv* recv = AXL_MEM_NEW (SendRecv);
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
DgramSock::completeSendRecv_wt (
	SendRecv* sendRecv,
	const err::Error& error,
	size_t actualSize
	)
{
	if (error)
	{
		sendRecv->m_onComplete->invoke (0, &error, NULL, 0);
	}
	else
	{
		SockAddrU address;
		address.fromWinSockAddr (&sendRecv->m_address);
		sendRecv->m_onComplete->invoke (0, &error, &address, actualSize);
	}

	m_sendRecvList.remove (sendRecv);
	AXL_MEM_DELETE (sendRecv);
}

//.............................................................................

} // namespace io
} // namespace axl
