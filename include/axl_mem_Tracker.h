// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_TRACKER_H

#include "axl_mt_Lock.h"
#include "axl_rtl_List.h"

namespace axl {
namespace mem {

//.............................................................................

class CTracker
{
public:
	struct TBlockHdr: rtl::TListLink
	{
		size_t m_size;
		size_t m_seqNum;
		const char* m_tag;
		const char* m_filePath;
		size_t m_line;
	};

protected:
	mt::CLock m_lock;
	rtl::CAuxListT <TBlockHdr> m_blockList;

	size_t m_peakBlockCount;
	size_t m_totalBlockCount;
	size_t m_size;
	size_t m_peakSize;
	size_t m_totalSize;

public:
	CTracker ();

	void
	add (TBlockHdr* hdr);

	void
	remove (TBlockHdr* hdr);

	void 
	trace ();
};

//.............................................................................

} // namespace mem
} // namespace axl
