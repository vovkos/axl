// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PCAP_H

#include "axl_sl_Handle.h"

namespace axl {
namespace io {

//.............................................................................

class ClosePCap
{
public:
	void
	operator () (pcap_t* h)
	{
		::pcap_close (h);
	}
};

//.............................................................................

class PCap: public sl::Handle <pcap_t*, ClosePCap>
{
public:
	bool
	openDevice (
		const char* device,
		size_t snapshotSize,
		bool isPromiscious,
		uint_t readTimeout
		);

	bool
	openFile (const char* fileName);

	bool
	setFilter (const char* filter);

	bool
	setBlockingMode (bool isBlocking);

	size_t
	read (
		void* p,
		size_t size,
		uint64_t* timestamp
		);

	size_t
	read (
		void* p,
		size_t size
		)
	{
		return read (p, size, NULL);
	}

	size_t
	write (
		const void* p,
		size_t size
		);
};

//.............................................................................

} // namespace io
} // namespace axl
