// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_PTR_H

#include "axl_ref_RefCount.h"

namespace axl {
namespace ref {

//.............................................................................

// forward

template <typename T>
class WeakPtr;

//.............................................................................

enum PtrKind
{
	PtrKind_Null = 0
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class Ptr
{
	template <typename T2>
	friend class Ptr;

	template <typename T2>
	friend class WeakPtr;

protected:
	T* m_p;
	RefCount* m_refCount;

public:
	Ptr ()
	{ 
		m_p = NULL;
		m_refCount = NULL; 
	}

	Ptr (PtrKind ptrKind)
	{ 
		ASSERT (ptrKind == PtrKind_Null);
		m_p = NULL;
		m_refCount = NULL; 
	}

	Ptr (const Ptr& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename A>
	Ptr (const Ptr <A>& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename A>
	Ptr (const WeakPtr <A>& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		if (src.m_refCount && src.m_refCount->addRefByWeakPtr ())
			attach (src.m_p, src.m_refCount);
	}

	template <typename A>
	Ptr (A* p)
	{
		m_p = NULL, m_refCount = NULL;
		copy (p, p);
	}

	Ptr (
		T* p, 
		RefCount* refCount
		)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (p, refCount);
	}

	~Ptr ()
	{ 
		clear (); 
	}
	
	operator T* () const
	{ 
		return m_p; 
	}

	T* 
	operator -> () const
	{ 
		ASSERT (m_p); 
		return m_p; 
	}

	Ptr& 
	operator = (const Ptr& src)
	{ 
		copy (src.m_p, src.m_refCount);
		return *this;
	}

	Ptr&
	operator = (PtrKind ptrKind)
	{ 
		ASSERT (ptrKind == PtrKind_Null);
		clear ();
		return *this;
	}

	T* 
	p () const
	{
		return m_p;
	}
	
	RefCount* 
	getRefCount () const
	{
		return m_refCount;
	}
	
	void 
	copy (
		T* p, 
		RefCount* refCount
		)
	{
		m_p = p;

		if (m_refCount == refCount)
			return;

		if (refCount)
			refCount->addRef (); 

		if (m_refCount)
			m_refCount->release ();

		m_refCount = refCount;
	}

	void 
	attach (
		T* p, 
		RefCount* refCount
		)
	{
		if (m_refCount)
			m_refCount->release ();

		m_p = p;
		m_refCount = refCount;
	}

	T* 
	detach (RefCount** refCount = NULL)
	{ 
		T* p = m_p;

		if (refCount)
			*refCount = m_refCount;

		m_p = NULL;
		m_refCount = NULL;
		return p;
	}

	void 
	clear ()
	{ 
		if (m_refCount)
			m_refCount->release ();

		m_p = NULL;
		m_refCount = NULL;
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
