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
// jmp		pf
//
// but that is not gonna work in Microsoft x64 calling convention
// so I revert back to ATL-style desruptive replacement of the first parameter
//

//.............................................................................

#pragma pack (push, 1)

struct TThunk
{
#if (_AXL_CPU == AXL_CPU_X86)

	ulong_t   m_mov_esp4;
	uintptr_t m_context;
	uchar_t   m_jmp;
	uintptr_t m_pfDiff;

#elif (_AXL_CPU == AXL_CPU_AMD64)

	ushort_t  m_mov_esp4;
	uintptr_t m_context;
	ushort_t  m_mov_rax;
	uintptr_t m_pf;
	ushort_t  m_jmp_rax;

#else
	#error unsupported processor
#endif
};

#pragma pack (pop)

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TThunk* 
createThunk (
	void* pf, 
	void* context
	);

//.............................................................................

} // namespace exe
} // namespace axl

