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

#include "pch.h"
#include "axl_g_Module.h"
#include "axl_sl_String.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_NtDll.h"
#endif

namespace axl {
namespace sys {

void
initPreciseTimestamps ();

} // namespace sys

namespace g {

//.............................................................................

// ensure module creation at startup

class ModuleCreator
{
public:
	ModuleCreator ()
	{
		getModule ();
	}
};

static ModuleCreator g_moduleCreator;

//..............................................................................

Module::Module ()
{
#ifdef _AXL_DEBUG
	m_tag = "<untagged-module>";
#endif

#if (_AXL_OS_WIN)
	m_hModule = ::GetModuleHandle (NULL);

	SYSTEM_INFO systemInfo;
	::GetSystemInfo (&systemInfo);
	m_systemInfo.m_processorCount     = systemInfo.dwNumberOfProcessors;
	m_systemInfo.m_pageSize           = systemInfo.dwPageSize;
	m_systemInfo.m_mappingAlignFactor = systemInfo.dwAllocationGranularity;

	sys::win::initNtDllFunctions ();

#elif (_AXL_OS_POSIX)
	m_systemInfo.m_processorCount     = sysconf (_SC_NPROCESSORS_ONLN);
	m_systemInfo.m_pageSize           = sysconf (_SC_PAGE_SIZE);
	m_systemInfo.m_mappingAlignFactor = sysconf (_SC_PAGE_SIZE);
#endif

	sys::initPreciseTimestamps ();
}

Module::~Module ()
{
	while (!m_finalizerList.isEmpty ())
	{
		FinalizerEntry* finalizerEntry = m_finalizerList.removeTail ();
		finalizerEntry->m_finalizer->finalize ();
		AXL_MEM_DELETE (finalizerEntry);
	}

	m_memTracker.trace ();
}

bool
Module::addFinalizer (const ref::Ptr <Finalizer>& finalizer)
{
	FinalizerEntry* finalizerEntry = AXL_MEM_NEW (FinalizerEntry);
	finalizerEntry->m_finalizer = finalizer;

	m_finalizerListLock.lock ();
	m_finalizerList.insertTail (finalizerEntry);
	m_finalizerListLock.unlock ();
	return true;
}

//..............................................................................

} // namespace g
} // namespace axl

#if (_AXL_OS_WIN)
void
axl_trace_va (
	const char* formatString,
	axl_va_list va
	)
{
	char buffer [512] = { 0 };
	axl::sl::String string (axl::ref::BufKind_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va);
	::OutputDebugStringA (string);
}
#endif

//..............................................................................
