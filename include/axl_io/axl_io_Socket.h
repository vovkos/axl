//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_IO_SOCKET_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_Socket.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_Socket.h"
#endif

#include "axl_io_SockAddr.h"

namespace axl {
namespace io {

//..............................................................................

enum SocketShutdownKind
{
#if (_AXL_OS_WIN)
	SocketShutdownKind_Recv = SD_RECEIVE,
	SocketShutdownKind_Send = SD_SEND,
	SocketShutdownKind_All  = SD_BOTH,
#elif (_AXL_OS_POSIX)
	SocketShutdownKind_Recv = SHUT_RD,
	SocketShutdownKind_Send = SHUT_WR,
	SocketShutdownKind_All  = SHUT_RDWR,
#endif
};

//..............................................................................

class Socket
{
public:
#if (_AXL_OS_WIN)
	win::Socket m_socket;
#elif (_AXL_OS_POSIX)
	psx::Socket m_socket;
#endif

public:
	bool
	isOpen()
	{
		return m_socket.isOpen();
	}

	bool
	open(
		int addressFamily,
		int sockKind,
		int protocol
		)
	{
		return m_socket.open(addressFamily, sockKind, protocol);
	}

	void
	close()
	{
		m_socket.close();
	}

	int
	getError()
	{
		return m_socket.getError();
	}

	bool
	setBlockingMode(bool isBlocking)
	{
		return m_socket.setBlockingMode(isBlocking);
	}

	size_t
	getIncomingDataSize()
	{
		return m_socket.getIncomingDataSize();
	}

	bool
	bind(const sockaddr* addr)
	{
		return m_socket.bind(addr);
	}

	bool
	bind(const SockAddr& addr)
	{
		return m_socket.bind(addr);
	}

	bool
	getAddress(SockAddr* addr)
	{
		return m_socket.getAddress(addr);
	}

	bool
	getPeerAddress(SockAddr* addr)
	{
		return m_socket.getPeerAddress(addr);
	}

	bool
	getOption(
		int level,
		int option,
		void* p,
		size_t size
		)
	{
		return m_socket.getOption(level, option, p, size);
	}

	bool
	setOption(
		int level,
		int option,
		const void* p,
		size_t size
		)
	{
		return m_socket.setOption(level, option, p, size);
	}

	bool
	listen(size_t backLog)
	{
		return m_socket.listen(backLog);
	}

	bool
	accept(
		Socket* socket,
		SockAddr* addr = NULL
		);

	bool
	connect(const sockaddr* addr)
	{
		return m_socket.connect(addr);
	}

	bool
	connect(const SockAddr& addr)
	{
		return m_socket.connect(addr);
	}

	bool
	shutdown(SocketShutdownKind shutdownKind = SocketShutdownKind_All)
	{
		return m_socket.shutdown(shutdownKind);
	}

	size_t
	send(
		const void* p,
		size_t size
		)
	{
		return m_socket.send(p, size);
	}

	size_t
	recv(
		void* p,
		size_t size
		)
	{
		return m_socket.recv(p, size);
	}

	size_t
	sendTo(
		void* p,
		size_t size,
		const sockaddr* addr
		)
	{
		return m_socket.sendTo(p, size, addr);
	}

	size_t
	sendTo(
		void* p,
		size_t size,
		const SockAddr& addr
		)
	{
		return m_socket.sendTo(p, size, addr);
	}

	size_t
	recvFrom(
		void* p,
		size_t size,
		SockAddr* addr
		)
	{
		return m_socket.recvFrom(p, size, addr);
	}
};

//..............................................................................

} // namespace io
} // namespace axl
