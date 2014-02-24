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
	CRefCount* m_pRefCount;

public:
	CPtrT ()
	{ 
		m_p = NULL;
		m_pRefCount = NULL; 
	}

	CPtrT (EPtr PtrKind)
	{ 
		ASSERT (PtrKind == EPtr_Null);
		m_p = NULL;
		m_pRefCount = NULL; 
	}

	CPtrT (const CPtrT& Src)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		Copy (Src.m_p, Src.m_pRefCount);
	}

	template <typename A>
	CPtrT (const CPtrT <A>& Src)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		Copy (Src.m_p, Src.m_pRefCount);
	}

	template <typename A>
	CPtrT (const CWeakPtrT <A>& Src)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		if (Src.m_pRefCount && Src.m_pRefCount->AddRefByWeakPtr ())
			Attach (Src.m_p, Src.m_pRefCount);
	}

	template <typename A>
	CPtrT (A* p)
	{
		m_p = NULL, m_pRefCount = NULL;
		Copy (p, p);
	}

	CPtrT (
		T* p, 
		CRefCount* pRefCount
		)
	{ 
		m_p = NULL, m_pRefCount = NULL;
		Copy (p, pRefCount);
	}

	~CPtrT ()
	{ 
		Clear (); 
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
	operator = (const CPtrT& Src)
	{ 
		Copy (Src.m_p, Src.m_pRefCount);
		return *this;
	}

	T* 
	p () const
	{
		return m_p;
	}
	
	CRefCount* 
	GetRefCount () const
	{
		return m_pRefCount;
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
			pRefCount->AddRef (); 

		if (m_pRefCount)
			m_pRefCount->Release ();

		m_pRefCount = pRefCount;
	}

	void 
	Attach (
		T* p, 
		CRefCount* pRefCount
		)
	{
		if (m_pRefCount)
			m_pRefCount->Release ();

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
			m_pRefCount->Release ();

		m_p = NULL;
		m_pRefCount = NULL;
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
