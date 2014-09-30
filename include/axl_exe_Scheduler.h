// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_SCHEDULER_H

#include "axl_exe_Function.h"
#include "axl_mem_Block.h"

namespace axl {
namespace exe {

struct IFunction;

//.............................................................................

struct IScheduler: obj::IRoot
{
	// {4C9A9E6F-8728-4AB9-BA4C-8134865DF339}
	AXL_OBJ_INTERFACE (
		IScheduler,
		0x4c9a9e6f, 0x8728, 0x4ab9, 0xba, 0x4c, 0x81, 0x34, 0x86, 0x5d, 0xf3, 0x39
		)
		
	enum EScheduleResult
	{
		EScheduleResult_Fail    = 0,
		EScheduleResult_Invoke  = 1,
		EScheduleResult_Pending = 2,
	};

	virtual 
	EScheduleResult
	scheduleV (
		IFunction* function, 
		axl_va_list va
		) = 0; // the only overridable

	EScheduleResult
	schedule (
		IFunction* function, 
		...
		)
	{
		AXL_VA_DECL (va, function);
		return scheduleV (function, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	scheduleV (
		ECallConv convention,
		void* pf, 
		axl_va_list va
		)
	{
		CFunctionT <exe::CArgT <void>, TArgPacker> function (convention, pf);
		return scheduleV (&function, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	schedule (
		ECallConv convention,
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return scheduleV <TArgPacker> (convention, pf, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	scheduleV (
		void* pf, 
		axl_va_list va
		)
	{
		return scheduleV <TArgPacker> (ECallConv_Cdecl, pf, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	schedule (
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return scheduleV <TArgPacker> (pf, va);
	}

	intptr_t
	syncScheduleV (
		IFunction* function, 
		axl_va_list va
		);

	intptr_t
	syncSchedule (
		IFunction* function, 
		...
		)
	{
		AXL_VA_DECL (va, function);
		return syncScheduleV (function, va);
	}

	template <typename TArgPacker>
	intptr_t
	syncScheduleV (
		ECallConv convention,
		void* pf, 
		axl_va_list va
		)
	{
		CFunctionT <exe::CArgT <void>, TArgPacker> function (convention, pf);
		return syncScheduleV (&function, va);
	}

	template <typename TArgPacker>
	intptr_t
	syncSchedule (
		ECallConv convention,
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return syncScheduleV <TArgPacker> (convention, pf, va);
	}

	template <typename TArgPacker>
	intptr_t
	syncScheduleV (
		void* pf, 
		axl_va_list va
		)
	{
		return syncScheduleV <TArgPacker> (ECallConv_Cdecl, pf, va);
	}

	template <typename TArgPacker>
	intptr_t
	syncSchedule (
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return syncScheduleV <TArgPacker> (pf, va);
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
