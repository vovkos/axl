#include "stdafx.h"
#include "axl_mem_Tracker.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace mem {

//.............................................................................

CTracker::CTracker ()
{
	m_PeakBlockCount = 0;
	m_TotalBlockCount = 0;
	m_Size = 0;
	m_PeakSize = 0;
	m_TotalSize = 0;
}

void
CTracker::Add (TBlockHdr* pHdr)
{
	m_Lock.Lock ();

	pHdr->m_SeqNum = m_TotalBlockCount;

	m_TotalBlockCount++;
	m_TotalSize += pHdr->m_Size;

	m_BlockList.InsertTail (pHdr);

	size_t BlockCount = m_BlockList.GetCount ();
	if (BlockCount > m_PeakBlockCount)
		m_PeakBlockCount = BlockCount;

	m_Size += pHdr->m_Size;

	if (m_Size > m_PeakSize)
		m_PeakSize = m_Size;

	m_Lock.Unlock ();
}

void
CTracker::Remove (TBlockHdr* pHdr)
{
	m_Lock.Lock ();

	m_BlockList.Remove (pHdr);
	m_Size -= pHdr->m_Size;

	m_Lock.Unlock ();
}

void 
CTracker::Trace ()
{
	m_Lock.Lock ();

	dbg::TraceEx (
		AXL_SUBSYS_MEM,
		dbg::ETraceLevel_Info, 
		"Allocated size:\n"
		"    Current.............................%d B\n"
		"    Peak................................%d B\n"
		"    Total...............................%d B\n",
		m_Size,
		m_PeakSize,
		m_TotalSize
		);

	dbg::TraceEx (
		AXL_SUBSYS_MEM,
		dbg::ETraceLevel_Info, 
		"Allocated blocks:\n"
		"    Current.............................%d\n"
		"    Peak................................%d\n"
		"    Total...............................%d\n",
		m_BlockList.GetCount (),
		m_PeakBlockCount,
		m_TotalBlockCount
		);

	if (!m_BlockList.IsEmpty ())
	{
		dbg::TraceEx (
			AXL_SUBSYS_REF,
			dbg::ETraceLevel_Info, 
			"*** Found %d unfreed blocks:\n",
			m_BlockList.GetCount ()
			);

		rtl::CIteratorT <TBlockHdr> It = m_BlockList.GetHead ();
		for (; It; It++)
		{
			dbg::TraceEx (
				AXL_SUBSYS_REF,
				dbg::ETraceLevel_Info, 
				"    %s(%d): %s seq: #%d size: %d B\n",
				It->m_pFilePath,
				It->m_Line,
				It->m_pTag,
				It->m_SeqNum,
				It->m_Size
				);
		}
	}

	m_Lock.Unlock ();
}

//.............................................................................

} // namespace err
} // namespace axl
