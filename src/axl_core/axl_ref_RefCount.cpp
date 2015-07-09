#include "pch.h"
#include "axl_ref_RefCount.h"
#include "axl_ref_Ptr.h"
#include "axl_ref_WeakPtr.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace ref {

//.............................................................................

RefCount::RefCount () 
{
	m_refCount = 0;
	m_weakRefCount = 1;
	m_object = NULL;
	m_destructFunc = NULL;
	m_freeFunc = NULL;
}

void
RefCount::setTarget (
	void* object,
	FreeFunc* destructFunc,
	FreeFunc* freeFunc
	)
{
	m_object = object;
	m_destructFunc = destructFunc;
	m_freeFunc = freeFunc;
}

size_t
RefCount::release ()
{
	int32_t refCount = mt::atomicDec (&m_refCount);
	 
	if (!refCount)
	{
		if (m_destructFunc)
			m_destructFunc (m_object);

		weakRelease (); 

		// WeakRelease () should be here and not in ~CRefCount ()
		// otherwise it's possible to free memory prematurely
	}

	return (uint32_t) refCount;
}

size_t
RefCount::weakRelease ()
{
	int32_t refCount = mt::atomicDec (&m_weakRefCount);

	if (!refCount && m_freeFunc != NULL && (size_t) m_freeFunc < -10)
		m_freeFunc (m_object);

	return (uint32_t) refCount;
}

size_t
RefCount::addRefByWeakPtr ()
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
