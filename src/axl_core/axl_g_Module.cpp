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
	m_pTag = _AXL_MODULE_TAG;
#	else
	m_pTag = "UNK_MOD";
#	endif
#endif

#if (_AXL_ENV == AXL_ENV_WIN)	
	m_hModule = ::GetModuleHandle (NULL);

	SYSTEM_INFO SystemInfo;
	::GetSystemInfo (&SystemInfo);
	m_SystemInfo.m_ProcessorCount     = SystemInfo.dwNumberOfProcessors;
	m_SystemInfo.m_PageSize           = SystemInfo.dwPageSize;
	m_SystemInfo.m_MappingAlignFactor = SystemInfo.dwAllocationGranularity;

#elif (_AXL_ENV == AXL_ENV_POSIX)

	m_SystemInfo.m_ProcessorCount     = sysconf (_SC_NPROCESSORS_ONLN);
	m_SystemInfo.m_PageSize           = sysconf (_SC_PAGE_SIZE);
	m_SystemInfo.m_MappingAlignFactor = sysconf (_SC_PAGE_SIZE);
		
#endif	
}

CModule::~CModule ()
{
	while (!m_FinalizerList.IsEmpty ())
	{
		CFinalizerEntry* pFinalizerEntry = m_FinalizerList.RemoveTail ();
		pFinalizerEntry->m_Finalizer->Finalize ();
		AXL_MEM_DELETE (pFinalizerEntry);
	}

	m_MemTracker.Trace ();
}

bool 
CModule::AddFinalizer (const ref::CPtrT <IFinalizer>& Finalizer)
{
	CFinalizerEntry* pFinalizerEntry = AXL_MEM_NEW (CFinalizerEntry);
	pFinalizerEntry->m_Finalizer = Finalizer;

	m_FinalizerListLock.Lock ();
	m_FinalizerList.InsertTail (pFinalizerEntry);
	m_FinalizerListLock.Unlock ();
	return true;
}

//.............................................................................

} // namespace g
} // namespace axl
