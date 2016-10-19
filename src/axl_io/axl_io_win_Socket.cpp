#include "pch.h"
#include "axl_io_win_Socket.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

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
Socket::getAddress (SockAddr* addr)
{
	int size = sizeof (SockAddr);
	int result = ::getsockname (m_h, (sockaddr*) addr, &size);
	return err::complete (result != SOCKET_ERROR);
}

bool
Socket::getPeerAddress (SockAddr* addr)
{
	int size = sizeof (SockAddr);
	int result = ::getpeername (m_h, (sockaddr*) addr, &size);
	return err::complete (result != SOCKET_ERROR);
}

SOCKET
Socket::accept (SockAddr* addr)
{
	int size = sizeof (SockAddr);
	SOCKET socket = ::accept (
		m_h,
		(sockaddr*) addr,
		addr ? &size : NULL
		);

	return err::complete (socket, INVALID_SOCKET);
}

size_t
Socket::recvFrom (
	void* p,
	size_t size,
	SockAddr* addr
	)
{
	int addrSize = sizeof (SockAddr);
	int result = ::recvfrom (
		m_h,
		(char*) p,
		(int) size,
		0,
		(sockaddr*) addr,
		addr ? &addrSize : NULL
		);

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
Socket::wsaAccept (SockAddr* addr)
{
	int addrSize = sizeof (SockAddr);
	SOCKET s = ::WSAAccept (
		m_h,
		(sockaddr*) addr,
		addr ? &addrSize : NULL,
		NULL,
		0
		);

	return err::complete (s, INVALID_SOCKET);
}

bool
Socket::wsaSend (
	const void* p,
	dword_t size,
	dword_t* actualSize,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	int result = ::WSASend (m_h, &buf, 1, actualSize, 0, overlapped, completionFunc);
	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaRecv (
	void* p,
	dword_t size,
	dword_t* actualSize,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	dword_t flags = 0;

	int result = ::WSARecv (m_h, &buf, 1, actualSize, &flags, overlapped, completionFunc);
	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaSendTo (
	const void* p,
	dword_t size,
	dword_t* actualSize,
	const sockaddr* addr,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	int result = ::WSASendTo (
		m_h,
		&buf,
		1,
		actualSize,
		0,
		addr,
		getSockAddrSize (addr),
		overlapped,
		completionFunc
		);

	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaRecvFrom (
	void* p,
	dword_t size,
	dword_t* actualSize,
	SockAddr* addr,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT (isOpen ());

	WSABUF buf;
	buf.buf = (char*) p;
	buf.len = size;

	dword_t flags = 0;
	int addrSize = sizeof (SockAddr);

	int result = ::WSARecvFrom (
		m_h,
		&buf,
		1,
		actualSize,
		&flags,
		(sockaddr*) addr,
		addr ? &addrSize : NULL,
		overlapped,
		completionFunc
		);

	return completeAsyncRequest (result, WSA_IO_PENDING);
}

bool
Socket::wsaIoctl (
	dword_t ioctlCode,
	const void* inBuffer,
	dword_t inBufferSize,
	void* outBuffer,
	dword_t outBufferSize,
	dword_t* actualSize,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT (isOpen ());

	int result = ::WSAIoctl (
		m_h,
		ioctlCode,
		(void*) inBuffer,
		inBufferSize,
		outBuffer,
		outBufferSize,
		actualSize,
		overlapped,
		completionFunc
		);

	return completeAsyncRequest (result, WSA_IO_PENDING);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl

