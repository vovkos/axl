// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_SCHEDULER_H

#include "axl_exe_Function.h"
#include "axl_mem_Block.h"

namespace axl {
namespace exe {

struct IFunction;

//.............................................................................

[uuid ("c6f5c201-a114-48d8-848b-87f9935b794f")]
struct IScheduler: obj::IRoot
{
public:
	enum EScheduleResult
	{
		EScheduleResult_Fail    = 0,
		EScheduleResult_Invoke  = 1,
		EScheduleResult_Pending = 2,
	};

public:
	virtual 
	EScheduleResult
	ScheduleV (
		IFunction* pFunction, 
		va_list va
		) = 0; // the only overridable

	EScheduleResult
	Schedule (
		IFunction* pFunction, 
		...
		)
	{
		return ScheduleV (pFunction, va_start_e (pFunction));
	}

	template <typename TArgPacker>
	EScheduleResult
	ScheduleV (
		ECallingConvention Convention,
		void* pfn, 
		va_list va
		)
	{
		CFunctionT <exe::CArgT <void>, TArgPacker> Function (Convention, pfn);
		return ScheduleV (&Function, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	Schedule (
		ECallingConvention Convention,
		void* pfn, 
		...
		)
	{
		return ScheduleV <TArgPacker> (Convention, pfn, va_start_e (pfn));
	}

	template <typename TArgPacker>
	EScheduleResult
	ScheduleV (
		void* pfn, 
		va_list va
		)
	{
		return ScheduleV <TArgPacker> (ECallingConvention_Cdecl, pfn, va);
	}

	template <typename TArgPacker>
	EScheduleResult
	Schedule (
		void* pfn, 
		...
		)
	{
		return ScheduleV <TArgPacker> (pfn, va_start_e (pfn));
	}

	intptr_t
	SyncScheduleV (
		IFunction* pFunction, 
		va_list va
		);

	intptr_t
	SyncSchedule (
		IFunction* pFunction, 
		...
		)
	{
		return SyncScheduleV (pFunction, va_start_e (pFunction));
	}

	template <typename TArgPacker>
	intptr_t
	SyncScheduleV (
		ECallingConvention Convention,
		void* pfn, 
		va_list va
		)
	{
		CFunctionT <exe::CArgT <void>, TArgPacker> Function (Convention, pfn);
		return SyncScheduleV (&Function, va);
	}

	template <typename TArgPacker>
	intptr_t
	SyncSchedule (
		ECallingConvention Convention,
		void* pfn, 
		...
		)
	{
		return SyncScheduleV <TArgPacker> (Convention, pfn, va_start_e (pfn));
	}

	template <typename TArgPacker>
	intptr_t
	SyncScheduleV (
		void* pfn, 
		va_list va
		)
	{
		return SyncScheduleV <TArgPacker> (ECallingConvention_Cdecl, pfn, va);
	}

	template <typename TArgPacker>
	intptr_t
	SyncSchedule (
		void* pfn, 
		...
		)
	{
		return SyncScheduleV <TArgPacker> (pfn, va_start_e (pfn));
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
