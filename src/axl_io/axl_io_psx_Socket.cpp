#include <asm-generic/errno.h>

#include "pch.h"
#include "axl_io_psx_Socket.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
Socket::open (
	int addressFamily,
	int sockKind,
	int protocol
	)
{
	close ();
	m_h = ::socket (addressFamily, sockKind, protocol);
	return err::complete (m_h != -1);
}

int
Socket::getError ()
{
	int error = 0;
	getOption (SOL_SOCKET, SO_ERROR, &error, sizeof (int));
	return error;
}

bool
Socket::setBlockingMode (bool isBlocking)
{
	int value = !isBlocking;
	int result = ::ioctl (m_h, FIONBIO, &value);
	return err::complete (m_h != -1);
}

bool
Socket::getAddress (sockaddr* addr)
{
	socklen_t size = sizeof (sockaddr);
	int result = ::getsockname (m_h, addr, &size);
	return err::complete (result != -1);
}

bool
Socket::getPeerAddress (sockaddr* addr)
{
	socklen_t size = sizeof (sockaddr);
	int result = ::getpeername (m_h, addr, &size);
	return err::complete (result != -1);
}

bool
Socket::connect (const sockaddr* addr)
{
	int result = ::connect (m_h, addr, sizeof (sockaddr));
	if (result != -1)
		return true;

	if (errno != EINPROGRESS)
	{
		err::setError (errno);
		return false;
	}

	return true;
}

int
Socket::accept (sockaddr* addr)
{
	socklen_t size = sizeof (sockaddr);
	int socket = ::accept (m_h, addr, &size);
	return err::complete (socket, -1);
}

size_t
Socket::recvFrom (
	void* p,
	size_t size,
	sockaddr* addr
	)
{
	socklen_t sockAddrSize = sizeof (sockaddr);
	int result = ::recvfrom (m_h, p, size, 0, addr, &sockAddrSize);
	return err::complete (result, -1);
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
