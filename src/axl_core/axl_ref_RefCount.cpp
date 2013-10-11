#include "pch.h"
#include "axl_ref_RefCount.h"
#include "axl_ref_Ptr.h"
#include "axl_ref_WeakPtr.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace ref {

//.............................................................................

CRefCount::CRefCount () 
{
	m_RefCount = 0;
	m_WeakRefCount = 1;
	m_pObject = NULL;
	m_pfDestruct = NULL;
	m_pfFree = NULL;
}

void
CRefCount::SetTarget (
	void* pObject,
	mem::FFree pfDestruct,
	mem::FFree pfFree
	)
{
	m_pObject = pObject;
	m_pfDestruct = pfDestruct;
	m_pfFree = pfFree;
}

size_t
CRefCount::Release ()
{
	int32_t RefCount = mt::AtomicDec (&m_RefCount);
	 
	if (!RefCount)
	{
		if (m_pfDestruct)
			m_pfDestruct (m_pObject);

		WeakRelease (); 

		// WeakRelease () should be here and not in ~CRefCount ()
		// otherwise it's possible to free memory prematurely
	}

	return (uint32_t) RefCount;
}

size_t
CRefCount::WeakRelease ()
{
	int32_t RefCount = mt::AtomicDec (&m_WeakRefCount);

	if (!RefCount && m_pfFree != NULL && (size_t) m_pfFree < -10)
		m_pfFree (m_pObject);

	return (uint32_t) RefCount;
}

size_t
CRefCount::AddRefByWeakPtr ()
{ 
	for (;;)
	{
		int32_t Old = m_RefCount; 
		if (Old == 0)
			return 0;

		if (mt::AtomicCmpXchg (&m_RefCount, Old, Old + 1) == Old)
			return (uint32_t) (Old + 1);
	}
}

//.............................................................................

} // namespace axl 
} // namespace ref 
