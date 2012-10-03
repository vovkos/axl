// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SOCK_H

#include "axl_io_Transport.h"
#include "axl_exe_WorkerThread.h"

namespace axl {
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

class CSock: public rtl::CHandleT <SOCKET, INVALID_SOCKET, CCloseSocket>
{
public:
	bool 
	Open (
		int AddressFamily,
		int SockKind,
		int Protocol
		)
	{
		Close ();

		m_h = ::WSASocket (AddressFamily, SockKind, Protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
		return err::Complete (m_h != INVALID_SOCKET);
	}

	bool
	Bind (const SOCKADDR* pAddr)
	{	
		ASSERT (IsOpen ());

		int Result = ::bind (m_h, pAddr, sizeof (SOCKADDR));
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool
	GetLocalAddress (SOCKADDR* pAddr);

	bool
	GetPeerAddress (SOCKADDR* pAddr);

	bool 
	Select (
		HANDLE hEvent,
		int Mask
		)
	{
		ASSERT (IsOpen ());

		int Result = ::WSAEventSelect (m_h, hEvent, Mask);
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
		ASSERT (IsOpen ());

		int Result = ::setsockopt (m_h, Level, Option, (char*) p, (int) Size);
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool 
	EnableBroadcast (bool IsEnabled)
	{
		ASSERT (IsOpen ());

		int Option = IsEnabled;
		return SetOption (SOL_SOCKET, SO_BROADCAST, &Option, sizeof (int));
	}

	bool
	Listen (int BackLog)
	{
		ASSERT (IsOpen ());

		int Result = ::listen (m_h, BackLog);
		return err::Complete (Result != SOCKET_ERROR);
	}

	SOCKET
	Accept (SOCKADDR* pAddr = NULL);

	bool
	Connect (const SOCKADDR* pAddr)
	{
		ASSERT (IsOpen ());

		int Result = ::WSAConnect (m_h, pAddr, sizeof (SOCKADDR), NULL, NULL, NULL, NULL);
		return CompleteAsyncOp (Result, WSAEWOULDBLOCK);
	}

	bool
	Shutdown (int Mode)
	{
		ASSERT (IsOpen ());

		int Result = ::shutdown (m_h, Mode);
		return err::Complete (Result);
	}
	
	bool
	Send (
		const void* p,
		ulong_t Size,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
		);

	bool
	Recv (
		const void* p,
		ulong_t Size,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
		);

	bool
	SendTo (
		const void* p,
		ulong_t Size,
		const SOCKADDR* pAddr,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
		);

	bool
	RecvFrom (
		const void* p,
		ulong_t Size,
		SOCKADDR* pAddr,
		int* pAddrSize,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
		);

protected:
	static
	bool
	CompleteAsyncOp (
		int Result,
		ulong_t PendingCode
		);
};

//.............................................................................

} // namespace win
} // namespace axl
