// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_PROXYFUNCTION_H

#include "axl_exe_Function.h"
#include "axl_exe_Scheduler.h"

namespace axl {
namespace exe {

//.............................................................................

class CProxyFunction: public IFunction
{
public:
	IScheduler* m_pScheduler;
	IFunction* m_pFunction;

	ref::CPtrT <void> m_KeepAlive;

public:
	CProxyFunction (
		IScheduler* pScheduler = NULL,
		IFunction* pFunction = NULL,
		ref::CPtrT <void> KeepAlive = ref::CPtrT <void> ()
		)
	{
		m_pFunction = pFunction;
		m_pScheduler = pScheduler;
		m_KeepAlive = KeepAlive;
	}

	CProxyFunction (const CProxyFunction& Src)
	{
		Copy (Src);
	}

	CProxyFunction& 
	operator = (const CProxyFunction& Src)
	{
		Copy (Src);
		return *this;
	}

	virtual 
	IArgPacker* 
	GetArgPacker ()
	{
		return m_pFunction->GetArgPacker ();
	}

	virtual 
	intptr_t 
	InvokeV (va_list va)
	{
		if (!m_pScheduler)
			return m_pFunction->InvokeV (va);

		m_pScheduler->ScheduleV (m_pFunction, va);
		return 0;
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
