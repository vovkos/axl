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
	typename TFunction,
	typename TArgument
	>
void 
callOnce (
	TFunction functor,
	TArgument argument,
	volatile int32_t* flag = NULL
	)
{
	enum EOnce
	{
		EOnce_Uninitialized = 0,
		EOnce_Initializing  = 1,
		EOnce_Initialized   = 2,
	};

	static volatile int32_t _Flag = 0;
	if (!flag)
		flag = &_Flag;

	int32_t value = *flag;
	if (value == EOnce_Initialized)
		return;

	if (value == EOnce_Uninitialized && // try to save one interlocked cmpxcg
		mt::atomicCmpXchg (flag, EOnce_Uninitialized, EOnce_Initializing) == EOnce_Uninitialized)
	{
		functor (argument);
		mt::atomicXchg (flag, EOnce_Initialized);
	}
	else do
	{
		mt::yieldProcessor ();
	} while (*flag != EOnce_Initialized);
}

//.............................................................................

} // namespace mt
} // namespace axl
