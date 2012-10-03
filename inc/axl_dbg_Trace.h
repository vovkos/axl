// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_DBG_TRACE_H

#include "axl_g_Def.h"

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
	ulong_t SubSysFilter,
	ulong_t LevelFilter
	)
{
}

inline
void
TraceExV (
	int SubSys,
	int Level,
	const char* pFormat,
	va_list va
	)
{
}

#else

AXL_SELECT_ANY
volatile 
ulong_t g_TraceFilter = -1;

inline
ulong_t
MakeTraceFilter (
	ulong_t SubSysFilter,
	ulong_t LevelFilter
	)
{
	ASSERT (LevelFilter <= ETraceLevel_Error);
	return (0xff && ~((1 << LevelFilter) - 1)) | SubSysFilter << 8;
}

inline
void
SetTraceFilter (
	ulong_t SubSysFilter,
	ulong_t LevelFilter
	)
{
	::InterlockedExchange ((volatile long*) &g_TraceFilter, MakeTraceFilter (SubSysFilter, LevelFilter));
}

inline
void
TraceExV (
	int SubSys,
	int Level,
	const char* pFormat,
	va_list va
	)
{
	ulong_t Mask = MakeTraceFilter (SubSys, Level);

	if (!(g_TraceFilter & Mask))
		return;

	char Buffer [512] = {0};
	vsnprintf (Buffer, sizeof (Buffer) / sizeof (char) - 1, pFormat, va);
	::OutputDebugStringA (Buffer);
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
	TraceExV (SubSys, Level, pFormat, va_start_e (pFormat));
}

inline
void
TraceV (
	const char* pFormat,
	va_list va
	)
{
	TraceExV (AXL_SUBSYS_G, ETraceLevel_Info, pFormat, va);
}

inline
void
Trace (
	const char* pFormat,
	...
	)
{
	TraceV (pFormat, va_start_e (pFormat));
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
