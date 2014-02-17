// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_PSX_SOCKET_H

#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CSocket: public CFd
{
public:
	bool
	Open (
		int AddressFamily,
		int SockKind,
		int Protocol
		);

	bool
	SetBlockingMode (bool IsBlocking); // blocking mode is set a bit differently

	int
	GetError ();

	bool
	Bind (const sockaddr* pAddr)
	{
		int Result = ::bind (m_h, pAddr, sizeof (sockaddr));
		return err::Complete (Result != -1);
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
		socklen_t Size
		)
	{
		int Result = ::getsockopt (m_h, Level, Option, p, &Size);
		return err::Complete (Result != -1);
	}

	bool
	SetOption (
		int Level,
		int Option,
		const void* p,
		socklen_t Size
		)
	{
		int Result = ::setsockopt (m_h, Level, Option, p, Size);
		return err::Complete (Result != -1);
	}

	bool
	EnableBroadcast (bool IsEnabled)
	{
		int Option = IsEnabled;
		return SetOption (SOL_SOCKET, SO_BROADCAST, &Option, sizeof (int));
	}

	bool
	Listen (size_t BackLog)
	{
		int Result = ::listen (m_h, (int) BackLog);
		return err::Complete (Result != -1);
	}

	int
	Accept (sockaddr* pAddr = NULL);

	bool
	Connect (const sockaddr* pAddr);

	bool
	Shutdown (int Mode = SHUT_RDWR)
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
		int Result = ::send (m_h, p, Size, 0);
		return err::Complete (Result, -1);
	}

	size_t
	Recv (
		void* p,
		size_t Size
		)
	{
		int Result = ::recv (m_h, p, Size, 0);
		return err::Complete (Result, -1);
	}

	size_t
	SendTo (
		void* p,
		size_t Size,
		const sockaddr* pAddr
		)
	{
		int Result = ::sendto (m_h, p, Size, 0, pAddr, sizeof (sockaddr));
		return err::Complete (Result, -1);
	}

	size_t
	RecvFrom (
		void* p,
		size_t Size,
		sockaddr* pAddr
		);
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
