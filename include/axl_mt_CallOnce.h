// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_MT_CALLONCE_H

#include "axl_mt_Atomic.h"

namespace axl {
namespace mt {

//.............................................................................

template <
	typename Functor,
	typename Argument
	>
void 
callOnce (
	Functor functor,
	Argument argument,
	volatile int32_t* flag = NULL
	)
{
	enum OnceFlag
	{
		OnceFlag_Uninitialized = 0,
		OnceFlag_Initializing  = 1,
		OnceFlag_Initialized   = 2,
	};

	static volatile int32_t defaultFlag = 0;
	if (!flag)
		flag = &defaultFlag;

	int32_t value = *flag;
	if (value == OnceFlag_Initialized)
		return;

	if (value == OnceFlag_Uninitialized && // try to save one interlocked cmpxcg
		mt::atomicCmpXchg (flag, OnceFlag_Uninitialized, OnceFlag_Initializing) == OnceFlag_Uninitialized)
	{
		functor (argument);
		mt::atomicXchg (flag, OnceFlag_Initialized);
	}
	else do
	{
		mt::yieldProcessor ();
	} while (*flag != OnceFlag_Initialized);
}

//.............................................................................

} // namespace mt
} // namespace axl
