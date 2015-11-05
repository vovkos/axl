// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SOCKET_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"
#include "axl_io_SockAddr.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class CloseSocket
{
public:
	void
	operator () (SOCKET socket)
	{
		::closesocket (socket);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Socket: public sl::Handle <SOCKET, CloseSocket, sl::MinusOne <SOCKET> >
{
public:
	bool
	open (
		int addressFamily,
		int sockKind,
		int protocol
		);

	int
	getError ();

	bool
	setBlockingMode (bool isBlocking);

	size_t
	getIncomingDataSize ();

	bool
	bind (const sockaddr* addr)
	{
		int result = ::bind (m_h, addr, getSockAddrSize (addr));
		return err::complete (result != SOCKET_ERROR);
	}

	bool
	getAddress (SockAddr* addr);

	bool
	getPeerAddress (SockAddr* addr);

	bool
	getOption (
		int level,
		int option,
		void* p,
		size_t size
		)
	{
		int result = ::getsockopt (m_h, level, option, (char*) p, (int*) &size);
		return err::complete (result != SOCKET_ERROR);
	}

	bool
	setOption (
		int level,
		int option,
		const void* p,
		size_t size
		)
	{
		int result = ::setsockopt (m_h, level, option, (char*) p, size);
		return err::complete (result != SOCKET_ERROR);
	}

	bool
	listen (size_t backLog)
	{
		int result = ::listen (m_h, (int) backLog);
		return err::complete (result != SOCKET_ERROR);
	}

	SOCKET
	accept (SockAddr* addr = NULL);

	bool
	connect (const sockaddr* addr)
	{
		int result = ::connect (m_h, addr, getSockAddrSize (addr));
		return completeAsyncRequest (result, WSAEWOULDBLOCK);
	}

	bool
	shutdown (int mode = SD_BOTH)
	{
		int result = ::shutdown (m_h, mode);
		return err::complete (result);
	}

	size_t
	send (
		const void* p,
		size_t size
		)
	{
		int result = ::send (m_h, (const char*) p, (int) size, 0);
		return err::complete (result, SOCKET_ERROR);
	}

	size_t
	recv (
		void* p,
		size_t size
		)
	{
		int result = ::recv (m_h, (char*) p, (int) size, 0);
		return err::complete (result, SOCKET_ERROR);
	}

	size_t
	sendTo (
		void* p,
		size_t size,
		const sockaddr* addr
		)
	{
		int result = ::sendto (m_h, (char*) p, (int) size, 0, addr, getSockAddrSize (addr));
		return err::complete (result, SOCKET_ERROR);
	}

	size_t
	recvFrom (
		void* p,
		size_t size,
		SockAddr* addr
		);

public:
	bool
	wsaOpen (
		int addressFamily,
		int sockKind,
		int protocol,
		dword_t flags = WSA_FLAG_OVERLAPPED
		);

	bool
	wsaEventSelect (
		HANDLE hEvent,
		int mask
		)
	{
		int result = ::WSAEventSelect (m_h, hEvent, mask);
		return err::complete (result != SOCKET_ERROR);
	}

	bool
	wsaEnumEvents (
		WSANETWORKEVENTS* events,
		HANDLE hEvent = NULL
		)
	{
		int result = ::WSAEnumNetworkEvents (m_h, hEvent, events);
		return err::complete (result != SOCKET_ERROR);
	}

	bool
	wsaConnect (const sockaddr* addr)
	{
		int result = ::WSAConnect (m_h, addr, getSockAddrSize (addr), NULL, NULL, NULL, NULL);
		return completeAsyncRequest (result, WSAEWOULDBLOCK);
	}

	SOCKET
	wsaAccept (SockAddr* addr);

	bool
	wsaSend (
		const void* p,
		dword_t size,
		dword_t* actualSize,
		WSAOVERLAPPED* overlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
		);

	bool
	wsaRecv (
		void* p,
		dword_t size,
		dword_t* actualSize,
		WSAOVERLAPPED* overlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
		);

	bool
	wsaSendTo (
		const void* p,
		dword_t size,
		dword_t* actualSize,
		const sockaddr* addr,
		WSAOVERLAPPED* overlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
		);

	bool
	wsaRecvFrom (
		void* p,
		dword_t size,
		dword_t* actualSize,
		SockAddr* addr,
		WSAOVERLAPPED* overlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
		);

	bool
	wsaIoctl (
		dword_t ioctlCode,
		const void* inBuffer,
		dword_t inBufferSize,
		void* outBuffer,
		dword_t outBufferSize,
		dword_t* actualSize,
		WSAOVERLAPPED* overlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE completionFunc
		);

protected:
	static
	bool
	completeAsyncRequest (
		int result,
		int pendingResult
		);
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
