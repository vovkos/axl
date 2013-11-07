// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_ARRAY_H

#include "axl_err_Error.h"

namespace axl {
namespace rtl {

//.............................................................................

// general case: full cycle of construction, copy, destruction

template <typename T>
class CArrayDetailsT
{
public:
	static 
	void 
	Construct (
		T* p, 
		size_t Count
		)
	{ 
		memset (p, 0, Count * sizeof (T)); // zero memory before construction

		T* pEnd = p + Count;
		for (; p < pEnd; p++)
			new (p) T;
	}

	static 
	void 
	ConstructCopy (
		T* pDst, 
		const T* pSrc, 
		size_t Count
		)
	{ 
		memset (pDst, 0, Count * sizeof (T)); // zero memory before construction

		T* pEnd = pDst + Count;
		for (; pDst < pEnd; pDst++, pSrc++)
			new (pDst) T (*pSrc);
	}

	static 
	void 
	Destruct (
		T* p, 
		size_t Count
		)
	{ 
		T* pEnd = p + Count;
		for (; p < pEnd; p++)
			p->~T ();
	}

	static 
	void 
	Copy (
		T* pDst, 
		const T* pSrc, 
		size_t Count
		)
	{ 
		if (pSrc > pDst || pSrc + Count <= pDst)
		{
			T* pEnd = pDst + Count;
			for (; pDst < pEnd; pDst++, pSrc++)
				*pDst = *pSrc;
		}
		else
		{
			T* pEnd = pDst;

			pDst += Count;
			pSrc += Count;

			while (pDst > pEnd)
			{
				pDst--;
				pSrc--;
				*pDst = *pSrc;
			}
		}
	}

	static 
	void 
	Clear (
		T* p, 
		size_t Count
		)
	{
		T* pBegin = p;
		T* pEnd = p + Count;

		for (; p > pEnd; p++)
			p->~T ();
		
		p = pBegin; 
		memset (p, 0, Count * sizeof (T)); 
		
		for (; p > pEnd; p++)
			new (p) T;
	}	
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// fast memory block operations for types that do not need ctor/dtor

template <typename T>
class CSimpleArrayDetailsT
{
public:
	static 
	void 
	Construct (
		T* p, 
		size_t Count
		)
	{ 
		memset (p, 0, Count * sizeof (T)); 
	}

	static 
	void 
	ConstructCopy (
		T* pDst, 
		const T* pSrc, 
		size_t Count
		)
	{ 
		memcpy (pDst, pSrc, Count * sizeof (T)); 
	}

	static 
	void 
	Destruct (
		T* p, 
		size_t Count
		)
	{
	}

	static 
	void 
	Copy (
		T* pDst, 
		const T* pSrc, 
		size_t Count
		)
	{ 
		memmove (pDst, pSrc, Count * sizeof (T)); 
	}

	static 
	void 
	Clear (
		T* p, 
		size_t Count
		)
	{ 
		memset (p, 0, Count * sizeof (T)); 
	}	
};

//.............................................................................

// specialization for simple types

template <>
class CArrayDetailsT <char>: public CSimpleArrayDetailsT <char>
{
};

template <>
class CArrayDetailsT <wchar_t>: public CSimpleArrayDetailsT <wchar_t>
{
};

template <>
class CArrayDetailsT <float>: public CSimpleArrayDetailsT <float>
{
};

template <>
class CArrayDetailsT <double>: public CSimpleArrayDetailsT <double>
{
};

template <>
class CArrayDetailsT <int8_t>: public CSimpleArrayDetailsT <int8_t>
{
};

template <>
class CArrayDetailsT <uint8_t>: public CSimpleArrayDetailsT <uint8_t>
{
};

template <>
class CArrayDetailsT <int16_t>: public CSimpleArrayDetailsT <int16_t>
{
};

template <>
class CArrayDetailsT <uint16_t>: public CSimpleArrayDetailsT <uint16_t>
{
};

template <>
class CArrayDetailsT <int32_t>: public CSimpleArrayDetailsT <int32_t>
{
};

template <>
class CArrayDetailsT <uint32_t>: public CSimpleArrayDetailsT <uint32_t>
{
};

template <>
class CArrayDetailsT <int64_t>: public CSimpleArrayDetailsT <int64_t>
{
};

template <>
class CArrayDetailsT <uint64_t>: public CSimpleArrayDetailsT <uint64_t>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for pointers

template <typename T>
class CArrayDetailsT <T*>: public CSimpleArrayDetailsT <T*>
{
};

//.............................................................................

template <typename T>
class CArrayT
{
public:
	class CHdr: public ref::CRefCount
	{
	public:
		size_t m_MaxCount;
		size_t m_Count;

		~CHdr ()
		{
			CArrayDetailsT <T>::Destruct ((T*) (this + 1), m_Count);
		}
	};

	typedef CArrayDetailsT <T> CDetails;

protected:
	T* m_p;

public:
	CArrayT ()
	{ 
		m_p = NULL;
	}

	CArrayT (T e)
	{ 
		m_p = NULL;
		Copy (&e, 1);
	}

