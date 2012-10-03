// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_REFCOUNT_H

#include "axl_obj_Root.h"
#include "axl_mem_Alloc.h"

namespace axl {
namespace ref {

//.............................................................................

// IRefCount decouples destruction and freeing but not in a traditional vtable way
// this is done to allow overriding allocation at run-time.
// IRefCount features:
// -weak references and strong references
// -using different allocators for creating ref-counted object
// -creating ref-counted objects as member fields 
// -creating ref-counted objects on static buffers
// -cloning ref-counted objects at run-time into arbitrary memory 

// pfnFree could be set to values -10..-1, NULL 
// these constants result in no built-in deallocation in IRefCount::WeakRelease ()
// could be used as marker for special kinds of deallocation or reference counting

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[uuid ("ed9609f0-56bc-4f20-a907-add30c62fff9")]
class IRefCount: public obj::IRoot
{
protected:
	volatile long m_RefCount;
	volatile long m_WeakRefCount;
	mem::FFree m_pfnFree;

public:
	IRefCount () 
	{
		Init ();
	}

	IRefCount (const IRefCount& Src)
	{       		
		Init (); // don't copy fields!
	}

	~IRefCount ()
	{ 
		ASSERT (m_RefCount == 0);
	}

	mem::FFree
	GetFree ()
	{
		return m_pfnFree;
	}

	void
	SetFree (mem::FFree pfnFree)
	{
		ASSERT (!m_pfnFree); // only once!
		m_pfnFree = pfnFree;
	}

	IRefCount&
	operator = (const IRefCount& Src) 
	{
		return *this; // don't copy fields!
	}

	size_t
	GetRefCount ()
	{ 
		return (ulong_t) m_RefCount; 
	}

	size_t
	GetWeakRefCount () 
	{ 
		return (ulong_t) m_WeakRefCount; 
	}

	size_t
	AddRef ()
	{ 
		return (ulong_t) ::InterlockedIncrement (&m_RefCount); 
	}

	size_t
	Release ()
	{
		long RefCount = ::InterlockedDecrement (&m_RefCount);

		if (!RefCount)
		{
			Destruct ();
			WeakRelease (); // should be here and not in ~IRefCount () !
		}

		return (ulong_t) RefCount;
	}

	size_t
	AddWeakRef ()
	{ 
		return (ulong_t) ::InterlockedIncrement (&m_WeakRefCount); 
	}

	size_t
	WeakRelease ()
	{
		long RefCount = ::InterlockedDecrement (&m_WeakRefCount);

		if (!RefCount && m_pfnFree != NULL && (size_t) m_pfnFree < -10)
			m_pfnFree (GetObject ());

		return (ulong_t) RefCount;
	}

	size_t
	AddRefByWeakPtr ()
	{ 
		for (;;)
		{
			long Old = m_RefCount; 
			if (Old == 0)
				return 0;

			if (::InterlockedCompareExchange (&m_RefCount, Old + 1, Old) == Old)
				return (ulong_t) (Old + 1);
		}
	}

protected:
	void
	Init ()
	{
		m_RefCount = 0;
		m_WeakRefCount = 1;
		m_pfnFree = NULL;
	}

	void
	Destruct ()
	{
		void* pVTable = *(void**) this; // save vtable for GetObject () in WeakRelease ()
		obj::IClass* pClass;
		void* p = GetObject (&pClass);
		pClass->Destruct (p);
		*(void**) this = pVTable; // restore vtable 
	}
};

//.............................................................................

template <typename T>
class CBoxT: 
	public IRefCount,
	public T
{
public:
	AXL_OBJ_SIMPLE_INTERFACE_MAP (CBoxT, IRefCount)
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBoxObjT: 
	public IRefCount,
	public T
{
public:
	AXL_OBJ_BEGIN_INTERFACE_MAP (CBoxObjT)
		AXL_OBJ_PARENT (T)
		AXL_OBJ_INTERFACE (IRefCount)
	AXL_OBJ_END_INTERFACE_MAP ()
};

//.............................................................................

} // namespace ref
} // namespace axl
