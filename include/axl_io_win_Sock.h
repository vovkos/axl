// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SOCK_H

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

class CSock: public rtl::CHandleT <SOCKET, CCloseSocket, rtl::CMinusOneT <SOCKET> >
{
public:
	bool 
	Open (
		int AddressFamily,
		int SockKind,
		int Protocol
		);

	bool
	Bind (const SOCKADDR* pAddr)
	{	
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
		int Result = ::setsockopt (m_h, Level, Option, (char*) p, Size);
		return err::Complete (Result != SOCKET_ERROR);
	}

	bool 
	EnableBroadcast (bool IsEnabled)
	{
		int Option = IsEnabled;
		return SetOption (SOL_SOCKET, SO_BROADCAST, &Option, sizeof (int));
	}

	bool
	Listen (int BackLog)
	{
		int Result = ::listen (m_h, BackLog);
		return err::Complete (Result != SOCKET_ERROR);
	}

	SOCKET
	Accept (SOCKADDR* pAddr = NULL);

	bool
	Connect (const SOCKADDR* pAddr)
	{
		int Result = ::WSAConnect (m_h, pAddr, sizeof (SOCKADDR), NULL, NULL, NULL, NULL);
		return CompleteAsyncOp (Result, WSAEWOULDBLOCK);
	}

	bool
	Shutdown (int Mode)
	{
		int Result = ::shutdown (m_h, Mode);
		return err::Complete (Result);
	}
	
	bool
	Send (
		const void* p,
		dword_t Size,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

	bool
	Recv (
		const void* p,
		dword_t Size,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

	bool
	SendTo (
		const void* p,
		dword_t Size,
		const SOCKADDR* pAddr,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

	bool
	RecvFrom (
		const void* p,
		dword_t Size,
		SOCKADDR* pAddr,
		int* pAddrSize,
		WSAOVERLAPPED* pOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		);

protected:
	static
	bool
	CompleteAsyncOp (
		int Result,
		dword_t PendingCode
		);
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
