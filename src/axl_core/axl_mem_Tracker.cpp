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

namespace axl {
namespace mem {

//..............................................................................

void
addTrackerBlock(TrackerBlockHdr* hdr) {
	g::getModule()->getMemTracker()->add(hdr);
}

void
removeTrackerBlock(TrackerBlockHdr* hdr) {
	g::getModule()->getMemTracker()->remove(hdr);
}

//..............................................................................

Tracker::Tracker() {
	m_peakBlockCount = 0;
	m_totalBlockCount = 0;
	m_size = 0;
	m_peakSize = 0;
	m_totalSize = 0;
}

void
Tracker::add(TrackerBlockHdr* hdr) {
	m_lock.lock();

	hdr->m_seqNum = m_totalBlockCount;

	m_totalBlockCount++;
	m_totalSize += hdr->m_size;

	m_blockList.insertTail(hdr);

	size_t blockCount = m_blockList.getCount();
	if (blockCount > m_peakBlockCount)
		m_peakBlockCount = blockCount;

	m_size += hdr->m_size;

	if (m_size > m_peakSize)
		m_peakSize = m_size;

	m_lock.unlock();
}

void
Tracker::remove(TrackerBlockHdr* hdr) {
	m_lock.lock();

	m_blockList.remove(hdr);
	m_size -= hdr->m_size;

	m_lock.unlock();
}

void
Tracker::trace(bool isDetailed) {
	m_lock.lock();

	TRACE(
		"%s: AXL memory stats:\n"
		"    Current...%d bytes (%d blocks)\n"
		"    Peak......%d bytes (%d blocks)\n"
		"    Total.....%d bytes (%d blocks)\n",
		g::getModule()->getTag(),
		m_size,
		m_blockList.getCount(),
		m_peakSize,
		m_peakBlockCount,
		m_totalSize,
		m_totalBlockCount
	);

	if (isDetailed && !m_blockList.isEmpty()) {
		TRACE(
			"*** Found %d unfreed blocks:\n",
			m_blockList.getCount()
		);

		sl::Iterator<TrackerBlockHdr> it = m_blockList.getHead();
		for (; it; it++) {
			TrackerBlockHdr* blockHdr = *it;

			TRACE(
				"    seq: #%05d; size: %d;\n",
				blockHdr->m_seqNum,
				blockHdr->m_size
			);
		}
	}

	m_lock.unlock();
}

//..............................................................................

} // namespace err
} // namespace axl
