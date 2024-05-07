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
#include "axl_io_SockAddrParser.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//..............................................................................

bool
SockAddrParser::parse(in_addr* addr) {
	bool result;

	uchar_t* ip = (uchar_t*)addr;
	for (size_t i = 0;; i++) {
		uint_t octet;
		result = parseInt(&octet, 10);
		if (!result)
			return false;

		ip[i] = octet;

		if (i >= 3)
			break;

		result = expectChar('.');
		if (!result)
			return false;
	}

	return true;
}

bool
SockAddrParser::parse(sockaddr_in* addr) {
	bool result;

	memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_family = AF_INET;

	const char* p0 = m_p;
	result = parse(&addr->sin_addr);
	if (!result) { // rollback and try single port
		m_p = p0;

		uint_t port;
		result = parseInt(&port, 10);
		if (!result)
			return false;

		skipWhiteSpace();
		if (m_p != m_end) {
			err::setError(err::SystemErrorCode_InvalidAddress);
			return false;
		}

		*(uint32_t*) &addr->sin_addr = 0;
		addr->sin_port = sl::swapByteOrder16((uint16_t)port);
		return true;
	}

	result = tryChar(':');
	if (result) {
		uint_t port;
		result = parseInt(&port, 10);
		if (!result)
			return false;

		addr->sin_port = sl::swapByteOrder16((uint16_t)port);
	}

	return true;
}

bool
SockAddrParser::parse(in6_addr* addr) {
	bool result;

	uint16_t* ip = (uint16_t*)addr;

	size_t zeroRunIdx = -1;
	bool isIp4 = false;
	uchar_t ip4[4];

	tryChar(':');

	size_t i = 0;
	while (i < 8) {
		result = tryChar(':');
		if (result)
			zeroRunIdx = i;

		uint_t word;
		result = parseInt(&word, 16);
		if (!result)
			return false;

		result = tryChar(':');
		if (!result) {
			result = tryChar('.');
			if (result) {
				if (i > 6) {
					err::setError(err::SystemErrorCode_InvalidAddress);
					return false;
				}

				ip4[0] = (uchar_t)
					((word & 0x0f) +
					((word & 0xf0) >> 4) * 10 +
					((word & 0xf00) >> 8) * 100);

				isIp4 = true;
				break;
			}
		}

		ip[i] = sl::swapByteOrder16((uint16_t)word);
		i++;

		if (!result)
			break;
	}

	if (isIp4) {
		for (size_t j = 1;; j++) {
			uint_t octet;
			result = parseInt(&octet, 10);
			if (!result)
				return false;

			ip4[j] = (uchar_t)octet;

			if (j >= 3)
				break;

			result = expectChar('.');
			if (!result)
				return false;
		}

		ASSERT(i <= 6);
		ip[i] = ip4[0] + (ip4[1] << 8);
		ip[i + 1] = ip4[2] + (ip4[3] << 8);
		i += 2;
	}

	if (zeroRunIdx != -1) {
		ASSERT(i <= 8 && zeroRunIdx <= i);

		size_t zeroRunLength = 8 - i;

		if (zeroRunIdx < i) {
			memmove(&ip[zeroRunIdx + zeroRunLength], &ip[zeroRunIdx], (i - zeroRunIdx) * sizeof(uint16_t));
			memset(&ip[zeroRunIdx], 0, zeroRunLength * sizeof(uint16_t));
		}
	} else if (i != 8) {
		err::setError(err::SystemErrorCode_InvalidAddress);
		return false;
	}

	return true;
}

bool
SockAddrParser::parse(sockaddr_in6* addr) {
	bool result;

	memset(addr, 0, sizeof(sockaddr_in6));
	addr->sin6_family = AF_INET6;

	bool hasBrackets = tryChar('[');

	result = parse(&addr->sin6_addr);
	if (!result)
		return false;

	result = tryChar('%');
	if (result) {
		uint_t scope;
		result = parseInt(&scope, 10);
		if (!result)
			return false;

		addr->sin6_scope_id = scope;
	}

	if (hasBrackets) {
		result = expectChar(']');
		if (!result)
			return false;

		result = tryChar(':');
		if (result) {
			uint_t port;
			result = parseInt(&port, 10);
			if (!result)
				return false;

			addr->sin6_port = sl::swapByteOrder16((uint16_t)port);
		}
	}

	return true;
}

