// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_WAITABLE_H

#include "axl_win_Handle.h"

namespace axl {
namespace win {

//.............................................................................

class CWaitableHandle: public CHandle
{
public:
	enum EWaitResult
	{
		EWaitResult_Fail         = -1,
		EWaitResult_Timeout      = -2,		
		EWaitResult_Object0      = 0,
		EWaitResult_IoCompletion = 0x100,
	};

public:
	EWaitResult
	Wait (
		ulong_t Timeout = -1, 
		bool IsAlertable = false
		)
	{
		ulong_t Result = ::WaitForSingleObjectEx (m_h, Timeout, IsAlertable);
		return CompleteWait (Result);
	}
	
	static 
	EWaitResult
	MultiWait (
		HANDLE* pWaitArray, 
		size_t Count, 
		bool DoWaitAll = false,
		ulong_t Timeout = -1, 
		bool IsAlertable = false
		)
	{
		ulong_t Result = ::WaitForMultipleObjectsEx (Count, pWaitArray, DoWaitAll, Timeout, IsAlertable);
		return CompleteWait (Result);
	}

protected:
	static
	EWaitResult 
	CompleteWait (ulong_t Result);
};

//.............................................................................

} // namespace win
} // namespace axl
