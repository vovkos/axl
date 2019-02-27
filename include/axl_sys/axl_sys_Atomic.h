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

#define _AXL_SYS_ATOMIC_H

#include "axl_g_Pch.h"

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

// for the sake of benchmarking intrinsic-vs-api, you can define:

// #define _AXL_USE_WINAPI_INTERLOCKED 1

// spoiler: no difference on amd64; about 15% speed up on x86

#if (_AXL_USE_WINAPI_INTERLOCKED)
#	define AXL_INTERLOCKED_FUNC32(name) (::Interlocked ## name)
#	define AXL_INTERLOCKED_FUNC64(name) (::Interlocked ## name)
#else // use intrinsics
#	pragma intrinsic(_InterlockedExchange)
#	pragma intrinsic(_InterlockedCompareExchange)
#	pragma intrinsic(_InterlockedIncrement)
#	pragma intrinsic(_InterlockedDecrement)
#	define AXL_INTERLOCKED_FUNC32(name) (_Interlocked ## name)
#	if (_AXL_CPU_AMD64)
#		pragma intrinsic(_InterlockedExchange64)
#		pragma intrinsic(_InterlockedCompareExchange64)
#		pragma intrinsic(_InterlockedIncrement64)
#		pragma intrinsic(_InterlockedDecrement64)
#		define AXL_INTERLOCKED_FUNC64(name) (_Interlocked ## name)
#	else
#		define AXL_INTERLOCKED_FUNC64(name) (::Interlocked ## name)
#	endif
#endif

inline
int32_t
atomicLoad(volatile int32_t* p)
{
	return *p;
}

inline
int32_t
atomicXchg(
	volatile int32_t* p,
	int32_t value
	)
{
	return AXL_INTERLOCKED_FUNC32(Exchange)((long*)p, value);
}

inline
int64_t
atomicXchg(
	volatile int64_t* p,
	int64_t value
	)
{
	return AXL_INTERLOCKED_FUNC64(Exchange64)(p, value);
}

inline
int32_t
atomicCmpXchg(
	volatile int32_t* p,
	int32_t cmpValue,
	int32_t newValue
	)
{
	return AXL_INTERLOCKED_FUNC32(CompareExchange)((long*)p, newValue, cmpValue); // inverse order!
}

inline
int64_t
atomicCmpXchg(
	volatile int64_t* p,
	int64_t cmpValue,
	int64_t newValue
	)
{
	return AXL_INTERLOCKED_FUNC64(CompareExchange64)(p, newValue, cmpValue); // inverse order!
}

inline
int32_t
atomicInc(volatile int32_t* p)
{
	return AXL_INTERLOCKED_FUNC32(Increment)((long*)p);
}

inline
int64_t
atomicInc(volatile int64_t* p)
{
	return AXL_INTERLOCKED_FUNC64(Increment64)(p);
}

inline
int32_t
atomicDec(volatile int32_t* p)
{
	return AXL_INTERLOCKED_FUNC32(Decrement)((long*)p);
}

inline
int64_t
atomicDec(volatile int64_t* p)
{
	return AXL_INTERLOCKED_FUNC64(Decrement64)(p);
}

#if (AXL_PTR_BITS == 64)

inline
int64_t
atomicLoad(volatile int64_t* p)
{
	return *p;
}

inline
size_t
atomicLoad(volatile size_t* p)
{
	return *p;
}

inline
size_t
atomicXchg(
	volatile size_t* p,
	size_t value
	)
{
	return AXL_INTERLOCKED_FUNC64(Exchange64)((int64_t*)p, value);
}

inline
size_t
atomicCmpXchg(
	volatile size_t* p,
	size_t cmpValue,
	size_t newValue
	)
{
	return AXL_INTERLOCKED_FUNC64(CompareExchange64)((int64_t*)p, newValue, cmpValue); // inverse order!
}

inline
size_t
atomicInc(volatile size_t* p)
{
	return AXL_INTERLOCKED_FUNC64(Increment64)((int64_t*)p);
}

inline
size_t
atomicDec(volatile size_t* p)
{
	return AXL_INTERLOCKED_FUNC64(Decrement64)((int64_t*)p);
}

#else

inline
int64_t
atomicLoad(volatile int64_t* p)
{
	return AXL_INTERLOCKED_FUNC64(CompareExchange64)(p, 0, 0); // any value will do
}

inline
size_t
atomicLoad(volatile size_t* p)
{
	return *p;
}

inline
size_t
atomicXchg(
	volatile size_t* p,
	size_t value
	)
{
	return AXL_INTERLOCKED_FUNC32(Exchange)((long*)p, value);
}

inline
size_t
atomicCmpXchg(
	volatile size_t* p,
	size_t cmpValue,
	size_t newValue
	)
{
	return AXL_INTERLOCKED_FUNC32(CompareExchange)((long*)p, newValue, cmpValue); // inverse order!
}

