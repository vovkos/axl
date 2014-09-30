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

enum EWaitResult
{
	EWaitResult_Fail         = -1,
	EWaitResult_Timeout      = -2,		
	EWaitResult_Object0      = 0,
	EWaitResult_IoCompletion = 0x100,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CWaitableHandle: public g::win::CHandle
{
public:
	EWaitResult
	wait (
		uint_t timeout = -1, 
		bool isAlertable = false
		)
	{
		dword_t result = ::WaitForSingleObjectEx (m_h, timeout, isAlertable);
		return completeWait (result);
	}
	
	static 
	EWaitResult
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
	EWaitResult 
	completeWait (dword_t result);
};

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
