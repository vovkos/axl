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

// finding lowest bit's index with binary search

inline
uint8_t
getLoBitIdx8 (uint8_t x)
{
	return (x & 0x0f) ?
		(x & 0x03) ?
			(x & 0x01) ? 0 : 1 :
			(x & 0x04) ? 2 : 3 :
		(x & 0x30) ?
			(x & 0x10) ? 4 : 5 :
			(x & 0x40) ? 6 : 7;
}

inline
uint8_t
getLoBitIdx16 (uint16_t x)
{
	return (x & 0x00ff) ?
		getLoBitIdx8 ((uint8_t) x) :
		(8 + getLoBitIdx8 ((uint8_t) (x >> 8)));
}

inline
uint8_t
getLoBitIdx32 (uint32_t x)
{
	return (x & 0x0000ffff) ?
		getLoBitIdx16 ((uint16_t) x) :
		(16 + getLoBitIdx16 ((uint16_t) (x >> 16)));
}

inline
uint8_t
getLoBitIdx64 (uint64_t x)
{
	return (x & 0x00000000ffffffffLL) ?
		getLoBitIdx32 ((uint32_t) x) :
		(32 + getLoBitIdx32 ((uint32_t) (x >> 32)));
}

//..............................................................................

// finding highest bit's index with binary search

inline
uint8_t
getHiBitIdx8 (uint8_t x)
{
	return (x & 0xf0) ?
		(x & 0xc0) ?
			(x & 0x80) ? 7 : 6 :
			(x & 0x20) ? 5 : 4 :
		(x & 0x0c) ?
			(x & 0x08) ? 3 : 2 :
			(x & 0x02) ? 1 : 0;
}

inline
uint8_t
getHiBitIdx16 (uint16_t x)
{
	return (x & 0xff00) ?
		(8 + getHiBitIdx8 ((uint8_t) (x >> 8))) :
		getHiBitIdx8 ((uint8_t) x);
}

inline
uint8_t
getHiBitIdx32 (uint32_t x)
{
	return (x & 0xffff0000) ?
		(16 + getHiBitIdx16 ((uint16_t) (x >> 16))) :
		getHiBitIdx16 ((uint16_t) x);
}

inline
uint8_t
getHiBitIdx64 (uint64_t x)
{
	return (x & 0xffffffff00000000LL) ?
		(32 + getHiBitIdx32 ((uint32_t) (x >> 32))) :
		getHiBitIdx32 ((uint32_t) x);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (AXL_PTR_BITS == 64)

#define getLoBitIdx  getLoBitIdx64
#define getHiBitIdx  getHiBitIdx64

#else

#define getLoBitIdx  getLoBitIdx32
#define getHiBitIdx  getHiBitIdx32

#endif

//..............................................................................

// bitmask generation

inline
uint8_t
getLoBitmask8 (size_t to)
{
	return to >= 8 ? (uint8_t) -1 : ((uint8_t) 1 << (uint8_t) to) - 1;
}

inline
uint8_t
getHiBitmask8 (size_t from)
{
	return from >= 8 ? 0 : ~(((uint8_t) 1 << (uint8_t) from) - 1);
}

inline
uint8_t
getBitmask8 (
	size_t from,
	size_t to
	)
{
	return getLoBitmask8 (to) & getHiBitmask8 (from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint16_t
getLoBitmask16 (size_t to)
{
	return to >= 16 ? (uint16_t) -1 : ((uint16_t) 1 << (uint16_t) to) - 1;
}

inline
uint16_t
getHiBitmask16 (size_t from)
{
	return from >= 16 ? 0 : ~(((uint16_t) 1 << (uint16_t) from) - 1);
}

inline
uint16_t
getBitmask16 (
	size_t from,
	size_t to
	)
{
	return getLoBitmask16 (to) & getHiBitmask16 (from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint32_t
getLoBitmask32 (size_t to)
{
	return to >= 32 ? (uint32_t) -1 : ((uint32_t) 1 << (uint32_t) to) - 1;
}

inline
uint32_t
getHiBitmask32 (size_t from)
{
	return from >= 32 ? 0 : ~(((uint32_t) 1 << (uint32_t) from) - 1);
}

inline
uint32_t
getBitmask32 (
	size_t from,
	size_t to
	)
{
	return getLoBitmask32 (to) & getHiBitmask32 (from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint64_t
getLoBitmask64 (size_t to)
{
	return to >= 64 ? (uint64_t) -1 : ((uint64_t) 1 << (uint64_t) to) - 1;
}

inline
uint64_t
getHiBitmask64 (size_t from)
{
	return from >= 64 ? 0 : ~(((uint64_t) 1 << (uint64_t) from) - 1);
}

inline
uint64_t
getBitmask64 (
	size_t from,
	size_t to
	)
{
	return getLoBitmask64 (to) & getHiBitmask64 (from);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (AXL_PTR_BITS == 64)

#define getBitmask   getBitmask64
#define getLoBitmask getLoBitmask64
#define getHiBitmask getHiBitmask64

#else

#define getBitmask   getBitmask32
#define getLoBitmask getLoBitmask32
#define getHiBitmask getHiBitmask32

#endif

//..............................................................................

// finding the closest power of 2 which is
// (gt) greater than... or
// (ge) greater or equal

inline
size_t
getMinPower2Gt (size_t size)
{
	size_t hiBitIdx = getHiBitIdx (size);
	return 2 << hiBitIdx;
}

inline
size_t
getMinPower2Ge (size_t size)
{
	return size && !(size & (size - 1)) ? size : getMinPower2Gt (size);
}

//..............................................................................

} // namespace sl
} // namespace axl
