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

#pragma once

#define _AXL_REF_REFCOUNT_H

#include "axl_sys_Atomic.h"

namespace axl {
namespace ref {

// RefCount decouples destruction and freeing -- this is done to allow overriding allocation at run-time.

// RefCount features:
// -weak references and strong references
// -using different allocators for creating ref-counted object
// -creating ref-counted objects on member fields buffers (be sure to make container non-copiable)
// -creating ref-counted objects on static/stack buffers

//..............................................................................

typedef
void
FreeFunc(void* p);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct RefCountAllocHdr
{
	union
	{
		//! \unnamed{union}
		FreeFunc* m_freeFunc;
		uint64_t m_padding; // ensure 8-byte alignment
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class RefCountAllocBuffer:
	public RefCountAllocHdr,
	public T
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RefCountFlag
{
	RefCountFlag_Allocated = 0x01, // must be freed, free func is in hdr

	// ok to define and use your own flags 0x08..0x80
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class RefCount
{
	AXL_DISABLE_COPY(RefCount)

protected:
	volatile int32_t m_refCount;
	volatile int32_t m_weakRefCount;
	uint_t m_parentOffset;
	uint_t m_flags;

public:
	RefCount();

	virtual
	~RefCount()
	{
		ASSERT(m_refCount == 0 && m_weakRefCount > 0);
	}

	void
	prime(
		RefCount* parent,
		uint_t flags = 0
		);

	size_t
	getRefCount()
	{
		return m_refCount;
	}

	size_t
	getWeakRefCount()
	{
		return m_weakRefCount;
	}

	uint_t
	getFlags()
	{
		return m_flags;
	}

	size_t
	addRef()
	{
		return sys::atomicInc(&m_refCount);
	}

	size_t
	release();

	size_t
	addWeakRef()
	{
		return sys::atomicInc(&m_weakRefCount);
	}

	size_t
	weakRelease();

	size_t
	addRefByWeakPtr();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
RefCount::RefCount()
{
	m_refCount = 0;
	m_weakRefCount = 1;
	m_parentOffset = 0;
	m_flags = 0;
}

inline
void
RefCount::prime(
	RefCount* parent,
	uint_t flags
	)
{
	ASSERT(m_refCount == 0); // should only be called once in the very beginning

	if (!parent)
	{
		m_parentOffset = 0;
	}
	else
	{
		ASSERT(parent < this);
		m_parentOffset = (uint_t)((char*)this - (char*)parent);
		parent->addWeakRef();
	}

	m_flags = flags;
}

inline
size_t
RefCount::release()
{
	intptr_t refCount = sys::atomicDec(&m_refCount);

	if (!refCount)
	{
		this->~RefCount();
		weakRelease(); // weakRelease () should be here, not in ~RefCount ()
	}

	return refCount;
}

inline
size_t
RefCount::weakRelease()
{
	intptr_t refCount = sys::atomicDec(&m_weakRefCount);

	if (!refCount)
		if (m_flags & RefCountFlag_Allocated)
		{
			RefCountAllocHdr* hdr = (RefCountAllocHdr*)this - 1;
			hdr->m_freeFunc(hdr);
		}
		else if (m_parentOffset)
		{
			RefCount* parent = (RefCount*)((char*)this - m_parentOffset);
			parent->weakRelease();
		}

	return refCount;
}

inline
size_t
RefCount::addRefByWeakPtr()
{
	for (;;)
	{
		int32_t old = m_refCount;
		if (old == 0)
			return 0;

		if (sys::atomicCmpXchg(&m_refCount, old, old + 1) == old)
			return old + 1;
	}
}

//..............................................................................

template <typename T>
class Box:
	public RefCount,
	public T
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class Release
{
public:
	void
	operator () (T* p) const
	{
		p->release();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class WeakRelease
{
public:
	void
	operator () (T* p) const
	{
		p->weakRelease();
	}
};

//..............................................................................

// forwards

template <typename T>
class Ptr;

template <typename T>
class WeakPtr;

AXL_SELECT_ANY struct NullPtr
{
} g_nullPtr;

//..............................................................................

} // namespace ref
} // namespace axl
