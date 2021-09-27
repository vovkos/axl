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
#include "axl_enc_HexEncoding.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getNetworkAdapterTypeString(NetworkAdapterType adapterKind) {
	static const char* stringTable[NetworkAdapterType__Count] = {
		"unknown-adapter-kind", // NetworkAdapterType_Unknown,
		"loopback",             // NetworkAdapterType_Loopback,
		"ethernet",             // NetworkAdapterType_Ethernet,
		"wireless",             // NetworkAdapterType_Wireless,
		"PPP",                  // NetworkAdapterType_Ppp,
		"token ring",           // NetworkAdapterType_TokenRing,
		"slip",                 // NetworkAdapterType_Slip,
		"tunnel",               // NetworkAdapterType_Tunnel,
	};

	return (size_t)adapterKind < countof(stringTable) ?
		stringTable[adapterKind] :
		stringTable[NetworkAdapterType_Unknown];
}

//..............................................................................

const char*
getNetworkAdapterFlagString(NetworkAdapterFlag flag) {
	static const char* stringTable[] = {
		"DHCP",      // NetworkAdapterFlag_Dhcp      = 0x01,
		"DDNS",      // NetworkAdapterFlag_Ddns      = 0x02,
		"broadcast", // NetworkAdapterFlag_Broadcast = 0x04,
		"multicast", // NetworkAdapterFlag_Multicast = 0x08,
		"IPv4",      // NetworkAdapterFlag_Ip4       = 0x10,
		"IPv6",      // NetworkAdapterFlag_Ip6       = 0x20,
	};

	size_t i = sl::getLoBitIdx32(flag);
	return i < countof(stringTable) ?
		stringTable[i] :
		"undefined-adapter-flag";
}

sl::String
getNetworkAdapterFlagString(uint_t flags) {
	if (!flags)
		return sl::String();

	NetworkAdapterFlag flag = getFirstNetworkAdapterFlag(flags);
	sl::String string = getNetworkAdapterFlagString(flag);
	flags &= ~flag;

	while (flags) {
		flag = getFirstNetworkAdapterFlag(flags);

		string += ' ';
		string += getNetworkAdapterFlagString(flag);

		flags &= ~flag;
	}

	return string;
}

//..............................................................................

size_t
getMacAddressString(
	sl::String* string,
	const uchar_t* macAddress,
	uint_t flags
) {
	return string->format(
		(flags & enc::HexEncodingFlag_NoSpace) ?
			(flags & enc::HexEncodingFlag_UpperCase) ?
				"%02X%02X%02X%02X%02X%02X" :
				"%02x%02x%02x%02x%02x%02x" :
			(flags & enc::HexEncodingFlag_UpperCase) ?
				"%02X:%02X:%02X:%02X:%02X:%02X" :
				"%02x:%02x:%02x:%02x:%02x:%02x",
		macAddress[0],
		macAddress[1],
		macAddress[2],
		macAddress[3],
		macAddress[4],
		macAddress[5]
	);
}

//..............................................................................

NetworkAdapterDesc::NetworkAdapterDesc() {
	m_type = NetworkAdapterType_Unknown;
	m_flags = 0;
	memset(m_macAddress, 0, sizeof(m_macAddress));
}

//..............................................................................

} // namespace io
} // namespace axl
