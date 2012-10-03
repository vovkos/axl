		.code	_TEXT

		public	axl_exe_InvokeDirect

;..............................................................................
;
;		intptr_t 
;		axl_exe_GetNextParam
;
;		r8  - pStack
;		r9  - Count
;		r10 - p
;		r11 - Size
;
;..............................................................................

axl_exe_GetNextParam proc
		xor		rax,	rax

	__loop:
		cmp		r11,	8
		jae		__8

		cmp		r11,	4
		jae		__4

		cmp		r11,	2
		jae		__2

		test	r11,	r11
		jnz		__1

		add		r8,		16 
		dec		r9
		jz		__fail

		mov     r10,	qword ptr [r8 + 0] ; pStack->m_p
		mov     r11,	qword ptr [r8 + 8] ; pStack->m_Size
		jmp		__loop
				
	__8:
		mov		rax,	qword ptr [r10]
		add		r10,	8
		sub		r11,	8
		jmp		__ok

	__4:
		mov		eax,	dword ptr [r10]
		add		r10,	4
		sub		r11,	4
		jmp		__ok

	__2:
		mov		ax,		word ptr [r10]
		add		r10,	2
		sub		r11,	2
		jmp		__ok

	__1:
		mov		al,		byte ptr [r10]
		inc		r10
		dec		r11
		
	__ok:		
		clc
		ret

	__fail:		
		stc
		ret
axl_exe_GetNextParam endp

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

		extern SocketTest:proc

axl_exe_InvokeDirect proc
		; initial parameters:
		; rcx - pfn 
		; rdx - CallingConention (ignored)
		; r8  - pStack
		; r9  - Count
		
		; registers through the routine:
		; r8  - pStack
		; r9  - Count
		; r10 - p
		; r11 - Size
				
		; locals
						
		_rbp	equ		qword ptr [rbp + 00h]
		_rsi	equ		qword ptr [rbp + 08h]
		_r8     equ		qword ptr [rbp + 10h]
		_r9     equ     qword ptr [rbp + 18h]
		pfn     equ     qword ptr [rbp + 20h]
		StackSize equ	qword ptr [rbp + 28h]
		Param1	equ		qword ptr [rbp + 30h]
		Param2	equ		qword ptr [rbp + 38h]
		Param3	equ		qword ptr [rbp + 40h]
		Param4	equ		qword ptr [rbp + 48h]

		push	rdi
		sub		rsp,	50h
		mov		[rsp],	rbp
		mov		rbp,	rsp
						
		mov		_rsi,	rsi
		mov		pfn,	rcx

		xor		rax,	rax		
		mov		StackSize, rax
		mov		Param1, rax
		mov		Param2, rax
		mov		Param3, rax
		mov		Param4, rax

		; extract first four parameters
		
		test	r9,		r9
		jz		__call
		mov     r10,	qword ptr [r8 + 0] ; pStack->m_p
		mov     r11,	qword ptr [r8 + 8] ; pStack->m_Size

		call	axl_exe_GetNextParam
		jc		__call
		mov		Param1,	rax

		call	axl_exe_GetNextParam
		jc		__call
		mov		Param2,	rax

		call	axl_exe_GetNextParam
		jc		__call
		mov		Param3,	rax

		call	axl_exe_GetNextParam
		jc		__call
		mov		Param4,	rax

		; count stack size 
	
		xor		eax,	eax

		mov		_r8,	r8
		mov		_r9,	r9

		; count leftover
	
		test	r11,	r11
		jz		__pre_count
		
		mov		rax,	r11     
		add		rax,	7       ; align on 64 bit
		and		rax,	not 7   ; 
		
	__pre_count:
		add		r8,		16
		dec		r9		
		jz		__count_done
 
	__count_loop:
		add		rax,	qword ptr [r8 + 8] ; pStack->m_Size
		add		rax,	7		; align on 64 bit
		and		rax,	not 7	; 
		add		r8,		16
		dec		r9
		jnz		__count_loop

	__count_done:	
		mov		r8,		_r8
		mov		r9,		_r9

		test	rax,	rax
		jz		__call		

		mov		StackSize, rax	

		; reserve space on the stack
		
		sub     rsp,    rax		
		mov		rax,	rsp

		; copy leftover
		
		test	r11,	r11
		jz		__pre_copy
		
		mov		rdi,	rax
		mov		rsi,	r10
		mov		rcx,	r11
		rep		movsb

		add		rax,	rcx
		add		rax,	7       ; align on 64 bit
		and		rax,	not 7   ; 

	__pre_copy:
		add		r8,		16
		dec		r9		
		jz		__call
		
	__copy_loop:
		mov     rdi,    rax
		mov     rsi,    qword ptr [r8 + 0] ; pStack->m_p
		mov		rcx,	qword ptr [r8 + 8] ; pStack->m_Size
		add		rax,	rcx
		add		rax,	7		; align on 64 bit
		and		rax,	not 7	; 
		rep     movsb
		
		add		r8,		16
		dec		r9
		jnz		__copy_loop

	__call:
		mov     rax,    pfn
		
		mov		rcx,	Param1
		mov		rdx,	Param2
		mov		r8,		Param3
		mov		r9, 	Param4
		
		sub		rsp,	32	; shadow space
		call    rax	
		add     rsp,    32	; shadow space
		add     rsp,    StackSize
		
	__ret:
		mov		rsi,	_rsi
		mov		rbp,	_rbp
		add		esp,	50h
		pop		rdi
		ret	
		
axl_exe_InvokeDirect endp

;..............................................................................

        end