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

#include "pch.h"
#include "axl_mem_Tracker.h"
#include "axl_g_Module.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace mem {

//..............................................................................

Tracker::Tracker ()
{
	m_peakBlockCount = 0;
	m_totalBlockCount = 0;
	m_size = 0;
	m_peakSize = 0;
	m_totalSize = 0;
}

void
Tracker::add (BlockHdr* hdr)
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
Tracker::remove (BlockHdr* hdr)
{
	m_lock.lock ();

	m_blockList.remove (hdr);
	m_size -= hdr->m_size;

	m_lock.unlock ();
}

void
Tracker::trace ()
{
	m_lock.lock ();

	dbg::trace (
		"Allocated memory:\n"
		"    Current.......%d bytes (%d blocks)\n"
		"    Peak..........%d bytes (%d blocks)\n"
		"    Total.........%d bytes (%d blocks)\n",
		m_size,
		m_blockList.getCount (),
		m_peakSize,
		m_peakBlockCount,
		m_totalSize,
		m_totalBlockCount
		);

	if (!m_blockList.isEmpty ())
	{
		dbg::trace (
			"*** Found %d unfreed blocks:\n",
			m_blockList.getCount ()
			);

		sl::Iterator <BlockHdr> it = m_blockList.getHead ();
		for (; it; it++)
		{
			BlockHdr* blockHdr = *it;

			dbg::trace (
				"    %s(%d): %s; seq = #%d; size = %d;\n",
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Tracker*
getTracker ()
{
	return g::getModule ()->getMemTracker ();
}

//..............................................................................

} // namespace err
} // namespace axl
