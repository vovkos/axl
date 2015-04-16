#include "pch.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

const char* 
getNetworkAdapterKindString (NetworkAdapterKind adapterKind)
{
	static const char* stringTable [NetworkAdapterKind__Count] = 
	{
		"unknown-adapter-kind", // NetworkAdapterKind_Unknown,
		"loopback",             // NetworkAdapterKind_Loopback,
		"ethernet",             // NetworkAdapterKind_Ethernet,
		"wireless",             // NetworkAdapterKind_Wireless,
		"PPP",                  // NetworkAdapterKind_Ppp,
		"token ring",           // NetworkAdapterKind_TokenRing,
		"slip",                 // NetworkAdapterKind_Slip,
		"tunnel",               // NetworkAdapterKind_Tunnel,
	};

	return (size_t) adapterKind < countof (stringTable) ? 
		stringTable [adapterKind] : 
		stringTable [NetworkAdapterKind_Unknown];
}

//.............................................................................

const char* 
getNetworkAdapterFlagString (NetworkAdapterFlag flag)
{
	static const char* stringTable [] =
	{
		"DHCP",      // NetworkAdapterFlag_Dhcp      = 0x01,
		"DDNS",      // NetworkAdapterFlag_Ddns      = 0x02,
		"multicast", // NetworkAdapterFlag_Multicast = 0x04,
		"IPv4",      // NetworkAdapterFlag_Ip4       = 0x10,
		"IPv6",      // NetworkAdapterFlag_Ip6       = 0x20,
	};

	size_t i = rtl::getLoBitIdx32 (flag);
	return i < countof (stringTable) ?
		stringTable [i] :
		"undefined-adapter-flag";
}

rtl::String
getNetworkAdapterFlagString (uint_t flags)
{
	if (!flags)
		return rtl::String ();

	NetworkAdapterFlag flag = getFirstNetworkAdapterFlag (flags);
	rtl::String string = getNetworkAdapterFlagString (flag);
	flags &= ~flag;

	while (flags)
	{
		flag = getFirstNetworkAdapterFlag (flags);

		string += ' ';
		string += getNetworkAdapterFlagString (flag);

		flags &= ~flag;
	}

	return string;
}

//.............................................................................

NetworkAdapter::NetworkAdapter ()
{
	m_adapterKind = NetworkAdapterKind_Unknown;
	m_flags = 0;
	memset (m_mac, 0, sizeof (m_mac));
}

//.............................................................................

} // namespace axl {
} // namespace io {
