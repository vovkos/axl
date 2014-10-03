// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_WAITABLEHANDLE_H

#include "axl_g_win_Handle.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

enum WaitResult
{
	WaitResult_Fail         = -1,
	WaitResult_Timeout      = -2,		
	WaitResult_Object0      = 0,
	WaitResult_IoCompletion = 0x100,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class WaitableHandle: public g::win::Handle
{
public:
	WaitResult
	wait (
		uint_t timeout = -1, 
		bool isAlertable = false
		)
	{
		dword_t result = ::WaitForSingleObjectEx (m_h, timeout, isAlertable);
		return completeWait (result);
	}
	
	static 
	WaitResult
	multiWait (
		HANDLE* waitArray, 
		dword_t count, 
		bool doWaitAll = false,
		uint_t timeout = -1, 
		bool isAlertable = false
		)
	{
		dword_t result = ::WaitForMultipleObjectsEx (count, waitArray, doWaitAll, timeout, isAlertable);
		return completeWait (result);
	}

protected:
	static
	WaitResult 
	completeWait (dword_t result);
};

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
