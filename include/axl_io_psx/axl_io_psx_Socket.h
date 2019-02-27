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

#define _AXL_PSX_SOCKET_H

#include "axl_io_psx_File.h"
#include "axl_io_SockAddr.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class Socket: public File
{
public:
	bool
	open(
		int addressFamily,
		int sockKind,
		int protocol
		);

	bool
	setBlockingMode(bool isBlocking); // blocking mode is set a bit differently

	int
	getError();

	bool
	bind(const sockaddr* addr)
	{
		int result = ::bind(m_h, addr, getSockAddrSize(addr));
		return err::complete(result != -1);
	}

	bool
	getAddress(SockAddr* addr);

	bool
	getPeerAddress(SockAddr* addr);

	bool
	getOption(
		int level,
		int option,
		void* p,
		socklen_t size
		)
	{
		int result = ::getsockopt(m_h, level, option, p, &size);
		return err::complete(result != -1);
	}

	bool
	setOption(
		int level,
		int option,
		const void* p,
		socklen_t size
		)
	{
		int result = ::setsockopt(m_h, level, option, p, size);
		return err::complete(result != -1);
	}

	bool
	listen(size_t backLog)
	{
		int result = ::listen(m_h, (int)backLog);
		return err::complete(result != -1);
	}

	int
	accept(SockAddr* addr = NULL);

	bool
	connect(const sockaddr* addr);

	bool
	shutdown(int mode = SHUT_RDWR)
	{
		int result = ::shutdown(m_h, mode);
		return err::complete(result);
	}

	size_t
	send(
		const void* p,
		size_t size
		)
	{
		int result = ::send(m_h, p, size, 0);
		return err::complete(result, -1);
	}

	size_t
	recv(
		void* p,
		size_t size
		)
	{
		int result = ::recv(m_h, p, size, 0);
		return err::complete(result, -1);
	}

	size_t
	sendTo(
		void* p,
		size_t size,
		const sockaddr* addr
		)
	{
		int result = ::sendto(m_h, p, size, 0, addr, getSockAddrSize(addr));
		return err::complete(result, -1);
	}

	size_t
	recvFrom(
		void* p,
		size_t size,
		SockAddr* addr
		);
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
