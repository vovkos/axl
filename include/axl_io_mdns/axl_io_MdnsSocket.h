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

#define _AXL_IO_MDNSSOCKET_H

#include "axl_io_MdnsRecord.h"

namespace axl {
namespace io {

//..............................................................................

// 224.0.0.251 / ff02::fb -- mdns.h open-codes both rather than naming them

extern AXL_SELECT_ANY const uint_t g_mdnsGroupIp4 = 0xe00000fb;

extern AXL_SELECT_ANY const in6_addr g_mdnsGroupIp6 = {{{
	0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb
}}};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
setupMdnsSocketOptions(
	Socket* socket,
	const sockaddr* ifaceAddr
);

inline
bool
setupMdnsSocketOptions(
	Socket* socket,
	uint_t family = AF_INET
) {
	union {
		sockaddr m_addr;
		sockaddr_in m_addr4;
		sockaddr_in6 m_addr6;
	} addr = { 0 };

	addr.m_addr.sa_family = family;
	return setupMdnsSocketOptions(socket, &addr.m_addr);
}

bool
setupMdnsServerSocket(
	Socket* socket,
	const sockaddr* ifaceAddr
);

inline
bool
setupMdnsServerSocket(
	Socket* socket,
	uint_t family = AF_INET
) {
	union {
		sockaddr m_addr;
		sockaddr_in m_addr4;
		sockaddr_in6 m_addr6;
	} addr = { 0 };

	addr.m_addr.sa_family = family;
	return setupMdnsServerSocket(socket, &addr.m_addr);
}

//..............................................................................

} // namespace io
} // namespace axl