bool
SockAddrParser::parse(
	sockaddr* addr,
	size_t size
) {
	skipWhiteSpace();
	if (m_p == m_end) {
		memset(addr, 0, sizeof(sockaddr_in));
		((sockaddr_in*)addr)->sin_family = AF_INET;
		return true;
	}

	SockAddrParser clone = *this;

	if (trySockAddr_ip4()) {
		if (size < sizeof(sockaddr_in)) {
			err::setError(err::SystemErrorCode_BufferTooSmall);
			return false;
		}

		*this = clone;
		return parse((sockaddr_in*)addr);
	}

	*this = clone;

	if (trySockAddr_ip6()) {
		if (size < sizeof(sockaddr_in6)) {
			err::setError(err::SystemErrorCode_BufferTooSmall);
			return false;
		}

		*this = clone;
		return parse((sockaddr_in6*)addr);
	}

	err::setError(err::SystemErrorCode_InvalidAddress);
	return false;
}

bool
SockAddrParser::tryAddr_ip4() {
	bool result;

	for (size_t i = 0;; i++) {
		result = tryInt(10);
		if (!result)
			return false;

		if (i >= 3)
			break;

		result = tryChar('.');
		if (!result)
			return false;
	}

	return true;
}

bool
SockAddrParser::tryAddr_ip6() {
	bool result;

	bool hasZeroRun = false;
	bool isIp4 = false;

	tryChar(':');

	size_t i = 0;
	while (i < 8) {
		result = tryChar(':');
		if (result)
			hasZeroRun = true;

		skipWhiteSpace();

		if (m_p >= m_end || !isxdigit(*m_p))
			break;

		result = tryInt(16);
		if (!result)
			return false;

		result = tryChar(':');
		if (!result) {
			result = tryChar('.');
			if (result) {
				if (i > 6)
					return false;

				isIp4 = true;
				break;
			}
		}

		i++;

		if (!result)
			break;
	}

	if (isIp4) {
		for (size_t j = 1;; j++) {
			result = tryInt(10);
			if (!result)
				return false;

			if (j >= 3)
				break;

			result = tryChar('.');
			if (!result)
				return false;
		}

		i += 2;
	}

	if (!hasZeroRun && i != 8)
		return false;

	return true;
}

bool
SockAddrParser::trySockAddr_ip4() {
	const char* p0 = m_p;

	bool result = tryAddr_ip4();
	if (!result) { // rollback and try single port
		m_p = p0;
		result = tryInt(10);
		if (!result)
			return false;

		skipWhiteSpace();
		return m_p == m_end;
	}

	result = tryChar(':');
	if (result) {
		result = tryInt(10);
		if (!result)
			return false;
	}

	return true;
}

bool
SockAddrParser::trySockAddr_ip6() {
	bool result;
	bool hasBrackets = tryChar('[');

	result = tryAddr_ip6();
	if (!result)
		return false;

	result = tryChar('%');
	if (result) {
		result = tryInt(10);
		if (!result)
			return false;
	}

	if (hasBrackets) {
		result = tryChar(']');
		if (!result)
			return false;

		result = tryChar(':');
		if (result) {
			result = tryInt(10);
			if (!result)
				return false;
		}
	}

	return true;
}

void
SockAddrParser::skipWhiteSpace() {
	while (m_p < m_end && isspace(*m_p))
		m_p++;
}

bool
SockAddrParser::parseInt(
	uint_t* p,
	int radix
) {
	skipWhiteSpace();

	char* end;
	if (m_p < m_end)
		*p = (uint_t)strtoul(m_p, &end, radix);
	else
		end = (char*)m_p;

	if (end == m_p) {
		err::setError(err::SystemErrorCode_InvalidAddress);
		return false;
	}

	m_p = end;
	return true;
}

bool
SockAddrParser::tryInt(int radix) {
	skipWhiteSpace();

	const char* p = m_p;

	if (radix == 16) {
		while (p < m_end && isxdigit(*p))
			p++;
	} else {
		while (p < m_end && isdigit(*p))
			p++;
	}

	if (m_p == p)
		return false;

	m_p = p;
	return true;
}

bool
SockAddrParser::expectChar(char c) {
	bool result = tryChar(c);

	if (!result) {
		err::setError(err::SystemErrorCode_InvalidAddress);
		return false;
	}

	return true;
}

bool
SockAddrParser::tryChar(char c) {
	skipWhiteSpace();

	if (m_p >= m_end || *m_p != c)
		return false;

	m_p++;
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
