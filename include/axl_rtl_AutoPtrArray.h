// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_AUTOPTRARRAY_H

#include "axl_rtl_Array.h"
#include "axl_rtl_Func.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename T,
	typename TDelete
	>
class CAutoPtrArrayDetailsT: public CSimpleArrayDetailsT <T*>
{
public:
	static 
	void 
	Destruct (
		T** p, 
		size_t Count
		)
	{ 
		T** pEnd = p + Count;
		for (; p < pEnd; p++)
			if (*p)
				TDelete () (*p);
	}

	static 
	void 
	Copy (
		T** pDst, 
		T* const* pSrc, 
		size_t Count
		)
	{ 
		if (pSrc > pDst || pSrc + Count <= pDst)
		{
			T** pEnd = pDst + Count;
			for (; pDst < pEnd; pDst++, pSrc++)
			{
				if (*pDst)
					TDelete () (*pDst);

				*pDst = *pSrc;
			}
		}
		else
		{
			T** pEnd = pDst;

			pDst += Count;
			pSrc += Count;

			while (pDst > pEnd)
			{
				pDst--;
				pSrc--;

				if (*pDst)
					TDelete () (*pDst);

				*pDst = *pSrc;
			}
		}
	}

	static 
	void 
	Clear (
		T** p, 
		size_t Count
		)
	{
		T** pBegin = p;
		T** pEnd = p + Count;

		for (; p < pEnd; p++)
			if (*p)
				TDelete () (*p);
		
		memset (pBegin, 0, Count * sizeof (T)); 
	}	
};

//.............................................................................

template <
	typename T,
	typename TDelete = mem::CStdFactoryT <T>::CDelete
	>
class CAutoPtrArrayT: public rtl::CArrayT <T*, CAutoPtrArrayDetailsT <T, TDelete> >
{
public:
	typedef rtl::CArrayT <T*, CAutoPtrArrayDetailsT <T, TDelete> > CBaseType;

public:
	CAutoPtrArrayT ()
	{ 
	}

	CAutoPtrArrayT (T* e):
		CBaseType (e)
	{
	}

	CAutoPtrArrayT (
		T* const* p, 
		size_t Count
		):
		CBaseType (p, Count)
	{ 
	}

	CAutoPtrArrayT (
		ref::EBuf BufKind,
		void* p, 
		size_t Size
		):
		CBaseType (BufKind, p, Size)
	{
	}

	operator const T** () const
	{ 
		return m_p; 
	}

	operator T** () 
	{ 
		return m_p; 
	}

	// .ca () is mostly for passing through vararg
	// thanks a lot gcc
	
	const T** 
	ca () const
	{ 
		return m_p; 
	}

	T** 
	a () 
	{ 
		return m_p; 
	}

	void
	TakeOver (CAutoPtrArrayT* pSrc)
	{
		Release ();
		m_p = pSrc;
		pSrc->m_p = NULL;
	}

	bool 
	Copy (
		T* const* p, 
		size_t Count
		)
	{
		return CBaseType::Copy (p, Count);
	}

	bool 
	Copy (T* e)
	{
		return CBaseType::Copy (e);
	}

	T** 
	Append (
		T* const* p, 
		size_t Count
		)
	{ 
		return CBaseType::Append (p, Count);
	}

	T**
	Append (T* e)
	{ 
		return CBaseType::Append (e);
	}

	T**
	Insert (
		size_t Index, 
		T* const* p, 
		size_t Count
		)
	{
		return CBaseType::Insert (Index, p, Count);
	}

	T**
	Insert (
		size_t Index, 
		T* e
		)
	{
		return CBaseType::Insert (Index, e);
	}

	size_t
	Pop (size_t Count = 1)
	{
		size_t OldCount = GetCount ();
		if (Count >= OldCount)
			Count = OldCount;

		SetCount (OldCount - Count);
		return Count;
	}

	T* 
	GetBackAndPop ()
	{
		T* e = GetBack ();
		Pop ();
		return e;
	}

private:
	CAutoPtrArrayT (const CAutoPtrArrayT& Src)
	{   
	}

	void
	operator = (const CAutoPtrArrayT& Src)
	{ 
	}

	T**
	AppendMultiply (
		T* e,
		size_t Count
		)
	{ 
	}

	T**
	InsertMultiply (
		size_t Index, 
		T* e, 
		size_t Count
		)
	{
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
