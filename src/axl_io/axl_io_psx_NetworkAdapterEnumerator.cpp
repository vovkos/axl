#include "pch.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_err_Error.h"
#include "axl_sl_StringHashTable.h"
#include "axl_sl_BitMap.h"
#include "axl_io_Socket.h"

namespace axl {
namespace io {

//.............................................................................

class NetworkAdapterEnumerator
{
public:
	static
	size_t
	createAdapterList (sl::StdList <NetworkAdapterDesc>* adapterList);

protected:
	static
	void
	setupAdapter (
		NetworkAdapterDesc* adapter,
		ifaddrs* iface
		);

	static
	void
	addAdapterAddress (
		NetworkAdapterDesc* adapter,
		const sockaddr* addr,
		const sockaddr* netMask
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
NetworkAdapterEnumerator::createAdapterList (sl::StdList <NetworkAdapterDesc>* adapterList)
{
	adapterList->clear ();

	ifaddrs* ifaceAddressList = NULL;
	int result = getifaddrs (&ifaceAddressList);
	if (result != 0 || !ifaceAddressList)
	{
		err::setLastSystemError ();
		return -1;
	}

	sl::StringHashTableMap <NetworkAdapterDesc*> adapterMap;

	for (ifaddrs* iface = ifaceAddressList; iface; iface = iface->ifa_next)
	{
		if (!(iface->ifa_flags & IFF_UP))
			continue;

		sl::StringHashTableMapIterator <NetworkAdapterDesc*> it = adapterMap.visit (iface->ifa_name);
		if (it->m_value)
		{
			if (iface->ifa_addr)
				addAdapterAddress (it->m_value, iface->ifa_addr, iface->ifa_netmask);

			continue;
		}

		NetworkAdapterDesc* adapter = AXL_MEM_NEW (NetworkAdapterDesc);
		setupAdapter (adapter, iface);
		adapterList->insertTail (adapter);

		it->m_value = adapter;
	}

	freeifaddrs (ifaceAddressList);
	return adapterList->getCount ();
}

void
NetworkAdapterEnumerator::setupAdapter (
	NetworkAdapterDesc* adapter,
	ifaddrs* iface
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
}

void
NetworkAdapterEnumerator::addAdapterAddress (
	NetworkAdapterDesc* adapter,
	const sockaddr* addr,
	const sockaddr* netMask
	)
{
	NetworkAdapterAddress* address = AXL_MEM_NEW (NetworkAdapterAddress);
	address->m_address.setup (addr);
	address->m_netMaskBitCount = netMask ? getSockAddrNetMaskBitCount (netMask) : 0;
	adapter->m_addressList.insertTail (address);	
}

//.............................................................................

size_t
createNetworkAdapterDescList (sl::StdList <NetworkAdapterDesc>* adapterList)
{
	return NetworkAdapterEnumerator::createAdapterList (adapterList);
}

//.............................................................................

} // namespace io
} // namespace axl