	CArrayT (const CArrayT& Src)
	{   
		m_p = NULL;
		Copy (Src); 
	}

	CArrayT (
		const T* p, 
		size_t Count
		)
	{ 
		m_p = NULL;
		Copy (p, Count); 
	}

	CArrayT (
		ref::EBuf Kind,
		void* p, 
		size_t Size
		)
	{
		m_p = NULL;
		SetBuffer (Kind, p, Size);
	}

	~CArrayT ()
	{
		Release ();
	}

	operator const T* () const
	{ 
		return m_p; 
	}

	operator T* () 
	{ 
		return m_p; 
	}

	CArrayT& 
	operator = (const CArrayT& Src)
	{ 
		Copy (Src);
		return *this;
	}

	// .ca () is mostly for passing through vararg
	// thanks a lot gcc
	
	const T* 
	ca () const
	{ 
		return m_p; 
	}

	T* 
	a () 
	{ 
		return m_p; 
	}
	
	size_t 
	GetCount () const
	{ 
		return m_p ? GetHdr ()->m_Count : 0; 
	}

	size_t 
	GetReservedCount () const
	{ 
		return m_p ? GetHdr ()->m_MaxCount : 0; 
	}

	bool 
	IsEmpty () const
	{ 
		return GetCount () == 0; 
	}

	const T&
	GetFront () const
	{
		ASSERT (!IsEmpty ());
		return m_p [0];
	}

	T&
	GetFront () 
	{
		ASSERT (!IsEmpty ());
		return m_p [0];
	}

	const T&
	GetBack () const
	{
		ASSERT (!IsEmpty ());
		return m_p [GetCount () - 1];
	}

	T&
	GetBack () 
	{
		ASSERT (!IsEmpty ());
		return m_p [GetCount () - 1];
	}

	void 
	Release ()
	{
		if (!m_p)
			return;

		GetHdr ()->Release ();
		m_p = NULL;
	}

	void 
	Clear ()
	{ 
		SetCount (0); 
	}

	bool
	Copy (const CArrayT& Src)
	{ 
		if (m_p == Src.m_p)
			return true;

		if (!Src.m_p)
		{
			Release ();
			return true; 
		}

		if (Src.GetHdr ()->GetFree () == (mem::FFree*) -1)
			return Copy (Src, Src.GetCount ()); 

		if (Src.m_p)
			Src.GetHdr ()->AddRef ();

		if (m_p)
			GetHdr ()->Release ();

		m_p = Src.m_p;
		return true;
	}

	bool 
	Copy (
		const T* p, 
		size_t Count
		)
	{
		if (Count == 0)
		{
			Clear ();
			return true;
		}

		bool Result = SetCount (Count);
		if (!Result)
			return false;

		CDetails::Copy (m_p, p, Count);
		return true;
	}

	bool 
	Copy (T e)
	{
		return Copy (&e, 1);
	}

	T* 
	Append (
		const T* p, 
		size_t Count
		)
	{ 
		return Insert (-1, p, Count); 
	}

	T*
	Append (T e)
	{ 
		return Insert (-1, &e, 1); 
	}

	T*
	Append (const CArrayT& Src)
	{ 
		return Insert (-1, Src, Src.GetCount ()); 
	}

	T*
	Insert (
		size_t Index, 
		const T* p, 
		size_t Count
		)
	{
		size_t OldCount = GetCount ();
		bool Result = SetCount (OldCount + Count);
		if (!Result)
			return NULL;

		if (Index > OldCount)
			Index = OldCount;

		T* pDst = m_p + Index;

		if (Count == 0)
			return pDst;

		if (Index < OldCount)
			CDetails::Copy (pDst + Count, pDst, OldCount - Index);

		if (p)
			CDetails::Copy (pDst, p, Count);
		else
			CDetails::Clear (pDst, Count);

		return pDst;
	}

	T*
	Insert (
		size_t Index, 
		T e
		)
	{
		return Insert (Index, &e, 1);
	}

	T* 
	Insert (
		size_t Index, 
		const CArrayT& Src
		)
	{ 
		return Insert (Index, Src, Src.GetCount ()); 
	}

