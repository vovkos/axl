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

#define _AXL_SL_BYTEORDER_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

inline
uint16_t
swapByteOrder16(uint16_t value)
{
#if (_AXL_CPP_MSC)
	return _byteswap_ushort(value);
#elif (_AXL_OS_DARWIN)
	return __DARWIN_OSSwapInt16(value);
#else
	return bswap_16(value);
#endif
}

inline
uint32_t
swapByteOrder32(uint32_t value)
{
#if (_AXL_CPP_MSC)
	return _byteswap_ulong(value);
#elif (_AXL_OS_DARWIN)
	return __DARWIN_OSSwapInt32(value);
#else
	return bswap_32(value);
#endif
}

inline
uint64_t
swapByteOrder64(uint64_t value)
{
#if (_AXL_CPP_MSC)
	return _byteswap_uint64(value);
#elif (_AXL_OS_DARWIN)
	return __DARWIN_OSSwapInt64(value);
#else
	return bswap_64(value);
#endif
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
swapByteOrder(
	void* dst,
	const void* src,
	size_t size
	)
{
	switch (size)
	{
	case 2:
		*(uint16_t*)dst = swapByteOrder16(*(uint16_t*)  src);
		break;

	case 4:
		*(uint32_t*)dst = swapByteOrder32(*(uint32_t*)  src);
		break;

	case 8:
		*(uint64_t*)dst = swapByteOrder64(*(uint64_t*)  src);
		break;

	default:
		ASSERT(false);
	}
}

//..............................................................................

} // namespace sl
} // namespace axl
