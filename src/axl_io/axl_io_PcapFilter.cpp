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
#include "axl_io_PcapFilter.h"
#include "axl_io_Pcap.h"

namespace axl {
namespace io {

//..............................................................................

bool
PcapFilter::compile(
	pcap_t* pcap,
	const sl::StringRef& filter,
	bool isOptimized,
	uint32_t netMask
	)
{
	free();

	int result = ::pcap_compile(pcap, this, (char*)filter.sz(), isOptimized, netMask);
	if (result == -1)
	{
		err::setError(::pcap_geterr(pcap));
		return false;
	}

	return true;
}

bool
PcapFilter::compile(
	size_t snapshotSize,
	int linkType,
	const sl::StringRef& filter,
	bool isOptimized,
	uint32_t netMask
	)
{
	Pcap pcap;

	return
		pcap.openDead(snapshotSize, linkType) &&
		compile(pcap, filter, isOptimized, netMask);
}

bool
PcapFilter::match(
	const void* p,
	size_t size
	)
{
	pcap_pkthdr hdr =
	{
		{ 0 }, // timestamp doesn't matter
		size,
		size
	};

	return ::pcap_offline_filter(this, &hdr, (const u_char*)p) > 0;
}

//..............................................................................

} // namespace io
} // namespace axl
