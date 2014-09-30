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
	m_refCount = 0;
	m_weakRefCount = 1;
	m_object = NULL;
	m_pfDestruct = NULL;
	m_pfFree = NULL;
}

void
CRefCount::setTarget (
	void* object,
	mem::FFree* pfDestruct,
	mem::FFree* pfFree
	)
{
	m_object = object;
	m_pfDestruct = pfDestruct;
	m_pfFree = pfFree;
}

size_t
CRefCount::release ()
{
	int32_t refCount = mt::atomicDec (&m_refCount);
	 
	if (!refCount)
	{
		if (m_pfDestruct)
			m_pfDestruct (m_object);

		weakRelease (); 

		// WeakRelease () should be here and not in ~CRefCount ()
		// otherwise it's possible to free memory prematurely
	}

	return (uint32_t) refCount;
}

size_t
CRefCount::weakRelease ()
{
	int32_t refCount = mt::atomicDec (&m_weakRefCount);

	if (!refCount && m_pfFree != NULL && (size_t) m_pfFree < -10)
		m_pfFree (m_object);

	return (uint32_t) refCount;
}

size_t
CRefCount::addRefByWeakPtr ()
{ 
	for (;;)
	{
		int32_t old = m_refCount; 
		if (old == 0)
			return 0;

		if (mt::atomicCmpXchg (&m_refCount, old, old + 1) == old)
			return (uint32_t) (old + 1);
	}
}

//.............................................................................

} // namespace axl 
} // namespace ref 
