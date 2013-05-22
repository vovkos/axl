#include "pch.h"
#include "axl_io_ConnectionSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
CConnectionSock::Open (
	ESockProto Protocol,
	ESockAddr AddrKind,
	const TSockAddr* pAddr
	)
{
	Close ();

	bool Result = m_Sock.Open (AddrKind, SOCK_STREAM, Protocol);
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

	exe::CFunctionT <exe::CArgT <CConnectionSock*>, exe::CArgT <void> > OnEvent (
		pvoid_cast (&CConnectionSock::OnSocketEvent_wt), 
		this
		);

	m_WorkerThread = exe::GetWorkerThread (&m_Event, &OnEvent, &m_hWorkerThreadEvent);
	if (!m_WorkerThread)
		return false;

	return true;
}

void
CConnectionSock::Close ()
{
	if (!IsOpen ())
		return;

	m_WorkerThread->SyncSchedule <exe::CArgT <CConnectionSock*> > (
		pvoid_cast (&CConnectionSock::Close_wt),
		this
		);
}

bool 
CConnectionSock::Connect (
	const TSockAddr* pAddr,
	uint_t Timeout,
	exe::IFunction* pOnComplete
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_4 <
		CConnectionSock*,
		TSockAddr*,
		uint_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::Connect_wt),
		this,
		pAddr,
		Timeout,
		pOnComplete
		) != 0;
}

bool 
CConnectionSock::Disconnect (
	uint_t Timeout,
	exe::IFunction* pOnComplete
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_3 <
		CConnectionSock*,
		uint_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::Disconnect_wt),
		this,
		Timeout,
		pOnComplete
		) != 0;
}

void 
OnSyncConnectDisconnectComplete (
	mt::CEvent* pEvent,
	err::CError* pError,
	const err::CError& Error
	)
{
	if (pError)
		*pError = Error;

	pEvent->Signal ();
}

bool 
CConnectionSock::SyncConnect (
	const TSockAddr* pAddr,
	uint_t Timeout
	)
{
	mt::CEvent Event;
	err::CError Error;
	
	exe::CFunctionT <
		exe::CArgSeqT_2 <mt::CEvent*, err::CError*>,
		COnConnectCompleteArg
		> OnComplete (OnSyncConnectDisconnectComplete, &Event, &Error);

	Connect (pAddr, Timeout, &OnComplete);
	Event.Wait ();

	if (Error)
	{
		err::SetError (Error);
		return false;
	}

	return true;
}

bool 
CConnectionSock::SyncDisconnect (uint_t Timeout)
{
	mt::CEvent Event;
	err::CError Error;

	exe::CFunctionT <
		exe::CArgSeqT_2 <mt::CEvent*, err::CError*>,
		COnDisconnectCompleteArg
		> OnComplete (OnSyncConnectDisconnectComplete, &Event, &Error);

	Disconnect (Timeout, &OnComplete);
	Event.Wait ();

	if (Error)
	{
		err::SetError (Error);
		return false;
	}

	return true;
}

bool
CConnectionSock::Send (
	const void* p,
	size_t Size,
	exe::IFunction* pOnComplete // void OnComplete (err::CError* pError, size_t ActualSize);
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_4 <
		CConnectionSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::Send_wt),
		this,
		p,
		Size,
		pOnComplete
		) != 0;
}

bool
CConnectionSock::Recv (
	void* p,
	size_t Size,
	exe::IFunction* pOnComplete // void OnComplete (err::CError* pError, size_t ActualSize);
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_4 <
		CConnectionSock*,
		void*,
		size_t,
		exe::IFunction*
		> > (
		
		pvoid_cast (&CConnectionSock::Recv_wt),
		this,
		p,
		Size,
		pOnComplete
		) != 0;
}

void
CConnectionSock::Close_wt ()
{
	m_Sock.Close ();
	
	if (m_OnConnectComplete)
		m_OnConnectComplete->Invoke (0, &err::CError (err::EStatus_Cancelled));

	if (m_OnDisconnectComplete)
		m_OnDisconnectComplete->Invoke (0, &err::CError (err::EStatus_Cancelled));

	while (!m_SendRecvList.IsEmpty ())
		::SleepEx (0, true);

	m_OnConnectComplete = ref::EPtr_Null;
	m_OnDisconnectComplete = ref::EPtr_Null;
	m_Event.Reset ();
	m_WorkerThread->RemoveEvent (m_hWorkerThreadEvent);
	m_WorkerThread = ref::EPtr_Null;
	m_hWorkerThreadEvent = NULL;
}

