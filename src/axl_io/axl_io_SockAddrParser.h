// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKADDRPARSER_H

#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

class SockAddrParser
{
protected:
	const char* m_p;
	const char* m_end;

public:
	SockAddrParser ()
	{
		initialize (NULL, 0);
	}

	SockAddrParser (
		const char* p,
		size_t length = -1
		)
	{
		initialize (p, length);
	}

	void
	initialize (
		const char* p,
		size_t length = -1
		);

	bool
	parse (
		sockaddr* addr,
		size_t size
		);

	bool
	parseIp4 (sockaddr_in* addr);

	bool
	parseIp6 (sockaddr_in6* addr);

protected:
	bool
	tryIp4 ();

	bool
	tryIp6 ();

	void
	skipWhiteSpace ();

	bool
	parseInt (
		uint_t* p,
		int radix
		);

	bool
	tryInt (int radix);

	bool
	expectChar (char c);

	bool
	tryChar (char c);
};

//.............................................................................

} // namespace io {
} // namespace axl {
