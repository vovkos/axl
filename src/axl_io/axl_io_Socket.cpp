#include "pch.h"
#include "axl_io_Socket.h"
#include "axl_io_SockAddrParser.h"

namespace axl {
namespace io {

//.............................................................................

bool
Socket::accept (
	Socket* socket,
	sockaddr* addr
	)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	SOCKET h = m_socket.accept (addr);
#else 
	int h = m_socket.accept (addr);
#endif

	if (h == -1)
		return false;

	socket->m_socket.attach (h);
	return true;
}

//.............................................................................

bool
parseSockAddr_ip4 (
	sockaddr_in* addr,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parseIp4 (addr);
}

bool
parseSockAddr_ip6 (
	sockaddr_in6* addr,
	const char* string,
	size_t length
	)
{
	SockAddrParser parser (string, length);
	return parser.parseIp6 (addr);
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
formatSockAddr_ip4 (
	rtl::String* string,
	const sockaddr_in* addr
	)
{
	const uchar_t* ip = (const uchar_t*) &addr->sin_addr;
	string->format ("%d.%d.%d.%d", ip [0], ip [1], ip [2], ip [3]);

	if (addr->sin_port)
		string->appendFormat (":%d", rtl::swapByteOrder16 (addr->sin_port));

	return string->getLength ();
}

size_t
formatSockAddr_ip6 (
	rtl::String* string,
	const sockaddr_in6* addr
	)
{
	if (addr->sin6_port)
		string->copy ('[');	
	else
		string->clear ();
	
	const uint16_t* ip = (const uint16_t*) &addr->sin6_addr;
	
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

	if (addr->sin6_scope_id)
		string->appendFormat ("%%%d", addr->sin6_scope_id);

	if (addr->sin6_port)
		string->appendFormat ("]:%d", rtl::swapByteOrder16 (addr->sin6_port));

	return string->getLength ();
}

size_t
formatSockAddr (
	rtl::String* string,
	const sockaddr* addr
	)
{
	switch (addr->sa_family)
	{
	case AF_INET:
		return formatSockAddr_ip4 (string, (const sockaddr_in*) addr);

	case AF_INET6:
		return formatSockAddr_ip6 (string, (const sockaddr_in6*) addr);

	default:
		string->clear ();

		err::setError (err::SystemErrorCode_InvalidParameter);
		return -1;
	}
}

//.............................................................................

} // namespace io
} // namespace axl
