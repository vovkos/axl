#include "pch.h"
#include "axl_io_SockAddrParser.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

void
SockAddrParser::initialize (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = p ? strlen (p) : 0;

	m_p = p;
	m_end = p + length;
}

bool
SockAddrParser::parse (
	sockaddr* addr,
	size_t size
	)
{
	skipWhiteSpace ();

	SockAddrParser clone = *this;

	if (tryIp4 ())
	{
		if (size < sizeof (sockaddr_in))
		{
			err::setError (err::SystemErrorCode_BufferTooSmall);
			return false;
		}

		*this = clone;
		return parseIp4 ((sockaddr_in*) addr);
	}

	*this = clone;

	if (tryIp6 ())
	{
		if (size < sizeof (sockaddr_in6))
		{
			err::setError (err::SystemErrorCode_BufferTooSmall);
			return false;
		}

		*this = clone;
		return parseIp6 ((sockaddr_in6*) addr);
	}

	err::setError (err::SystemErrorCode_InvalidAddress);
	return false;
}

bool
SockAddrParser::tryIp4 ()
{
	bool result;

	for (size_t i = 0;; i++)
	{
		result = tryInt (10);
		if (!result)
			return false;

		if (i >= 3)
			break;

		result = tryChar ('.');
		if (!result)
			return false;
	}

	result = tryChar (':');
	if (result)
	{
		result = tryInt (10);
		if (!result)
			return false;
	}

	return true;
}

bool
SockAddrParser::parseIp4 (sockaddr_in* addr)
{
	bool result;

	memset (addr, 0, sizeof (sockaddr_in));
	addr->sin_family = AF_INET;

	uchar_t* ip = (uchar_t*) &addr->sin_addr;
	for (size_t i = 0;; i++)
	{
		uint_t octet;
		result = parseInt (&octet, 10);
		if (!result)
			return false;

		ip [i] = octet;

		if (i >= 3)
			break;

		result = expectChar ('.');
		if (!result)
			return false;
	}

	result = tryChar (':');
	if (result)
	{
		uint_t port;
		result = parseInt (&port, 10);
		if (!result)
			return false;

		addr->sin_port = rtl::swapByteOrder16 ((uint16_t) port);
	}

	return true;
}

bool
SockAddrParser::tryIp6 ()
{
	bool result;
	bool hasBrackets = tryChar ('[');

	tryChar (':');

	bool isIp4 = false;

	for (size_t i = 0; i < 8; i++)
	{
		tryChar (':');

		skipWhiteSpace ();
		
		if (m_p >= m_end || !isxdigit (*m_p))
			break;

		result = tryInt (16);
		if (!result)
			return false;

		result = tryChar (':');
		if (!result)
		{
			result = tryChar ('.');
			if (result)
			{
				if (i > 6)
					return false;

				isIp4 = true;
				break;
			}
		}
	}

	if (isIp4)
	{
		for (size_t j = 1;; j++)
		{
			result = tryInt (10);
			if (!result)
				return false;

			if (j >= 3)
				break;

			result = tryChar ('.');
			if (!result)
				return false;
		}
	}

	result = tryChar ('%');
	if (result)
	{
		result = tryInt (10);
		if (!result)
			return false;
	}

	if (hasBrackets)
	{
		result = tryChar (']');
		if (!result)
			return false;

		result = tryChar (':');
		if (result)
		{
			result = tryInt (10);
			if (!result)
				return false;
		}
	}

	return true;
}

bool
SockAddrParser::parseIp6 (sockaddr_in6* addr)
{
	bool result;

	memset (addr, 0, sizeof (sockaddr_in6));
	addr->sin6_family = AF_INET6;

	bool hasBrackets = tryChar ('[');

	uint16_t* ip = (uint16_t*) &addr->sin6_addr;

	size_t zeroRunIdx = -1;

	tryChar (':');

	bool isIp4 = false;
	uchar_t ip4 [4];

	size_t i = 0;
	while (i < 8)
	{
		result = tryChar (':');
		if (result)
			zeroRunIdx = i;

		skipWhiteSpace ();
		
		if (m_p >= m_end || !isxdigit (*m_p))
			break;

		uint_t word;		
		result = parseInt (&word, 16);
		if (!result)
			return false;

		result = tryChar (':');
		if (!result)
		{
			result = tryChar ('.');
			if (result)
			{
				if (i > 6)
				{
					err::setError (err::SystemErrorCode_InvalidAddress);
					return false;
				}

				ip4 [0] = (uchar_t) 
					((word & 0x0f) + 
					((word & 0xf0) >> 4) * 10 + 
					((word & 0xf00) >> 8) * 100);

				isIp4 = true;
				break;
			}
		}

		ip [i] = rtl::swapByteOrder16 ((uint16_t) word);
		i++;

		if (!result)
			break;
	}

	if (isIp4)
	{
		for (size_t j = 1;; j++)
		{
			uint_t octet;
			result = parseInt (&octet, 10);
			if (!result)
				return false;

			ip4 [j] = (uchar_t) octet;

			if (j >= 3)
				break;

			result = expectChar ('.');
			if (!result)
				return false;
		}

		ASSERT (i <= 6);
		ip [i] = ip4 [0] + (ip4 [1] << 8);
		ip [i + 1] = ip4 [2] + (ip4 [3] << 8);
		i += 2;
	}

	if (zeroRunIdx != -1)
	{
		ASSERT (i <= 8 && zeroRunIdx <= i);

		size_t zeroRunLength = 8 - i;

		if (zeroRunIdx < i)
		{
			memmove (&ip [zeroRunIdx + zeroRunLength], &ip [zeroRunIdx], (i - zeroRunIdx) * sizeof (uint16_t));
			memset (&ip [zeroRunIdx], 0, zeroRunLength * sizeof (uint16_t));
		}
	}

	result = tryChar ('%');
	if (result)
	{
		uint_t scope;
		result = parseInt (&scope, 10);
		if (!result)
			return false;

		addr->sin6_scope_id = scope;
	}

	if (hasBrackets)
	{
		result = expectChar (']');
		if (!result)
			return false;

		result = tryChar (':');
		if (result)
		{
			uint_t port;
			result = parseInt (&port, 10);
			if (!result)
				return false;

			addr->sin6_port = rtl::swapByteOrder16 ((uint16_t) port);
		}
	}

	return true;
}

void
SockAddrParser::skipWhiteSpace ()
{
	while (m_p < m_end && isspace (*m_p)) 
		m_p++;
}

bool
SockAddrParser::parseInt (
	uint_t* p,
	int radix
	)
{
	skipWhiteSpace ();

	char* end;
	*p = (uint_t) strtoul (m_p, &end, radix);

	if (end == m_p)
	{
		err::setError (err::SystemErrorCode_InvalidAddress);
		return false;
	}

	m_p = end;
	return true;
}

bool
SockAddrParser::tryInt (int radix)
{
	skipWhiteSpace ();

	const char* p = m_p;

	if (radix == 16)
	{
		while (p < m_end && isxdigit (*p))
			p++;
	}
	else
	{
		while (p < m_end && isdigit (*p))
			p++;
	}

	if (m_p == p)
		return false;

	m_p = p;
	return true;
}

bool
SockAddrParser::expectChar (char c)
{
	bool result = tryChar (c);

	if (!result)
	{
		err::setError (err::SystemErrorCode_InvalidAddress);
		return false;
	}

	return true;
}

bool
SockAddrParser::tryChar (char c)
{
	skipWhiteSpace ();

	if (m_p >= m_end || *m_p != c)
		return false;

	m_p++;
	return true;
}

//.............................................................................

} // namespace io
} // namespace axl
