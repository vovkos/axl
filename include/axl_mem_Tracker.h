// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
		size_t m_Size;
		size_t m_SeqNum;
		const char* m_pTag;
		const char* m_pFilePath;
		size_t m_Line;
	};

protected:
	mt::CLock m_Lock;
	rtl::CAuxListT <TBlockHdr> m_BlockList;

	size_t m_PeakBlockCount;
	size_t m_TotalBlockCount;
	size_t m_Size;
	size_t m_PeakSize;
	size_t m_TotalSize;

public:
	CTracker ();

	void
	Add (TBlockHdr* pHdr);

	void
	Remove (TBlockHdr* pHdr);

	void 
	Trace ();
};

//.............................................................................

} // namespace mem
} // namespace axl
