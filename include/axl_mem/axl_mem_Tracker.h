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

#define _AXL_MEM_TRACKER_H

#include "axl_mem_TrackerBlock.h"
#include "axl_sl_List.h"
#include "axl_sys_Lock.h"

namespace axl {
namespace mem {

//..............................................................................

class Tracker
{
protected:
	sys::Lock m_lock;
	sl::AuxList<TrackerBlockHdr> m_blockList;

	size_t m_peakBlockCount;
	size_t m_totalBlockCount;
	size_t m_size;
	size_t m_peakSize;
	size_t m_totalSize;

public:
	Tracker();

	void
	add(TrackerBlockHdr* hdr);

	void
	remove(TrackerBlockHdr* hdr);

	void
	trace();
};

//..............................................................................

} // namespace mem
} // namespace axl
