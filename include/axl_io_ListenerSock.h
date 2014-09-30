// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	win::CSock m_sock;

protected:
	ref::CPtrT <exe::CWorkerThread> m_workerThread;
	mt::CEvent m_event;
	handle_t m_hWorkerThreadEvent;
	exe::CFunction m_onAccept;

public:
	CListenerSock ()
	{
		m_hWorkerThreadEvent = NULL;
	}

	~CListenerSock ()
	{
		close ();
	}

	bool
	isOpen ()
	{
		return m_workerThread != NULL;
	}

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
		);

	void
	close ();

	bool
	listen (
		size_t backLog,
		const exe::CFunction& onAccept // void OnAccept (ref::CPtrT <CConnectionSocket> Socket)
		);

protected:
	void
	AXL_CDECL
	close_wt ();

	bool
	AXL_CDECL
	listen_wt (
		size_t backLog,
		const exe::CFunction& onAccept
		);

	void
	AXL_CDECL
	onSocketEvent_wt ();
};

//.............................................................................

} // namespace sys
} // namespace axl
