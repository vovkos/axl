#include "pch.h"
#include "axl_g_Module.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {

void
initQpc ();

} // namespace sys

namespace g {

//.............................................................................

Module::Module ()
{	
#ifdef _AXL_DEBUG
#	ifdef _AXL_MODULE_TAG
	m_tag = _AXL_MODULE_TAG;
#	else
	m_tag = "UNK_MOD";
#	endif
#endif

#if (_AXL_OS_WIN)	
	m_hModule = ::GetModuleHandle (NULL);

	SYSTEM_INFO systemInfo;
	::GetSystemInfo (&systemInfo);
	m_systemInfo.m_processorCount     = systemInfo.dwNumberOfProcessors;
	m_systemInfo.m_pageSize           = systemInfo.dwPageSize;
	m_systemInfo.m_mappingAlignFactor = systemInfo.dwAllocationGranularity;

	sys::initQpc ();

#elif (_AXL_OS_POSIX)

	m_systemInfo.m_processorCount     = sysconf (_SC_NPROCESSORS_ONLN);
	m_systemInfo.m_pageSize           = sysconf (_SC_PAGE_SIZE);
	m_systemInfo.m_mappingAlignFactor = sysconf (_SC_PAGE_SIZE);
		
#endif	
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

//.............................................................................

} // namespace g
} // namespace axl
