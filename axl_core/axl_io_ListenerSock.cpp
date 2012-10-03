#include "stdafx.h"
#include "axl_io_ListenerSock.h"
#include "axl_io_ConnectionSock.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace io {

//.............................................................................

bool
CListenerSock::Open (
	ESockProto Protocol,
	const TSockAddr* pAddr
	)
{
	Close ();

	SOCKADDR Addr;

	bool Result = 
		pAddr->ToWinSockAddr (&Addr) &&
		m_Sock.Open (pAddr->m_Kind, SOCK_STREAM, Protocol) &&
		m_Sock.Bind (&Addr);

	if (!Result)
		return false;

	exe::CFunctionT <exe::CArgT <CListenerSock*>, exe::CArgT <void> > OnEvent (
		pvoid_cast (&CListenerSock::OnSocketEvent_wt), 
		this
		);

	m_WorkerThread = exe::GetWorkerThread (&m_Event, &OnEvent, &m_hWorkerThreadEvent);
	if (!m_WorkerThread)
		return false;

	return true;
}

void
CListenerSock::Close ()
{
	if (!IsOpen ())
		return;

	m_WorkerThread->SyncSchedule <exe::CArgT <CListenerSock*> > (
		pvoid_cast (&CListenerSock::Close_wt),
		this
		);
}

bool
CListenerSock::Listen (
	size_t BackLog,
	exe::IFunction* pOnAccept
	)
{
	ASSERT (IsOpen ());

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_3 <CListenerSock*, size_t, exe::IFunction*> > (
		pvoid_cast (&CListenerSock::Listen_wt),
		this,
		BackLog,
		pOnAccept
		) != 0;
}

void
CListenerSock::Close_wt ()
{
	m_Sock.Close ();
	m_OnAccept = NULL;
	m_Event.Reset ();
	m_WorkerThread->RemoveEvent (m_hWorkerThreadEvent);
	m_WorkerThread = NULL;
	m_hWorkerThreadEvent = NULL;
}

bool
CListenerSock::Listen_wt (
	size_t BackLog,
	exe::IFunction* pOnAccept
	)
{
	bool Result = 
		m_Sock.Select (m_Event, FD_ACCEPT) &&
		m_Sock.Listen (BackLog);	

	if (!Result)
		return false;

	m_OnAccept = ref::GetPtrOrClone (pOnAccept);
	return true;
}

void
CListenerSock::OnSocketEvent_wt ()
{
	WSANETWORKEVENTS Events = { 0 };

	int Result = WSAEnumNetworkEvents (m_Sock, NULL, &Events);
	if (Result == SOCKET_ERROR)
		return ;

	if (!(Events.lNetworkEvents & FD_ACCEPT))
		return;

	Result = Events.iErrorCode [FD_ACCEPT_BIT];
	if (Result != ERROR_SUCCESS)
		return;

	SOCKET s = m_Sock.Accept ();
	if (s == INVALID_SOCKET)
		return;

	ref::CPtrT <CConnectionSock> Socket = AXL_REF_NEW (ref::CBoxObjT <CConnectionSock>);
	Socket->m_Sock.Attach (s);

	TSockAddrU Addr;
	Socket->GetPeerAddress (&Addr);

	m_OnAccept->Invoke (0, Socket);
}

//.............................................................................

} // namespace io
} // namespace axl
