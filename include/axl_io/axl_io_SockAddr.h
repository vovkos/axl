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
#include "axl_sl_BitMap.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getSockAddrFamilyString(uint_t family);

size_t
getSockAddrFamilySize(uint_t family);

inline
size_t
getSockAddrSize(const sockaddr* addr) {
	return getSockAddrFamilySize(addr->sa_family);
}

const char*
getSockProtoString(uint_t proto);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
getSockAddrNetMaskBitCount_ip4(const sockaddr_in* addr) {
	size_t bitIdx = sl::findZeroBit((size_t*)&addr->sin_addr, 1, 0);
	return AXL_MIN(bitIdx, 32);
}

inline
size_t
getSockAddrNetMaskBitCount_ip6(const sockaddr_in6* addr) {
	size_t bitIdx = sl::findZeroBit(
		(size_t*)&addr->sin6_addr,
		sizeof(addr->sin6_addr) / sizeof(size_t),
		0
	);

	return AXL_MIN(bitIdx, 128);
}


size_t
getSockAddrNetMaskBitCount(const sockaddr* addr);

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
isSockAddrEqual_ip4(
	const sockaddr_in* addr1,
	const sockaddr_in* addr2
) {
	return
		addr1->sin_port == addr2->sin_port &&
		*(const uint32_t*)&addr1->sin_addr == *(const uint32_t*)&addr2->sin_addr;
}

inline
bool
isSockAddrEqual_ip6(
	const sockaddr_in6* addr1,
	const sockaddr_in6* addr2
) {
	return
		addr1->sin6_port == addr2->sin6_port &&
		addr1->sin6_scope_id == addr2->sin6_scope_id &&
		memcmp(&addr1->sin6_addr, &addr2->sin6_addr, sizeof(addr1->sin6_addr)) == 0;
}

#if (_AXL_IO_SOCKADDR_UN)

inline
bool
isSockAddrEqual_un(
	const sockaddr_un* addr1,
	const sockaddr_un* addr2
) {
	return strncmp(addr1->sun_path, addr2->sun_path, sizeof(addr1->sun_path)) == 0;
}

#endif

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

#if (_AXL_IO_SOCKADDR_UN)

inline
bool
isSockAddrMatch_un(
	const sockaddr_un* addr,
	const sockaddr_un* filterAddr
) {
   return !filterAddr->sun_path[0] || isSockAddrEqual_un(addr, filterAddr); // empty path matches any
}

#endif