	bool 
	Remove (
		size_t Index, 
		size_t Count = 1
		)
	{
		if (Count == 0)
			return true;

		size_t OldCount = GetCount ();
		if (Index >= OldCount)
			return true;

		if (Index + Count >= OldCount)
			return SetCount (Index);

		bool Result = EnsureExclusive ();
		if (!Result)
			return false;

		size_t NewCount = OldCount - Count;

		T* pDst = m_p + Index;
		CDetails::Copy (pDst, pDst + Count, NewCount - Index);

		return SetCount (NewCount);
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

	T 
	GetBackAndPop ()
	{
		T e = GetBack ();
		Pop ();
		return e;
	}

	bool 
	Move (
		size_t IndexDst, 
		size_t IndexSrc, 
		size_t Count = 1
		)
	{
		if (Count == 0 || IndexDst == IndexSrc)
			return true;

		size_t OldCount = GetCount ();

		if (IndexDst + Count > OldCount || IndexSrc + Count > OldCount)
		{
			err::SetError (err::EStatus_InvalidParameter);
			return false;
		}

		T* pTemp = (T*) AXL_MEM_ALLOC (Count * sizeof (T));
		if (!pTemp)
			return false;

		T* pDst = m_p + IndexDst;
		T* pSrc = m_p + IndexSrc;

		CDetails::ConstructCopy (pTemp, pSrc, Count);

		if (IndexSrc < IndexDst)
			CDetails::Copy (pSrc, pSrc + Count, IndexDst - IndexSrc);
		else
			CDetails::Copy (pDst + Count, pDst, IndexSrc - IndexDst);

		CDetails::Copy (pDst, pTemp, Count);

		CDetails::Destruct (pTemp, Count);
		AXL_MEM_FREE (pTemp);

		return true;
	}

	size_t
	Find (T e)
	{
		size_t Count = GetCount ();
		for (size_t i = 0; i < Count; i++)
			if (m_p [i] == e)
				return i;

		return -1;
	}

	size_t
	FindReverse (T e)
	{
		size_t Count = GetCount ();
		for (intptr_t i = Count - 1; i >= 0; i--)
			if (m_p [i] == e)
				return i;

		return -1;
	}

	T* 
	GetBuffer ()
	{ 
		return EnsureExclusive () ? m_p : NULL; 
	}

	T* 
	GetBuffer (size_t Count)
	{ 
		return SetCount (Count) ? m_p : NULL; 
	}

	bool 
	Reserve (size_t Count)
	{
		if (Count <= GetReservedCount ())
			return EnsureExclusive ();

		size_t MaxCount = rtl::GetMinPower2Ge (Count);
		size_t Size = MaxCount * sizeof (T);

		ref::CPtrT<CHdr> NewHdr = AXL_REF_NEW_EXTRA (CHdr, Size);
		if (!NewHdr)
			return false;

		size_t OldCount = GetCount ();

		NewHdr->m_Count = OldCount;
		NewHdr->m_MaxCount = MaxCount;

		T* p = (T*) (NewHdr + 1);

		if (OldCount)
		{
			CDetails::ConstructCopy (p, m_p, OldCount);
			GetHdr ()->Release ();
		}

		m_p = p;

		NewHdr.Detach ();
		return true;
	}

	size_t 
	EnsureCount (size_t Count)
	{
		if (GetCount () < Count)
			SetCount (Count);

		return GetCount ();
	}

	bool 
	SetCount (size_t Count)
	{
		CHdr* pOldHdr = GetHdr ();

		if (pOldHdr && pOldHdr->GetRefCount () == 1)
		{
			if (pOldHdr->m_Count == Count)
				return true;

			if (pOldHdr->m_MaxCount >= Count)
			{
				size_t OldCount = pOldHdr->m_Count;
				if (Count > OldCount)
					CDetails::Construct (m_p + OldCount, Count - OldCount);
				else
					CDetails::Destruct (m_p + Count, OldCount - Count);

				pOldHdr->m_Count = Count;
				return true;
			}
		}

		if (Count == 0)
		{
			Release ();
			return true;
		}

		if (!pOldHdr)
		{
			bool Result = Reserve (Count);
			if (!Result)
				return false;

			CDetails::Construct (m_p, Count);
			GetHdr ()->m_Count = Count;
			return true;
		}

		size_t MaxCount = rtl::GetMinPower2Gt (Count); // make a room
		size_t Size = MaxCount * sizeof (T);

		ref::CPtrT<CHdr> NewHdr = AXL_REF_NEW_EXTRA (CHdr, Size);
		if (!NewHdr)
			return false;

		NewHdr->m_Count = Count;
		NewHdr->m_MaxCount = MaxCount;

		T* p = (T*) (NewHdr + 1);

		if (Count <= pOldHdr->m_Count)
		{
			CDetails::ConstructCopy (p, m_p, Count);
		}
		else
		{
			CDetails::ConstructCopy (p, m_p, pOldHdr->m_Count);
			CDetails::Construct (p + pOldHdr->m_Count, Count - pOldHdr->m_Count);
		}
		
		pOldHdr->Release ();

		m_p = p;

		NewHdr.Detach ();
		return true;
	}

	bool 
	EnsureExclusive ()
	{ 
		return m_p ? SetCount (GetCount ()) : true; 
	}

	void
	SetBuffer (
		ref::EBuf Kind,
		void* p,
		size_t Size
		)
	{
		ASSERT (Size >= sizeof (CHdr) + sizeof (T));

		CHdr* pOldHdr = GetHdr ();
		
		mem::FFree* pfFree = Kind == ref::EBuf_Static ? NULL : (mem::FFree*) -1;
		ref::CPtrT <CHdr> NewHdr = AXL_REF_NEW_INPLACE (CHdr, p, pfFree);
		NewHdr->m_Count = 0;
		NewHdr->m_MaxCount = (Size - sizeof (CHdr)) / sizeof (T);

		if (pOldHdr)
			pOldHdr->Release ();

		m_p = (T*) (NewHdr + 1);

		NewHdr.Detach ();
	}

protected:
	CHdr*
	GetHdr () const
	{
		return m_p ? (CHdr*) m_p - 1 : NULL;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
