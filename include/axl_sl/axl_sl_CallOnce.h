//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_SL_CALLONCE_H

#include "axl_sys_Atomic.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Functor,
	typename Argument
	>
void
callOnce(
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
		sys::atomicCmpXchg(flag, OnceFlag_Uninitialized, OnceFlag_Initializing) == OnceFlag_Uninitialized)
	{
		functor(argument);
		sys::atomicXchg(flag, OnceFlag_Initialized);
	}
	else do
	{
		sys::yieldProcessor();
	} while (*flag != OnceFlag_Initialized);
}

//..............................................................................

} // namespace sl
} // namespace axl
