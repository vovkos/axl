// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PCAP_H

#include "axl_io_PcapPch.h"

namespace axl {
namespace io {

//..............................................................................

class ClosePcap
{
public:
	void
	operator () (pcap_t* h)
	{
		::pcap_close (h);
	}
};

//..............................................................................

class Pcap: public sl::Handle <pcap_t*, ClosePcap>
{
public:
	bool
	openDevice (
		const sl::StringRef& device,
		size_t snapshotSize,
		bool isPromiscious,
		uint_t readTimeout
		);

	bool
	openFile (const sl::StringRef& fileName);

	bool
	setFilter (const sl::StringRef& filter);

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

//..............................................................................

} // namespace io
} // namespace axl
