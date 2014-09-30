// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKET_H

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_io_win_Socket.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_Socket.h"
#endif

namespace axl {
namespace io {

//.............................................................................

enum ESocketShutdown
{
	ESocketShutdown_Recv,
	ESocketShutdown_Send,
	ESocketShutdown_All,
};

//.............................................................................

class CSocket
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	win::CSocket m_socket;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CSocket m_socket;
#endif

public:
	bool
	isOpen ()
	{
		return m_socket.isOpen ();
	}

	bool
	open (
		int addressFamily,
		int sockKind,
		int protocol
		)
	{
		return m_socket.open (addressFamily, sockKind, protocol);
	}

	void
	close ()
	{
		m_socket.close ();
	}

	int
	getError ()
	{
		return m_socket.getError ();
	}

	bool
	setBlockingMode (bool isBlocking)
	{
		return m_socket.setBlockingMode (isBlocking);
	}

	size_t
	getIncomingDataSize ()
	{
		return m_socket.getIncomingDataSize ();
	}

	bool
	bind (const sockaddr* addr)
	{
		return m_socket.bind (addr);
	}

	bool
	getAddress (sockaddr* addr)
	{
		return m_socket.getAddress (addr);
	}

	bool
	getPeerAddress (sockaddr* addr)
	{
		return m_socket.getPeerAddress (addr);
	}

	bool
	getOption (
		int level,
		int option,
		void* p,
		size_t size
		)
	{
		return m_socket.getOption (level, option, p, size);
	}

	bool
	setOption (
		int level,
		int option,
		const void* p,
		size_t size
		)
	{
		return m_socket.setOption (level, option, p, size);
	}

	bool
	listen (size_t backLog)
	{
		return m_socket.listen (backLog);
	}

	bool
	accept (
		CSocket* socket,
		sockaddr* addr = NULL
		);

	bool
	connect (const sockaddr* addr)
	{
		return m_socket.connect (addr);
	}

	bool
	shutdown (ESocketShutdown shutdownKind = ESocketShutdown_All)
	{
		return m_socket.shutdown (shutdownKind);
	}

	size_t
	send (
		const void* p,
		size_t size
		)
	{
		return m_socket.send (p, size);
	}

	size_t
	recv (
		void* p,
		size_t size
		)
	{
		return m_socket.recv (p, size);
	}

	size_t
	sendTo (
		void* p,
		size_t size,
		const sockaddr* addr
		)
	{
		return m_socket.sendTo (p, size, addr);
	}

	size_t
	recvFrom (
		void* p,
		size_t size,
		sockaddr* addr
		)
	{
		return m_socket.recvFrom (p, size, addr);
	}
};

//.............................................................................

} // namespace io {
} // namespace axl {
