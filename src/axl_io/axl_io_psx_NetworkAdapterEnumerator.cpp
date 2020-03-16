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
#include "axl_io_NetworkAdapter.h"
#include "axl_err_Error.h"
#include "axl_sl_StringHashTable.h"
#include "axl_sl_BitMap.h"
#include "axl_io_Socket.h"

#ifdef SIOCGIFHWADDR
#	define _AXL_IO_USE_SIOCGIFHWADDR 0
#endif

namespace axl {
namespace io {

//..............................................................................

class NetworkAdapterEnumerator
{
public:
	static
	size_t
	createAdapterList(sl::List<NetworkAdapterDesc>* adapterList);

protected:
	static
	void
	setupAdapter(
		NetworkAdapterDesc* adapter,
		ifaddrs* iface,
		io::psx::Socket* socket
		);

	static
	void
	addAdapterAddress(
		NetworkAdapterDesc* adapter,
		const sockaddr* addr,
		const sockaddr* netMask
		);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
NetworkAdapterEnumerator::createAdapterList(sl::List<NetworkAdapterDesc>* adapterList)
{
	adapterList->clear();

	ifaddrs* ifaceAddressList = NULL;
	int result = ::getifaddrs(&ifaceAddressList);
	if (result != 0 || !ifaceAddressList)
	{
		err::setLastSystemError();
		return -1;
	}

	io::psx::Socket socket;
#if (_AXL_IO_USE_SIOCGIFHWADDR)
	socket.open(PF_INET, SOCK_DGRAM, 0);
#endif

	sl::StringHashTable<NetworkAdapterDesc*> adapterMap;

	for (ifaddrs* iface = ifaceAddressList; iface; iface = iface->ifa_next)
	{
		if (!(iface->ifa_flags & IFF_UP))
			continue;

		sl::StringHashTableIterator<NetworkAdapterDesc*> it = adapterMap.visit(iface->ifa_name);
		if (!it->m_value)
		{
			NetworkAdapterDesc* adapter = AXL_MEM_NEW(NetworkAdapterDesc);
			setupAdapter(adapter, iface, &socket);
			adapterList->insertTail(adapter);
			it->m_value = adapter;
		}

		if (iface->ifa_addr)
			addAdapterAddress(it->m_value, iface->ifa_addr, iface->ifa_netmask);
	}

	::freeifaddrs(ifaceAddressList);
	return adapterList->getCount();
}

void
NetworkAdapterEnumerator::setupAdapter(
	NetworkAdapterDesc* adapter,
	ifaddrs* iface,
	io::psx::Socket* socket
	)
{
	adapter->m_type =
		(iface->ifa_flags & IFF_LOOPBACK) ? NetworkAdapterType_Loopback :
		(iface->ifa_flags & IFF_POINTOPOINT) ? NetworkAdapterType_Ppp :
		NetworkAdapterType_Ethernet;

	adapter->m_flags = 0;

	if (iface->ifa_flags & IFF_BROADCAST)
		adapter->m_flags |= NetworkAdapterFlag_Broadcast;

	if (iface->ifa_flags & IFF_MULTICAST)
		adapter->m_flags |= NetworkAdapterFlag_Multicast;

	adapter->m_name = iface->ifa_name;
	adapter->m_description = iface->ifa_name; // no special description

#if (_AXL_IO_USE_SIOCGIFHWADDR)
	if (socket->isOpen()) // try to get MAC-address
	{
		struct ifreq req;
		strncpy(req.ifr_name, iface->ifa_name, countof(req.ifr_name));
		int result = socket->ioctl(SIOCGIFHWADDR, &req);
		if (result >= 0)
			memcpy(adapter->m_macAddress, req.ifr_ifru.ifru_hwaddr.sa_data, sizeof(adapter->m_macAddress));
	}
#endif
}

void
NetworkAdapterEnumerator::addAdapterAddress(
	NetworkAdapterDesc* adapter,
	const sockaddr* addr,
	const sockaddr* netMask
	)
{
#if (_AXL_OS_DARWIN)
	if (addr->sa_family == AF_LINK)
	{
		const sockaddr_dl* sdl = (sockaddr_dl*)addr;
		memcpy(adapter->m_macAddress, LLADDR(sdl), sizeof(adapter->m_macAddress));
		return;
	}
#elif (_AXL_OS_LINUX)
	if (addr->sa_family == AF_PACKET)
	{
		const sockaddr_ll* sll = (sockaddr_ll*)addr;
		memcpy(adapter->m_macAddress, sll->sll_addr, sizeof(adapter->m_macAddress));
		return;
	}
#endif

	NetworkAdapterAddress* address = AXL_MEM_NEW(NetworkAdapterAddress);
	address->m_address.setup(addr);
	address->m_netMaskBitCount = netMask ? getSockAddrNetMaskBitCount(netMask) : 0;
	adapter->m_addressList.insertTail(address);
}

//..............................................................................

size_t
createNetworkAdapterDescList(sl::List<NetworkAdapterDesc>* adapterList)
{
	return NetworkAdapterEnumerator::createAdapterList(adapterList);
}

//..............................................................................

} // namespace io
} // namespace axl
