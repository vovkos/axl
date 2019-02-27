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

#define _AXL_SYS_WIN_WAITABLETIMER_H

#include "axl_sys_win_WaitableTimer.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class WaitableTimer: public WaitableHandle
{
public:
	bool
	create(
		SECURITY_ATTRIBUTES* secAttr = NULL,
		bool isManualReset = false,
		const sl::StringRef_w& name = NULL
		)
	{
		close();

		m_h = ::CreateWaitableTimerW(secAttr, isManualReset, name);
		return err::complete(m_h != NULL);
	}

	bool
	open(
		uint_t access = EVENT_ALL_ACCESS,
		bool doInheritHandle = false,
		const sl::StringRef_w& name = NULL
		)
	{
		close();

		m_h = ::OpenWaitableTimerW(access, doInheritHandle, name);
		return err::complete(m_h != NULL);
	}

	bool
	setTimer(
		uint64_t dueTime,
		uint_t period = 0,
		PTIMERAPCROUTINE completionRoutine = NULL,
		void* completionContext = NULL,
		bool resume = false
		)
	{
		bool_t result = ::SetWaitableTimer(m_h, (LARGE_INTEGER*) &dueTime, period, completionRoutine, completionContext, resume);
		return err::complete(result);
	}

	bool
	setRelativeTimer(
		uint_t timeout,
		uint_t period = 0,
		PTIMERAPCROUTINE completionRoutine = NULL,
		void* completionContext = NULL,
		bool resume = false
		)
	{
		uint64_t dueTime = (int64_t) -10000 * timeout;
		return setTimer(dueTime, period, completionRoutine, completionContext, resume);
	}

	bool
	cancel()
	{
		bool_t result = ::CancelWaitableTimer(m_h);
		return err::complete(result);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
