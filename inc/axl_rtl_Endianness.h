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
	uint16_t Lo = Value & 0x00ff;
	uint16_t Hi = (Value & 0xff00) >> 8;
	return (Lo << 8) | Hi;
}

inline 
uint32_t
SwapEndianness32 (uint32_t Value)
{
	uint32_t Lo = SwapEndianness16 ((uint16_t) (Value & 0x0000ffff));
	uint32_t Hi = SwapEndianness16 ((uint16_t) ((Value & 0xffff0000) >> 16));
	return (Lo << 16) | Hi;
}

inline 
uint64_t
SwapEndianness64 (uint64_t Value)
{
	uint64_t Lo = SwapEndianness32 ((uint32_t) (Value & 0x00000000ffffffff));
	uint64_t Hi = SwapEndianness32 ((uint32_t) ((Value & 0xffffffff00000000) >> 32));
	return (Lo << 32) | Hi;
}

//.............................................................................

} // namespace rtl
} // namespace axl
