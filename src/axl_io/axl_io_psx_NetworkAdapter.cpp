#include "pch.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_err_Error.h"
#include "axl_rtl_StringHashTable.h"
#include "axl_rtl_BitMap.h"
#include "axl_io_Socket.h"

namespace axl {
namespace io {

//.............................................................................

class NetworkAdapterEnumerator
{
public:
	static
	size_t
	buildAdapterList (rtl::StdList <NetworkAdapter>* adapterList);

protected:
	static
	void
	setupAdapter (
		NetworkAdapter* adapter,
		ifaddrs* iface
		);

	static
	void
	addAdapterAddress (
		NetworkAdapter* adapter,
		const sockaddr* addr,
		const sockaddr* netMask
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
NetworkAdapterEnumerator::buildAdapterList (rtl::StdList <NetworkAdapter>* adapterList)
{
	adapterList->clear ();

	ifaddrs* ifaceAddressList = NULL;
	int result = getifaddrs (&ifaceAddressList);
	if (result != 0 || !ifaceAddressList)
	{
		err::setLastSystemError ();
		return -1;
	}

	rtl::StringHashTableMap <NetworkAdapter*> adapterMap;

	for (ifaddrs* iface = ifaceAddressList; iface; iface = iface->ifa_next)
	{
		if (!(iface->ifa_flags & IFF_UP))
			continue;

		rtl::StringHashTableMapIterator <NetworkAdapter*> it = adapterMap.visit (iface->ifa_name);
		if (it->m_value)
		{
			if (iface->ifa_addr)
				addAdapterAddress (it->m_value, iface->ifa_addr, iface->ifa_netmask);

			continue;
		}

		NetworkAdapter* adapter = AXL_MEM_NEW (NetworkAdapter);
		setupAdapter (adapter, iface);
		adapterList->insertTail (adapter);

		it->m_value = adapter;
	}

	freeifaddrs (ifaceAddressList);
	return adapterList->getCount ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
NetworkAdapterEnumerator::setupAdapter (
	NetworkAdapter* adapter,
	ifaddrs* iface
	)
{
	adapter->m_adapterKind =
		(iface->ifa_flags & IFF_LOOPBACK) ? NetworkAdapterKind_Loopback :
		(iface->ifa_flags & IFF_POINTOPOINT) ? NetworkAdapterKind_Ppp :
		NetworkAdapterKind_Ethernet;

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
	NetworkAdapter* adapter,
	const sockaddr* addr,
	const sockaddr* netMask
	)
{
	NetworkAdapterAddress* address = AXL_MEM_NEW (NetworkAdapterAddress);
	address->m_address = *addr;
	address->m_netMaskBitCount = rtl::setB*netMask;
	adapter->m_addressList.insertTail (address);	
}

//.............................................................................

size_t
buildNetworkAdapterList (rtl::StdList <NetworkAdapter>* adapterList)
{
	return NetworkAdapterEnumerator::buildAdapterList (adapterList);
}

//.............................................................................

} // namespace axl {
} // namespace io {
