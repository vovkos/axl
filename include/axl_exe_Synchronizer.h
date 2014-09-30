// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_SYNCHRONIZER_H

#include "axl_exe_Scheduler.h"
#include "axl_exe_InvokeList.h"
#include "axl_mt_Lock.h"

namespace axl {
namespace exe {

//.............................................................................

class Synchronizer: public IScheduler
{
public:
	AXL_OBJ_CLASS_0 (Synchronizer, IScheduler)

	enum FlagsKind
	{
		FlagsKind_AlwaysEnqueue = 1 // even if Schedule () is called in the same thread
	};

protected:
	mt::Lock m_lock;
	InvokeList m_invokeList;

	ulong_t m_invokeThreadId;
	int m_flags;

public:
	Synchronizer (int flags = 0);

	virtual 
	ScheduleResultKind
	scheduleV (
		IFunction* function, 
		axl_va_list va
		);
};

//.............................................................................

} // namespace exe
} // namespace axl
