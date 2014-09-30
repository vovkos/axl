// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_MT_CALLONCE_H

#include "axl_mt_Atomic.h"

namespace axl {
namespace mt {

//.............................................................................

template <
	typename Function,
	typename Argument
	>
void 
callOnce (
	Function functor,
	Argument argument,
	volatile int32_t* flag = NULL
	)
{
	enum OnceKind
	{
		OnceKind_Uninitialized = 0,
		OnceKind_Initializing  = 1,
		OnceKind_Initialized   = 2,
	};

	static volatile int32_t _Flag = 0;
	if (!flag)
		flag = &_Flag;

	int32_t value = *flag;
	if (value == OnceKind_Initialized)
		return;

	if (value == OnceKind_Uninitialized && // try to save one interlocked cmpxcg
		mt::atomicCmpXchg (flag, OnceKind_Uninitialized, OnceKind_Initializing) == OnceKind_Uninitialized)
	{
		functor (argument);
		mt::atomicXchg (flag, OnceKind_Initialized);
	}
	else do
	{
		mt::yieldProcessor ();
	} while (*flag != OnceKind_Initialized);
}

//.............................................................................

} // namespace mt
} // namespace axl
