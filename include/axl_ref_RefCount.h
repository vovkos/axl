// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_REFCOUNT_H

#include "axl_mem_Alloc.h"
#include "axl_mt_Atomic.h"

namespace axl {
namespace ref {

//.............................................................................

// CRefCount decouples destruction and freeing but not in a traditional vtable way
// this is done to allow overriding allocation at run-time.

// CRefCount features:
// -weak references and strong references
// -using different allocators for creating ref-counted object
// -creating ref-counted objects on member fields buffers (be sure to make container non-copiable)
// -creating ref-counted objects on static/stack buffers

// pfFree could be set to values -10..-1, NULL 
// these constants result in no built-in deallocation in CRefCount::WeakRelease ()
// could be used as marker for special kinds of deallocation or reference counting

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CRefCount
{
	AXL_DISABLE_COPY (CRefCount)
	
protected:
	volatile int32_t m_RefCount;
	volatile int32_t m_WeakRefCount;

	void* m_pObject;
	mem::FFree m_pfDestruct;
	mem::FFree m_pfFree;

public:
	CRefCount ();

	~CRefCount ()
	{ 
		ASSERT (m_RefCount == 0 && m_WeakRefCount > 0);
	}

	void
	SetTarget (
		void* pObject,
		mem::FFree pfDestruct,
		mem::FFree pfFree
		);

	size_t
	GetRefCount ()
	{ 
		return m_RefCount; 
	}

	size_t
	GetWeakRefCount () 
	{ 
		return m_WeakRefCount; 
	}

	void*
	GetObject ()
	{
		return m_pObject;
	}

	mem::FFree
	GetDestruct ()
	{
		return m_pfDestruct;
	}

	mem::FFree
	GetFree ()
	{
		return m_pfFree;
	}

	size_t
	AddRef ()
	{ 
		return mt::AtomicInc (&m_RefCount); 
	}

	size_t
	Release ();

	size_t
	AddWeakRef ()
	{ 
		return mt::AtomicInc (&m_WeakRefCount); 
	}

	size_t
	WeakRelease ();

	size_t
	AddRefByWeakPtr ();
};

//.............................................................................

template <typename T>
class CBoxT: 
	public CRefCount,
	public T
{
};

//.............................................................................

} // namespace ref
} // namespace axl
