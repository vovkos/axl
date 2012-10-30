// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_SWAPBYTEORDER_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

inline 
uint16_t
SwapByteOrder16 (uint16_t Value)
{
	return _byteswap_ushort (Value);
}

inline 
uint32_t
SwapByteOrder32 (uint32_t Value)
{
	return _byteswap_ulong (Value);
}

inline 
uint64_t
SwapByteOrder64 (uint64_t Value)
{
	return _byteswap_uint64 (Value);
}

inline 
void
SwapByteOrder (
	void* pDst,
	const void* pSrc,
	size_t Size
	)
{
	switch (Size)
	{
	case 2:
		*(uint16_t*) pDst = SwapByteOrder16 (*(uint16_t*)  pSrc);
		break;

	case 4:
		*(uint32_t*) pDst = SwapByteOrder32 (*(uint32_t*)  pSrc);
		break;

	case 8:
		*(uint64_t*) pDst = SwapByteOrder64 (*(uint64_t*)  pSrc);
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

} // namespace rtl
} // namespace axl
