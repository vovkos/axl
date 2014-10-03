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

class DgramSock
{
public:
	typedef 
	exe::ArgSeqEx_3 <
		exe::ArgObjPtr <err::Error>, 
		exe::ArgObjPtr <SockAddrU>,
		exe::Arg <size_t> 
		> OnSendRecvCompleteArg;

protected:
	struct SendRecv: rtl::ListLink
	{
		DgramSock* m_sock;
		SOCKADDR m_address;
		int m_addressSize;
		OVERLAPPED m_overlapped;
		exe::Function m_onComplete;		
	};

public:
	win::Sock m_sock;

protected:
	ref::Ptr <exe::Workerhread> m_workerThread;
	rtl::StdList <SendRecv> m_sendRecvList;

public:
	~DgramSock ()
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
	getLocalAddress (SockAddrU* addr)
	{
		SOCKADDR addr;
		return 
			m_sock.getLocalAddress (&addr) &&
			addr->fromWinSockAddr (&addr);
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

	bool 
	sendTo (
		const void* p,
		size_t size,
		const SockAddr* addr,
		const exe::Function& onComplete
		);

	bool 
	recvFrom (
		void* p,
		size_t size,
		const exe::Function& onComplete
		);

	size_t
	syncSendTo (
		const void* p,
		size_t size,
		const SockAddr* addr
		);

	size_t 
	syncRecvFrom (
		void* p,
		size_t size,
		SockAddrU* from
		);

protected:
	bool
	open (
		SockProto protocol,
		SockAddrKind addrKind,
		const SockAddr* addr
		);

	void
	AXL_CDECL
	close_wt ();

	bool 
	AXL_CDECL
	sendTo_wt (
		const void* p,
		size_t size,
		const SockAddr* addr,
		const exe::Function& onComplete
		);

	bool 
	AXL_CDECL
	recvFrom_wt (
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
