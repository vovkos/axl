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

enum PcapLoopResult
{
	PcapLoopResult_Success = 0,
	PcapLoopResult_Error   = -1,
	PcapLoopResult_Break   = -2,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Pcap: public sl::Handle<pcap_t*, ClosePcap>
{
public:
	bool
	openDevice(const sl::StringRef& device);

	bool
	openLive(
		const sl::StringRef& device,
		size_t snapshotSize,
		bool isPromiscious,
		uint_t timeout
		);

	bool
	openFile(const sl::StringRef& fileName);

	bool
	openDead(
		int linkType,
		size_t snapshotSize
		);

	bool
	activate();

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

	bool
	setSnapshotSize(size_t size);

	bool
	setBufferSize(size_t size);

	bool
	setPromiscious(bool isPromiscious);

	bool
	setTimeout(uint_t timeout);

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

#if (_AXL_POSIX)
	int
	getSelectableFd()
	{
		ASSERT(m_h);
		return ::pcap_get_selectable_fd(m_h);
	}
#endif

	bool
	getBlockingMode();

	bool
	setBlockingMode(bool isBlocking);

	void
	breakLoop()
	{
		ASSERT(m_h);
		::pcap_breakloop(m_h);
	}

	PcapLoopResult
	loop(
		size_t count,
		::pcap_handler handler,
		void* context
		);

	size_t
	dispatch(
		size_t count,
		::pcap_handler handler,
		void* context
		);

	size_t
	read(
		void* p,
		size_t size,
		uint64_t* timestamp = NULL
		);

	size_t
	write(
		const void* p,
		size_t size
		);

protected:
	bool
	complete(bool result);
};

//..............................................................................

} // namespace io
} // namespace axl
