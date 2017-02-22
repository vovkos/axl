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
FreeFunc (void* p);

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

enum RefCountFlag
{
	RefCountFlag_Allocated = 0x01, // must be freed, free func is in hdr

	// ok to define and use your own flags 0x08..0x80
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class RefCount
{
	AXL_DISABLE_COPY (RefCount)

protected:
	volatile intptr_t m_refCount;
	volatile intptr_t m_weakRefCount;

	uintptr_t m_flags        : 8;

#if (AXL_PTR_BITS == 64)
	uintptr_t m_parentOffset : 56;
#else
	uintptr_t m_parentOffset : 24; // more than enough
#endif

public:
	RefCount ();

	virtual
	~RefCount ()
	{
		ASSERT (m_refCount == 0 && m_weakRefCount > 0);
	}

	void
	prime (
		RefCount* parent,
		uint_t flags = 0
		);

	size_t
	getRefCount ()
	{
		return m_refCount;
	}

	size_t
	getWeakRefCount ()
	{
		return m_weakRefCount;
	}

	uint_t
	getFlags ()
	{
		return m_flags;
	}

	size_t
	addRef ()
	{
		return sys::atomicInc (&m_refCount);
	}

	size_t
	release ();

	size_t
	addWeakRef ()
	{
		return sys::atomicInc (&m_weakRefCount);
	}

	size_t
	weakRelease ();

	size_t
	addRefByWeakPtr ();
};

//..............................................................................

template <typename T>
class Box:
	public RefCount,
	public T
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class AllocBox:
	public RefCountAllocHdr,
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
		p->release ();
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
		p->weakRelease ();
	}
};

//..............................................................................

} // namespace ref
} // namespace axl
