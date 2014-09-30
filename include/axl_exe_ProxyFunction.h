// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_PROXYFUNCTION_H

#include "axl_exe_Function.h"
#include "axl_exe_Scheduler.h"

namespace axl {
namespace exe {

//.............................................................................

class ProxyFunction: public IFunction
{
public:
	IScheduler* m_scheduler;
	IFunction* m_function;

	ref::Ptr <void> m_keepAlive;

public:
	ProxyFunction (
		IScheduler* scheduler = NULL,
		IFunction* function = NULL,
		ref::Ptr <void> keepAlive = ref::Ptr <void> ()
		)
	{
		m_function = function;
		m_scheduler = scheduler;
		m_keepAlive = keepAlive;
	}

	ProxyFunction (const ProxyFunction& src)
	{
		copy (src);
	}

	ProxyFunction& 
	operator = (const ProxyFunction& src)
	{
		copy (src);
		return *this;
	}

	virtual 
	IArgPacker* 
	getArgPacker ()
	{
		return m_function->getArgPacker ();
	}

	virtual 
	intptr_t 
	invokeV (axl_va_list va)
	{
		if (!m_scheduler)
			return m_function->invokeV (va);

		m_scheduler->scheduleV (m_function, va);
		return 0;
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
