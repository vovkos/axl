#include "pch.h"
#include "axl_exe_Thunk.h"
#include "axl_err_Error.h"

namespace axl {
namespace exe {

//.............................................................................

#pragma AXL_TODO ("implement proper thunk allocator")

TThunk* 
createThunk (
	void* pf,
	void* context
	)
{
	TThunk* thunk = (TThunk*) AXL_MEM_ALLOC (sizeof (TThunk));

#if (_AXL_CPU == AXL_CPU_X86)

	static TThunk _Thunk = 
	{
		0x042444c7, 0,  // mov		dword ptr [esp + 4], pContext
		0xe9,       0   // jmp		pf
	};

	*thunk = _Thunk;
	thunk->m_context = (uintptr_t) context;
	thunk->m_pfDiff = (uintptr_t) pf - (uintptr_t) (thunk + 1);

#elif (_AXL_CPU == AXL_CPU_AMD64)

	static TThunk _Thunk = 
	{
		0xb948,     0,  // mov		rcx,	pContext
		0xb848,     0,  // mov		rax,	pf
		0xe0ff	        // jmp		rax
	};

	*thunk = _Thunk;
	thunk->m_context = (uintptr_t) context;
	thunk->m_pf = (uintptr_t) pf;

#else
	#error unsupported processor
#endif

#if (_AXL_ENV == AXL_ENV_WIN)

	ulong_t oldProtect;
	
	bool_t result = 
		virtualProtect (thunk, sizeof (TThunk), PAGE_EXECUTE_READWRITE, &oldProtect) &&
		flushInstructionCache (getCurrentProcess (), thunk, sizeof (TThunk));

	if (!result)
	{
		err::setLastSystemError ();
		return NULL;
	}

#elif _AXL_ENV == AXL_ENV_NT

	zwFlushInstructionCache (zwCurrentProcess (), thunk, sizeof (TThunk));

#endif

	return thunk;
}

//.............................................................................

} // namespace exe
} // namespace axl
