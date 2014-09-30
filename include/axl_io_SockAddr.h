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

enum ESockAddr
{
	ESockAddr_Ip   = 2,
	ESockAddr_Ip6  = 23,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESockProto
{
	ESockProto_Icmp = 1,
	ESockProto_Udp  = 17,
	ESockProto_Tcp  = 6,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char* 
getProtocolString (ESockProto protocol)
{
	switch (protocol)
	{
	case ESockProto_Icmp:
		return "ICMP";

	case ESockProto_Udp:
		return "UDP";

	case ESockProto_Tcp:
		return "TCP";

	default:
		return "UNKNOWN_PROTOCOL";
	}
}

//.............................................................................

union TSockAddr
{
	sockaddr_in m_ip4;
	sockaddr_in6 m_ip6;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSockAddrIp: TSockAddr
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
		m_kind = ESockAddr_Ip;
		m_size = sizeof (TSockAddrIp);
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

	rtl::CString
	toString () const
	{
		rtl::CString string;

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

struct TSockAddrIp6: TSockAddr
{
	uint16_t m_ip6 [8];
	uint16_t m_port;

	void
	setup (
		const uint16_t* ip6, 
		uint16_t port
		)
	{
		m_kind = ESockAddr_Ip;
		m_size = sizeof (TSockAddrIp6);
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

	rtl::CString
	toString () const
	{
		rtl::CString string;

		string.format ("%04x", m_ip6 [0]);

		for (size_t i = 1; i < 8; i++)
			if (m_ip6 [i])
				string.appendFormat ("::%04x", m_ip6 [i]);

		string.appendFormat (".%d", m_port);
		return string;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

union TSockAddrU
{
	TSockAddr m_hdr;
	TSockAddrIp m_ip;
	TSockAddrIp6 m_ip6;

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
			return err::fail (err::EStatus_InvalidParameter);
		}

		return true;
	}

	bool
	toWinSockAddr (SOCKADDR* addr) const
	{
		switch (m_hdr.m_kind)
		{
		case ESockAddr_Ip:
			m_ip.toWinSockAddr ((SOCKADDR_IN*) addr);
			break;

		case ESockAddr_Ip6:
			m_ip6.toWinSockAddr ((SOCKADDR_IN6*) addr);
			break;

		default:
			return err::fail (err::EStatus_InvalidParameter);
		}

		return true;
	}

	rtl::CString
	toString () const
	{
		switch (m_hdr.m_kind)
		{
		case ESockAddr_Ip:
			return m_ip.toString ();

		case ESockAddr_Ip6:
			return m_ip6.toString ();

		default:
			return rtl::CString::format_s ("<address-kind=%d>", m_hdr.m_kind);
		}
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
TSockAddr::toWinSockAddr (SOCKADDR* addr) const
{
	return ((TSockAddrU*) this)->toWinSockAddr (addr);
}

inline
rtl::CString
TSockAddr::toString () const
{
	return ((TSockAddrU*) this)->toString ();
}

//.............................................................................

class CSockAddrIp: public TSockAddrIp
{
public:
	CSockAddrIp (uint16_t port = 0)
	{
		setup (0, port);
	}

	CSockAddrIp (
		uint32_t ip, 
		uint16_t port
		)
	{
		setup (ip, port);
	}

	// this operator is needed to pass CSockAddrIp to functions accepting TSockAddr*

	operator TSockAddrIp* ()
	{
		return this;
	}
};

//.............................................................................

} // namespace io
} // namespace axl
