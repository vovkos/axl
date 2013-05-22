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
	typename TCtxArg,
	typename TParArg
	>
class CScheduledFunctionT: public IFunction
{
public:
	AXL_OBJ_CLASS_0 (CScheduledFunctionT, IFunction)

protected:
	typedef CFunctionT <TCtxArg, TParArg> CFunction;

	IScheduler* m_pScheduler;
	CFunction m_Function;

public:
	CScheduledFunctionT (IScheduler* pScheduler = NULL)
	{
		m_pScheduler = pScheduler;
	}

	CScheduledFunctionT (
		IScheduler* pScheduler,
		int Convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (pScheduler, Convention, pf, va);
	}

	CScheduledFunctionT (
		IScheduler* pScheduler,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (pScheduler, pf, va);
	}

	void
	SetupV (
		IScheduler* pScheduler,
		ECallConv Convention,
		void* pf,
		axl_va_list va
		)
	{
		m_pScheduler = pScheduler;
		m_Function.SetupV (Convention, pf, va);
	}

	void
	SetupV (
		IScheduler* pScheduler,
		void* pf,
		axl_va_list va
		)
	{
		SetupV (pScheduler, ECallConv_Cdecl, pf, va);
	}

	void
	Setup (
		IScheduler* pScheduler,
		ECallConv Convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (pScheduler, Convention, pf, va);
	}

	void
	Setup (
		IScheduler* pScheduler,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (pScheduler, pf, va);
	}

	virtual 
	IArgPacker* 
	GetArgPacker ()
	{
		return m_Function.GetArgPacker ();
	}

	virtual 
	intptr_t 
	InvokeV (axl_va_list va)
	{
		if (!m_pScheduler)
			return m_Function.InvokeV (va);

		m_pScheduler->ScheduleV (&m_Function, va);
		return 0;
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