inline
size_t
atomicInc(volatile size_t* p)
{
	return AXL_INTERLOCKED_FUNC32(Increment)((long*)p);
}

inline
size_t
atomicDec(volatile size_t* p)
{
	return AXL_INTERLOCKED_FUNC32(Decrement)((long*)p);
}

#endif

#elif (_AXL_CPP_GCC)

inline
int32_t
atomicLoad(volatile int32_t* p)
{
	return *p;
}

inline
int32_t
atomicXchg(
	volatile int32_t* p,
	int32_t value
	)
{
	return __sync_lock_test_and_set(p, value);
}

inline
int64_t
atomicXchg(
	volatile int64_t* p,
	int64_t value
	)
{
	return __sync_lock_test_and_set(p, value);
}

inline
size_t
atomicXchg(
	volatile size_t* p,
	size_t value
	)
{
	return __sync_lock_test_and_set(p, value);
}

inline
int32_t
atomicCmpXchg(
	volatile int32_t* p,
	int32_t cmpValue,
	int32_t newValue
	)
{
	return __sync_val_compare_and_swap(p, cmpValue, newValue);
}

inline
int64_t
atomicCmpXchg(
	volatile int64_t* p,
	int64_t cmpValue,
	int64_t newValue
	)
{
	return __sync_val_compare_and_swap(p, cmpValue, newValue);
}

inline
size_t
atomicCmpXchg(
	volatile size_t* p,
	size_t cmpValue,
	size_t newValue
	)
{
	return __sync_val_compare_and_swap(p, cmpValue, newValue);
}

inline
int32_t
atomicInc(volatile int32_t* p)
{
	return __sync_add_and_fetch(p, 1);
}

inline
int64_t
atomicInc(volatile int64_t* p)
{
	return __sync_add_and_fetch(p, 1);
}

inline
size_t
atomicInc(volatile size_t* p)
{
	return __sync_add_and_fetch(p, 1);
}

inline
int32_t
atomicDec(volatile int32_t* p)
{
	return __sync_sub_and_fetch(p, 1);
}

inline
int64_t
atomicDec(volatile int64_t* p)
{
	return __sync_sub_and_fetch(p, 1);
}

inline
size_t
atomicDec(volatile size_t* p)
{
	return __sync_sub_and_fetch(p, 1);
}

#if (_AXL_OS_DARWIN)

inline
intptr_t
atomicXchg(
	volatile intptr_t* p,
	intptr_t value
	)
{
	return __sync_lock_test_and_set(p, value);
}

inline
intptr_t
atomicCmpXchg(
	volatile intptr_t* p,
	intptr_t cmpValue,
	intptr_t newValue
	)
{
	return __sync_val_compare_and_swap(p, cmpValue, newValue);
}

inline
intptr_t
atomicInc(volatile intptr_t* p)
{
	return __sync_add_and_fetch(p, 1);
}

inline
intptr_t
atomicDec(volatile intptr_t* p)
{
	return __sync_sub_and_fetch(p, 1);
}

#endif

#if (AXL_PTR_BITS == 64)

inline
int64_t
atomicLoad(volatile int64_t* p)
{
	return *p;
}

inline
int128_t
atomicLoad(volatile int128_t* p)
{
	return __sync_val_compare_and_swap(p, 0, 0); // any value will do
}

inline
int128_t
atomicXchg(
	volatile int128_t* p,
	int128_t value
	)
{
	return __sync_lock_test_and_set(p, value);
}

inline
int128_t
atomicCmpXchg(
	volatile int128_t* p,
	int128_t cmpValue,
	int128_t newValue
	)
{
	return __sync_val_compare_and_swap(p, cmpValue, newValue);
}

inline
int128_t
atomicInc(volatile int128_t* p)
{
	return __sync_add_and_fetch(p, 1);
}

inline
int128_t
atomicDec(volatile int128_t* p)
{
	return __sync_sub_and_fetch(p, 1);
}

#else // 32

inline
int64_t
atomicLoad(volatile int64_t* p)
{
	return __sync_val_compare_and_swap(p, 0, 0); // any value will do
}

#endif

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
atomicUpdatePeak(
	volatile int32_t* peak,
	int32_t current
	)
{
	for (;;)
	{
		int32_t old = *peak;

		if (current <= old)
			break;

		if (atomicCmpXchg(peak, old, current) == old)
			break;
	}
}

inline
void
yieldProcessor()
{
#if (_AXL_OS_WIN)
	::YieldProcessor();
#elif (_AXL_OS_POSIX)
	sched_yield();
#endif
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
atomicLock(volatile int32_t* lock)
{
	for (;;)
	{
		uint32_t result = sys::atomicCmpXchg(lock, 0, 1);
		if (result == 0)
			break;

		sys::yieldProcessor();
	}
}

inline
void
atomicUnlock(volatile int32_t* lock)
{
	uint32_t result = sys::atomicCmpXchg(lock, 1, 0);
	ASSERT(result == 1);
}

//..............................................................................

} // namespace sys
} // namespace axl
