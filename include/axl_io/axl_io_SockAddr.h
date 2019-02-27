//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_IO_SOCKADDR_H

#include "axl_io_Pch.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getSockAddrFamilyString(uint_t family);

size_t
getSockAddrFamilySize(uint_t family);

inline
size_t
getSockAddrSize(const sockaddr* addr)
{
	return getSockAddrFamilySize(addr->sa_family);
}

const char*
getSockProtoString(uint_t proto);

//..............................................................................

size_t
getSockAddrNetMaskBitCount_ip4(const sockaddr_in* addr);

size_t
getSockAddrNetMaskBitCount_ip6(const sockaddr_in6* addr);

size_t
getSockAddrNetMaskBitCount(const sockaddr* addr);

//..............................................................................

void
createSockAddrNetMask_ip4(
	sockaddr_in* addr,
	size_t bitCount
	);

void
createSockAddrNetMask_ip6(
	sockaddr_in6* addr,
	size_t bitCount
	);

//..............................................................................

bool
isSockAddrEqual_ip4(
	const sockaddr_in* addr1,
	const sockaddr_in* addr2
	);

bool
isSockAddrEqual_ip6(
	const sockaddr_in6* addr1,
	const sockaddr_in6* addr2
	);

bool
isSockAddrEqual(
	const sockaddr* addr1,
	const sockaddr* addr2
	);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
isSockAddrMatch_ip4(
	const sockaddr_in* addr,
	const sockaddr_in* filterAddr
	);

bool
isSockAddrMatch_ip6(
	const sockaddr_in6* addr,
	const sockaddr_in6* filterAddr
	);

bool
isSockAddrMatch(
	const sockaddr* addr,
	const sockaddr* filterAddr
	);

//..............................................................................

bool
parseAddr_ip4(
	in_addr* addr,
	const sl::StringRef& string
	);

bool
parseAddr_ip6(
	in6_addr* addr,
	const sl::StringRef& string
	);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
parseSockAddr_ip4(
	sockaddr_in* addr,
	const sl::StringRef& string
	);

bool
parseSockAddr_ip6(
	sockaddr_in6* addr,
	const sl::StringRef& string
	);

bool
parseSockAddr(
	sockaddr* addr,
	size_t size,
	const sl::StringRef& string
	);

//..............................................................................

size_t
getAddrString_ip4(
	sl::String* string,
	const in_addr* addr
	);

inline
sl::String
getAddrString_ip4(const in_addr* addr)
{
	sl::String string;
	getAddrString_ip4(&string, addr);
	return string;
}

size_t
getAddrString_ip6(
	sl::String* string,
	const in6_addr* addr
	);

