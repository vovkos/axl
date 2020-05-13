//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#include "axl_g_Pch.h"

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_CPU_AMD64)
#	if (_AXL_CPP_MSC)

struct RegArgBlock
{
	uint64_t m_rcx;
	uint64_t m_rdx;
	uint64_t m_r8;
	uint64_t m_r9;
	double m_xmm0[2];
	double m_xmm1[2];
	double m_xmm2[2];
	double m_xmm3[2];
};

struct RegRetBlock
{
	uint64_t m_rax;
};

#	elif (_AXL_CPP_GCC)

struct RegRetBlock
{
	uint64_t m_rax;
	uint64_t m_rdx;
	double m_xmm0[2];
	double m_xmm1[2];
};

struct RegArgBlock
{
	uint64_t m_rdi;
	uint64_t m_rsi;
	uint64_t m_rdx;
	uint64_t m_rcx;
	uint64_t m_r8;
	uint64_t m_r9;
	double m_xmm0[2];
	double m_xmm1[2];
	double m_xmm2[2];
	double m_xmm3[2];
	double m_xmm4[2];
	double m_xmm5[2];
	double m_xmm6[2];
	double m_xmm7[2];
};

#	endif
#elif (_AXL_CPU_X86)

struct RegRetBlock
{
	uint32_t m_rax;
};

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum FrameOffset
{
#if (_AXL_CPU_X86)
	FrameOffset_StackArgBlock = 8,
	FrameOffset_RegRetBlock   = -(int)sizeof(RegRetBlock),
#elif (_AXL_CPU_AMD64)
#	if (_AXL_CPP_MSC)
	FrameOffset_RegArgBlock   = -(int)sizeof(RegArgBlock),
	FrameOffset_RegRetBlock   = -(int)(sizeof(RegArgBlock) + sizeof(RegRetBlock) + 8),
	FrameOffset_StackArgBlock = 16 + 8 * 4,
#	elif (_AXL_CPP_GCC)
	FrameOffset_RegArgBlock   = -(int)sizeof(RegArgBlock),
	FrameOffset_RegRetBlock   = -(int)(sizeof(RegArgBlock) + sizeof(RegRetBlock)),
	FrameOffset_StackArgBlock = 16,
#	endif
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct VaList
{
	char* m_p;
};

inline
void
vaStart(
	VaList& va,
	size_t frameBase
	)
{
	va.m_p = (char*)(frameBase + FrameOffset_StackArgBlock);
}

template<typename T>
T&
vaArg(VaList& va)
{
	T* p = (T*)va.m_p;
	va.m_p += (sizeof(T) + sizeof(intptr_t) - 1) & ~(sizeof(intptr_t) - 1);
	return *p;
}

inline
void
vaEnd(VaList& va)
{
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum HookAction
{
	HookAction_Default    = 0,
	HookAction_Return     = 0x01,
	HookAction_JumpTarget = 0x02,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef
HookAction
HookEnterFunc(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	);

typedef
void
HookLeaveFunc(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	);

#if (_AXL_CPP_MSC && _AXL_CPU_AMD64)

typedef
void
HookExceptionFunc(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase,
	EXCEPTION_RECORD* exception,
	CONTEXT* context
	);

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Hook; // ABI-dependent

class HookArena
{
protected:
	void* m_impl;

public:
	HookArena();
	~HookArena(); // does NOT free allocated executable pages

	Hook*
	allocate(
		void* targetFunc,
		void* callbackParam,
		HookEnterFunc* enterFunc,
		HookLeaveFunc* leaveFunc
		);

	void
	free(); // CAUTION: normally, you DO NOT WANT to ever unhook and free thunks
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// for trampoline-based hooking, we need to adjust targetFunc AFTER injection

void
setHookTargetFunc(
	Hook* hook,
	void* targetFunc
	);

#if (_AXL_CPP_MSC && _AXL_CPU_AMD64)

void
setHookExceptionFunc(
	Hook* hook,
	HookExceptionFunc* exceptionFunc
	);

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
enableHooks();

void
disableHooks();

void
enableCurrentThreadHooks();

void
disableCurrentThreadHooks();

//..............................................................................

} // namespace spy
} // namespace axl
