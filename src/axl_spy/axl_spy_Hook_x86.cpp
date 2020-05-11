﻿#include "pch.h"
#include "axl_spy_Hook.h"
#include "axl_spy_HookCommon.h"
#include "axl_mem_ExecutableBlockArena.h"

namespace axl {
namespace spy {

//..............................................................................

// nasm -fwin32 -lthunk_x86.asm.lst thunk_x86.asm
// perl nasm-list-to-cpp.pl thunk_x86.asm.lst

const uint8_t g_thunkCode[] =
{
	0x55,                                      // 00000000  push    ebp
	0x89, 0xE5,                                // 00000001  mov     ebp, esp
	0x83, 0xEC, 0x08,                          // 00000003  sub     esp, STACK_FRAME_SIZE
	0x83, 0xEC, 0x10,                          // 00000006  sub     esp, 16
	0xC7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00,  // 00000009  mov     dword [esp + 0], hook
	0x89, 0x6C, 0x24, 0x04,                    // 00000010  mov     [esp + 4], ebp
	0x8B, 0x45, 0x04,                          // 00000014  mov     eax, [ebp + 4]
	0x89, 0x44, 0x24, 0x08,                    // 00000017  mov     [esp + 8], eax
	0xB8, 0x00, 0x00, 0x00, 0x00,              // 0000001B  mov     eax, hookEnter
	0xFF, 0xD0,                                // 00000020  call    eax
	0x83, 0xC4, 0x10,                          // 00000022  add     esp, 16
	0x83, 0xC4, 0x08,                          // 00000025  add     esp, STACK_FRAME_SIZE
	0x5D,                                      // 00000028  pop     ebp
	0xB8, 0x00, 0x00, 0x00, 0x00,              // 00000029  mov     eax, hookRet
	0x89, 0x04, 0x24,                          // 0000002E  mov     [esp], eax
	0xB8, 0x00, 0x00, 0x00, 0x00,              // 00000031  mov     eax, targetFunc
	0xFF, 0xE0,                                // 00000036  jmp     eax
	0x83, 0xEC, 0x04,                          // 00000038  sub     esp, 4  ; <<< hook_ret
	0x55,                                      // 0000003B  push    ebp
	0x89, 0xE5,                                // 0000003C  mov     ebp, esp
	0x83, 0xEC, 0x08,                          // 0000003E  sub     esp, STACK_FRAME_SIZE
	0x89, 0x45, 0xFC,                          // 00000041  mov     [ebp - 4], eax
	0x83, 0xEC, 0x10,                          // 00000044  sub     esp, 16
	0xC7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00,  // 00000047  mov     dword [esp + 0], hook
	0x89, 0x6C, 0x24, 0x04,                    // 0000004E  mov     [esp + 4], ebp
	0x89, 0x44, 0x24, 0x08,                    // 00000052  mov     [esp + 8], eax
	0xB8, 0x00, 0x00, 0x00, 0x00,              // 00000056  mov     eax, hookLeave
	0xFF, 0xD0,                                // 0000005B  call    eax
	0x83, 0xC4, 0x10,                          // 0000005D  add     esp, 16
	0x89, 0x45, 0x04,                          // 00000060  mov     [ebp + 4], eax
	0x8B, 0x45, 0xFC,                          // 00000063  mov     eax, [ebp - 4]
	0x83, 0xC4, 0x08,                          // 00000066  add     esp, STACK_FRAME_SIZE
	0x5D,                                      // 00000069  pop     ebp
	0xC3,                                      // 0000006A  ret
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ThunkCodeOffset
{
	ThunkCodeOffset_HookPtr1      = 0x0c,
	ThunkCodeOffset_HookEnterPtr  = 0x1c,
	ThunkCodeOffset_HookRetPtr    = 0x2a,
	ThunkCodeOffset_TargetFuncPtr = 0x32,
	ThunkCodeOffset_HookRet       = 0x38,
	ThunkCodeOffset_HookPtr2      = 0x4a,
	ThunkCodeOffset_HookLeavePtr  = 0x57,
	ThunkCodeOffset_End           = sizeof(g_thunkCode),
};

//..............................................................................

struct Hook
{
	uint8_t m_thunkCode[(ThunkCodeOffset_End & ~7) + 8]; // align on 8
	HookCommonContext m_context;
};

//..............................................................................

HookAction
hookEnter(
	Hook* hook,
	uint32_t ebp,
	uint32_t originalRet
	)
{
	return hookEnterCommon(&hook->m_context, ebp, originalRet);
}

uint32_t
hookLeave(
	Hook* hook,
	uint32_t ebp,
	uint32_t eax
	)
{
	return hookLeaveCommon(&hook->m_context, ebp);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

HookArena::HookArena()
{
	m_impl = AXL_MEM_NEW(mem::ExecutableBlockArena<Hook>);
}

HookArena::~HookArena()
{
	((mem::ExecutableBlockArena<Hook>*)m_impl)->detach(); // don't free unless explicitly requested
	AXL_MEM_DELETE((mem::ExecutableBlockArena<Hook>*)m_impl);
}

Hook*
HookArena::allocate(
	void* targetFunc,
	void* callbackParam,
	HookEnterFunc* enterFunc,
	HookLeaveFunc* leaveFunc
)
{
	Hook* hook = ((mem::ExecutableBlockArena<Hook>*)m_impl)->allocate();
	if (!hook)
		return NULL;

	memcpy(hook->m_thunkCode, g_thunkCode, sizeof(g_thunkCode));
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_HookPtr1) = hook;
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_HookPtr2) = hook;
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_TargetFuncPtr) = targetFunc;
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_HookRetPtr) = hook->m_thunkCode + ThunkCodeOffset_HookRet;
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_HookEnterPtr) = (void*)hookEnter;
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_HookLeavePtr) = (void*)hookLeave;

	hook->m_context.m_targetFunc = targetFunc;
	hook->m_context.m_callbackParam = callbackParam;
	hook->m_context.m_enterFunc = enterFunc;
	hook->m_context.m_leaveFunc = leaveFunc;
	return hook;
}

void
HookArena::free()
{
	((mem::ExecutableBlockArena<Hook>*)m_impl)->free();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
setHookTargetFunc(
	Hook* hook,
	void* targetFunc
)
{
	*(void**)(hook->m_thunkCode + ThunkCodeOffset_TargetFuncPtr) = targetFunc;
}

//..............................................................................

} // namespace spy
} // namespace axl
