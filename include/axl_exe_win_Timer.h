// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_TIMER_H

#include "axl_exe_Function.h"
#include "axl_win_WaitableHandle.h"

namespace axl {
namespace win {

//.............................................................................

class CTimer: public CWaitableHandle
{
public:
	bool
	Create (
		SECURITY_ATTRIBUTES* pSecAttr,
		bool IsManualReset,
		const tchar_t* pName
		);

	bool
	Open (
		ulong_t Access,
		bool DoInheritHandle,
		const tchar_t* pName
		);

	bool 
	SetTimer (	
		uint64_t _DueTime,
		ulong_t Period, 
		PTIMERAPCROUTINE pfTimerProc,
		void* pContext
		);

	void
	Cancel ()
	{
		ASSERT (IsOpen ());
		::CancelWaitableTimer (m_h);
	}

	static
	ulong_t 
	GetTickCount ()
	{
		return ::GetTickCount ();
	}
};

//.............................................................................

} // namespace win
} // namespace axl
