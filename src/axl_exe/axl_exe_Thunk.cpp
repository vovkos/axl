#include "pch.h"
#include "axl_exe_Thunk.h"
#include "axl_err_Error.h"

namespace axl {
namespace exe {

//.............................................................................

#pragma AXL_TODO ("implement proper thunk allocator")

TThunk* 
CreateThunk (
	void* pf,
	void* pContext
	)
{
	TThunk* pThunk = (TThunk*) AXL_MEM_ALLOC (sizeof (TThunk));

#if (_AXL_CPU == AXL_CPU_X86)

	static TThunk _Thunk = 
	{
		0x042444c7, 0,  // mov		dword ptr [esp + 4], pContext
		0xe9,       0   // jmp		pf
	};

	*pThunk = _Thunk;
	pThunk->m_pContext = (uintptr_t) pContext;
	pThunk->m_pfDiff = (uintptr_t) pf - (uintptr_t) (pThunk + 1);

#elif (_AXL_CPU == AXL_CPU_AMD64)

	static TThunk _Thunk = 
	{
		0xb948,     0,  // mov		rcx,	pContext
		0xb848,     0,  // mov		rax,	pf
		0xe0ff	        // jmp		rax
	};

	*pThunk = _Thunk;
	pThunk->m_pContext = (uintptr_t) pContext;
	pThunk->m_pf = (uintptr_t) pf;

#else
	#error Unsupported processor
#endif

#if (_AXL_ENV == AXL_ENV_WIN)

	ulong_t OldProtect;
	
	bool_t Result = 
		VirtualProtect (pThunk, sizeof (TThunk), PAGE_EXECUTE_READWRITE, &OldProtect) &&
		FlushInstructionCache (GetCurrentProcess (), pThunk, sizeof (TThunk));

	if (!Result)
	{
		err::SetLastSystemError ();
		return NULL;
	}

#elif _AXL_ENV == AXL_ENV_NT

	ZwFlushInstructionCache (ZwCurrentProcess (), pThunk, sizeof (TThunk));

#endif

	return pThunk;
}

//.............................................................................

} // namespace exe
} // namespace axl