inline
sl::String
getAddrString_ip6(const in6_addr* addr)
{
	sl::String string;
	getAddrString_ip6(&string, addr);
	return string;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
getSockAddrString_ip4(
	sl::String* string,
	const sockaddr_in* addr
	);

inline
sl::String
getSockAddrString_ip4(const sockaddr_in* addr)
{
	sl::String string;
	getSockAddrString_ip4(&string, addr);
	return string;
}

size_t
getSockAddrString_ip6(
	sl::String* string,
	const sockaddr_in6* addr
	);

inline
sl::String
getSockAddrString_ip6(const sockaddr_in6* addr)
{
	sl::String string;
	getSockAddrString_ip6(&string, addr);
	return string;
}

size_t
getSockAddrString(
	sl::String* string,
	const sockaddr* addr
	);

inline
sl::String
getSockAddrString(const sockaddr* addr)
{
	sl::String string;
	getSockAddrString(&string, addr);
	return string;
}

//..............................................................................

struct SockAddr
{
	union
	{
		//! \unnamed{union}
		sockaddr m_addr;
		sockaddr_in m_addr_ip4;
		sockaddr_in6 m_addr_ip6;
	};

	SockAddr()
	{
		clear();
	}

	SockAddr(const sockaddr* addr)
	{
		setup(addr);
	}

	SockAddr(
		const in_addr* addr,
		uint_t port = 0
		)
	{
		setup_ip4(addr, port);
	}

	SockAddr(
		uint_t ip,
		uint_t port = 0
		)
	{
		setup_ip4(ip, port);
	}

	SockAddr(
		const in6_addr* addr,
		uint_t port = 0,
		uint_t scope = 0
		)
	{
		setup_ip6(addr, port, scope);
	}

	SockAddr(
		const uint16_t* ip,
		uint_t port = 0,
		uint_t scope = 0
		)
	{
		setup_ip6(ip, port, scope);
	}

	SockAddr(const sl::StringRef& string)
	{
		parse(string);
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
	clear()
	{
		memset(this, 0, sizeof(SockAddr));
	}

	bool
	isEqual(const sockaddr* addr) const
	{
		return isSockAddrEqual(&m_addr, addr);
	}

	bool
	isEqual(const SockAddr* addr) const
	{
		return isSockAddrEqual(&m_addr, &addr->m_addr);
	}

	bool
	isMatch(const sockaddr* addr) const
	{
		return isSockAddrMatch(&m_addr, addr);
	}

	bool
	isMatch(const SockAddr* addr) const
	{
		return isSockAddrMatch(&m_addr, &addr->m_addr);
	}

	uint_t
	getPort()
	{
		return sl::swapByteOrder16(m_addr_ip4.sin_port);
	}

	void
	setPort(uint_t port)
	{
		m_addr_ip4.sin_port = sl::swapByteOrder16((uint16_t)port);
	}

	void
	setup(const sockaddr* addr);

	void
	setup(
		uint_t family,
		const void* addr,
		size_t size
		);

	void
	setup_ip4(
		const in_addr* addr,
		uint_t port = 0
		);

	void
	setup_ip4(
		uint_t ip,
		uint_t port = 0
		)
	{
		setup_ip4((const in_addr*) &ip, port);
	}

	void
	setup_ip6(
		const in6_addr* addr,
		uint_t port = 0,
		uint_t scope = 0
		);

	void
	setup_ip6(
		const uint16_t* ip,
		uint_t port = 0,
		uint_t scope = 0
		)
	{
		setup_ip6((const in6_addr*) ip, port);
	}

	size_t
	getNetMaskBitCount()
	{
		return getSockAddrNetMaskBitCount(&m_addr);
	}

	size_t
	getNetMaskBitCount_ip4()
	{
		return getSockAddrNetMaskBitCount_ip4(&m_addr_ip4);
	}

	size_t
	getNetMaskBitCount_ip6()
	{
		return getSockAddrNetMaskBitCount_ip6(&m_addr_ip6);
	}

	void
	createNetMask(
		uint_t family,
		size_t bitCount
		);

	void
	createNetMask_ip4(size_t bitCount)
	{
		createSockAddrNetMask_ip4(&m_addr_ip4, bitCount);
	}

	void
	createNetMask_ip6(size_t bitCount)
	{
		createSockAddrNetMask_ip6(&m_addr_ip6, bitCount);
	}

	bool
	parse(const sl::StringRef& string)
	{
		return parseSockAddr(&m_addr, sizeof(SockAddr), string);
	}

	bool
	parse_ip4(const sl::StringRef& string)
	{
		return parseSockAddr_ip4(&m_addr_ip4, string);
	}

	bool
	parse_ip6(const sl::StringRef& string)
	{
		return parseSockAddr_ip6(&m_addr_ip6, string);
	}

	size_t
	getString(sl::String* string) const
	{
		return getSockAddrString(string, &m_addr);
	}

	inline
	sl::String
	getString() const
	{
		return getSockAddrString(&m_addr);
	}

	size_t
	getString_ip4(sl::String* string) const
	{
		return getSockAddrString_ip4(string, &m_addr_ip4);
	}

	sl::String
	getString_ip4() const
	{
		return getSockAddrString_ip4(&m_addr_ip4);
	}

	size_t
	getString_ip6(sl::String* string) const
	{
		return getSockAddrString_ip6(string, &m_addr_ip6);
	}

	sl::String
	getString_ip6(const sockaddr_in6* addr) const
	{
		return getSockAddrString_ip6(&m_addr_ip6);
	}
};

//..............................................................................

bool
resolveHostName(
	sl::Array<SockAddr>* addrArray,
	const sl::StringRef& name,
	uint_t family = AF_UNSPEC
	);

inline
sl::Array<SockAddr>
resolveHostName(
	const sl::StringRef& name,
	uint_t family = AF_UNSPEC
	)
{
	sl::Array<SockAddr> addrArray;
	resolveHostName(&addrArray, name, family);
	return addrArray;
}

//..............................................................................

} // namespace io
} // namespace axl
