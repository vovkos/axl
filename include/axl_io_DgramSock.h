// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_DGRAMSOCK_H

#include "axl_io_SockAddr.h"
#include "axl_exe_WorkerThread.h"

#include "axl_io_win_Sock.h"

namespace axl {
namespace io {

//.............................................................................

class CDgramSock
{
public:
	typedef 
	exe::CArgSeqExT_3 <
		exe::CArgObjPtrT <err::CError>, 
		exe::CArgObjPtrT <TSockAddrU>,
		exe::CArgT <size_t> 
		> COnSendRecvCompleteArg;

protected:
	struct TSendRecv: rtl::TListLink
	{
		CDgramSock* m_sock;
		SOCKADDR m_address;
		int m_addressSize;
		OVERLAPPED m_overlapped;
		exe::CFunction m_onComplete;		
	};

public:
	win::CSock m_sock;

protected:
	ref::CPtrT <exe::CWorkerThread> m_workerThread;
	rtl::CStdListT <TSendRecv> m_sendRecvList;

public:
	~CDgramSock ()
	{
		close ();
	}

	bool
	isOpen ()
	{
		return m_sock.isOpen ();
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

	bool 
	sendTo (
		const void* p,
		size_t size,
		const TSockAddr* addr,
		const exe::CFunction& onComplete
		);

	bool 
	recvFrom (
		void* p,
		size_t size,
		const exe::CFunction& onComplete
		);

	size_t
	syncSendTo (
		const void* p,
		size_t size,
		const TSockAddr* addr
		);

	size_t 
	syncRecvFrom (
		void* p,
		size_t size,
		TSockAddrU* from
		);

protected:
	bool
	open (
		ESockProto protocol,
		ESockAddr addrKind,
		const TSockAddr* addr
		);

	void
	AXL_CDECL
	close_wt ();

	bool 
	AXL_CDECL
	sendTo_wt (
		const void* p,
		size_t size,
		const TSockAddr* addr,
		const exe::CFunction& onComplete
		);

	bool 
	AXL_CDECL
	recvFrom_wt (
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
