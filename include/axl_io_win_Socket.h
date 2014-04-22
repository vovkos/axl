// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SOCKET_H

#include "axl_rtl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class CCloseSocket
{
public:
	void
	operator () (SOCKET Socket)
	{
		::closesocket (Socket);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSocket: public rtl::CHandleT <SOCKET, CCloseSocket, rtl::CMinusOneT <SOCKET> >
{
public:
	bool
	Open (
		int AddressFamily,
		int SockKind,
		int Protocol
		);

	int
	GetError ();

	bool
	SetBlockingMode (bool IsBlocking);

	size_t
	GetIncomingDataSize ();

	bool
	Bind (const sockaddr* pAddr)
	{
		int Result = ::bind (m_h, pAddr, sizeof (sockaddr));
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool
	GetAddress (sockaddr* pAddr);

	bool
	GetPeerAddress (sockaddr* pAddr);

	bool
	GetOption (
		int Level,
		int Option,
		void* p,
		size_t Size
		)
	{
		int Result = ::getsockopt (m_h, Level, Option, (char*) p, (int*) &Size);
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool
	SetOption (
		int Level,
		int Option,
		const void* p,
		size_t Size
		)
	{
		int Result = ::setsockopt (m_h, Level, Option, (char*) p, Size);
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool
	Listen (size_t BackLog)
	{
		int Result = ::listen (m_h, (int) BackLog);
		return err::Complete (Result != SOCKET_ERROR);
	}

	SOCKET
	Accept (sockaddr* pAddr = NULL);

	bool
	Connect (const sockaddr* pAddr)
	{
		int Result = ::connect (m_h, pAddr, sizeof (sockaddr));
		return CompleteAsyncRequest (Result, WSAEWOULDBLOCK);
	}

	bool
	Shutdown (int Mode = SD_BOTH)
	{
		int Result = ::shutdown (m_h, Mode);
		return err::Complete (Result);
	}

	size_t
	Send (
		const void* p,
		size_t Size
		)
	{
		int Result = ::send (m_h, (const char*) p, (int) Size, 0);
		return err::Complete (Result, SOCKET_ERROR);
	}

	size_t
	Recv (
		void* p,
		size_t Size
		)
	{
		int Result = ::recv (m_h, (char*) p, (int) Size, 0);
		return err::Complete (Result, SOCKET_ERROR);
	}

	size_t
	SendTo (
		void* p,
		size_t Size,
		const sockaddr* pAddr
		)
	{
		int Result = ::sendto (m_h, (char*) p, (int) Size, 0, pAddr, sizeof (sockaddr));
		return err::Complete (Result, SOCKET_ERROR);
	}

	size_t
	RecvFrom (
		void* p,
		size_t Size,
		sockaddr* pAddr
		);

public:
	bool
	WsaOpen (
		int AddressFamily,
		int SockKind,
		int Protocol,
		dword_t Flags = WSA_FLAG_OVERLAPPED
		);

	bool
	WsaEventSelect (
		HANDLE hEvent,
		int Mask
		)
	{
		int Result = ::WSAEventSelect (m_h, hEvent, Mask);
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool
	WsaEnumEvents (
		WSANETWORKEVENTS* pEvents,
		HANDLE hEvent = NULL
		)
	{
		int Result = ::WSAEnumNetworkEvents (m_h, hEvent, pEvents);
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool
	WsaConnect (const sockaddr* pAddr)
	{
		int Result = ::WSAConnect (m_h, pAddr, sizeof (sockaddr), NULL, NULL, NULL, NULL);
		return CompleteAsyncRequest (Result, WSAEWOULDBLOCK);
	}

	SOCKET
	WsaAccept (sockaddr* pAddr);

	bool
	WsaSend (
		const void* p,
		dword_t Size,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

	bool
	WsaRecv (
		void* p,
		dword_t Size,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

	bool
	WsaSendTo (
		const void* p,
		dword_t Size,
		const sockaddr* pAddr,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

	bool
	WsaRecvFrom (
		void* p,
		dword_t Size,
		sockaddr* pAddr,
		int* pAddrSize,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

protected:
	static
	bool
	CompleteAsyncRequest (
		int Result,
		int PendingResult
		);
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
