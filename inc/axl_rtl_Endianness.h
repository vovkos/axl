// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_ENDIANNESS_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

inline 
uint16_t
SwapEndianness16 (uint16_t Value)
{
	return _byteswap_ushort (Value);
}

inline 
uint32_t
SwapEndianness32 (uint32_t Value)
{
	return _byteswap_ulong (Value);
}

inline 
uint64_t
SwapEndianness64 (uint64_t Value)
{
	return _byteswap_uint64 (Value);
}

//.............................................................................

} // namespace rtl
} // namespace axl
