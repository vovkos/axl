// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_TRACKER_H

#include "axl_sys_Lock.h"
#include "axl_sl_ListBase.h"

namespace axl {
namespace mem {

//.............................................................................

class Tracker
{
public:
	struct BlockHdr: sl::ListLink
	{
		size_t m_size;
		size_t m_seqNum;
		const char* m_tag;
		const char* m_filePath;
		size_t m_line;
	};

protected:
	sys::Lock m_lock;
	sl::AuxList <BlockHdr> m_blockList;

	size_t m_peakBlockCount;
	size_t m_totalBlockCount;
	size_t m_size;
	size_t m_peakSize;
	size_t m_totalSize;

public:
	Tracker ();

	void
	add (BlockHdr* hdr);

	void
	remove (BlockHdr* hdr);

	void 
	trace ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Tracker*
getTracker ();

//.............................................................................

} // namespace mem
} // namespace axl
