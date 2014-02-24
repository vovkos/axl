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
		CDgramSock* m_pSock;
		SOCKADDR m_Address;
		int m_AddressSize;
		OVERLAPPED m_Overlapped;
		exe::CFunction m_OnComplete;		
	};

public:
	win::CSock m_Sock;

protected:
	ref::CPtrT <exe::CWorkerThread> m_WorkerThread;
	rtl::CStdListT <TSendRecv> m_SendRecvList;

public:
	~CDgramSock ()
	{
		Close ();
	}

	bool
	IsOpen ()
	{
		return m_Sock.IsOpen ();
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

	bool 
	SendTo (
		const void* p,
		size_t Size,
		const TSockAddr* pAddr,
		const exe::CFunction& OnComplete
		);

	bool 
	RecvFrom (
		void* p,
		size_t Size,
		const exe::CFunction& OnComplete
		);

	size_t
	SyncSendTo (
		const void* p,
		size_t Size,
		const TSockAddr* pAddr
		);

	size_t 
	SyncRecvFrom (
		void* p,
		size_t Size,
		TSockAddrU* pFrom
		);

protected:
	bool
	Open (
		ESockProto Protocol,
		ESockAddr AddrKind,
		const TSockAddr* pAddr
		);

	void
	AXL_CDECL
	Close_wt ();

	bool 
	AXL_CDECL
	SendTo_wt (
		const void* p,
		size_t Size,
		const TSockAddr* pAddr,
		const exe::CFunction& OnComplete
		);

	bool 
	AXL_CDECL
	RecvFrom_wt (
		void* p,
		size_t Size,
		const exe::CFunction& OnComplete
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
