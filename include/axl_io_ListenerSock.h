// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_LISTENERSOCK_H

#include "axl_io_ConnectionSock.h"

namespace axl {
namespace io {

//.............................................................................

class ListenerSock
{
public:
	typedef exe::Arg <ref::Ptr <ConnectionSock> > OnAcceptArg;

public:
	win::Sock m_sock;

protected:
	ref::Ptr <exe::Workerhread> m_workerThread;
	mt::Event m_event;
	handle_t m_hWorkerThreadEvent;
	exe::Function m_onAccept;

public:
	ListenerSock ()
	{
		m_hWorkerThreadEvent = NULL;
	}

	~ListenerSock ()
	{
		close ();
	}

	bool
	isOpen ()
	{
		return m_workerThread != NULL;
	}

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
		);

	void
	close ();

	bool
	listen (
		size_t backLog,
		const exe::Function& onAccept // void OnAccept (ref::CPtrT <CConnectionSocket> Socket)
		);

protected:
	void
	AXL_CDECL
	close_wt ();

	bool
	AXL_CDECL
	listen_wt (
		size_t backLog,
		const exe::Function& onAccept
		);

	void
	AXL_CDECL
	onSocketEvent_wt ();
};

//.............................................................................

} // namespace sys
} // namespace axl
