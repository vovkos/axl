#include "pch.h"
#include "axl_io_win_Socket.h"

namespace axl {
namespace io {
namespace win {

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
	return err::complete (m_h != INVALID_SOCKET);
}

int
Socket::getError ()
{
	int error;
	getOption (SOL_SOCKET, SO_ERROR, &error, sizeof (int));
	return error;
}

bool
Socket::setBlockingMode (bool isBlocking)
{
	ulong_t value = !isBlocking;
	int result = ::ioctlsocket (m_h, FIONBIO, &value);
	return err::complete (m_h != -1);
}

size_t
Socket::getIncomingDataSize ()
{
	ulong_t value;
	int result = ::ioctlsocket (m_h, FIONREAD, &value);
	if (result == -1)
	{
		err::setLastSystemError ();
		return -1;
	}

	return value;
}

bool
Socket::getAddress (sockaddr* addr)
{
	int size = sizeof (sockaddr);
	int result = ::getsockname (m_h, addr, &size);
	return err::complete (result != SOCKET_ERROR);
}

bool
Socket::getPeerAddress (sockaddr* addr)
{
	int size = sizeof (sockaddr);
	int result = ::getpeername (m_h, addr, &size);
	return err::complete (result != SOCKET_ERROR);
}

SOCKET
Socket::accept (sockaddr* addr)
{
	int size = sizeof (sockaddr);
	SOCKET socket = ::accept (m_h, addr, &size);
	return err::complete (socket, INVALID_SOCKET);
}

size_t
Socket::recvFrom (
	void* p,
	size_t size,
	sockaddr* addr
	)
{
	int sockAddrSize = sizeof (sockaddr);
	int result = ::recvfrom (m_h, (char*) p, (int) size, 0, addr, &sockAddrSize);
	return err::complete (result, SOCKET_ERROR);
}

bool
Socket::completeAsyncRequest (
	int result,
	int pendingResult
	)
{
	if (result != SOCKET_ERROR)
		return true;

	dword_t error = WSAGetLastError ();
	if (error != pendingResult)
	{
		err::setError (error);
		return false;
	}

	return true;
}

bool
Socket::wsaOpen (
	int addressFamily,
	int sockKind,
	int protocol,
	dword_t flags
	)
{
	close ();
	m_h = ::WSASocket (addressFamily, sockKind, protocol, NULL, 0, flags);
	return err::complete (m_h != INVALID_SOCKET);
}

SOCKET
Socket::wsaAccept (sockaddr* addr)
{
	int size = sizeof (sockaddr);
	SOCKET s = ::WSAAccept (m_h, addr, addr ? &size : NULL, NULL, 0);
	return err::complete (s, INVALID_SOCKET);
}

bool
Socket::wsaSend (
	const void* p,
	dword_t size,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	int result = WSASend (m_h, &buf, 1, NULL, 0, overlapped, pfOnComplete);
	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaRecv (
	void* p,
	dword_t size,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	dword_t actualSize;
	dword_t flags;

	int result = WSARecv (m_h, &buf, 1, &actualSize, &flags, overlapped, pfOnComplete);
	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaSendTo (
	const void* p,
	dword_t size,
	const sockaddr* addr,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	int result = WSASendTo (m_h, &buf, 1, NULL, 0, addr, sizeof (sockaddr), overlapped, pfOnComplete);
	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaRecvFrom (
	void* p,
	dword_t size,
	sockaddr* addr,
	int* addrSize,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	dword_t actualSize;
	dword_t flags;

	int result = WSARecvFrom (m_h, &buf, 1, &actualSize, &flags, addr, addrSize, overlapped, pfOnComplete);
	return completeAsyncRequest (result, WSA_IO_PENDING);
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

