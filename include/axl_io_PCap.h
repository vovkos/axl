// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PCAP_H

#include "axl_rtl_Handle.h"

namespace axl {
namespace io {

//.............................................................................

class CClosePCap
{
public:
	void
	operator () (pcap_t* h)
	{
		::pcap_close (h);
	}
};

//.............................................................................

class CPCap: public rtl::CHandleT <pcap_t*, CClosePCap>
{
public:
	bool
	open (
		const char* device,
		size_t snapshotSize,
		bool isPromiscious,
		uint_t readTimeout
		);

	bool
	setFilter (const char* filter);

	bool
	setBlockingMode (bool isBlocking);

	size_t
	read (
		void* p,
		size_t size
		);

	size_t
	write (
		const void* p,
		size_t size
		);
};

//.............................................................................

} // namespace io {
} // namespace axl {
