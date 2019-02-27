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
#include "axl_io_win_Socket.h"

namespace axl {
namespace io {
namespace win {

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
	return complete(m_h != INVALID_SOCKET);
}

int
Socket::getError()
{
	int error;
	getOption(SOL_SOCKET, SO_ERROR, &error, sizeof(int));
	return error;
}

bool
Socket::setBlockingMode(bool isBlocking)
{
	ulong_t value = !isBlocking;
	int result = ::ioctlsocket(m_h, FIONBIO, &value);
	return complete(m_h != -1);
}

size_t
Socket::getIncomingDataSize()
{
	ulong_t value;
	int result = ::ioctlsocket(m_h, FIONREAD, &value);
	if (result == -1)
	{
		err::setLastSystemError();
		return -1;
	}

	return value;
}

bool
Socket::getAddress(SockAddr* addr)
{
	int size = sizeof(SockAddr);
	int result = ::getsockname(m_h, (sockaddr*)addr, &size);
	return complete(result != SOCKET_ERROR);
}

bool
Socket::getPeerAddress(SockAddr* addr)
{
	int size = sizeof(SockAddr);
	int result = ::getpeername(m_h, (sockaddr*)addr, &size);
	return complete(result != SOCKET_ERROR);
}

SOCKET
Socket::accept(SockAddr* addr)
{
	int size = sizeof(SockAddr);
	SOCKET socket = ::accept(
		m_h,
		(sockaddr*)addr,
		addr ? &size : NULL
		);

	return complete(socket, INVALID_SOCKET);
}

size_t
Socket::recvFrom(
	void* p,
	size_t size,
	SockAddr* addr
	)
{
	int addrSize = sizeof(SockAddr);
	int result = ::recvfrom(
		m_h,
		(char*)p,
		(int)size,
		0,
		(sockaddr*)addr,
		addr ? &addrSize : NULL
		);

	return complete(result, SOCKET_ERROR);
}

bool
Socket::completeAsyncRequest(
	int result,
	int pendingResult
	)
{
	if (result != SOCKET_ERROR)
		return true;

	dword_t error = WSAGetLastError();
	if (error != pendingResult)
	{
		err::setError(error);
		return false;
	}

	return true;
}

bool
Socket::wsaOpen(
	int addressFamily,
	int sockKind,
	int protocol,
	dword_t flags
	)
{
	close();
	m_h = ::WSASocket(addressFamily, sockKind, protocol, NULL, 0, flags);
	return complete(m_h != INVALID_SOCKET);
}

SOCKET
Socket::wsaAccept(SockAddr* addr)
{
	int addrSize = sizeof(SockAddr);
	SOCKET s = ::WSAAccept(
		m_h,
		(sockaddr*)addr,
		addr ? &addrSize : NULL,
		NULL,
		0
		);

	return complete(s, INVALID_SOCKET);
}

bool
Socket::wsaSend(
	const void* p,
	dword_t size,
	dword_t* actualSize,
	dword_t flags,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT(isOpen());

	WSABUF buf;
	buf.buf = (char*)p;
	buf.len = size;

	int result = ::WSASend(m_h, &buf, 1, actualSize, 0, overlapped, completionFunc);
	return completeAsyncRequest(result, WSA_IO_PENDING);
}

bool
Socket::wsaRecv(
	void* p,
	dword_t size,
	dword_t* actualSize,
	dword_t* flags,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT(isOpen());

	WSABUF buf;
	buf.buf = (char*)p;
	buf.len = size;

	int result = ::WSARecv(m_h, &buf, 1, actualSize, flags, overlapped, completionFunc);
	return completeAsyncRequest(result, WSA_IO_PENDING);
}

bool
Socket::wsaSendTo(
	const void* p,
	dword_t size,
	dword_t* actualSize,
	dword_t flags,
	const sockaddr* addr,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT(isOpen());

	WSABUF buf;
	buf.buf = (char*)p;
	buf.len = size;

	int result = ::WSASendTo(
		m_h,
		&buf,
		1,
		actualSize,
		flags,
		addr,
		getSockAddrSize(addr),
		overlapped,
		completionFunc
		);

	return completeAsyncRequest(result, WSA_IO_PENDING);
}

bool
Socket::wsaRecvFrom(
	void* p,
	dword_t size,
	dword_t* actualSize,
	dword_t* flags,
	SockAddr* addr,
	socklen_t* addrSize,
	WSAOVERLAPPED* overlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
	)
{
	ASSERT(isOpen());

	WSABUF buf;
	buf.buf = (char*)p;
	buf.len = size;

	int result = ::WSARecvFrom(
		m_h,
		&buf,
		1,
		actualSize,
		flags,
		(sockaddr*)addr,
		addrSize,
		overlapped,
		completionFunc
		);

	return completeAsyncRequest(result, WSA_IO_PENDING);
}

bool
Socket::wsaIoctl(
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
	ASSERT(isOpen());

	int result = ::WSAIoctl(
		m_h,
		ioctlCode,
		(void*)inBuffer,
		inBufferSize,
		outBuffer,
		outBufferSize,
		actualSize,
		overlapped,
		completionFunc
		);

	return completeAsyncRequest(result, WSA_IO_PENDING);
}

bool
Socket::wsaGetOverlappedResult(
	WSAOVERLAPPED* overlapped,
	dword_t* actualSize,
	dword_t* flags
	) const
{
	dword_t unused;
	bool_t result = ::WSAGetOverlappedResult(m_h, overlapped, actualSize, true, flags ? flags : &unused);
	return complete(result);
}

size_t
Socket::wsaGetOverlappedResult(WSAOVERLAPPED* overlapped) const
{
	dword_t actualSize;
	bool result = wsaGetOverlappedResult(overlapped, &actualSize);
	return result ? (size_t)actualSize : -1;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
