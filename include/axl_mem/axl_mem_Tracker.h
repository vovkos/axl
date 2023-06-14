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

#define _AXL_MEM_TRACKER_H

#include "axl_sl_List.h"
#include "axl_sl_CallOnce.h"
#include "axl_sl_Construct.h"
#include "axl_sys_Lock.h"

namespace axl {
namespace mem {

//..............................................................................

struct TrackerBlock: sl::ListLink {
	size_t m_size;
	size_t m_seqNum;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Tracker {
public:
	enum DispatchCode {
		DispatchCode_Trace,
		DispatchCode_SetWatermarkSeqNum,
	};

protected:
	sys::Lock m_lock;
	sl::AuxList<TrackerBlock> m_blockList;

	size_t m_peakBlockCount;
	size_t m_totalBlockCount;
	size_t m_size;
	size_t m_peakSize;
	size_t m_totalSize;
	size_t m_watermarkSeqNum;

public:
	size_t m_breakSeqNum; // freely adjustible

public:
	Tracker();

	~Tracker() {
		ASSERT(false); // should never be destroyed
	}

	size_t
	getBlockCount() {
		return m_blockList.getCount();
	}

	size_t
	getPeakBlockCount() {
		return m_peakBlockCount;
	}

	size_t
	getTotalBlockCount() {
		return m_totalBlockCount;
	}

	size_t
	getSize() {
		return m_size;
	}

	size_t
	getPeakSize() {
		return m_peakSize;
	}

	size_t
	getTotalSize() {
		return m_totalSize;
	}

	void
	add(TrackerBlock* block);

	void
	remove(TrackerBlock* block);

	void
	trace(bool isDetailed = true);

protected:
	static
	void
	dispatch(DispatchCode code);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Tracker*
getTracker() {
	static char buffer[sizeof(Tracker)];
	sl::callOnce(sl::Construct<Tracker>(), (Tracker*)buffer);
	return (Tracker*)buffer;
}

AXL_SELECT_ANY void (*g_trackerDispatchFunc)(Tracker::DispatchCode code) = NULL;

//..............................................................................

} // namespace mem
} // namespace axl
