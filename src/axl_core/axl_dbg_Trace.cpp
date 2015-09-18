#include "pch.h"
#include "axl_dbg_Trace.h"
#include "axl_rtl_String.h"

namespace axl {
namespace dbg {

//.............................................................................

#ifdef _AXL_DEBUG

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
	rtl::String string (ref::BufKind_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va.m_va);
	::OutputDebugStringA (string);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	vfprintf (g_traceFile, formatString, va.m_va);
	fflush (g_traceFile);
#endif
}

#endif

//.............................................................................

} // namespace dbg
} // namespace axl
