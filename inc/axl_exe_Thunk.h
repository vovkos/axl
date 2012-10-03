// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2011. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_EXE_THUNK_H

#include "axl_g_Def.h"

namespace axl {
namespace exe {

//.............................................................................

//
// originally I had a non-desruptive thunk:
// pop		eax
// push		context
// push		eax
// jmp		pfn
//
// but that is not gonna work in Microsoft x64 calling convention
// so I revert back to ATL-style desruptive replacement of the first parameter
//

//.............................................................................

#pragma pack (push, 1)

struct TThunk
{
#if defined (_M_IX86)

	ulong_t   m_mov_esp4;
	uintptr_t m_pContext;
	uchar_t   m_jmp;
	uintptr_t m_pfnDiff;

#elif defined (_M_AMD64)

	ushort_t  m_mov_esp4;
	uintptr_t m_pContext;
	ushort_t  m_mov_rax;
	uintptr_t m_pfn;
	ushort_t  m_jmp_rax;

#else
	#error Unsupported processor
#endif
};

#pragma pack (pop)

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TThunk* 
CreateThunk (
	void* pfn, 
	void* pContext
	);

//.............................................................................

} // namespace exe
} // namespace axl

