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
#include "axl_sl_Array.h"

namespace axl {
namespace io {

//..............................................................................

class NetworkAdapterEnumerator {
public:
	static
	size_t
	createAdapterList(sl::List<NetworkAdapterDesc>* adapterList);

protected:
	static
	void
	setupAdapter(
		NetworkAdapterDesc* adapter,
		IP_ADAPTER_ADDRESSES* srcAdapter
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
NetworkAdapterEnumerator::createAdapterList(sl::List<NetworkAdapterDesc>* adapterList) {
	adapterList->clear();

	ulong_t flags =
		GAA_FLAG_SKIP_ANYCAST |
		GAA_FLAG_SKIP_MULTICAST |
		GAA_FLAG_SKIP_DNS_SERVER;

	sl::Array<char> buffer;
	ulong_t size = 15 * 1024; // Microsoft recommends starting with 15K

	for (;;) {
		bool result = buffer.setCount(size);
		if (!result)
			return -1;

		ulong_t error = ::GetAdaptersAddresses(AF_UNSPEC, flags, NULL, (IP_ADAPTER_ADDRESSES*)buffer.p(), &size);
		if (error == ERROR_SUCCESS)
			break;

		if (error != ERROR_BUFFER_OVERFLOW) {
			err::setError(error);
			return -1;
		}
	}

	IP_ADAPTER_ADDRESSES* srcAdapter = (IP_ADAPTER_ADDRESSES*)buffer.p();
	for (; srcAdapter; srcAdapter = srcAdapter->Next) {
		NetworkAdapterDesc* adapter = AXL_MEM_NEW(NetworkAdapterDesc);
		setupAdapter(adapter, srcAdapter);
		adapterList->insertTail(adapter);
	}

	return adapterList->getCount();
}

void
NetworkAdapterEnumerator::setupAdapter(
	NetworkAdapterDesc* adapter,
	IP_ADAPTER_ADDRESSES* srcAdapter
) {
	switch (srcAdapter->IfType) {
	case IF_TYPE_SOFTWARE_LOOPBACK:
		adapter->m_type = NetworkAdapterType_Loopback;
		break;

	case IF_TYPE_ETHERNET_CSMACD:
		adapter->m_type = NetworkAdapterType_Ethernet;
		break;

	case IF_TYPE_IEEE80211:
		adapter->m_type = NetworkAdapterType_Wireless;
		break;

	case IF_TYPE_PPP:
		adapter->m_type = NetworkAdapterType_Ppp;
		break;

	case IF_TYPE_ISO88025_TOKENRING:
		adapter->m_type = NetworkAdapterType_TokenRing;
		break;

	case IF_TYPE_SLIP:
		adapter->m_type = NetworkAdapterType_Slip;
		break;

	case IF_TYPE_TUNNEL:
		adapter->m_type = NetworkAdapterType_Tunnel;
		break;

	default:
		adapter->m_type = NetworkAdapterType_Unknown;
	}

	adapter->m_flags = 0;

	if (srcAdapter->Flags & IP_ADAPTER_DHCP_ENABLED)
		adapter->m_flags |= NetworkAdapterFlag_Dhcp;

	if (srcAdapter->Flags & IP_ADAPTER_DDNS_ENABLED)
		adapter->m_flags |= NetworkAdapterFlag_Ddns;

	if (srcAdapter->Flags & IP_ADAPTER_IPV4_ENABLED)
		adapter->m_flags |= NetworkAdapterFlag_Ip4;

	if (srcAdapter->Flags & IP_ADAPTER_IPV6_ENABLED)
		adapter->m_flags |= NetworkAdapterFlag_Ip6;

	if (!(srcAdapter->Flags & IP_ADAPTER_NO_MULTICAST))
		adapter->m_flags |= NetworkAdapterFlag_Multicast;

	adapter->m_name = srcAdapter->AdapterName;
	adapter->m_description = srcAdapter->Description;
	memcpy(adapter->m_macAddress, srcAdapter->PhysicalAddress, 6);

	IP_ADAPTER_UNICAST_ADDRESS* srcAddress = srcAdapter->FirstUnicastAddress;
	for (; srcAddress; srcAddress = srcAddress->Next) {
		if (!srcAddress->Address.lpSockaddr || srcAddress->Address.iSockaddrLength > sizeof(io::SockAddr))
			continue;

		NetworkAdapterAddress* address = AXL_MEM_NEW(NetworkAdapterAddress);
		memcpy(&address->m_address, srcAddress->Address.lpSockaddr, srcAddress->Address.iSockaddrLength);

		if (srcAddress->Length >= sizeof(IP_ADAPTER_UNICAST_ADDRESS))
			address->m_netMaskBitCount = srcAddress->OnLinkPrefixLength;

		adapter->m_addressList.insertTail(address);
	}
}

//..............................................................................

size_t
enumerateNetworkAdapters(sl::List<NetworkAdapterDesc>* adapterList) {
	return NetworkAdapterEnumerator::createAdapterList(adapterList);
}

//..............................................................................

} // namespace io
} // namespace axl
