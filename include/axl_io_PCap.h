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
	Open (
		const char* pDevice,
		size_t SnapshotSize,
		bool IsPromiscious,
		uint_t ReadTimeout
		);

	bool
	SetFilter (const char* pFilter);

	bool
	SetBlockingMode (bool IsBlocking);

	size_t
	Read (
		void* p,
		size_t Size
		);

	size_t
	Write (
		const void* p,
		size_t Size
		);
};

//.............................................................................

} // namespace io {
} // namespace axl {
