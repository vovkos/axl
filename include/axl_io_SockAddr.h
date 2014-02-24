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
GetProtocolString (ESockProto Protocol)
{
	switch (Protocol)
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
	sockaddr_in m_Ip4;
	sockaddr_in6 m_Ip6;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSockAddrIp: TSockAddr
{
	union
	{
		uint32_t m_Ip;
		uint8_t m_IpBytes [4];
	};

	uint16_t m_Port;

	void
	Setup (
		uint32_t Ip, 
		uint16_t Port
		)
	{
		m_Kind = ESockAddr_Ip;
		m_Size = sizeof (TSockAddrIp);
		m_Ip   = Ip;
		m_Port = Port;
	}

	void
	FromWinSockAddr (const SOCKADDR_IN* pAddr)
	{
		Setup (ntohl (pAddr->sin_addr.S_un.S_addr), ntohs (pAddr->sin_port));
	}

	void
	ToWinSockAddr (SOCKADDR_IN* pAddr) const
	{
		memset (pAddr, 0, sizeof (SOCKADDR_IN));
		pAddr->sin_family = AF_INET;
		pAddr->sin_addr.S_un.S_addr = htonl (m_Ip);
		pAddr->sin_port = htons (m_Port);
	}

	rtl::CString
	ToString () const
	{
		rtl::CString String;

		if (m_Ip)
			String.Format ("%d.%d.%d.%d:%d", 
				m_IpBytes [3], 
				m_IpBytes [2], 
				m_IpBytes [1], 
				m_IpBytes [0], 
				m_Port
				);
		else
			String.Format ("%d", m_Port);

		return String;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSockAddrIp6: TSockAddr
{
	uint16_t m_Ip6 [8];
	uint16_t m_Port;

	void
	Setup (
		const uint16_t* pIp6, 
		uint16_t Port
		)
	{
		m_Kind = ESockAddr_Ip;
		m_Size = sizeof (TSockAddrIp6);
		memcpy (m_Ip6, pIp6, sizeof (m_Ip6));
		m_Port = Port;
	}

	void
	FromWinSockAddr (const SOCKADDR_IN6* pAddr)
	{
		Setup (pAddr->sin6_addr.u.Word, ntohs (pAddr->sin6_port));
	}

	void
	ToWinSockAddr (SOCKADDR_IN6* pAddr) const
	{
		memset (pAddr, 0, sizeof (SOCKADDR_IN6));
		pAddr->sin6_family = AF_INET6;
		memcpy (pAddr->sin6_addr.u.Word, m_Ip6, sizeof (m_Ip6));
		pAddr->sin6_port = htons (m_Port);
	}

	rtl::CString
	ToString () const
	{
		rtl::CString String;

		String.Format ("%04x", m_Ip6 [0]);

		for (size_t i = 1; i < 8; i++)
			if (m_Ip6 [i])
				String.AppendFormat ("::%04x", m_Ip6 [i]);

		String.AppendFormat (".%d", m_Port);
		return String;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

union TSockAddrU
{
	TSockAddr m_Hdr;
	TSockAddrIp m_Ip;
	TSockAddrIp6 m_Ip6;

	bool
	FromWinSockAddr (const SOCKADDR* pAddr)
	{
		switch (pAddr->sa_family)
		{
		case AF_INET:
			m_Ip.FromWinSockAddr ((SOCKADDR_IN*) pAddr);
			break;

		case AF_INET6:
			m_Ip6.FromWinSockAddr ((SOCKADDR_IN6*) pAddr);
			break;

		default:
			return err::Fail (err::EStatus_InvalidParameter);
		}

		return true;
	}

	bool
	ToWinSockAddr (SOCKADDR* pAddr) const
	{
		switch (m_Hdr.m_Kind)
		{
		case ESockAddr_Ip:
			m_Ip.ToWinSockAddr ((SOCKADDR_IN*) pAddr);
			break;

		case ESockAddr_Ip6:
			m_Ip6.ToWinSockAddr ((SOCKADDR_IN6*) pAddr);
			break;

		default:
			return err::Fail (err::EStatus_InvalidParameter);
		}

		return true;
	}

	rtl::CString
	ToString () const
	{
		switch (m_Hdr.m_Kind)
		{
		case ESockAddr_Ip:
			return m_Ip.ToString ();

		case ESockAddr_Ip6:
			return m_Ip6.ToString ();

		default:
			return rtl::CString::Format_s ("<address-kind=%d>", m_Hdr.m_Kind);
		}
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
TSockAddr::ToWinSockAddr (SOCKADDR* pAddr) const
{
	return ((TSockAddrU*) this)->ToWinSockAddr (pAddr);
}

inline
rtl::CString
TSockAddr::ToString () const
{
	return ((TSockAddrU*) this)->ToString ();
}

//.............................................................................

class CSockAddrIp: public TSockAddrIp
{
public:
	CSockAddrIp (uint16_t Port = 0)
	{
		Setup (0, Port);
	}

	CSockAddrIp (
		uint32_t Ip, 
		uint16_t Port
		)
	{
		Setup (Ip, Port);
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
