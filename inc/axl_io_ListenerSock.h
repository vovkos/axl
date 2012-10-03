// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_LISTENERSOCK_H

#include "axl_io_ConnectionSock.h"

namespace axl {
namespace io {

//.............................................................................

class CListenerSock
{
public:
	typedef exe::CArgT <ref::CPtrT <CConnectionSock> > COnAcceptArg;

public:
	win::CSock m_Sock;

protected:
	ref::CPtrT <exe::CWorkerThread> m_WorkerThread;
	mt::CEvent m_Event;
	handle_t m_hWorkerThreadEvent;
	ref::CPtrT <exe::IFunction> m_OnAccept;

public:
	CListenerSock ()
	{
		m_hWorkerThreadEvent = NULL;
	}

	~CListenerSock ()
	{
		Close ();
	}

	bool
	IsOpen ()
	{
		return m_WorkerThread != NULL;
	}

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
		);

	void
	Close ();

	bool
	Listen (
		size_t BackLog,
		exe::IFunction* pOnAccept // void OnAccept (ref::CPtrT <CConnectionSocket> Socket)
		);

protected:
	void
	AXL_CDECL
	Close_wt ();

	bool
	AXL_CDECL
	Listen_wt (
		size_t BackLog,
		exe::IFunction* pOnAccept
		);

	void
	AXL_CDECL
	OnSocketEvent_wt ();
};

//.............................................................................

} // namespace sys
} // namespace axl
