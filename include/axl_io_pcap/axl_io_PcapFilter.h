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

class PcapFilter: public bpf_program {
public:
	PcapFilter() {
		init();
	}

	~PcapFilter() {
		free();
	}

	void
	free() {
		::pcap_freecode(this);
		init();
	}

	bool
	isEmpty() {
		return bf_insns == NULL;
	}

	bool
	compile(
		pcap_t* pcap,
		const sl::StringRef& filter,
		bool isOptimized = true,
		uint32_t netMask = PCAP_NETMASK_UNKNOWN
	);

	bool
	compile(
		int linkType,
		size_t snapshotSize,
		const sl::StringRef& filter,
		bool isOptimized = true,
		uint32_t netMask = PCAP_NETMASK_UNKNOWN
	);

	bool
	match(
		const void* p,
		size_t size
	);

protected:
	void
	init() {
		static bpf_program null = { 0 };
		*(bpf_program*)this = null;
	}
};

//..............................................................................

} // namespace io
} // namespace axl
