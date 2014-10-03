// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKADDR_H

#include "axl_io_Transport.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

enum SockAddrKind
{
	SockAddrKind_Ip   = 2,
	SockAddrKind_Ip6  = 23,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SockProto
{
	SockProto_Icmp = 1,
	SockProto_Udp  = 17,
	SockProto_Tcp  = 6,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char* 
getProtocolString (SockProto protocol)
{
	switch (protocol)
	{
	case SockProto_Icmp:
		return "ICMP";

	case SockProto_Udp:
		return "UDP";

	case SockProto_Tcp:
		return "TCP";

	default:
		return "UNKNOWN_PROTOCOL";
	}
}

//.............................................................................

union SockAddr
{
	sockaddr_in m_ip4;
	sockaddr_in6 m_ip6;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SockAddrIp: SockAddr
{
	union
	{
		uint32_t m_ip;
		uint8_t m_ipBytes [4];
	};

	uint16_t m_port;

	void
	setup (
		uint32_t ip, 
		uint16_t port
		)
	{
		m_kind = SockAddrKind_Ip;
		m_size = sizeof (SockAddrIp);
		m_ip   = ip;
		m_port = port;
	}

	void
	fromWinSockAddr (const SOCKADDR_IN* addr)
	{
		setup (ntohl (addr->sin_addr.S_un.S_addr), ntohs (addr->sin_port));
	}

	void
	toWinSockAddr (SOCKADDR_IN* addr) const
	{
		memset (addr, 0, sizeof (SOCKADDR_IN));
		addr->sin_family = AF_INET;
		addr->sin_addr.S_un.S_addr = htonl (m_ip);
		addr->sin_port = htons (m_port);
	}

	rtl::String
	toString () const
	{
		rtl::String string;

		if (m_ip)
			string.format ("%d.%d.%d.%d:%d", 
				m_ipBytes [3], 
				m_ipBytes [2], 
				m_ipBytes [1], 
				m_ipBytes [0], 
				m_port
				);
		else
			string.format ("%d", m_port);

		return string;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SockAddrIp6: SockAddr
{
	uint16_t m_ip6 [8];
	uint16_t m_port;

	void
	setup (
		const uint16_t* ip6, 
		uint16_t port
		)
	{
		m_kind = SockAddrKind_Ip;
		m_size = sizeof (SockAddrIp6);
		memcpy (m_ip6, ip6, sizeof (m_ip6));
		m_port = port;
	}

	void
	fromWinSockAddr (const SOCKADDR_IN6* addr)
	{
		setup (addr->sin6_addr.u.Word, ntohs (addr->sin6_port));
	}

	void
	toWinSockAddr (SOCKADDR_IN6* addr) const
	{
		memset (addr, 0, sizeof (SOCKADDR_IN6));
		addr->sin6_family = AF_INET6;
		memcpy (addr->sin6_addr.u.Word, m_ip6, sizeof (m_ip6));
		addr->sin6_port = htons (m_port);
	}

	rtl::String
	toString () const
	{
		rtl::String string;

		string.format ("%04x", m_ip6 [0]);

		for (size_t i = 1; i < 8; i++)
			if (m_ip6 [i])
				string.appendFormat ("::%04x", m_ip6 [i]);

		string.appendFormat (".%d", m_port);
		return string;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

union SockAddrU
{
	SockAddr m_hdr;
	SockAddrIp m_ip;
	SockAddrIp6 m_ip6;

	bool
	fromWinSockAddr (const SOCKADDR* addr)
	{
		switch (addr->sa_family)
		{
		case AF_INET:
			m_ip.fromWinSockAddr ((SOCKADDR_IN*) addr);
			break;

		case AF_INET6:
			m_ip6.fromWinSockAddr ((SOCKADDR_IN6*) addr);
			break;

		default:
			return err::fail (err::SystemErrorCode_InvalidParameter);
		}

		return true;
	}

	bool
	toWinSockAddr (SOCKADDR* addr) const
	{
		switch (m_hdr.m_kind)
		{
		case SockAddrKind_Ip:
			m_ip.toWinSockAddr ((SOCKADDR_IN*) addr);
			break;

		case SockAddrKind_Ip6:
			m_ip6.toWinSockAddr ((SOCKADDR_IN6*) addr);
			break;

		default:
			return err::fail (err::SystemErrorCode_InvalidParameter);
		}

		return true;
	}

	rtl::String
	toString () const
	{
		switch (m_hdr.m_kind)
		{
		case SockAddrKind_Ip:
			return m_ip.toString ();

		case SockAddrKind_Ip6:
			return m_ip6.toString ();

		default:
			return rtl::String::format_s ("<address-kind=%d>", m_hdr.m_kind);
		}
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
SockAddr::toWinSockAddr (SOCKADDR* addr) const
{
	return ((SockAddrU*) this)->toWinSockAddr (addr);
}

inline
rtl::String
SockAddr::toString () const
{
	return ((SockAddrU*) this)->toString ();
}

//.............................................................................

class SockAddrIp: public SockAddrIp
{
public:
	SockAddrIp (uint16_t port = 0)
	{
		setup (0, port);
	}

	SockAddrIp (
		uint32_t ip, 
		uint16_t port
		)
	{
		setup (ip, port);
	}

	// this operator is needed to pass CSockAddrIp to functions accepting TSockAddr*

	operator SockAddrIp* ()
	{
		return this;
	}
};

//.............................................................................

} // namespace io
} // namespace axl
