// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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

class CConnectionSock
{
public:
	typedef exe::CArgObjPtrT <err::CError> COnConnectCompleteArg;
	typedef exe::CArgObjPtrT <err::CError> COnDisconnectCompleteArg;

protected:
	struct TSendRecv: rtl::TListLink
	{
		CConnectionSock* m_sock;
		OVERLAPPED m_overlapped;
		exe::CFunction m_onComplete;		
	};

public:
	win::CSock m_sock;

protected:
	ref::CPtrT <exe::CWorkerThread> m_workerThread;
	mt::CEvent m_event;
	handle_t m_hWorkerThreadEvent;
	exe::CFunction m_onConnectComplete;
	exe::CFunction m_onDisconnectComplete;	
	rtl::CStdListT <TSendRecv> m_sendRecvList;

public:
	CConnectionSock ()
	{
		m_hWorkerThreadEvent = NULL;
	}

	~CConnectionSock ()
	{
		close ();
	}

	bool
	isOpen ()
	{
		return m_workerThread != NULL;
	}

	bool
	open (
		ESockProto protocol,
		const TSockAddr* addr
		)
	{
		return open (protocol, (ESockAddr) addr->m_kind, addr);
	}

	bool
	open (
		ESockProto protocol,
		ESockAddr addrKind
		)
	{
		return open (protocol, addrKind, NULL);
	}

	void
	close ();

	bool
	getLocalAddress (TSockAddrU* addr)
	{
		SOCKADDR addr;
		return 
			m_sock.getLocalAddress (&addr) &&
			addr->fromWinSockAddr (&addr);
	}

	bool
	getPeerAddress (TSockAddrU* addr)
	{
		SOCKADDR addr;
		return 
			m_sock.getPeerAddress (&addr) &&
			addr->fromWinSockAddr (&addr);
	}

	bool 
	connect (
		const TSockAddr* addr,
		uint_t timeout,
		const exe::CFunction& onComplete // void OnComplete (err::CError* pError)
		);

	bool 
	disconnect (
		uint_t timeout,
		const exe::CFunction& onComplete // void OnComplete (err::CError* pError)
		);

	bool 
	syncConnect (
		const TSockAddr* addr,
		uint_t timeout
		);

	bool 
	syncDisconnect (uint_t timeout);

	bool
	send (
		const void* p,
		size_t size,
		const exe::CFunction& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
		);

	bool
	recv (
		void* p,
		size_t size,
		const exe::CFunction& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
		);

protected:
	bool
	open (
		ESockProto protocol,
		ESockAddr addKind,
		const TSockAddr* addr
		);

	void
	AXL_CDECL
	close_wt ();

	bool 
	AXL_CDECL
	connect_wt (
		const TSockAddr* addr,
		uint_t timeout,
		const exe::CFunction& onComplete
		);

	bool 
	AXL_CDECL
	disconnect_wt (
		uint_t timeout,
		const exe::CFunction& onComplete
		);

	void
	AXL_CDECL
	onSocketEvent_wt ();

	bool
	AXL_CDECL
	send_wt (
		const void* p,
		size_t size,
		const exe::CFunction& onComplete
		);

	bool
	AXL_CDECL
	recv_wt (
		void* p,
		size_t size,
		const exe::CFunction& onComplete
		);

	static 
	void 
	CALLBACK 
	onSendRecvComplete_wt (
		dword_t error, 
		dword_t actualSize, 
		WSAOVERLAPPED* overlapped, 
		dword_t flags
		)
	{
		TSendRecv* sendRecv = (TSendRecv*) overlapped->hEvent;
		sendRecv->m_sock->completeSendRecv_wt (sendRecv, error, actualSize);
	}

	void
	completeSendRecv_wt (
		TSendRecv* sendRecv,
		const err::CError& error,
		size_t actualSize
		);
};

//.............................................................................

} // namespace sys
} // namespace axl
