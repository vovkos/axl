// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
CallOnce (
	TFunction Functor,
	TArgument Argument,
	volatile int32_t* pFlag = NULL
	)
{
	enum EOnce
	{
		EOnce_Uninitialized = 0,
		EOnce_Initializing  = 1,
		EOnce_Initialized   = 2,
	};

	static volatile int32_t _Flag = 0;
	if (!pFlag)
		pFlag = &_Flag;

	int32_t Value = *pFlag;
	if (Value == EOnce_Initialized)
		return;

	if (Value == EOnce_Uninitialized && // try to save one interlocked cmpxcg
		mt::AtomicCmpXchg (pFlag, EOnce_Uninitialized, EOnce_Initializing) == EOnce_Uninitialized)
	{
		Functor (Argument);
		mt::AtomicXchg (pFlag, EOnce_Initialized);
	}
	else do
	{
		mt::YieldProcessor ();
	} while (*pFlag != EOnce_Initialized);
}

//.............................................................................

} // namespace mt
} // namespace axl
