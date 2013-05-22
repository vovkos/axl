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
	ScheduleV (
		IFunction* pFunction, 
		axl_va_list va
		) = 0; // the only overridable

	EScheduleResult
	Schedule (
		IFunction* pFunction, 
		...
		)
	{
		AXL_VA_DECL (va, pFunction);
		return ScheduleV (pFunction, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	ScheduleV (
		ECallConv Convention,
		void* pf, 
		axl_va_list va
		)
	{
		CFunctionT <exe::CArgT <void>, TArgPacker> Function (Convention, pf);
		return ScheduleV (&Function, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	Schedule (
		ECallConv Convention,
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return ScheduleV <TArgPacker> (Convention, pf, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	ScheduleV (
		void* pf, 
		axl_va_list va
		)
	{
		return ScheduleV <TArgPacker> (ECallConv_Cdecl, pf, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	Schedule (
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return ScheduleV <TArgPacker> (pf, va);
	}

	intptr_t
	SyncScheduleV (
		IFunction* pFunction, 
		axl_va_list va
		);

	intptr_t
	SyncSchedule (
		IFunction* pFunction, 
		...
		)
	{
		AXL_VA_DECL (va, pFunction);
		return SyncScheduleV (pFunction, va);
	}

	template <typename TArgPacker>
	intptr_t
	SyncScheduleV (
		ECallConv Convention,
		void* pf, 
		axl_va_list va
		)
	{
		CFunctionT <exe::CArgT <void>, TArgPacker> Function (Convention, pf);
		return SyncScheduleV (&Function, va);
	}

	template <typename TArgPacker>
	intptr_t
	SyncSchedule (
		ECallConv Convention,
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return SyncScheduleV <TArgPacker> (Convention, pf, va);
	}

	template <typename TArgPacker>
	intptr_t
	SyncScheduleV (
		void* pf, 
		axl_va_list va
		)
	{
		return SyncScheduleV <TArgPacker> (ECallConv_Cdecl, pf, va);
	}

	template <typename TArgPacker>
	intptr_t
	SyncSchedule (
		void* pf, 
		...
		)
	{
		AXL_VA_DECL (va, pf);
		return SyncScheduleV <TArgPacker> (pf, va);
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
