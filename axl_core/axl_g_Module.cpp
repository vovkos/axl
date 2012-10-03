#include "stdafx.h"
#include "axl_g_Module.h"
#include "axl_obj_Ptr.h"
#include "axl_ref_Clone.h"

#include "axl_rtl_Singleton.h"

namespace axl {
namespace g {

//.............................................................................

CModule::CModule ()
{
#ifdef _DEBUG
#ifdef _AXL_MODULE_TAG
	m_pTag = _AXL_MODULE_TAG;
#else
	m_pTag = "UNK_MOD";
#endif
#endif

	m_hModule = ::GetModuleHandle (NULL);

	SYSTEM_INFO SystemInfo;
	::GetSystemInfo (&SystemInfo);

	m_SystemInfo.m_PageSize              = SystemInfo.dwPageSize;
	m_SystemInfo.m_AllocationGranularity = SystemInfo.dwAllocationGranularity;
	m_SystemInfo.m_ProcessorCount        = SystemInfo.dwNumberOfProcessors;
	m_SystemInfo.m_ProcessorArchitecture = SystemInfo.wProcessorArchitecture;
	m_SystemInfo.m_ProcessorLevel        = SystemInfo.wProcessorLevel;
	m_SystemInfo.m_ProcessorRevision     = SystemInfo.wProcessorRevision;
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
CModule::AddFinalizer (IFinalizer* pFinalizer)
{
	CFinalizerEntry* pFinalizerEntry = AXL_MEM_NEW (CFinalizerEntry);
	pFinalizerEntry->m_Finalizer = ref::GetPtrOrClone (pFinalizer);

	m_FinalizerListLock.Lock ();
	m_FinalizerList.InsertTail (pFinalizerEntry);
	m_FinalizerListLock.Unlock ();
	return true;
}

//.............................................................................

} // namespace g
} // namespace axl
