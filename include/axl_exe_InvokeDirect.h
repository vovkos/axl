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
	intptr_t CallConv,
	axl::mem::TBlock* pStack,
	size_t Count
	);

namespace axl {
namespace exe {

//.............................................................................

enum ECallConv
{
	ECallConv_Cdecl   = 0,
	ECallConv_Stdcall = 1,
};

//.............................................................................

inline
intptr_t
InvokeDirect (
	void* pf,
	ECallConv CallConv,
	mem::TBlock* pStack,
	size_t Count
	)
{
	return axl_exe_InvokeDirect (pf, CallConv, pStack, Count);
}

//.............................................................................

} // namespace exe
} // namespace axl
