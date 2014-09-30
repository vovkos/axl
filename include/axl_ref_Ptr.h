// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_PTR_H

#include "axl_ref_RefCount.h"

namespace axl {
namespace ref {

//.............................................................................

// forward

template <typename T>
class CWeakPtrT;

//.............................................................................

enum EPtr
{
	EPtr_Null = 0
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CPtrT
{
	template <typename T2>
	friend class CPtrT;

	template <typename T2>
	friend class CWeakPtrT;

protected:
	T* m_p;
	CRefCount* m_refCount;

public:
	CPtrT ()
	{ 
		m_p = NULL;
		m_refCount = NULL; 
	}

	CPtrT (EPtr ptrKind)
	{ 
		ASSERT (ptrKind == EPtr_Null);
		m_p = NULL;
		m_refCount = NULL; 
	}

	CPtrT (const CPtrT& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename A>
	CPtrT (const CPtrT <A>& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename A>
	CPtrT (const CWeakPtrT <A>& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		if (src.m_refCount && src.m_refCount->addRefByWeakPtr ())
			attach (src.m_p, src.m_refCount);
	}

	template <typename A>
	CPtrT (A* p)
	{
		m_p = NULL, m_refCount = NULL;
		copy (p, p);
	}

	CPtrT (
		T* p, 
		CRefCount* refCount
		)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (p, refCount);
	}

	~CPtrT ()
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

	CPtrT& 
	operator = (const CPtrT& src)
	{ 
		copy (src.m_p, src.m_refCount);
		return *this;
	}

	T* 
	p () const
	{
		return m_p;
	}
	
	CRefCount* 
	getRefCount () const
	{
		return m_refCount;
	}
	
	void 
	copy (
		T* p, 
		CRefCount* refCount
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
		CRefCount* refCount
		)
	{
		if (m_refCount)
			m_refCount->release ();

		m_p = p;
		m_refCount = refCount;
	}

	void 
	detach (
		T** pp_o = NULL, 
		CRefCount** refCount_o = NULL
		)
	{ 
		if (pp_o)
			*pp_o = m_p;
		
		if (refCount_o)
			*refCount_o = m_refCount;

		m_p = NULL;
		m_refCount = NULL; 
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
