#include "pch.h"
#include "axl_dbg_Trace.h"
#include "axl_sl_String.h"

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

#if (_AXL_OS_WIN)
	char buffer [512] = { 0 };
	sl::String string (ref::BufKind_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va);
	::OutputDebugStringA (string);
#elif (_AXL_OS_POSIX)
	vfprintf (g_traceFile, formatString, va);
	fflush (g_traceFile);
#endif
}

#endif

//.............................................................................

} // namespace dbg
} // namespace axl
