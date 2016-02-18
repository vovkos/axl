#include "pch.h"
#include "axl_ref_RefCount.h"
#include "axl_ref_New.h"

namespace axl {
namespace ref {

//.............................................................................

RefCount::RefCount () 
{
	m_refCount = 0;
	m_weakRefCount = 1;
	m_parentOffset = 0;
	m_flags = 0;
}

void
RefCount::prime (
	RefCount* parent,
	uint_t flags
	)
{
	ASSERT (!parent || parent < this);
	ASSERT (m_refCount == 0); // should only be called once in the very beginning

	m_parentOffset = parent ? (char*) this - (char*) parent : 0;
	m_flags = flags;
}

size_t
RefCount::release ()
{
	intptr_t refCount = sys::atomicDec (&m_refCount);
	 
	if (!refCount)
	{
		this->~RefCount ();
		weakRelease (); // weakRelease () should be here, not in ~RefCount ()
	}

	return refCount;
}

size_t
RefCount::weakRelease ()
{
	intptr_t refCount = sys::atomicDec (&m_weakRefCount);

	if (!refCount)
		if (m_flags & RefCountFlag_Allocated)
		{
			RefCountAllocHdr* hdr = (RefCountAllocHdr*) this - 1;
			hdr->m_freeFunc (hdr);
		}
		else if (m_parentOffset)
		{
			RefCount* parent = (RefCount*) ((char*) this - m_parentOffset);
			parent->weakRelease ();
		}

	return refCount;
}

size_t
RefCount::addRefByWeakPtr ()
{ 
	for (;;)
	{
		intptr_t old = m_refCount; 
		if (old == 0)
			return 0;

		if (sys::atomicCmpXchg (&m_refCount, old, old + 1) == old)
			return old + 1;
	}
}

//.............................................................................

} // namespace axl 
} // namespace ref 
