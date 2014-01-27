// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKET_H

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_io_win_Socket.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_Socket.h"
#endif

namespace axl {
namespace io {

//.............................................................................

enum ESocketShutdown
{
	ESocketShutdown_Recv,
	ESocketShutdown_Send,
	ESocketShutdown_All,
};

//.............................................................................

class CSocket
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	win::CSocket m_Socket;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CSocket m_Socket;
#endif

public:
	bool
	IsOpen ()
	{
		return m_Socket.IsOpen ();
	}

	bool
	Open (
		int AddressFamily,
		int SockKind,
		int Protocol
		)
	{
		return m_Socket.Open (AddressFamily, SockKind, Protocol);
	}

	void
	Close ()
	{
		m_Socket.Close ();
	}

	int
	GetError ()
	{
		return m_Socket.GetError ();
	}

	bool
	SetBlockingMode (bool IsBlocking)
	{
		return m_Socket.SetBlockingMode (IsBlocking);
	}

	size_t
	GetIncomingDataSize ()
	{
		return m_Socket.GetIncomingDataSize ();
	}

	bool
	Bind (const sockaddr* pAddr)
	{
		return m_Socket.Bind (pAddr);
	}

	bool
	GetAddress (sockaddr* pAddr)
	{
		return m_Socket.GetAddress (pAddr);
	}

	bool
	GetPeerAddress (sockaddr* pAddr)
	{
		return m_Socket.GetPeerAddress (pAddr);
	}

	bool
	SetOption (
		int Level,
		int Option,
		const void* p,
		size_t Size
		)
	{
		return m_Socket.SetOption (Level, Option, p, Size);
	}

	bool
	EnableBroadcast (bool IsEnabled)
	{
		return m_Socket.EnableBroadcast (IsEnabled);
	}

	bool
	Listen (size_t BackLog)
	{
		return m_Socket.Listen (BackLog);
	}

	bool
	Accept (
		CSocket* pSocket,
		sockaddr* pAddr = NULL
		);

	bool
	Connect (const sockaddr* pAddr)
	{
		return m_Socket.Connect (pAddr);
	}

	bool
	Shutdown (ESocketShutdown ShutdownKind = ESocketShutdown_All)
	{
		return m_Socket.Shutdown (ShutdownKind);
	}

	size_t
	Send (
		const void* p,
		size_t Size
		)
	{
		return m_Socket.Send (p, Size);
	}

	size_t
	Recv (
		void* p,
		size_t Size
		)
	{
		return m_Socket.Recv (p, Size);
	}

	size_t
	SendTo (
		void* p,
		size_t Size,
		const sockaddr* pAddr
		)
	{
		return m_Socket.SendTo (p, Size, pAddr);
	}

	size_t
	RecvFrom (
		void* p,
		size_t Size,
		sockaddr* pAddr
		)
	{
		return m_Socket.RecvFrom (p, Size, pAddr);
	}
};

//.............................................................................

} // namespace io {
} // namespace axl {
