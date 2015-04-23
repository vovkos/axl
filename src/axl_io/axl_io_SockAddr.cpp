#include "pch.h"
#include "axl_io_SockAddr.h"
#include "axl_io_SockAddrParser.h"
#include "axl_err_Error.h"
#include "axl_rtl_BitMap.h"

namespace axl {
namespace io {

//.............................................................................

const char* 
getSockAddrFamilyString (uint_t family)
{
	switch (family)
	{
	case AF_INET:
		return "IPv4";

	case AF_INET6:
		return "IPv6";

	default:
		return "unknown-sock-addr-family";
	}
}

size_t
getSockAddrFamilySize (uint_t family)
{
	switch (family)
	{
	case AF_INET:
		return sizeof (sockaddr_in);

	case AF_INET6:
		return sizeof (sockaddr_in6);

	default:
		return sizeof (sockaddr);
	}
}

const char* 
getSockProtoString (uint_t proto)
{
	switch (proto)
	{
	case IPPROTO_ICMP:
		return "ICMP";

	case IPPROTO_UDP:
		return "UDP";

	case IPPROTO_TCP:
		return "TCP";

	default:
		return "unknown-sock-proto";
	}
}

//.............................................................................

size_t
getSockAddrNetMaskBitCount_ip4 (const sockaddr_in* addr)
{
	size_t bitIdx = rtl::findBit ((size_t*) &addr->sin_addr, 1, 0, false);
	return AXL_MIN (bitIdx, 32);
}

size_t
getSockAddrNetMaskBitCount_ip6 (const sockaddr_in6* addr)
{
	size_t bitIdx = rtl::findBit (
		(size_t*) &addr->sin6_addr,
		sizeof (addr->sin6_addr) / sizeof (size_t),
		0, 
		false
		);

	return AXL_MIN (bitIdx, 128);
}

size_t
getSockAddrNetMaskBitCount (const sockaddr* addr)
{
	switch (addr->sa_family)
	{
	case AF_INET:
		return getSockAddrNetMaskBitCount_ip4 ((const sockaddr_in*) addr);

	case AF_INET6:
		return getSockAddrNetMaskBitCount_ip6 ((const sockaddr_in6*) addr);

	default:
		return 0;
	}
}

//.............................................................................

void
createSockAddrNetMask_ip4 (
	sockaddr_in* addr,
	size_t bitCount
	)
{
	memset (addr, 0, sizeof (sockaddr_in));
	addr->sin_family = AF_INET;
	
	size_t maxBitCount = sizeof (addr->sin_addr) * 8;
	if (bitCount > maxBitCount)
		bitCount = maxBitCount;

	rtl::setBitRange ((size_t*) &addr->sin_addr, 1, 0, bitCount);
}

void
createSockAddrNetMask_ip6 (
	sockaddr_in6* addr,
	size_t bitCount
	)
{
	memset (addr, 0, sizeof (sockaddr_in6));
	addr->sin6_family = AF_INET6;
	
	size_t maxBitCount = sizeof (addr->sin6_addr) * 8;
	if (bitCount > maxBitCount)
		bitCount = maxBitCount;

	rtl::setBitRange (
		(size_t*) &addr->sin6_addr, 
		sizeof (addr->sin6_addr) / sizeof (size_t), 
		0,
		bitCount
		);
}

//.............................................................................

bool
parseAddr_ip4 (
	in_addr* addr,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parse (addr);
}

bool
parseAddr_ip6 (
	in6_addr* addr,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parse (addr);
}

bool
parseSockAddr_ip4 (
	sockaddr_in* addr,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parse (addr);
}

bool
parseSockAddr_ip6 (
	sockaddr_in6* addr,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parse (addr);
}

bool
parseSockAddr (
	sockaddr* addr,
	size_t size,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parse (addr, size);
}

//.............................................................................

size_t
getAddrString_ip4 (
	rtl::String* string,
	const in_addr* addr
	)
{
	const uchar_t* ip = (const uchar_t*) addr;
	return string->format ("%d.%d.%d.%d", ip [0], ip [1], ip [2], ip [3]);
}

size_t
getAddrString_ip6 (
	rtl::String* string,
	const in6_addr* addr
	)
{
	string->clear ();
	
	const uint16_t* ip = (const uint16_t*) addr;
	
	size_t zeroRunIdx = -1;
	size_t zeroRunLength = 0;
	size_t maxZeroRunIdx = -1;
	size_t maxZeroRunLength = 1; // ignore single zeros

	for (size_t i = 0; i < 8; i++)
	{
		if (!zeroRunLength)
		{
			if (!ip [i])
			{
				zeroRunIdx = i;
				zeroRunLength = 1;
			}
		}
		else
		{
			if (!ip [i])
			{
				zeroRunLength++;
			}
			else 
			{
				if (zeroRunLength > maxZeroRunLength)
				{
					maxZeroRunIdx = zeroRunIdx;
					maxZeroRunLength = zeroRunLength;
				}

				zeroRunLength = 0;
			}
		}
	}

	if (zeroRunLength > maxZeroRunLength)
	{
		maxZeroRunIdx = zeroRunIdx;
		maxZeroRunLength = zeroRunLength;
	}

	if (maxZeroRunIdx == -1)
	{
		string->appendFormat ("%x", rtl::swapByteOrder16 (ip [0]));

		for (size_t i = 1; i < 8; i++)
			string->appendFormat (":%x", rtl::swapByteOrder16 (ip [i]));
	}
	else
	{
		bool isIp4 = false;

		if (!maxZeroRunIdx)
		{
			if (maxZeroRunLength == 6 && ip [6] != 0)
			{
				string->append ("::", 2);
				isIp4 = true;
			}
#if (_AXL_ENV == AXL_ENV_WIN) // try to comply with WSAAddressToStringA...
			else if (maxZeroRunLength == 5 && ip [5] == 0xffff && ip [6] != 0)
#else // ...or with inet_ntop
			else if (maxZeroRunLength == 5 && ip [5] == 0xffff)
#endif
			{
				string->append ("::ffff:", 7);
				isIp4 = true;
			}
			else
			{
				string->append (':');
			}
		}
		else
		{
			for (size_t i = 0; i < maxZeroRunIdx; i++)
				string->appendFormat ("%x:", rtl::swapByteOrder16 (ip [i]));
		}

		if (isIp4)
		{
			string->appendFormat (
				"%d.%d.%d.%d", 
				ip [6] & 0xff, 
				(ip [6] >> 8) & 0xff, 
				ip [7] & 0xff,
				(ip [7] >> 8) & 0xff
				);
		}
		else
		{
			size_t maxZeroRunEndIdx = maxZeroRunIdx + maxZeroRunLength;
			if (maxZeroRunEndIdx >= 8)
			{
				string->append (':');
			}
			else
			{
				for (size_t i = maxZeroRunEndIdx; i < 8; i++)
					string->appendFormat (":%x", rtl::swapByteOrder16 (ip [i]));
			}
		}
	}

	return string->getLength ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
getSockAddrString_ip4 (
	rtl::String* string,
	const sockaddr_in* addr
	)
{
	getAddrString_ip4 (string, &addr->sin_addr);

	if (addr->sin_port)
		string->appendFormat (":%d", rtl::swapByteOrder16 (addr->sin_port));

	return string->getLength ();
}

size_t
getSockAddrString_ip6 (
	rtl::String* string,
	const sockaddr_in6* addr
	)
{
	getAddrString_ip6 (string, &addr->sin6_addr);

	if (addr->sin6_scope_id)
		string->appendFormat ("%%%d", addr->sin6_scope_id);

	if (addr->sin6_port)
	{
		string->insert (0, '[');
		string->appendFormat ("]:%d", rtl::swapByteOrder16 (addr->sin6_port));
	}

	return string->getLength ();
}

size_t
getSockAddrString (
	rtl::String* string,
	const sockaddr* addr
	)
{
	switch (addr->sa_family)
	{
	case AF_INET:
		return getSockAddrString_ip4 (string, (const sockaddr_in*) addr);

	case AF_INET6:
		return getSockAddrString_ip6 (string, (const sockaddr_in6*) addr);

	default:
		string->clear ();
		return 0;
	}
}

//.............................................................................

void
SockAddr::setup (const sockaddr* addr)
{
	switch (addr->sa_family)
	{
	case AF_INET:
		m_addr_ip4 = *(sockaddr_in*) addr;
		break;
	
	case AF_INET6:
		m_addr_ip6 = *(sockaddr_in6*) addr;
		break;

	default:
		m_addr = *addr;
	}
}

void
SockAddr::setup (
	uint_t family,
	const void* addr,
	size_t size
	)
{
	switch (family)
	{
	case AF_INET:
		if (size >= sizeof (in_addr))
			setup_ip4 ((const in_addr*) addr);
		break;
	
	case AF_INET6:
		if (size >= sizeof (in6_addr))
			setup_ip6 ((const in6_addr*) addr);
		break;
	}
}

void
SockAddr::setup_ip4 (
	const in_addr* addr,
	uint_t port
	)
{
	m_addr_ip4.sin_family = AF_INET;
	m_addr_ip4.sin_addr = *addr;
	m_addr_ip4.sin_port = rtl::swapByteOrder16 (port);
	memset (m_addr_ip4.sin_zero, 0, sizeof (m_addr_ip4.sin_zero));
}

void
SockAddr::setup_ip6 (
	const in6_addr* addr,
	uint_t port,
	uint_t scope
	)
{
	m_addr_ip6.sin6_family = AF_INET6;
	m_addr_ip6.sin6_addr = *addr;
	m_addr_ip6.sin6_port = rtl::swapByteOrder16 (port);
	m_addr_ip6.sin6_flowinfo = 0;
	m_addr_ip6.sin6_scope_id = scope;
}

void
SockAddr::createNetMask (
	uint_t family,
	size_t bitCount
	)
{
	switch (family)
	{
	case AF_INET:
		createNetMask_ip4 (bitCount);
		break;

	case AF_INET6:
		createNetMask_ip6 (bitCount);
		break;

	default:
		clear ();
		m_addr.sa_family = family;
	}
}

//.............................................................................

bool 
resolveHostName (
	rtl::Array <SockAddr>* addrArray,
	const char* name,
	uint_t family
	)
{
	addrArray->clear ();

	addrinfo hintAddrInfo = { 0 };
	hintAddrInfo.ai_family = family;

	addrinfo* addrInfoList;
	int result = getaddrinfo (name, NULL, &hintAddrInfo, &addrInfoList);
	if (result)
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		err::setLastSystemError ();
#else
		err::setLastError (gai_strerror (result));
#endif
		return false;
	}

	if (!addrInfoList)
	{
		err::setError (err::SystemErrorCode_InvalidParameter);
		return false;
	}

	addrinfo* addrInfo = addrInfoList;
	addrArray->reserve (addrInfo->ai_next ? 4 : 1);

	size_t i = 0;
	for (; addrInfo; addrInfo = addrInfo->ai_next, i++)
		addrArray->append (addrInfo->ai_addr);

	freeaddrinfo (addrInfoList);
	return true;
}

//.............................................................................

} // namespace io
} // namespace axl
