// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_ATOMIC_H

#include "axl_g_Def.h"

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN || _AXL_ENV == AXL_ENV_NT)

inline
int32_t
AtomicXchg (
	volatile int32_t* p,
	int32_t Value
	)	
{
	return InterlockedExchange ((long*) p, Value);
}

inline
int64_t
AtomicXchg (
	volatile int64_t* p,
	int64_t Value
	)	
{
	return InterlockedExchange64 (p, Value);
}

inline
int32_t
AtomicCmpXchg (
	volatile int32_t* p,
	int32_t CmpValue,
	int32_t NewValue
	)	
{
	return InterlockedCompareExchange ((long*) p, NewValue, CmpValue); // inverse order!
}

inline
int64_t
AtomicCmpXchg (
	volatile int64_t* p,
	int64_t CmpValue,
	int64_t NewValue
	)	
{
	return InterlockedCompareExchange64 (p, NewValue, CmpValue); // inverse order!
}

inline
int32_t
AtomicInc (volatile int32_t* p)	
{
	return InterlockedIncrement ((long*) p);
}

inline
int64_t
AtomicInc (volatile int64_t* p)	
{
	return InterlockedIncrement64 (p);
}

inline
int32_t
AtomicDec (volatile int32_t* p)	
{
	return InterlockedDecrement ((long*) p);
}

inline
int64_t
AtomicDec (volatile int64_t* p)	
{
	return InterlockedDecrement64 (p);
}

#elif (_AXL_CPP == AXL_CPP_GCC)

inline
int32_t
AtomicXchg (
	volatile int32_t* p,
	int32_t Value
	)	
{
	return __sync_lock_test_and_set (p, Value);
}

inline
int64_t
AtomicXchg (
	volatile int64_t* p,
	int64_t Value
	)	
{
	return __sync_lock_test_and_set (p, Value);
}

inline
int32_t
AtomicCmpXchg (
	volatile int32_t* p,
	int32_t CmpValue,
	int32_t NewValue
	)	
{
	return __sync_val_compare_and_swap (p, CmpValue, NewValue);
}

inline
int64_t
AtomicCmpXchg (
	volatile int64_t* p,
	int64_t CmpValue,
	int64_t NewValue
	)	
{
	return __sync_val_compare_and_swap (p, CmpValue, NewValue);
}

inline
int32_t
AtomicInc (volatile int32_t* p)	
{
	return __sync_add_and_fetch (p, 1);
}

inline
int64_t
AtomicInc (volatile int64_t* p)	
{
	return __sync_add_and_fetch (p, 1);
}

inline
int32_t
AtomicDec (volatile int32_t* p)	
{
	return __sync_sub_and_fetch (p, 1);
}

inline
int64_t
AtomicDec (volatile int64_t* p)	
{
	return __sync_sub_and_fetch (p, 1);
}

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
AtomicUpdatePeak (
	volatile int32_t* pPeak,
	int32_t Current
	)
{
	for (;;)
	{
		int32_t Old = *pPeak;
		
		if (Current <= Old)
			break;

		if (AtomicCmpXchg (pPeak, Old, Current) == Old)
			break;
	}
}

//.............................................................................
	
inline
void
YieldProcessor ()
{
#if (_AXL_ENV == AXL_ENV_WIN || _AXL_ENV == AXL_ENV_NT)
	::YieldProcessor ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	sched_yield ();
#endif
}

//.............................................................................

} // namespace mt
} // namespace axl
