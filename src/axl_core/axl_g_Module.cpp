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

#if (_AXL_DEBUG)
#	define _AXL_MEM_TRACKER _AXL_MEM_TRACKER_DEBUG
#else
#	define _AXL_MEM_TRACKER _AXL_MEM_TRACKER_RELEASE
#endif

#if (_AXL_MEM_TRACKER)
#	include "axl_mem_Tracker.h"
#endif

namespace axl {
namespace sys {

void
initPreciseTimestamps();

} // namespace sys

namespace g {

//.............................................................................

// ensure early initialization of the module during startup

static struct ModuleInit {
	ModuleInit() {
		getModule();
	}
} g_moduleInit;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Module::Module() {
#if (_AXL_OS_WIN)
	m_hModule = ::GetModuleHandle(NULL);

	SYSTEM_INFO systemInfo;
	::GetSystemInfo(&systemInfo);
	m_systemInfo.m_processorCount     = systemInfo.dwNumberOfProcessors;
	m_systemInfo.m_pageSize           = systemInfo.dwPageSize;
	m_systemInfo.m_mappingAlignFactor = systemInfo.dwAllocationGranularity;

	sys::win::initNtDllFunctions();

#elif (_AXL_OS_POSIX)
	m_systemInfo.m_processorCount     = sysconf(_SC_NPROCESSORS_ONLN);
	m_systemInfo.m_pageSize           = sysconf(_SC_PAGE_SIZE);
	m_systemInfo.m_mappingAlignFactor = sysconf(_SC_PAGE_SIZE);
#endif

	sys::initPreciseTimestamps();

#if (_AXL_MEM_TRACKER)
	if (mem::g_trackerDispatchFunc)
		mem::g_trackerDispatchFunc(mem::Tracker::DispatchCode_SetWatermarkSeqNum);
#endif
}

Module::~Module() {
	while (!m_finalizerList.isEmpty()) {
		FinalizerEntry* finalizerEntry = m_finalizerList.removeTail();
		finalizerEntry->m_finalizer->finalize();
		AXL_MEM_DELETE(finalizerEntry);
	}

#if (_AXL_MEM_TRACKER)
	if (mem::g_trackerDispatchFunc)
		mem::g_trackerDispatchFunc(mem::Tracker::DispatchCode_Trace);
#endif
}

bool
Module::addFinalizer(const rc::Ptr<Finalizer>& finalizer) {
	FinalizerEntry* finalizerEntry = AXL_MEM_NEW(FinalizerEntry);
	finalizerEntry->m_finalizer = finalizer;

	m_finalizerListLock.lock();
	m_finalizerList.insertTail(finalizerEntry);
	m_finalizerListLock.unlock();
	return true;
}

//..............................................................................

} // namespace g
} // namespace axl

#if (_AXL_OS_WIN && _AXL_DEBUG)
void
axl_trace_va(
	const char* formatString,
	axl_va_list va
) {
	char buffer[512] = { 0 };
	axl::sl::String string(axl::rc::BufKind_Stack, buffer, sizeof(buffer));
	size_t length = string.format_va(formatString, va);

	// send it both to the debugger and to the trace file

	::OutputDebugStringA(string);
	fwrite(string, length, 1, axl_g_traceFile);
	fflush(axl_g_traceFile);
}
#endif

//..............................................................................
