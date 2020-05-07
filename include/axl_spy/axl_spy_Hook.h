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

#	elif (_AXL_CPP_GCC)

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
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum FrameOffset
{
#if (_AXL_CPU_X86)
	FrameOffset_StackArgBlock = 8,
#elif (_AXL_CPU_AMD64)
#	if (_AXL_CPP_MSC)
	FrameOffset_RegArgBlock   = -(int)sizeof(RegArgBlock),
	FrameOffset_StackArgBlock = 16 + 8 * 4,
#	elif (_AXL_CPP_GCC)
	FrameOffset_RegArgBlock   = -(int)sizeof(RegArgBlock),
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

typedef
void
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
	size_t frameBase,
	size_t returnValue
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

struct Hook;

Hook*
allocateHook(
	void* targetFunc,
	void* callbackParam,
	HookEnterFunc* enterFunc,
	HookLeaveFunc* leaveFunc
	);

void
freeHook(Hook* hook);

#if (_AXL_CPP_MSC && _AXL_CPU_AMD64)

void
setHookExceptionFunc(
	Hook* hook,
	HookExceptionFunc* exceptionFunc
	);

#endif

//..............................................................................

} // namespace spy
} // namespace axl
