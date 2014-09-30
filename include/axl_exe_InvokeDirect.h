// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_INVOKEDIRECT_H

#include "axl_mem_Block.h"

//.............................................................................

extern "C" 
intptr_t
AXL_STDCALL
axl_exe_InvokeDirect (
	void* pf,
	intptr_t callConv,
	axl::mem::Block* stack,
	size_t count
	);

namespace axl {
namespace exe {

//.............................................................................

enum CallConvKind
{
	CallConvKind_Cdecl   = 0,
	CallConvKind_Stdcall = 1,
};

//.............................................................................

inline
intptr_t
invokeDirect (
	void* pf,
	CallConvKind callConv,
	mem::Block* stack,
	size_t count
	)
{
	return axl_exe_InvokeDirect (pf, callConv, stack, count);
}

//.............................................................................

} // namespace exe
} // namespace axl
