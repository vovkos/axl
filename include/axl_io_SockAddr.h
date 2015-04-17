// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKADDR_H

#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

const char*
getSockAddrFamilyString (uint_t family);

size_t
getSockAddrFamilySize (uint_t family);

inline
size_t 
getSockAddrSize (const sockaddr* addr)
{
	return getSockAddrFamilySize (addr->sa_family);
}

const char*
getSockProtoString (uint_t proto);

//.............................................................................

size_t
getSockAddrNetMaskBitCount_ip4 (const sockaddr_in* addr);

size_t
getSockAddrNetMaskBitCount_ip6 (const sockaddr_in6* addr);

size_t
getSockAddrNetMaskBitCount (const sockaddr* addr);

//.............................................................................

void
createSockAddrNetMask_ip4 (
	sockaddr_in* addr,
	size_t bitCount
	);

void
createSockAddrNetMask_ip6 (
	sockaddr_in6* addr,
	size_t bitCount
	);

//.............................................................................

bool
parseSockAddr_ip4 (
	sockaddr_in* addr,
	const char* string,
	size_t length = -1
	);

bool
parseSockAddr_ip6 (
	sockaddr_in6* addr,
	const char* string,
	size_t length = -1
	);

bool
parseSockAddr (
	sockaddr* addr,
	size_t size,
	const char* string,
	size_t length = -1
	);

//.............................................................................

size_t
getSockAddrString_ip4 (
	rtl::String* string,
	const sockaddr_in* addr
	);

inline
rtl::String
getSockAddrString_ip4 (const sockaddr_in* addr)
{
	rtl::String string;
	getSockAddrString_ip4 (&string, addr);
	return string;
}

size_t
getSockAddrString_ip6 (
	rtl::String* string,
	const sockaddr_in6* addr
	);

inline
rtl::String
getSockAddrString_ip6 (const sockaddr_in6* addr)
{
	rtl::String string;
	getSockAddrString_ip6 (&string, addr);
	return string;
}

size_t
getSockAddrString (
	rtl::String* string,
	const sockaddr* addr
	);

inline
rtl::String
getSockAddrString (const sockaddr* addr)
{
	rtl::String string;
	getSockAddrString (&string, addr);
	return string;
}

//.............................................................................

size_t
getSockAddrSize (const sockaddr* addr);

//.............................................................................

struct SockAddr
{
	union
	{
		sockaddr m_addr;
		sockaddr_in m_addr_ip4;
		sockaddr_in6 m_addr_ip6;
	};

	SockAddr ()
	{
		clear ();
	}

	SockAddr (const sockaddr* addr)
	{
		setup (addr);
	}

	SockAddr (
		const in_addr* addr,
		uint_t port = 0
		)
	{
		setup_ip4 (addr, port);
	}

	SockAddr (
		uint_t ip,
		uint_t port = 0
		)
	{
		setup_ip4 (ip, port);
	}

	SockAddr (
		const in6_addr* addr,
		uint_t port = 0,
		uint_t scope = 0
		)
	{
		setup_ip6 (addr, port, scope);
	}

	SockAddr (
		const uint16_t* ip,
		uint_t port = 0,
		uint_t scope = 0
		)
	{
		setup_ip6 (ip, port, scope);
	}

	SockAddr (
		const char* string,
		size_t length = -1
		)
	{
		parse (string, length);
	}

	operator const sockaddr* () const
	{
		return &m_addr;
	}

	operator const sockaddr_in* () const
	{
		return &m_addr_ip4;
	}

	operator const sockaddr_in6* () const
	{
		return &m_addr_ip6;
	}

	void
	clear ()
	{
		memset (this, 0, sizeof (SockAddr));	
	}

	void
	setup (const sockaddr* addr);

	void
	setup_ip4 (
		const in_addr* addr,
		uint_t port = 0
		);

	void
	setup_ip4 (
		uint_t ip,
		uint_t port = 0
		)
	{
		setup_ip4 ((const in_addr*) &ip, port);
	}

	void
	setup_ip6 (
		const in6_addr* addr,
		uint_t port = 0,
		uint_t scope = 0
		);

	void
	setup_ip6 (
		const uint16_t* ip,
		uint_t port = 0,
		uint_t scope = 0
		)
	{
		setup_ip6 ((const in6_addr*) ip, port);
	}

	size_t
	getNetMaskBitCount ()
	{
		return getSockAddrNetMaskBitCount (&m_addr);
	}

	size_t
	getNetMaskBitCount_ip4 ()
	{
		return getSockAddrNetMaskBitCount_ip4 (&m_addr_ip4);
	}

	size_t
	getNetMaskBitCount_ip6 ()
	{
		return getSockAddrNetMaskBitCount_ip6 (&m_addr_ip6);
	}

	void
	createNetMask (
		uint_t family,
		size_t bitCount
		);

	void
	createNetMask_ip4 (size_t bitCount)
	{
		createSockAddrNetMask_ip4 (&m_addr_ip4, bitCount);
	}

	void
	createNetMask_ip6 (size_t bitCount)
	{
		createSockAddrNetMask_ip6 (&m_addr_ip6, bitCount);
	}

	bool
	parse (
		const char* string,
		size_t length = -1
		)
	{
		return parseSockAddr (&m_addr, sizeof (SockAddr), string, length);
	}

	bool
	parse_ip4 (
		const char* string,
		size_t length = -1
		)
	{
		return parseSockAddr_ip4 (&m_addr_ip4, string, length);
	}

	bool
	parse_ip6 (
		const char* string,
		size_t length = -1
		)
	{
		return parseSockAddr_ip6 (&m_addr_ip6, string, length);
	}

	size_t
	getString (rtl::String* string) const
	{
		return getSockAddrString (string, &m_addr);
	}

	inline
	rtl::String
	getString () const
	{
		return getSockAddrString (&m_addr);
	}

	size_t
	getString_ip4 (rtl::String* string) const
	{
		return getSockAddrString_ip4 (string, &m_addr_ip4);
	}

	rtl::String
	getString_ip4 () const
	{
		return getSockAddrString_ip4 (&m_addr_ip4);
	}

	size_t
	getString_ip6 (rtl::String* string) const
	{
		return getSockAddrString_ip6 (string, &m_addr_ip6);
	}

	rtl::String
	getString_ip6 (const sockaddr_in6* addr) const
	{
		return getSockAddrString_ip6 (&m_addr_ip6);
	}
};

//.............................................................................

} // namespace io
} // namespace axl
