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
atomicXchg (
	volatile int32_t* p,
	int32_t value
	)	
{
	return ::InterlockedExchange ((long*) p, value);
}

inline
int64_t
atomicXchg (
	volatile int64_t* p,
	int64_t value
	)	
{
	return ::InterlockedExchange64 (p, value);
}

inline
int32_t
atomicCmpXchg (
	volatile int32_t* p,
	int32_t cmpValue,
	int32_t newValue
	)	
{
	return ::InterlockedCompareExchange ((long*) p, newValue, cmpValue); // inverse order!
}

inline
int64_t
atomicCmpXchg (
	volatile int64_t* p,
	int64_t cmpValue,
	int64_t newValue
	)	
{
	return ::InterlockedCompareExchange64 (p, newValue, cmpValue); // inverse order!
}

inline
int32_t
atomicInc (volatile int32_t* p)	
{
	return ::InterlockedIncrement ((long*) p);
}

inline
int64_t
atomicInc (volatile int64_t* p)	
{
	return ::InterlockedIncrement64 (p);
}

inline
int32_t
atomicDec (volatile int32_t* p)	
{
	return ::InterlockedDecrement ((long*) p);
}

inline
int64_t
atomicDec (volatile int64_t* p)	
{
	return ::InterlockedDecrement64 (p);
}

#elif (_AXL_CPP == AXL_CPP_GCC)

inline
int32_t
atomicXchg (
	volatile int32_t* p,
	int32_t value
	)	
{
	return __sync_lock_test_and_set (p, value);
}

inline
int64_t
atomicXchg (
	volatile int64_t* p,
	int64_t value
	)	
{
	return __sync_lock_test_and_set (p, value);
}

inline
int32_t
atomicCmpXchg (
	volatile int32_t* p,
	int32_t cmpValue,
	int32_t newValue
	)	
{
	return __sync_val_compare_and_swap (p, cmpValue, newValue);
}

inline
int64_t
atomicCmpXchg (
	volatile int64_t* p,
	int64_t cmpValue,
	int64_t newValue
	)	
{
	return __sync_val_compare_and_swap (p, cmpValue, newValue);
}

inline
int32_t
atomicInc (volatile int32_t* p)	
{
	return __sync_add_and_fetch (p, 1);
}

inline
int64_t
atomicInc (volatile int64_t* p)	
{
	return __sync_add_and_fetch (p, 1);
}

inline
int32_t
atomicDec (volatile int32_t* p)	
{
	return __sync_sub_and_fetch (p, 1);
}

inline
int64_t
atomicDec (volatile int64_t* p)	
{
	return __sync_sub_and_fetch (p, 1);
}

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
atomicUpdatePeak (
	volatile int32_t* peak,
	int32_t current
	)
{
	for (;;)
	{
		int32_t old = *peak;
		
		if (current <= old)
			break;

		if (atomicCmpXchg (peak, old, current) == old)
			break;
	}
}

inline
void
yieldProcessor ()
{
#if (_AXL_ENV == AXL_ENV_WIN || _AXL_ENV == AXL_ENV_NT)
	::YieldProcessor ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	sched_yield ();
#endif
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void 
atomicLock (volatile int32_t* lock)
{
	for (;;)
	{
		uint32_t result = mt::atomicCmpXchg (lock, 0, 1);
		if (result == 0)
			break;

		mt::yieldProcessor ();
	}
}

inline
void 
atomicUnlock (volatile int32_t* lock)
{
	uint32_t result = mt::atomicCmpXchg (lock, 1, 0);
	ASSERT (result == 1);
}

//.............................................................................

} // namespace mt
} // namespace axl
