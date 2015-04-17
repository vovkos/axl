// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_List.h"
#include "axl_rtl_String.h"
#include "axl_io_SockAddr.h"

#define _AXL_IO_NETWORKADAPTER_H

namespace axl {
namespace io {

class NetworkAdapterEnumerator;

//.............................................................................

enum NetworkAdapterKind
{
	NetworkAdapterKind_Unknown,
	NetworkAdapterKind_Loopback,
	NetworkAdapterKind_Ethernet,
	NetworkAdapterKind_Wireless,
	NetworkAdapterKind_Ppp,
	NetworkAdapterKind_TokenRing,
	NetworkAdapterKind_Slip,
	NetworkAdapterKind_Tunnel,
	NetworkAdapterKind__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* 
getNetworkAdapterKindString (NetworkAdapterKind adapterKind);

//.............................................................................

enum NetworkAdapterFlag
{
	NetworkAdapterFlag_Dhcp      = 0x01,
	NetworkAdapterFlag_Ddns      = 0x02,
	NetworkAdapterFlag_Broadcast = 0x04,
	NetworkAdapterFlag_Multicast = 0x08,
	NetworkAdapterFlag_Ip4       = 0x10,
	NetworkAdapterFlag_Ip6       = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
NetworkAdapterFlag
getFirstNetworkAdapterFlag (uint_t flags)
{
	return (NetworkAdapterFlag) (1 << rtl::getLoBitIdx (flags));
}

const char* 
getNetworkAdapterFlagString (NetworkAdapterFlag flag);

rtl::String
getNetworkAdapterFlagString (uint_t flags);

//.............................................................................

struct NetworkAdapterAddress: rtl::ListLink
{
	SockAddr m_address;
	size_t m_netMaskBitCount;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class NetworkAdapter: public rtl::ListLink
{
	friend class NetworkAdapterEnumerator;

protected:
	NetworkAdapterKind m_adapterKind;
	uint_t m_flags;
	rtl::String m_name;
	rtl::String m_description;
	uchar_t m_mac [6];
	rtl::StdList <NetworkAdapterAddress> m_addressList;

public:
	NetworkAdapter ();

	NetworkAdapterKind 
	getAdapterKind () const
	{
		return m_adapterKind;
	}
	
	uint_t 
	getFlags () const
	{
		return m_flags;
	}

	rtl::String 
	getName ()
	{
		return m_name;
	}

	rtl::String 
	getDescription ()
	{
		return m_description;
	}

	const uchar_t* 
	getMac () const
	{
		return m_mac;
	}

	rtl::ConstList <NetworkAdapterAddress> 
	getAddressList ()
	{
		return m_addressList;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
buildNetworkAdapterList (rtl::StdList <NetworkAdapter>* adapterList);

//.............................................................................

} // namespace io
} // namespace axl
