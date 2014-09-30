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

class CSynchronizer: public IScheduler
{
public:
	AXL_OBJ_CLASS_0 (CSynchronizer, IScheduler)

	enum EFlags
	{
		EFlags_AlwaysEnqueue = 1 // even if Schedule () is called in the same thread
	};

protected:
	mt::CLock m_lock;
	CInvokeList m_invokeList;

	ulong_t m_invokeThreadId;
	int m_flags;

public:
	CSynchronizer (int flags = 0);

	virtual 
	EScheduleResult
	scheduleV (
		IFunction* function, 
		axl_va_list va
		);
};

//.............................................................................

} // namespace exe
} // namespace axl
