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
		::pcap_close(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Pcap: public sl::Handle<pcap_t*, ClosePcap>
{
public:
	bool
	openDevice(
		const sl::StringRef& device,
		size_t snapshotSize,
		bool isPromiscious,
		uint_t readTimeout
		);

	bool
	openFile(const sl::StringRef& fileName);

	bool
	openDead(
		size_t snapshotSize,
		int linkType
		);

	sl::StringRef
	getLastErrorDescription()
	{
		ASSERT(m_h);
		return ::pcap_geterr(m_h);
	}

	size_t
	setLastError()
	{
		ASSERT(m_h);
		return err::setError(getLastErrorDescription());
	}

	bool
	setFilter(const bpf_program* filter);

	bool
	setFilter(
		const sl::StringRef& filter,
		bool isOptimized = true,
		uint32_t netMask = PCAP_NETMASK_UNKNOWN
		);

	bool
	setBlockingMode(bool isBlocking);

	int
	getLinkType()
	{
		ASSERT(m_h);
		return ::pcap_datalink(m_h);
	}

	size_t
	getSnapshotSize()
	{
		ASSERT(m_h);
		return ::pcap_snapshot(m_h);
	}

	size_t
	read(
		void* p,
		size_t size,
		uint64_t* timestamp
		);

	size_t
	read(
		void* p,
		size_t size
		)
	{
		return read(p, size, NULL);
	}

	size_t
	write(
		const void* p,
		size_t size
		);
};

//..............................................................................

} // namespace io
} // namespace axl
