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

#pragma once

#define _AXL_IO_SOCKADDRPARSER_H

#include "axl_io_Pch.h"

namespace axl {
namespace io {

//..............................................................................

class SockAddrParser
{
protected:
	const char* m_p;
	const char* m_end;

public:
	SockAddrParser()
	{
		m_p = NULL;
		m_end = NULL;
	}

	SockAddrParser(const sl::StringRef& source)
	{
		create(source);
	}

	void
	create(const sl::StringRef& source)
	{
		m_p = source.cp();
		m_end = source.getEnd();
	}

	bool
	parse(in_addr* addr);

	bool
	parse(in6_addr* addr);

	bool
	parse(sockaddr_in* addr);

	bool
	parse(sockaddr_in6* addr);

	bool
	parse(
		sockaddr* addr,
		size_t size
		);

protected:
	bool
	tryAddr_ip4();

	bool
	tryAddr_ip6();

	bool
	trySockAddr_ip4();

	bool
	trySockAddr_ip6();

	void
	skipWhiteSpace();

	bool
	parseInt(
		uint_t* p,
		int radix
		);

	bool
	tryInt(int radix);

	bool
	expectChar(char c);

	bool
	tryChar(char c);

	void
	setParseError();
};

//..............................................................................

} // namespace io
} // namespace axl
