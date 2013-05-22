// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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

// pfFree could be set to values -10..-1, NULL 
// these constants result in no built-in deallocation in IRefCount::WeakRelease ()
// could be used as marker for special kinds of deallocation or reference counting

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct IRefCount: obj::IRoot
{
	// {39F60DA6-DC14-40F7-93C7-81725A0FCA02}
	AXL_OBJ_INTERFACE (
		IRefCount,
		0x39f60da6, 0xdc14, 0x40f7, 0x93, 0xc7, 0x81, 0x72, 0x5a, 0x0f, 0xca, 0x02
		)
		
protected:
	volatile int32_t m_RefCount;
	volatile int32_t m_WeakRefCount;
	mem::FFree m_pfFree;

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
		return m_pfFree;
	}

	void
	SetFree (mem::FFree pfFree)
	{
		ASSERT (!m_pfFree); // only once!
		m_pfFree = pfFree;
	}

	IRefCount&
	operator = (const IRefCount& Src) 
	{
		return *this; // don't copy fields!
	}

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

	size_t
	AddRef ()
	{ 
		return mt::AtomicInc (&m_RefCount); 
	}

	size_t
	Release ()
	{
		int32_t RefCount = mt::AtomicDec (&m_RefCount);

		if (!RefCount)
		{
			Destruct ();
			WeakRelease (); // should be here and not in ~IRefCount () !
		}

		return (uint32_t) RefCount;
	}

	size_t
	AddWeakRef ()
	{ 
		return mt::AtomicInc (&m_WeakRefCount); 
	}

	size_t
	WeakRelease ()
	{
		int32_t RefCount = mt::AtomicDec (&m_WeakRefCount);

		if (!RefCount && m_pfFree != NULL && (size_t) m_pfFree < -10)
			m_pfFree (GetObject ());

		return (uint32_t) RefCount;
	}

	size_t
	AddRefByWeakPtr ()
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

protected:
	void
	Init ()
	{
		m_RefCount = 0;
		m_WeakRefCount = 1;
		m_pfFree = NULL;
	}

	void
	Destruct ()
	{
		void* pVTable = *(void**) this; // save vtable for GetObject () in WeakRelease ()
		obj::IType* pType;
		void* p = GetObject (&pType);
		pType->Destruct (p);
		*(void**) this = pVTable; // restore vtable 
	}
};

//.............................................................................

// this macro will fire a compilation error when AXL_OBJ_CLASS () macros are 
// used on ref::IRefCount-derived class
// this is done to prevent making ref::IRefCount-derived class directly creatable
// ref::IRefCount-derived classes should be created with ref::CFactoryT / CRtFactoryT / CChildT
// more straighforward solution would be adding a virtual function guard to ref::IRefCount and only 
// implementing it in ref::CCreatableClassT / CRtFactoryT::CBox
// but this solution is free, and virtual function guard is not

#undef AXL_OBJ_REF_COUNT_MISCREATION_GUARD
#define AXL_OBJ_REF_COUNT_MISCREATION_GUARD(Class) \
	static \
	void \
	__RefCountMiscreationGuard () \
	{ \
		class CRefCountMiscreationGuard: \
			public Class, \
			public axl::ref::IRefCount \
		{ \
		}; \
	}

//.............................................................................

// convenient macros for declaring ref-counted classes (directly non-creatable)

#define AXL_REF_BEGIN_CLASS(Class, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_GUID_TAG (l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (Class) \
		AXL_OBJ_INTERFACE_ENTRY (axl::ref::IRefCount) \

#define AXL_REF_BEGIN_CLASS_E(Class, Guid) \
	AXL_OBJ_GUID_TAG_E (Guid) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (Class) \
		AXL_OBJ_INTERFACE_ENTRY (axl::ref::IRefCount) 

#define AXL_REF_BEGIN_CLASS_0(Class) \
	AXL_OBJ_GUID_TAG_E (axl::rtl::GUID_Null) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (Class) \
		AXL_OBJ_INTERFACE_ENTRY (axl::ref::IRefCount)

#define AXL_REF_END_CLASS() \
	AXL_OBJ_END_CLASS ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_REF_CLASS(Class, Iface, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_REF_BEGIN_CLASS (Class, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
		AXL_OBJ_INTERFACE_ENTRY (Iface) \
	AXL_REF_END_CLASS()

#define AXL_REF_CLASS_E(Class, Iface, Guid) \
	AXL_REF_BEGIN_CLASS_E (Class, Guid) \
		AXL_OBJ_INTERFACE_ENTRY (Iface) \
	AXL_REF_END_CLASS()

#define AXL_REF_CLASS_0(Class, Iface) \
	AXL_REF_BEGIN_CLASS_0 (Class) \
		AXL_OBJ_INTERFACE_ENTRY (Iface) \
	AXL_REF_END_CLASS()

//.............................................................................

template <typename T>
class CBoxT: 
	public IRefCount,
	public T
{
public:
	AXL_REF_BEGIN_CLASS_0 (CBoxT)
	AXL_REF_END_CLASS()
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CClassBoxT: 
	public IRefCount,
	public T
{
public:
	AXL_REF_BEGIN_CLASS_E (CClassBoxT, AXL_OBJ_GUIDOF (T))
		AXL_OBJ_INTERFACE_CHAIN (T)
	AXL_REF_END_CLASS ()
};

//.............................................................................

} // namespace ref
} // namespace axl
