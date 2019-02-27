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

#define _AXL_MEM_TRACKERBLOCK_H

#include "axl_sl_Iterator.h"

namespace axl {
namespace mem {

//..............................................................................

struct TrackerBlockHdr: sl::ListLink
{
	size_t m_size;
	size_t m_seqNum;
	const char* m_tag;
	const char* m_filePath;
	size_t m_line;
	size_t padding; // ensure dual-pointer alignment
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
addTrackerBlock(TrackerBlockHdr* hdr);

void
removeTrackerBlock(TrackerBlockHdr* hdr);

//..............................................................................

} // namespace mem
} // namespace axl
