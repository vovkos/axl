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
		
	enum ScheduleResultKind
	{
		ScheduleResultKind_Fail    = 0,
		ScheduleResultKind_Invoke  = 1,
		ScheduleResultKind_Pending = 2,
	};

	virtual 
	ScheduleResultKind
	scheduleV (
		IFunction* function, 
		axl_va_list va
		) = 0; // the only overridable

	ScheduleResultKind
	schedule (
		IFunction* function, 
		...
		)
	{
		AXL_VA_DECL (va, function);
		return scheduleV (function, va);
	}

	template <typename ArgPacker>
	ScheduleResultKind
	scheduleV (
		CallConvKind convention,
		void* pf, 
		axl_va_list va
		)
	{
		Function <exe::Arg <void>, ArgPacker> function (convention, pf);
		return scheduleV (&function, va);
	}

	template <typename ArgPacker>
	ScheduleResultKind
	schedule (
		CallConvKind convention,
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return scheduleV <ArgPacker> (convention, pf, va);
	}

	template <typename ArgPacker>
	ScheduleResultKind
	scheduleV (
		void* pf, 
		axl_va_list va
		)
	{
		return scheduleV <ArgPacker> (CallConvKind_Cdecl, pf, va);
	}

	template <typename ArgPacker>
	ScheduleResultKind
	schedule (
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return scheduleV <ArgPacker> (pf, va);
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

	template <typename ArgPacker>
	intptr_t
	syncScheduleV (
		CallConvKind convention,
		void* pf, 
		axl_va_list va
		)
	{
		Function <exe::Arg <void>, ArgPacker> function (convention, pf);
		return syncScheduleV (&function, va);
	}

	template <typename ArgPacker>
	intptr_t
	syncSchedule (
		CallConvKind convention,
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return syncScheduleV <ArgPacker> (convention, pf, va);
	}

	template <typename ArgPacker>
	intptr_t
	syncScheduleV (
		void* pf, 
		axl_va_list va
		)
	{
		return syncScheduleV <ArgPacker> (CallConvKind_Cdecl, pf, va);
	}

	template <typename ArgPacker>
	intptr_t
	syncSchedule (
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return syncScheduleV <ArgPacker> (pf, va);
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
