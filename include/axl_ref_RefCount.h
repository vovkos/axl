// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_REFCOUNT_H

#include "axl_mem_Alloc.h"
#include "axl_mt_Atomic.h"

namespace axl {
namespace ref {

//.............................................................................

typedef 
void 
FreeFunc (void* p);

//.............................................................................

// CRefCount decouples destruction and freeing but not in a traditional vtable way
// this is done to allow overriding allocation at run-time.

// CRefCount features:
// -weak references and strong references
// -using different allocators for creating ref-counted object
// -creating ref-counted objects on member fields buffers (be sure to make container non-copiable)
// -creating ref-counted objects on static/stack buffers

// freeFunc could be set to values -10..-1, NULL 
// these constants result in no built-in deallocation in CRefCount::WeakRelease ()
// could be used as marker for special kinds of deallocation or reference counting

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class RefCount
{
	AXL_DISABLE_COPY (RefCount)
	
protected:
	volatile int32_t m_refCount;
	volatile int32_t m_weakRefCount;

	void* m_object;
	FreeFunc* m_destructFunc;
	FreeFunc* m_freeFunc;

public:
	RefCount ();

	~RefCount ()
	{ 
		ASSERT (m_refCount == 0 && m_weakRefCount > 0);
	}

	void
	setTarget (
		void* object,
		FreeFunc* destructFunc,
		FreeFunc* freeFunc
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

	void*
	getObject ()
	{
		return m_object;
	}

	FreeFunc*
	getDestruct ()
	{
		return m_destructFunc;
	}

	FreeFunc*
	getFree ()
	{
		return m_freeFunc;
	}

	size_t
	addRef ()
	{ 
		return mt::atomicInc (&m_refCount); 
	}

	size_t
	release ();

	size_t
	addWeakRef ()
	{ 
		return mt::atomicInc (&m_weakRefCount); 
	}

	size_t
	weakRelease ();

	size_t
	addRefByWeakPtr ();
};

//.............................................................................

template <typename T>
class Box: 
	public RefCount,
	public T
{
};

//.............................................................................

} // namespace ref
} // namespace axl
