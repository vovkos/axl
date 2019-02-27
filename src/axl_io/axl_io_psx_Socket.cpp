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

#include "pch.h"
#include "axl_io_psx_Socket.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

bool
Socket::open(
	int addressFamily,
	int sockKind,
	int protocol
	)
{
	close();
	m_h = ::socket(addressFamily, sockKind, protocol);
	return err::complete(m_h != -1);
}

int
Socket::getError()
{
	int error = 0;
	getOption(SOL_SOCKET, SO_ERROR, &error, sizeof(int));
	return error;
}

bool
Socket::setBlockingMode(bool isBlocking)
{
	int value = !isBlocking;
	int result = ::ioctl(m_h, FIONBIO, &value);
	return err::complete(result != -1);
}

bool
Socket::getAddress(SockAddr* addr)
{
	socklen_t size = sizeof(SockAddr);
	int result = ::getsockname(m_h, (sockaddr*)addr, &size);
	return err::complete(result != -1);
}

bool
Socket::getPeerAddress(SockAddr* addr)
{
	socklen_t size = sizeof(SockAddr);
	int result = ::getpeername(m_h, (sockaddr*)addr, &size);
	return err::complete(result != -1);
}

bool
Socket::connect(const sockaddr* addr)
{
	int result = ::connect(m_h, addr, getSockAddrSize(addr));
	if (result != -1)
		return true;

	if (errno != EINPROGRESS)
	{
		err::setError(errno);
		return false;
	}

	return true;
}

int
Socket::accept(SockAddr* addr)
{
	socklen_t addrSize = sizeof(SockAddr);
	int socket = ::accept(
		m_h,
		(sockaddr*)addr,
		addr ? &addrSize : NULL
		);

	return err::complete(socket, -1);
}

size_t
Socket::recvFrom(
	void* p,
	size_t size,
	SockAddr* addr
	)
{
	socklen_t addrSize = sizeof(SockAddr);
	int result = ::recvfrom(
		m_h,
		p,
		size,
		0,
		(sockaddr*)addr,
		addr ? &addrSize : NULL
		);

	return err::complete(result, -1);
}

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
