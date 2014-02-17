#include "pch.h"
#include "axl_io_psx_Socket.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
CSocket::Open (
	int AddressFamily,
	int SockKind,
	int Protocol
	)
{
	Close ();
	m_h = ::socket (AddressFamily, SockKind, Protocol);
	return err::Complete (m_h != -1);
}

int
CSocket::GetError ()
{
	int Error = 0;
	GetOption (SOL_SOCKET, SO_ERROR, &Error, sizeof (int));
	return Error;
}

bool
CSocket::SetBlockingMode (bool IsBlocking)
{
	int Value = IsBlocking;
	int Result = ::ioctl (m_h, FIONBIO, &Value);
	return err::Complete (m_h != -1);
}

bool
CSocket::GetAddress (sockaddr* pAddr)
{
	socklen_t Size = sizeof (sockaddr);
	int Result = ::getsockname (m_h, pAddr, &Size);
	return err::Complete (Result != -1);
}

bool
CSocket::GetPeerAddress (sockaddr* pAddr)
{
	socklen_t Size = sizeof (sockaddr);
	int Result = ::getpeername (m_h, pAddr, &Size);
	return err::Complete (Result != -1);
}

bool
CSocket::Connect (const sockaddr* pAddr)
{
	int Result = ::connect (m_h, pAddr, sizeof (sockaddr));
	if (Result != -1)
		return true;

	if (errno != EWOULDBLOCK)
	{
		err::SetError (errno);
		return false;
	}

	return true;
}

int
CSocket::Accept (sockaddr* pAddr)
{
	socklen_t Size = sizeof (sockaddr);
	int Socket = ::accept (m_h, pAddr, &Size);
	return err::Complete (Socket, -1);
}

size_t
CSocket::RecvFrom (
	void* p,
	size_t Size,
	sockaddr* pAddr
	)
{
	socklen_t SockAddrSize = sizeof (sockaddr);
	int Result = ::recvfrom (m_h, p, Size, 0, pAddr, &SockAddrSize);
	return err::Complete (Result, -1);
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
