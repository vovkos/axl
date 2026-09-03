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

#include "pch.h"
#include "axl_io_MdnsSocket.h"

namespace axl {
namespace io {

//..............................................................................

// RFC 6762 (11): mDNS traffic goes out with TTL/hop-limit 255 -- queriers which
// verify it use that to tell the datagram did not cross a router

bool
setupMdnsSocketOptions(
	Socket* socket,
	const sockaddr* ifaceAddr
) {
	if (ifaceAddr->sa_family == AF_INET6) {
		uint_t hops = 255;
		uint_t loop = 1;
		uint_t scope = ((const sockaddr_in6*)ifaceAddr)->sin6_scope_id; // 0 = auto-select the interface

		return
			socket->setOption(IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops, sizeof(hops)) &&
			socket->setOption(IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loop, sizeof(loop)) &&
			socket->setOption(IPPROTO_IPV6, IPV6_MULTICAST_IF, &scope, sizeof(scope));
	} else {
		ASSERT(ifaceAddr->sa_family == AF_INET);

		uchar_t ttl = 255;
		uchar_t loop = 1;
		in_addr inaddr = ((const sockaddr_in*)ifaceAddr)->sin_addr; // 0 = auto-select the interface

		return
			socket->setOption(IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) &&
			socket->setOption(IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) &&
			socket->setOption(IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(inaddr));
	}
}

bool
setupMdnsServerSocket(
	Socket* socket,
	const sockaddr* ifaceAddr
) {
	bool result = setupMdnsSocketOptions(socket, ifaceAddr);
	if (!result)
		return false;

	int reuse = 1; // the port is already taken
	socket->setOption(SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#ifdef SO_REUSEPORT
	socket->setOption(SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
#endif

	if (ifaceAddr->sa_family == AF_INET6) {
		uint_t scope = ((const sockaddr_in6*)ifaceAddr)->sin6_scope_id;
		ipv6_mreq req = { 0 };
		req.ipv6mr_multiaddr = g_mdnsGroupIp6;
		req.ipv6mr_interface = scope;

		return
			socket->setOption(IPPROTO_IPV6, IPV6_JOIN_GROUP, &req, sizeof(req)) &&
			socket->bind(SockAddr(
#if (_AXL_OS_WIN)
				&((const sockaddr_in6*)ifaceAddr)->sin6_addr,
				MDNS_PORT,
				scope
#else
				&in6addr_any,
				MDNS_PORT,
				0
#endif
			));
	} else {
		ip_mreq req = { 0 };
		req.imr_multiaddr.s_addr = htonl(g_mdnsGroupIp4);
		req.imr_interface = ((const sockaddr_in*)ifaceAddr)->sin_addr;

		return
			socket->setOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, &req, sizeof(req)) &&
			socket->bind(SockAddr(
#if (_AXL_OS_WIN)
				&((const sockaddr_in*)ifaceAddr)->sin_addr,
#else
				(uint_t)0,
#endif
				MDNS_PORT
			));
	}
}

//..............................................................................

} // namespace io
} // namespace axl
