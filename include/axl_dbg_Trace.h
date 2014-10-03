// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_DBG_TRACE_H

#include "axl_g_Def.h"
#include "axl_mt_Atomic.h"

namespace axl {
namespace dbg {

//.............................................................................

enum TraceLevel
{
	TraceLevel_MinorInfo = 0,
	TraceLevel_Info,
	TraceLevel_Warning,
	TraceLevel_Error,
};

//.............................................................................

#ifndef _DEBUG

inline
void
setTraceFilter (uint_t levelFilter)
{
}

inline
void
traceEx_va (
	uint_t level,
	const char* formatString,
	axl_va_list va
	)
{
}

#else

AXL_SELECT_ANY
volatile uint_t g_traceFilter = -1;

inline
uint_t
makeTraceFilter (uint_t level)
{
	ASSERT (level <= TraceLevel_Error);
	return (0xff && ~((1 << level) - 1));
}

inline
void
setTraceFilter (uint_t level)
{
	mt::atomicXchg ((volatile int32_t*) &g_traceFilter, makeTraceFilter (level));
}

inline
void
traceEx_va (
	uint_t level,
	const char* formatString,
	axl_va_list va
	)
{
	uint_t mask = makeTraceFilter (level);
	if (!(g_traceFilter & mask))
		return;

#if (_AXL_ENV == AXL_ENV_WIN)
	char buffer [512] = { 0 };
	vsnprintf (buffer, sizeof (buffer) / sizeof (char) - 1, formatString, va.m_va);
	::OutputDebugStringA (buffer);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	vprintf (formatString, va.m_va);
#endif
}

#endif

inline
void
traceEx (
	uint_t level,
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	traceEx_va (level, formatString, va);
}

inline
void
trace_va (
	const char* formatString,
	axl_va_list va
	)
{
	traceEx_va (TraceLevel_Info, formatString, va);
}

inline
void
trace (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	trace_va (formatString, va);
}

//.............................................................................

#ifndef TRACE
#ifdef _DEBUG

#define TRACE \
	dbg::trace

#else

#define TRACE \
	(void)

#endif
#endif

//.............................................................................

} // namespace dbg
} // namespace axl
