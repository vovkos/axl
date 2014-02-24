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
	CRefCount* m_pRefCount;

public:
	CWeakPtrT (int i = 0)
	{ 
		m_p = NULL;
		m_pRefCount = NULL; 
	}

	CWeakPtrT (const CWeakPtrT& Src)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		Copy (Src.m_p, Src.m_pRefCount);
	}

	template <typename T2>
	CWeakPtrT (const CWeakPtrT <T2>& Src)
	{
		m_p = NULL, m_pRefCount = NULL;
		Copy (Src.m_p, Src.m_pRefCount);
	}

	template <typename T2>
	CWeakPtrT (const CPtrT <T2>& Src)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		Copy (Src.m_p, Src.m_pRefCount);
	}

	template <typename T2>
	CWeakPtrT (T2* p)
	{
		m_p = NULL, m_pRefCount = NULL;
		Copy (p, p);
	}

	CWeakPtrT (
		T* p, 
		CRefCount* pRefCount
		)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		Copy (p, pRefCount);
	}

	~CWeakPtrT ()
	{ 
		Clear (); 
	}

	CWeakPtrT& 
	operator = (const CWeakPtrT& Src)
	{ 
		Copy (Src.m_p, Src.m_pRefCount);
		return *this;
	}

	void 
	Copy (
		T* p, 
		CRefCount* pRefCount
		)
	{
		m_p = p;

		if (m_pRefCount == pRefCount)
			return;

		if (pRefCount)
			pRefCount->AddWeakRef (); 

		if (m_pRefCount)
			m_pRefCount->WeakRelease ();

		m_pRefCount = pRefCount;
	}

	void 
	Attach (
		T* p, 
		CRefCount* pRefCount
		)
	{
		if (m_pRefCount)
			m_pRefCount->WeakRelease ();

		m_p = p;
		m_pRefCount = pRefCount;
	}

	void 
	Detach (
		T** pp = NULL, 
		CRefCount** ppRefCount = NULL
		)
	{ 
		if (pp)
			*pp = m_p;
		
		if (ppRefCount)
			*ppRefCount = m_pRefCount;

		m_p = NULL;
		m_pRefCount = NULL; 
	}

	void 
	Clear ()
	{ 
		if (m_pRefCount)
			m_pRefCount->WeakRelease ();

		m_p = NULL;
		m_pRefCount = NULL;
	}

	CRefCount* 
	GetRefCount () const
	{
		return m_pRefCount;
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
