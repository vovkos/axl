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

namespace axl {
namespace mem {

#if (_AXL_MEM_TRACKER)

// ensure static initialization of the tracker during startup

static struct TrackerInit {
	TrackerInit() {
		getTracker();
	}
} g_moduleInit;

//..............................................................................

Tracker::Tracker() {
	m_peakBlockCount = 0;
	m_totalBlockCount = 0;
	m_size = 0;
	m_peakSize = 0;
	m_totalSize = 0;
	m_breakSeqNum = -1;
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

#if (_AXL_OS_WIN)
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

void
Tracker::trace(bool isDetailed) {
	m_lock.lock();
	size_t blockCount = m_blockList.getCount();

#if (_AXL_OS_WIN)
	// on Windows, we add tracker to DLLs as well; therefore, want to know a particular DLL name

	wchar_t imageFileName[MAX_PATH] = { 0 };
	GetModuleFileName((HMODULE)&__ImageBase, imageFileName, lengthof(imageFileName));

	TRACE(
		"AXL memory tracker stats:\n"
		"    Module .... %S\n",
		imageFileName
	);
#else
	TRACE("AXL memory tracker stats:\n");
#endif

	TRACE(
		"    Current ... %d byte(s) %d block(s)\n"
		"    Peak ...... %d byte(s) %d block(s)\n"
		"    Total ..... %d byte(s) %d block(s)\n",
		m_size,
		blockCount,
		m_peakSize,
		m_peakBlockCount,
		m_totalSize,
		m_totalBlockCount
	);

	if (isDetailed && blockCount) {
		TRACE("*** Found %d unfreed block(s):\n", blockCount);

		sl::Iterator<TrackerBlock> it = m_blockList.getHead();
		for (; it; it++)
			TRACE(
				"    #%d: %d byte(s)\n",
				it->m_seqNum,
				it->m_size
			);
	}

	m_lock.unlock();
}

//..............................................................................

#endif  // _AXL_MEM_TRACKER

} // namespace mem
} // namespace axl
