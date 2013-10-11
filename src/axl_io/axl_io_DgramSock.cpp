#include "pch.h"
#include "axl_io_DgramSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
CDgramSock::Open (
	ESockProto Protocol,
	ESockAddr AddrKind,
	const TSockAddr* pAddr
	)
{
	Close ();

	bool Result = m_Sock.Open (AddrKind, SOCK_DGRAM, Protocol);
	if (!Result)
		return false;

	if (pAddr)
	{
		SOCKADDR Addr;
		
		Result = 
			pAddr->ToWinSockAddr (&Addr) &&
			m_Sock.Bind (&Addr);

		if (!Result)
			return false;
	}

	m_WorkerThread = exe::GetWorkerThread ();
	if (!m_WorkerThread)
		return false;

	return true;
}

void
CDgramSock::Close ()
{
	if (!IsOpen ())
		return;

	m_WorkerThread->SyncSchedule <exe::CArgT <CDgramSock*> > (
		pvoid_cast (&CDgramSock::Close_wt),
		this
		);
}

bool 
CDgramSock::SendTo (
	const void* p,
	size_t Size,
	const TSockAddr* pAddr,
	const exe::CFunction& OnComplete
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_5 <
		CDgramSock*,
		void*,
		size_t,
		TSockAddr*,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CDgramSock::SendTo_wt),
		this,
		p,
		Size,
		pAddr,
		pOnComplete
		) != 0;
}

bool 
CDgramSock::RecvFrom (
	void* p,
	size_t Size,
	const exe::CFunction& OnComplete
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_4 <
		CDgramSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CDgramSock::RecvFrom_wt),
		this,
		p,
		Size,
		pOnComplete
		) != 0;
}

void 
OnSyncSendRecvComplete (
	mt::CEvent* pEvent,
	err::CError* pError,
	TSockAddrU* pAddrFromBuf,
	size_t* pActualSize,
	
	const err::CError& Error,
	const TSockAddrU* pAddrFrom,
	size_t ActualSize
	)
{
	if (pError)
		*pError = Error;
	
	if (pAddrFromBuf)
		*pAddrFromBuf = *pAddrFrom;
	
	*pActualSize = ActualSize;
	pEvent->Signal ();
}

size_t 
CDgramSock::SyncSendTo (
	const void* p,
	size_t Size,
	const TSockAddr* pAddr
	)
{
	mt::CEvent Event;
	err::CError Error;
	size_t ActualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_4 <mt::CEvent*, err::CError*, TSockAddrU*, size_t*>,
		COnSendRecvCompleteArg
		> OnComplete (OnSyncSendRecvComplete, &Event, &Error, NULL, &ActualSize);

	SendTo (p, Size, pAddr, &OnComplete);
	Event.Wait ();

	if (Error)
	{
		err::SetError (Error);
		return -1;
	}

	return ActualSize;
}

size_t
CDgramSock::SyncRecvFrom (
	void* p,
	size_t Size,
	TSockAddrU* pFrom
	)
{
	mt::CEvent Event;
	err::CError Error;
	size_t ActualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_4 <mt::CEvent*, err::CError*, TSockAddrU*, size_t*>,
		COnSendRecvCompleteArg
		> OnComplete (OnSyncSendRecvComplete, &Event, &Error, pFrom, &ActualSize);

	RecvFrom (p, Size, &OnComplete);
	Event.Wait ();

	if (Error)
	{
		err::SetError (Error);
		return -1;
	}

	return ActualSize;
}

void
CDgramSock::Close_wt ()
{
	m_Sock.Close ();
	
	while (!m_SendRecvList.IsEmpty ())
		::SleepEx (0, true);

	m_WorkerThread = ref::EPtr_Null;
}

bool
CDgramSock::SendTo_wt (
	const void* p,
	size_t Size,
	const TSockAddr* pAddr,
	const exe::CFunction& OnComplete
	)
{
	TSendRecv* pSend = AXL_MEM_NEW (TSendRecv);
	pSend->m_pSock = this;
	pSend->m_OnComplete = ref::GetPtrOrClone (pOnComplete);
	pSend->m_Overlapped.hEvent = pSend;
	m_SendRecvList.InsertTail (pSend);

	bool Result = 
		pAddr->ToWinSockAddr (&pSend->m_Address) &&
		m_Sock.SendTo (p, Size, &pSend->m_Address, &pSend->m_Overlapped, OnSendRecvComplete_wt);
	
	if (!Result)
		CompleteSendRecv_wt (pSend, err::GetError (), 0);

	return Result;
}

bool
CDgramSock::RecvFrom_wt (
	void* p,
	size_t Size,
	const exe::CFunction& OnComplete
	)
{
	TSendRecv* pRecv = AXL_MEM_NEW (TSendRecv);
	pRecv->m_pSock = this;
	pRecv->m_OnComplete = ref::GetPtrOrClone (pOnComplete);
	pRecv->m_Overlapped.hEvent = pRecv;
	pRecv->m_AddressSize = sizeof (SOCKADDR);
	m_SendRecvList.InsertTail (pRecv);

	bool Result = m_Sock.RecvFrom (
		p, Size, 
		&pRecv->m_Address, 
		&pRecv->m_AddressSize, 
		&pRecv->m_Overlapped, 
		OnSendRecvComplete_wt
		);

	if (!Result)	
		CompleteSendRecv_wt (pRecv, err::GetError (), 0); 

	return Result;
}

void
CDgramSock::CompleteSendRecv_wt (
	TSendRecv* pSendRecv,
	const err::CError& Error,
	size_t ActualSize
	)
{
	if (Error)
	{
		pSendRecv->m_OnComplete->Invoke (0, &Error, NULL, 0);
	}
	else
	{
		TSockAddrU Address;
		Address.FromWinSockAddr (&pSendRecv->m_Address);
		pSendRecv->m_OnComplete->Invoke (0, &Error, &Address, ActualSize);
	}

	m_SendRecvList.Remove (pSendRecv);
	AXL_MEM_DELETE (pSendRecv);
}

//.............................................................................

} // namespace io
} // namespace axl
