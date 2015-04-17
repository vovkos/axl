#include "pch.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_err_Error.h"
#include "axl_rtl_Array.h"

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
		IP_ADAPTER_ADDRESSES* srcAdapter
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
NetworkAdapterEnumerator::buildAdapterList (rtl::StdList <NetworkAdapter>* adapterList)
{
	adapterList->clear ();

	ulong_t size = 15 * 1024; // Microsoft recommends starting with 15K

	rtl::Array <char> buffer;
	buffer.setCount (size); 

	IP_ADAPTER_ADDRESSES* srcAdapter = (IP_ADAPTER_ADDRESSES*) buffer.a ();

	ulong_t flags = 
		GAA_FLAG_SKIP_ANYCAST |
		GAA_FLAG_SKIP_MULTICAST |
		GAA_FLAG_SKIP_DNS_SERVER;

	for (;;)
	{
		ulong_t error = ::GetAdaptersAddresses (AF_UNSPEC, flags, NULL, srcAdapter, &size);
		if (error == ERROR_SUCCESS)
			break;

		if (error != ERROR_BUFFER_OVERFLOW)
		{
			err::setError (error);
			return -1;
		}
	}

	for (; srcAdapter; srcAdapter = srcAdapter->Next)
	{
		NetworkAdapter* adapter = AXL_MEM_NEW (NetworkAdapter);
		setupAdapter (adapter, srcAdapter);
		adapterList->insertTail (adapter);
	}

	return adapterList->getCount ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
NetworkAdapterEnumerator::setupAdapter (
	NetworkAdapter* adapter,
	IP_ADAPTER_ADDRESSES* srcAdapter
	)
{
	switch (srcAdapter->IfType)
	{
	case IF_TYPE_SOFTWARE_LOOPBACK:
		adapter->m_adapterKind = NetworkAdapterKind_Loopback;
		break;

	case IF_TYPE_ETHERNET_CSMACD:
		adapter->m_adapterKind = NetworkAdapterKind_Ethernet;
		break;

	case IF_TYPE_IEEE80211:
		adapter->m_adapterKind = NetworkAdapterKind_Wireless;
		break;

	case IF_TYPE_PPP:
		adapter->m_adapterKind = NetworkAdapterKind_Ppp;
		break;

	case IF_TYPE_ISO88025_TOKENRING:
		adapter->m_adapterKind = NetworkAdapterKind_TokenRing;
		break;

	case IF_TYPE_SLIP:
		adapter->m_adapterKind = NetworkAdapterKind_Slip;
		break;

	case IF_TYPE_TUNNEL:
		adapter->m_adapterKind = NetworkAdapterKind_Tunnel;
		break;

	default:
		adapter->m_adapterKind = NetworkAdapterKind_Unknown;
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
	memcpy (adapter->m_mac, srcAdapter->PhysicalAddress, 6);
	
	IP_ADAPTER_UNICAST_ADDRESS* srcAddress = srcAdapter->FirstUnicastAddress;
	for (; srcAddress; srcAddress = srcAddress->Next)
	{
		if (!srcAddress->Address.lpSockaddr || srcAddress->Address.iSockaddrLength > sizeof (io::SockAddr))
			continue;

		NetworkAdapterAddress* address = AXL_MEM_NEW (NetworkAdapterAddress);		
		memcpy (&address->m_address, srcAddress->Address.lpSockaddr, srcAddress->Address.iSockaddrLength);

		if (srcAddress->Length >= sizeof (IP_ADAPTER_UNICAST_ADDRESS)) 
			address->m_netMaskBitCount = srcAddress->OnLinkPrefixLength;

		adapter->m_addressList.insertTail (address);
	}
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
