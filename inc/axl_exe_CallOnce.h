// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_CALLONCE_H

#include "axl_g_Def.h"

namespace axl {
namespace exe {

//.............................................................................

template <typename T>
void 
CallOnce (
	T Functor,
	volatile long* pFlag = NULL
	)
{
	enum EOnce
	{
		EOnce_Uninitialized = 0,
		EOnce_Initializing  = 1,
		EOnce_Initialized   = 2,
	};

	static volatile long _Flag = 0;
	if (!pFlag)
		pFlag = &_Flag;

	long Value = *pFlag;
	if (Value == EOnce_Initialized)
		return;

	if (Value == EOnce_Uninitialized && // try to save one interlocked cmpxcg
		::InterlockedCompareExchange (pFlag, EOnce_Initializing, EOnce_Uninitialized) == EOnce_Uninitialized)
	{
		Functor ();
		::InterlockedExchange (pFlag, EOnce_Initialized);
	}
	else do
	{
		::YieldProcessor ();
	} while (*pFlag != EOnce_Initialized);
}

//.............................................................................

} // namespace exe
} // namespace axl