bool
isSockAddrMatch(
	const sockaddr* addr,
	const sockaddr* filterAddr
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

#if (_AXL_IO_SOCKADDR_UN)

bool
parseSockAddr_un(
	sockaddr_un* addr,
	const sl::StringRef& string
);

#endif

bool
parseSockAddr(
	sockaddr* addr,
	size_t size,
	const sl::StringRef& string
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
getAddrString_ip4(
	sl::String* string,
	const in_addr* addr
);

inline
sl::String
getAddrString_ip4(const in_addr* addr) {
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
getAddrString_ip6(const in6_addr* addr) {
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
getSockAddrString_ip4(const sockaddr_in* addr) {
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
getSockAddrString_ip6(const sockaddr_in6* addr) {
	sl::String string;
	getSockAddrString_ip6(&string, addr);
	return string;
}

#if (_AXL_IO_SOCKADDR_UN)

inline
size_t
getSockAddrString_un(
	sl::String* string,
	const sockaddr_un* addr
) {
	return string->copy(addr->sun_path, strnlen(addr->sun_path, sizeof(addr->sun_path)));
}

inline
sl::String
getSockAddrString_un(const sockaddr_un* addr) {
	sl::String string;
	getSockAddrString_un(&string, addr);
	return string;
}

#endif

size_t
getSockAddrString(
	sl::String* string,
	const sockaddr* addr
);

inline
sl::String
getSockAddrString(const sockaddr* addr) {
	sl::String string;
	getSockAddrString(&string, addr);
	return string;
}

//..............................................................................

template <typename Storage>
struct SockAddrBase: Storage {
	SockAddrBase() {
		clear();
	}

	operator const sockaddr* () const {
		return &this->m_addr;
	}

	void
	clear() {
		memset(this, 0, sizeof(Storage));
	}

	bool
	isEqual(const sockaddr* addr) const {
		return isSockAddrEqual(&this->m_addr, addr);
	}

	bool
	isMatch(const sockaddr* addr) const {
		return isSockAddrMatch(&this->m_addr, addr);
	}

	bool
	parse(const sl::StringRef& string) {
		return parseSockAddr(&this->m_addr, sizeof(Storage), string);
	}

	size_t
	getString(sl::String* string) const {
		return getSockAddrString(string, &this->m_addr);
	}

	sl::String
	getString() const {
		return getSockAddrString(&this->m_addr);
	}
};

//..............................................................................

// IPv4 & IPv6

struct SockAddrStorage {
	union {
		sockaddr m_addr;
		sockaddr_in m_addr_ip4;
		sockaddr_in6 m_addr_ip6;
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SockAddr: SockAddrBase<SockAddrStorage> {
	SockAddr() {}

	SockAddr(const sockaddr* addr) {
		setup(addr);
	}

	SockAddr(
		const in_addr* addr,
		uint_t port = 0
	) {
		setup_ip4(addr, port);
	}

	SockAddr(
		uint_t ip,
		uint_t port = 0
	) {
		setup_ip4(ip, port);
	}

	SockAddr(
		const in6_addr* addr,
		uint_t port = 0,
		uint_t scope = 0
	) {
		setup_ip6(addr, port, scope);
	}

	SockAddr(
		const uint16_t* ip,
		uint_t port = 0,
		uint_t scope = 0
	) {
		setup_ip6(ip, port, scope);
	}

	SockAddr(const sl::StringRef& string) {
		parse(string);
	}

	operator const sockaddr_in* () const {
		return &m_addr_ip4;
	}

	operator const sockaddr_in6* () const {
		return &m_addr_ip6;
	}

	uint_t
	getPort() const {
		return sl::swapByteOrder16(m_addr_ip4.sin_port);
	}

	void
	setPort(uint_t port) {
		m_addr_ip4.sin_port = sl::swapByteOrder16((uint16_t)port);
	}

	void
	setup(const sockaddr* addr);

	void
	setup(
		uint_t family,
		const void* p,
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
	) {
		ip = sl::swapByteOrder32(ip);
		setup_ip4((const in_addr*)&ip, port);
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
	) {
		setup_ip6((const in6_addr*)ip, port);
	}

	size_t
	getNetMaskBitCount() const {
		return getSockAddrNetMaskBitCount(&m_addr);
	}

	size_t
	getNetMaskBitCount_ip4() const {
		return getSockAddrNetMaskBitCount_ip4(&m_addr_ip4);
	}

	size_t
	getNetMaskBitCount_ip6() const {
		return getSockAddrNetMaskBitCount_ip6(&m_addr_ip6);
	}

	void
	createNetMask(
		uint_t family,
		size_t bitCount
	);

	void
	createNetMask_ip4(size_t bitCount) {
		createSockAddrNetMask_ip4(&m_addr_ip4, bitCount);
	}

	void
	createNetMask_ip6(size_t bitCount) {
		createSockAddrNetMask_ip6(&m_addr_ip6, bitCount);
	}

	bool
	parse_ip4(const sl::StringRef& string) {
		return parseSockAddr_ip4(&m_addr_ip4, string);
	}

	bool
	parse_ip6(const sl::StringRef& string) {
		return parseSockAddr_ip6(&m_addr_ip6, string);
	}

	size_t
	getString_ip4(sl::String* string) const {
		return getSockAddrString_ip4(string, &m_addr_ip4);
	}

	sl::String
	getString_ip4() const {
		return getSockAddrString_ip4(&m_addr_ip4);
	}

	size_t
	getString_ip6(sl::String* string) const {
		return getSockAddrString_ip6(string, &m_addr_ip6);
	}

	sl::String
	getString_ip6() const {
		return getSockAddrString_ip6(&m_addr_ip6);
	}
};

#if (_AXL_IO_SOCKADDR_UN)

//..............................................................................

// Unix sockets

struct UnixSockAddrStorage {
	union {
		sockaddr m_addr;
		sockaddr_un m_addr_un;
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UnixSockAddr: SockAddrBase<UnixSockAddrStorage> {
	UnixSockAddr() {}

	UnixSockAddr(const sockaddr* addr) {
		setup(addr);
	}

	UnixSockAddr(const sl::StringRef& path) {
		parse(path);
	}

	operator const sockaddr_un* () const {
		return &m_addr_un;
	}

	void
	setup(const sockaddr* addr);

	bool
	parse(const sl::StringRef& path) { // the whole string is the path
		return parseSockAddr_un(&m_addr_un, path);
	}
};

#endif

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
) {
	sl::Array<SockAddr> addrArray;
	resolveHostName(&addrArray, name, family);
	return addrArray;
}

//..............................................................................

} // namespace io
} // namespace axl
