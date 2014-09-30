// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_WEAKPTR_H

#include "axl_ref_RefCount.h"

namespace axl {
namespace ref {

//.............................................................................

// forward

template <typename T>
class CPtrT;

//.............................................................................

// weak ptr

template <typename T>
class CWeakPtrT
{
	template <typename T2>
	friend class CPtrT;

	template <typename T2>
	friend class CWeakPtrT;

protected:
	T* m_p;
	CRefCount* m_refCount;

public:
	CWeakPtrT (int i = 0)
	{ 
		m_p = NULL;
		m_refCount = NULL; 
	}

	CWeakPtrT (const CWeakPtrT& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename T2>
	CWeakPtrT (const CWeakPtrT <T2>& src)
	{
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename T2>
	CWeakPtrT (const CPtrT <T2>& src)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (src.m_p, src.m_refCount);
	}

	template <typename T2>
	CWeakPtrT (T2* p)
	{
		m_p = NULL, m_refCount = NULL;
		copy (p, p);
	}

	CWeakPtrT (
		T* p, 
		CRefCount* refCount
		)
	{ 
		m_p = NULL, m_refCount = NULL;
		copy (p, refCount);
	}

	~CWeakPtrT ()
	{ 
		clear (); 
	}

	CWeakPtrT& 
	operator = (const CWeakPtrT& src)
	{ 
		copy (src.m_p, src.m_refCount);
		return *this;
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
			refCount->addWeakRef (); 

		if (m_refCount)
			m_refCount->weakRelease ();

		m_refCount = refCount;
	}

	void 
	attach (
		T* p, 
		CRefCount* refCount
		)
	{
		if (m_refCount)
			m_refCount->weakRelease ();

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
			m_refCount->weakRelease ();

		m_p = NULL;
		m_refCount = NULL;
	}

	CRefCount* 
	getRefCount () const
	{
		return m_refCount;
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
