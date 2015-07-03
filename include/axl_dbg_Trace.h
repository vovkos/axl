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

#ifndef _AXL_DEBUG

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

inline
void
traceEx (
	uint_t level,
	const char* formatString,
	...
	)
{
}

inline
void
trace_va (
	const char* formatString,
	axl_va_list va
	)
{
}

inline
void
trace (
	const char* formatString,
	...
	)
{
}

#else

AXL_SELECT_ANY volatile uint_t g_traceFilter = -1;

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

void
traceEx_va (
	uint_t level,
	const char* formatString,
	axl_va_list va
	);

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

#endif

//.............................................................................

#ifndef TRACE
#ifdef _AXL_DEBUG

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