bool 
CConnectionSock::Connect_wt (
	const TSockAddr* pAddr,
	uint_t Timeout,
	exe::IFunction* pOnComplete
	)
{
	SOCKADDR Addr;

	bool Result = 
		pAddr->ToWinSockAddr (&Addr) && 
		m_Sock.Select (m_Event.m_Event, FD_CONNECT) &&
		m_Sock.Connect (&Addr);

	if (!Result)
	{
		pOnComplete->Invoke (0, err::GetError ());
		return false;
	}

	m_OnConnectComplete = ref::GetPtrOrClone (pOnComplete);
	return true;
}

bool 
CConnectionSock::Disconnect_wt (
	uint_t Timeout,
	exe::IFunction* pOnComplete
	)
{
	bool Result = 
		m_Sock.Select (m_Event.m_Event, FD_CLOSE) &&
		m_Sock.Shutdown (SD_BOTH);

	if (!Result)
	{
		pOnComplete->Invoke (0, false, err::GetError ());
		return false;
	}

	m_OnDisconnectComplete = ref::GetPtrOrClone (pOnComplete);
	return true;
}

void
CConnectionSock::OnSocketEvent_wt ()
{
	WSANETWORKEVENTS Events = { 0 };

	int Result = WSAEnumNetworkEvents (m_Sock, NULL, &Events);
	if (Result == SOCKET_ERROR)
		return ;

	if (Events.lNetworkEvents & FD_CONNECT)
	{
		Result = Events.iErrorCode [FD_CONNECT_BIT];

		err::CError e (Result);

		m_OnConnectComplete->Invoke (0, Result ? &e : NULL);

		memset (&e, -1, sizeof (e));

		m_OnConnectComplete = ref::EPtr_Null;
	}

	if (Events.lNetworkEvents & FD_CLOSE)
	{
		Result = Events.iErrorCode [FD_CLOSE_BIT];

		m_OnDisconnectComplete->Invoke (0, Result ? &err::CError (Result) : NULL);
		m_OnDisconnectComplete = ref::EPtr_Null;
	}
}

bool
CConnectionSock::Send_wt (
	const void* p,
	size_t Size,
	exe::IFunction* pOnComplete
	)
{
	TSendRecv* pSend = AXL_MEM_NEW (TSendRecv);
	pSend->m_pSock = this;
	pSend->m_OnComplete = ref::GetPtrOrClone (pOnComplete);
	pSend->m_Overlapped.hEvent = pSend;
	m_SendRecvList.InsertTail (pSend);

	bool Result = m_Sock.Send (p, Size, &pSend->m_Overlapped, OnSendRecvComplete_wt);
	if (!Result)	
		CompleteSendRecv_wt (pSend, err::GetError (), 0); 

	return Result;
}

bool
CConnectionSock::Recv_wt (
	void* p,
	size_t Size,
	exe::IFunction* pOnComplete
	)
{
	TSendRecv* pRecv = AXL_MEM_NEW (TSendRecv);
	pRecv->m_pSock = this;
	pRecv->m_OnComplete = ref::GetPtrOrClone (pOnComplete);
	pRecv->m_Overlapped.hEvent = pRecv;
	m_SendRecvList.InsertTail (pRecv);

	bool Result = m_Sock.Recv (p, Size, &pRecv->m_Overlapped, OnSendRecvComplete_wt);
	if (!Result)	
		CompleteSendRecv_wt (pRecv, err::GetError (), 0); 

	return Result;
}

void
CConnectionSock::CompleteSendRecv_wt (
	TSendRecv* pSendRecv,
	const err::CError& Error,
	size_t ActualSize
	)
{
	pSendRecv->m_OnComplete->Invoke (0, &Error, ActualSize);
	m_SendRecvList.Remove (pSendRecv);
	AXL_MEM_DELETE (pSendRecv);
}

//.............................................................................

} // namespace io
} // namespace axl
