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

class ConnectionSock
{
public:
	typedef exe::ArgObjPtr <err::Error> OnConnectCompleteArg;
	typedef exe::ArgObjPtr <err::Error> OnDisconnectCompleteArg;

protected:
	struct SendRecv: sl::ListLink
	{
		ConnectionSock* m_sock;
		OVERLAPPED m_overlapped;
		exe::Function m_onComplete;		
	};

public:
	win::Sock m_sock;

protected:
	ref::Ptr <exe::Workerhread> m_workerThread;
	mt::Event m_event;
	handle_t m_hWorkerThreadEvent;
	exe::Function m_onConnectComplete;
	exe::Function m_onDisconnectComplete;	
	sl::StdList <SendRecv> m_sendRecvList;

public:
	ConnectionSock ()
	{
		m_hWorkerThreadEvent = NULL;
	}

	~ConnectionSock ()
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
		SockProto protocol,
		const SockAddr* addr
		)
	{
		return open (protocol, (SockAddrKind) addr->m_kind, addr);
	}

	bool
	open (
		SockProto protocol,
		SockAddrKind addrKind
		)
	{
		return open (protocol, addrKind, NULL);
	}

	void
	close ();

	bool
	getLocalAddress (SockAddrU* addr)
	{
		SOCKADDR addr;
		return 
			m_sock.getLocalAddress (&addr) &&
			addr->fromWinSockAddr (&addr);
	}

	bool
	getPeerAddress (SockAddrU* addr)
	{
		SOCKADDR addr;
		return 
			m_sock.getPeerAddress (&addr) &&
			addr->fromWinSockAddr (&addr);
	}

	bool 
	connect (
		const SockAddr* addr,
		uint_t timeout,
		const exe::Function& onComplete // void OnComplete (err::CError* pError)
		);

	bool 
	disconnect (
		uint_t timeout,
		const exe::Function& onComplete // void OnComplete (err::CError* pError)
		);

	bool 
	syncConnect (
		const SockAddr* addr,
		uint_t timeout
		);

	bool 
	syncDisconnect (uint_t timeout);

	bool
	send (
		const void* p,
		size_t size,
		const exe::Function& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
		);

	bool
	recv (
		void* p,
		size_t size,
		const exe::Function& onComplete // void OnComplete (err::CError* pError, size_t ActualSize);
		);

protected:
	bool
	open (
		SockProto protocol,
		SockAddrKind addKind,
		const SockAddr* addr
		);

	void
	AXL_CDECL
	close_wt ();

	bool 
	AXL_CDECL
	connect_wt (
		const SockAddr* addr,
		uint_t timeout,
		const exe::Function& onComplete
		);

	bool 
	AXL_CDECL
	disconnect_wt (
		uint_t timeout,
		const exe::Function& onComplete
		);

	void
	AXL_CDECL
	onSocketEvent_wt ();

	bool
	AXL_CDECL
	send_wt (
		const void* p,
		size_t size,
		const exe::Function& onComplete
		);

	bool
	AXL_CDECL
	recv_wt (
		void* p,
		size_t size,
		const exe::Function& onComplete
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
		SendRecv* sendRecv = (SendRecv*) overlapped->hEvent;
		sendRecv->m_sock->completeSendRecv_wt (sendRecv, error, actualSize);
	}

	void
	completeSendRecv_wt (
		SendRecv* sendRecv,
		const err::Error& error,
		size_t actualSize
		);
};

//.............................................................................

} // namespace sys
} // namespace axl
