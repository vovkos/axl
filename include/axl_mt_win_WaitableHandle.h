// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	Wait (
		uint_t Timeout = -1, 
		bool IsAlertable = false
		)
	{
		dword_t Result = ::WaitForSingleObjectEx (m_h, Timeout, IsAlertable);
		return CompleteWait (Result);
	}
	
	static 
	EWaitResult
	MultiWait (
		HANDLE* pWaitArray, 
		dword_t Count, 
		bool DoWaitAll = false,
		uint_t Timeout = -1, 
		bool IsAlertable = false
		)
	{
		dword_t Result = ::WaitForMultipleObjectsEx (Count, pWaitArray, DoWaitAll, Timeout, IsAlertable);
		return CompleteWait (Result);
	}

protected:
	static
	EWaitResult 
	CompleteWait (dword_t Result);
};

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
