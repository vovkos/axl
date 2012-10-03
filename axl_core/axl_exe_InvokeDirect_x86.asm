		.386
		.model	small, C
		.code	_TEXT

		axl_exe_ECallingConvention_Cdecl   equ 0
		axl_exe_ECallingConvention_Stdcall equ 1
		 
		public	axl_exe_InvokeDirect@16

;..............................................................................
;
;		intptr_t
;		__stdcall
;		axl_exe_InvokeDirect (
;			void* pfn,
;			int CallingConvention,
;			axl_mem_TBlock* pStack,
;			size_t Count
;			);
;
;..............................................................................

axl_exe_InvokeDirect@16 proc \
		pfn               : ptr byte, \
		CallingConvention : dword, \
		pStack            : ptr byte, \
		Count             : dword \

		local _esi		  : dword
		local _ebx		  : dword
		local StackSize   : dword

		push	edi
		mov		_esi,	esi
		mov		_ebx,	ebx
	
		; count the full stack size		
		; ecx - loop index
		; eax - accumulator
		; edx - axl_exe_TBlock*
	
		xor		eax,	eax		
		mov		StackSize, eax
		mov		ecx,	Count
		test	ecx,	ecx
		jz		__call
		mov		edx,	pStack

	__count:
		add		eax,	dword ptr [edx + 4] ; pStack->m_Size
		add		edx,	8
		dec		ecx
		jnz		__count	
		mov		StackSize, eax

		; fill the stack
		; ebx - loop index (cause we need ecx for repsb)
		; eax - stack target
		; edx - axl_exe_TBlock*		

		sub     esp,    eax
		mov		eax,	esp
		mov		ebx,	Count
		mov		edx,	pStack

	__copy:
		mov     edi,    eax
		mov     esi,    dword ptr [edx + 0] ; pStack->m_p
		mov		ecx,	dword ptr [edx + 4] ; pStack->m_Size
		add		eax,	ecx
		add		edx,	8
		rep     movsb
		
		dec		ebx
		jnz		__copy

	__call:
		mov     eax,    pfn
		call    eax
		
		cmp		CallingConvention, axl_exe_ECallingConvention_Stdcall
		jnz		__ret			
		add     esp,    StackSize
		
	__ret:
		mov		esi,	_esi
		mov		ebx,	_ebx
		pop		edi
		ret		16
		
axl_exe_InvokeDirect@16 endp

;..............................................................................

        end















