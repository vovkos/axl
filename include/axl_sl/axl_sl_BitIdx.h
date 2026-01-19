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

#define _AXL_SL_BITIDX_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

// finding the lowest set bit via two's complement

inline
uint8_t
getLoBit8(uint8_t x) {
	return x & -x;
}

inline
uint16_t
getLoBit16(uint16_t x) {
	return x & -x;
}

inline
uint32_t
getLoBit32(uint32_t x) {
	return x & -x;
}

inline
uint64_t
getLoBit64(uint64_t x) {
	return x & -x;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// finding the highest set bit, branch-free -- from Hacker's Delight

inline
uint8_t
getHiBit8(uint8_t x) {
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	return x - (x >> 1);
}

inline
uint16_t
getHiBit16(uint16_t x) {
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	return x - (x >> 1);
}

inline
uint32_t
getHiBit32(uint32_t x) {
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x - (x >> 1);
}

inline
uint64_t
getHiBit64(uint64_t x) {
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	x = x | (x >> 32);
	return x - (x >> 1);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// finding closest power-of-2, branch-free -- from Hacker's Delight

inline
uint8_t
getPowerOf2Ge8(uint8_t x) {
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	return x + 1;
}

inline
uint16_t
getPowerOf2Ge16(uint16_t x) {
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	return x + 1;
}

inline
uint32_t
getPowerOf2Ge32(uint32_t x) {
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
}

inline
uint64_t
getPowerOf2Ge64(uint64_t x) {
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	x = x | (x >> 32);
	return x + 1;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (AXL_PTR_BITS == 64)
#	define getLoBit      getLoBit64
#	define getHiBit      getHiBit64
#	define getPowerOf2Ge getPowerOf2Ge64
#else
#	define getLoBit      getLoBit32
#	define getHiBit      getHiBit32
#	define getPowerOf2Ge getPowerOf2Ge32
#endif

//..............................................................................

// finding lowest/highest bit's index using compiler intrinsics

#if (_AXL_CPP_GCC)

inline
uint8_t
getLoBitIdx32(uint32_t x) {
	ASSERT(x);
    return (uint8_t)__builtin_ctz(x);
}

inline
uint8_t
getLoBitIdx64(uint64_t x) {
	ASSERT(x);
    return (uint8_t)__builtin_ctzll(x);
}

inline
uint8_t
getHiBitIdx32(uint32_t x) {
	ASSERT(x);
    return (uint8_t)(31 - __builtin_clz(x));
}

inline
uint8_t
getHiBitIdx64(uint64_t x) {
	ASSERT(x);
	return (uint8_t)(63 - __builtin_clzll(x));
}

#elif (_AXL_CPP_MSC)

inline
uint8_t
getLoBitIdx32(uint32_t x) {
	ASSERT(x);
    ulong_t i;
    _BitScanForward(&i, x);
    return (uint8_t)i;
}

inline
uint8_t
getHiBitIdx32(uint32_t x) {
	ASSERT(x);
    ulong_t i;
    _BitScanReverse(&i, x);
    return (uint8_t)i;
}

inline
uint8_t
getLoBitIdx64(uint64_t x) {
	ASSERT(x);
    ulong_t i;
#if (_AXL_CPU_AMD64 || _AXL_CPU_ARM64)
    _BitScanForward64(&i, x);
#else
	if (x & 0xffffffff)
		_BitScanForward(&i, (uint32_t)x);
	else {
		_BitScanForward(&i, (uint32_t)(x >> 32));
		i += 32;
	}
#endif
    return (uint8_t)i;
}

inline
uint8_t
getHiBitIdx64(uint64_t x) {
	ASSERT(x);
    ulong_t i;
#if (_AXL_CPU_AMD64 || _AXL_CPU_ARM64)
    _BitScanReverse64(&i, x);
#else
	if (x & 0xffffffff00000000) {
		_BitScanReverse(&i, (uint32_t)(x >> 32));
		i += 32;
	} else
		_BitScanReverse(&i, (uint32_t)x);
#endif
    return (uint8_t)i;
}

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint8_t
getLoBitIdx8(uint8_t x) {
    return getLoBitIdx32(x);
}

inline
uint8_t
getHiBitIdx8(uint8_t x) {
    return getHiBitIdx32(x);
}

inline
uint8_t
getLoBitIdx16(uint16_t x) {
    return getLoBitIdx32(x);
}

inline
uint8_t
getHiBitIdx16(uint16_t x) {
    return getHiBitIdx32(x);
}

#if (AXL_PTR_BITS == 64)
#	define getLoBitIdx  getLoBitIdx64
#	define getHiBitIdx  getHiBitIdx64
#else
#	define getLoBitIdx  getLoBitIdx32
#	define getHiBitIdx  getHiBitIdx32
#endif

//..............................................................................

// bitmask generation

// bit index 0..8

inline
uint8_t
getLoBitmask8(size_t to) {
	return (1 << (to & 15)) - 1;
}

inline
uint8_t
getHiBitmask8(size_t from) {
	return ~((1 << (from & 15)) - 1);
}

inline
uint8_t
getBitmask8(
	size_t from,
	size_t to
) {
	return getLoBitmask8(to) & getHiBitmask8(from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bit index 0..16 (inclusive)

inline
uint16_t
getLoBitmask16(size_t to) {
	return (1 << (to & 31)) - 1;
}

inline
uint16_t
getHiBitmask16(size_t from) {
	return ~((1 << (from & 31)) - 1);
}

inline
uint16_t
getBitmask16(
	size_t from,
	size_t to
) {
	return getLoBitmask16(to) & getHiBitmask16(from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bit index 0..32 (inclusive)

inline
uint32_t
getLoBitmask32(size_t to) {
	return ~(((to & 32) >> 5) - 1) | ((1 << (to & 31)) - 1);
}

inline
uint32_t
getHiBitmask32(size_t from) {
	return (((from & 32) >> 5) - 1) & ~((1 << (from & 31)) - 1);
}

inline
uint32_t
getBitmask32(
	size_t from,
	size_t to
) {
	return getLoBitmask32(to) & getHiBitmask32(from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bit index 0..64 (inclusive)

inline
uint64_t
getLoBitmask64(size_t to) {
	return ~(((to & 64ULL) >> 6) - 1) | ((1ULL << (to & 63)) - 1);
}

inline
uint64_t
getHiBitmask64(size_t from) {
	return (((from & 64ULL) >> 6) - 1) & ~((1ULL << (from & 63)) - 1);
}

inline
uint64_t
getBitmask64(
	size_t from,
	size_t to
) {
	return getLoBitmask64(to) & getHiBitmask64(from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (AXL_PTR_BITS == 64)
#	define getBitmask   getBitmask64
#	define getLoBitmask getLoBitmask64
#	define getHiBitmask getHiBitmask64
#else
#	define getBitmask   getBitmask32
#	define getLoBitmask getLoBitmask32
#	define getHiBitmask getHiBitmask32
#endif

//..............................................................................

template <typename T>
bool
isPowerOf2(T x) {
	return !(x & (x - 1));
}

template <typename T>
bool
isAligned(
	T x,
	size_t factor
) {
	ASSERT(isPowerOf2(factor));
	return !(x & (factor - 1));
}

template <
	size_t factor,
	typename T
>
bool
isAligned(T x) {
	ASSERT(isPowerOf2(factor));
	return !(x & (factor - 1));
}

template <typename T>
T
align(
	T x,
	size_t factor
) {
	ASSERT(isPowerOf2(factor));
	return (x + factor - 1) & ~((T)factor - 1);
}

template <
	size_t factor,
	typename T
>
T
align(T x) {
	ASSERT(isPowerOf2(factor));
	return (x + factor - 1) & ~((T)factor - 1);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <size_t growLimit>
size_t
getAllocSize(size_t size) {
	ASSERT(isPowerOf2(growLimit));
	return size < growLimit ? getPowerOf2Ge(size) : align<growLimit> (size);
}

inline
size_t
getAllocSize(size_t size) {
	return getAllocSize<4 * 1024> (size); // 4K grow limit
}

//..............................................................................

} // namespace sl
} // namespace axl
