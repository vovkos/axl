// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_INVOKEDIRECT_H

#include "axl_mem_Block.h"

//.............................................................................

extern "C" 
intptr_t
AXL_STDCALL
axl_exe_InvokeDirect (
	void* pfn,
	intptr_t CallingConvention,
	axl::mem::TBlock* pStack,
	size_t Count
	);

namespace axl {
namespace exe {

//.............................................................................

enum ECallingConvention 
{
	ECallingConvention_Cdecl   = 0,
	ECallingConvention_Stdcall = 1,
};

//.............................................................................

inline
intptr_t
InvokeDirect (
	void* pfn,
	ECallingConvention CallingConvention,
	mem::TBlock* pStack,
	size_t Count
	)
{
	return axl_exe_InvokeDirect (pfn, CallingConvention, pStack, Count);
}

//.............................................................................

} // namespace exe
} // namespace axl
