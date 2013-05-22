// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_DBG_TRACE_H

#include "axl_g_Def.h"
#include "axl_mt_Atomic.h"

namespace axl {
namespace dbg {

//.............................................................................

enum ETraceLevel
{
	ETraceLevel_MinorInfo = 0,
	ETraceLevel_Info,
	ETraceLevel_Warning,
	ETraceLevel_Error,
};

//.............................................................................

#ifndef _DEBUG

inline
void
SetTraceFilter (
	uint_t SubSysFilter,
	uint_t LevelFilter
	)
{
}

inline
void
TraceEx_va (
	int SubSys,
	int Level,
	const char* pFormat,
	axl_va_list va
	)
{
}

#else

AXL_SELECT_ANY
volatile uint_t g_TraceFilter = -1;

inline
uint_t
MakeTraceFilter (
	uint_t SubSysFilter,
	uint_t LevelFilter
	)
{
	ASSERT (LevelFilter <= ETraceLevel_Error);
	return (0xff && ~((1 << LevelFilter) - 1)) | SubSysFilter << 8;
}

inline
void
SetTraceFilter (
	uint_t SubSysFilter,
	uint_t LevelFilter
	)
{
	mt::AtomicXchg ((volatile int32_t*) &g_TraceFilter, MakeTraceFilter (SubSysFilter, LevelFilter));
}

inline
void
TraceEx_va (
	int SubSys,
	int Level,
	const char* pFormat,
	axl_va_list va
	)
{
	uint_t Mask = MakeTraceFilter (SubSys, Level);
	if (!(g_TraceFilter & Mask))
		return;

#if (_AXL_ENV == AXL_ENV_WIN)
	char Buffer [512] = { 0 };
	vsnprintf (Buffer, sizeof (Buffer) / sizeof (char) - 1, pFormat, va.m_va);
	OutputDebugStringA (Buffer);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	vprintf (pFormat, va.m_va);
#endif
}

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
TraceEx (
	int SubSys,
	int Level,
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	TraceEx_va (SubSys, Level, pFormat, va);
}

inline
void
Trace_va (
	const char* pFormat,
	axl_va_list va
	)
{
	TraceEx_va (AXL_SUBSYS_G, ETraceLevel_Info, pFormat, va);
}

inline
void
Trace (
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	Trace_va (pFormat, va);
}

//.............................................................................

#ifndef TRACE
#ifdef _DEBUG

#define TRACE \
	dbg::Trace

#else

#define TRACE \
	(void)

#endif
#endif

//.............................................................................

} // namespace dbg
} // namespace axl
