#include "pch.h"
#include "axl_g_Module.h"
#include "axl_rtl_String.h"

namespace axl {
namespace g {

//.............................................................................

CModule::CModule ()
{	
#ifdef _DEBUG
#	ifdef _AXL_MODULE_TAG
	m_tag = _AXL_MODULE_TAG;
#	else
	m_tag = "UNK_MOD";
#	endif
#endif

#if (_AXL_ENV == AXL_ENV_WIN)	
	m_hModule = ::GetModuleHandle (NULL);

	SYSTEM_INFO systemInfo;
	::GetSystemInfo (&systemInfo);
	m_systemInfo.m_processorCount     = systemInfo.dwNumberOfProcessors;
	m_systemInfo.m_pageSize           = systemInfo.dwPageSize;
	m_systemInfo.m_mappingAlignFactor = systemInfo.dwAllocationGranularity;

#elif (_AXL_ENV == AXL_ENV_POSIX)

	m_systemInfo.m_processorCount     = sysconf (_SC_NPROCESSORS_ONLN);
	m_systemInfo.m_pageSize           = sysconf (_SC_PAGE_SIZE);
	m_systemInfo.m_mappingAlignFactor = sysconf (_SC_PAGE_SIZE);
		
#endif	
}

CModule::~CModule ()
{
	while (!m_finalizerList.isEmpty ())
	{
		CFinalizerEntry* finalizerEntry = m_finalizerList.removeTail ();
		finalizerEntry->m_finalizer->finalize ();
		AXL_MEM_DELETE (finalizerEntry);
	}

	m_memTracker.trace ();
}

bool 
CModule::addFinalizer (const ref::CPtrT <CFinalizer>& finalizer)
{
	CFinalizerEntry* finalizerEntry = AXL_MEM_NEW (CFinalizerEntry);
	finalizerEntry->m_finalizer = finalizer;

	m_finalizerListLock.lock ();
	m_finalizerList.insertTail (finalizerEntry);
	m_finalizerListLock.unlock ();
	return true;
}

//.............................................................................

} // namespace g
} // namespace axl
