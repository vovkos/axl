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

#define _AXL_SYS_WIN_WAITABLEHANDLE_H

#include "axl_sys_win_Handle.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

enum WaitResult
{
	WaitResult_Fail         = -1,
	WaitResult_Timeout      = -2,
	WaitResult_Object0      = 0,
	WaitResult_IoCompletion = 0x100,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class WaitableHandle: public sys::win::Handle
{
public:
	WaitResult
	wait(
		uint_t timeout = -1,
		bool isAlertable = false
		)
	{
		dword_t result = ::WaitForSingleObjectEx(m_h, timeout, isAlertable);
		return completeWait(result);
	}

	static
	WaitResult
	multiWait(
		HANDLE* waitArray,
		dword_t count,
		bool doWaitAll = false,
		uint_t timeout = -1,
		bool isAlertable = false
		)
	{
		dword_t result = ::WaitForMultipleObjectsEx(count, waitArray, doWaitAll, timeout, isAlertable);
		return completeWait(result);
	}

protected:
	static
	WaitResult
	completeWait(dword_t result);
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
