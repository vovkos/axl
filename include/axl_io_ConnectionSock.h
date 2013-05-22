// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_CONNECTIONSOCK_H

#include "axl_io_Transport.h"
#include "axl_io_SockAddr.h"
#include "axl_exe_WorkerThread.h"

#include "axl_io_win_Sock.h"

namespace axl {
namespace io {

//.............................................................................

class CConnectionSock: public io::ITransport
{
public:
	AXL_OBJ_CLASS_0 (CConnectionSock, io::ITransport)

public:
	typedef exe::CArgObjPtrT <err::CError> COnConnectCompleteArg;
	typedef exe::CArgObjPtrT <err::CError> COnDisconnectCompleteArg;

protected:
	struct TSendRecv: rtl::TListLink
	{
		CConnectionSock* m_pSock;
		OVERLAPPED m_Overlapped;
		ref::CPtrT <exe::IFunction> m_OnComplete;		
	};

public:
	win::CSock m_Sock;

protected:
	ref::CPtrT <exe::CWorkerThread> m_WorkerThread;
	mt::CEvent m_Event;
	handle_t m_hWorkerThreadEvent;
	ref::CPtrT <exe::IFunction> m_OnConnectComplete;
	ref::CPtrT <exe::IFunction> m_OnDisconnectComplete;	
	rtl::CStdListT <TSendRecv> m_SendRecvList;

public:
	CConnectionSock ()
	{
		m_hWorkerThreadEvent = NULL;
	}

	~CConnectionSock ()
	{
		Close ();
	}

	bool
	IsOpen ()
	{
		return m_WorkerThread != NULL;
	}

	bool
	Open (
		ESockProto Protocol,
		const TSockAddr* pAddr
		)
	{
		return Open (Protocol, (ESockAddr) pAddr->m_Kind, pAddr);
	}

	bool
	Open (
		ESockProto Protocol,
		ESockAddr AddrKind
		)
	{
		return Open (Protocol, AddrKind, NULL);
	}

	void
	Close ();

	bool
	GetLocalAddress (TSockAddrU* pAddr)
	{
		SOCKADDR Addr;
		return 
			m_Sock.GetLocalAddress (&Addr) &&
			pAddr->FromWinSockAddr (&Addr);
	}

	bool
	GetPeerAddress (TSockAddrU* pAddr)
	{
		SOCKADDR Addr;
		return 
			m_Sock.GetPeerAddress (&Addr) &&
			pAddr->FromWinSockAddr (&Addr);
	}

	bool 
	Connect (
		const TSockAddr* pAddr,
		uint_t Timeout,
		exe::IFunction* pOnComplete // void OnComplete (err::CError* pError)
		);

	bool 
	Disconnect (
		uint_t Timeout,
		exe::IFunction* pOnComplete // void OnComplete (err::CError* pError)
		);

	bool 
	SyncConnect (
		const TSockAddr* pAddr,
		uint_t Timeout
		);

	bool 
	SyncDisconnect (uint_t Timeout);

	virtual
	bool
	Send (
		const void* p,
		size_t Size,
		exe::IFunction* pOnComplete // void OnComplete (err::CError* pError, size_t ActualSize);
		);

	virtual
	bool
	Recv (
		void* p,
		size_t Size,
		exe::IFunction* pOnComplete // void OnComplete (err::CError* pError, size_t ActualSize);
		);

protected:
	bool
	Open (
		ESockProto Protocol,
		ESockAddr AddKind,
		const TSockAddr* pAddr
		);

	void
	AXL_CDECL
	Close_wt ();

	bool 
	AXL_CDECL
	Connect_wt (
		const TSockAddr* pAddr,
		uint_t Timeout,
		exe::IFunction* pOnComplete
		);

	bool 
	AXL_CDECL
	Disconnect_wt (
		uint_t Timeout,
		exe::IFunction* pOnComplete
		);

	void
	AXL_CDECL
	OnSocketEvent_wt ();

	bool
	AXL_CDECL
	Send_wt (
		const void* p,
		size_t Size,
		exe::IFunction* pOnComplete
		);

	bool
	AXL_CDECL
	Recv_wt (
		void* p,
		size_t Size,
		exe::IFunction* pOnComplete
		);

	static 
	void 
	CALLBACK 
	OnSendRecvComplete_wt (
		dword_t Error, 
		dword_t ActualSize, 
		WSAOVERLAPPED* pOverlapped, 
		dword_t Flags
		)
	{
		TSendRecv* pSendRecv = (TSendRecv*) pOverlapped->hEvent;
		pSendRecv->m_pSock->CompleteSendRecv_wt (pSendRecv, Error, ActualSize);
	}

	void
	CompleteSendRecv_wt (
		TSendRecv* pSendRecv,
		const err::CError& Error,
		size_t ActualSize
		);
};

//.............................................................................

} // namespace sys
} // namespace axl
