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

#if (_AXL_DEBUG)
#	define _AXL_MEM_TRACKER _AXL_MEM_TRACKER_DEBUG
#else
#	define _AXL_MEM_TRACKER _AXL_MEM_TRACKER_RELEASE
#endif

namespace axl {
namespace mem {

#if (_AXL_MEM_TRACKER)

//..............................................................................

Tracker::Tracker() {
	m_peakBlockCount = 0;
	m_totalBlockCount = 0;
	m_size = 0;
	m_peakSize = 0;
	m_totalSize = 0;
	m_breakSeqNum = -1;
	g_trackerDispatchFunc = dispatch;
}

void
Tracker::add(TrackerBlock* block) {
	m_lock.lock();

	block->m_seqNum = m_totalBlockCount;
	if (m_totalBlockCount == m_breakSeqNum)
		AXL_DEBUG_BREAK();

	m_totalBlockCount++;
	m_totalSize += block->m_size;

	m_blockList.insertTail(block);

	size_t blockCount = m_blockList.getCount();
	if (blockCount > m_peakBlockCount)
		m_peakBlockCount = blockCount;

	m_size += block->m_size;

	if (m_size > m_peakSize)
		m_peakSize = m_size;

	m_lock.unlock();
}

void
Tracker::remove(TrackerBlock* block) {
	m_lock.lock();
	m_blockList.remove(block);
	m_size -= block->m_size;
	m_lock.unlock();
}

void
Tracker::trace(bool isDetailed) {
	m_lock.lock();

	size_t aboveSize = 0;
	size_t aboveCount = 0;
	size_t belowSize = 0;
	size_t belowCount = 0;

	sl::Iterator<TrackerBlock> it = m_blockList.getHead();
	for (; it; it++)
		if (it->m_seqNum >= m_watermarkSeqNum) {
			aboveSize += it->m_size;
			aboveCount++;
		} else {
			belowSize += it->m_size;
			belowCount++;
		}

	TRACE(
		"AXL memory tracker stats:\n"
		"    Current:\n"
		"        Above ... %d byte(s) %d block(s)\n"
		"        Below ... %d byte(s) %d block(s)\n"
		"    Peak ........ %d byte(s) %d block(s)\n"
		"    Total ....... %d byte(s) %d block(s)\n",
		aboveSize,
		aboveCount,
		belowSize,
		belowCount,
		m_peakSize,
		m_peakBlockCount,
		m_totalSize,
		m_totalBlockCount
	);

	if (isDetailed && aboveCount) {
		TRACE("*** %d unfreed block(s):\n", aboveCount);

		sl::Iterator<TrackerBlock> it = m_blockList.getHead();
		for (; it; it++) {
			if (it->m_seqNum >= m_watermarkSeqNum) // ignore blocks below watermark
				TRACE(
					"    #%d: %d byte(s)\n",
					it->m_seqNum,
					it->m_size
				);
		}
	}

	m_lock.unlock();
}

void
Tracker::dispatch(DispatchCode code) {
	Tracker* self = getTracker();

	switch (code) {
	case DispatchCode_Trace:
		self->trace();
		break;

	case DispatchCode_SetWatermarkSeqNum:
		self->m_watermarkSeqNum = self->m_blockList.getCount();
		break;
	}
}

//..............................................................................

#endif  // _AXL_MEM_TRACKER

} // namespace mem
} // namespace axl
