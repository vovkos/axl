#include "pch.h"
#include "axl_mem_Tracker.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace mem {

//.............................................................................

CTracker::CTracker ()
{
	m_peakBlockCount = 0;
	m_totalBlockCount = 0;
	m_size = 0;
	m_peakSize = 0;
	m_totalSize = 0;
}

void
CTracker::add (TBlockHdr* hdr)
{
	m_lock.lock ();

	hdr->m_seqNum = m_totalBlockCount;

	m_totalBlockCount++;
	m_totalSize += hdr->m_size;

	m_blockList.insertTail (hdr);

	size_t blockCount = m_blockList.getCount ();
	if (blockCount > m_peakBlockCount)
		m_peakBlockCount = blockCount;

	m_size += hdr->m_size;

	if (m_size > m_peakSize)
		m_peakSize = m_size;

	m_lock.unlock ();
}

void
CTracker::remove (TBlockHdr* hdr)
{
	m_lock.lock ();

	m_blockList.remove (hdr);
	m_size -= hdr->m_size;

	m_lock.unlock ();
}

void 
CTracker::trace ()
{
	m_lock.lock ();

	dbg::trace (
		"Allocated size:\n"
		"    Current.............................%d B\n"
		"    Peak................................%d B\n"
		"    Total...............................%d B\n",
		m_size,
		m_peakSize,
		m_totalSize
		);

	dbg::trace (
		"Allocated blocks:\n"
		"    Current.............................%d\n"
		"    Peak................................%d\n"
		"    Total...............................%d\n",
		m_blockList.getCount (),
		m_peakBlockCount,
		m_totalBlockCount
		);

	if (!m_blockList.isEmpty ())
	{
		dbg::trace (
			"*** Found %d unfreed blocks:\n",
			m_blockList.getCount ()
			);

		rtl::CIteratorT <TBlockHdr> it = m_blockList.getHead ();
		for (; it; it++)
		{
			TBlockHdr* blockHdr = *it;

			dbg::trace (
				"    %s(%d): %s seq: #%d size: %d B\n",
				blockHdr->m_filePath,
				blockHdr->m_line,
				blockHdr->m_tag,
				blockHdr->m_seqNum,
				blockHdr->m_size
				);
		}
	}

	m_lock.unlock ();
}

//.............................................................................

} // namespace err
} // namespace axl
