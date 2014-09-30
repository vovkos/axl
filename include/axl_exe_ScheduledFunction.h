// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_SCHEDULEDFUNCTION_H

#include "axl_exe_Function.h"
#include "axl_exe_Scheduler.h"

namespace axl {
namespace exe {

//.............................................................................

template <
	typename CtxArg,
	typename ParArg
	>
class ScheduledFunction: public IFunction
{
public:
	AXL_OBJ_CLASS_0 (ScheduledFunction, IFunction)

protected:
	typedef Function <CtxArg, ParArg> Function;

	IScheduler* m_scheduler;
	Function m_function;

public:
	ScheduledFunction (IScheduler* scheduler = NULL)
	{
		m_scheduler = scheduler;
	}

	ScheduledFunction (
		IScheduler* scheduler,
		int convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (scheduler, convention, pf, va);
	}

	ScheduledFunction (
		IScheduler* scheduler,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (scheduler, pf, va);
	}

	void
	setupV (
		IScheduler* scheduler,
		CallConvKind convention,
		void* pf,
		axl_va_list va
		)
	{
		m_scheduler = scheduler;
		m_function.setupV (convention, pf, va);
	}

	void
	setupV (
		IScheduler* scheduler,
		void* pf,
		axl_va_list va
		)
	{
		setupV (scheduler, CallConvKind_Cdecl, pf, va);
	}

	void
	setup (
		IScheduler* scheduler,
		CallConvKind convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (scheduler, convention, pf, va);
	}

	void
	setup (
		IScheduler* scheduler,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (scheduler, pf, va);
	}

	virtual 
	IArgPacker* 
	getArgPacker ()
	{
		return m_function.getArgPacker ();
	}

	virtual 
	intptr_t 
	invokeV (axl_va_list va)
	{
		if (!m_scheduler)
			return m_function.invokeV (va);

		m_scheduler->scheduleV (&m_function, va);
		return 0;
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
