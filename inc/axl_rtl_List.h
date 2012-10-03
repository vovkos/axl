// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_LIST_H

#include "axl_rtl_Func.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace rtl {

//.............................................................................

struct TListLink
{
	TListLink* m_pNext;
	TListLink* m_pPrev;
};

//.............................................................................

template <typename T>
class CIteratorBaseT
{
protected:
	TListLink* m_p;

public:
	CIteratorBaseT ()
	{ 
		m_p = NULL; 
	}

	operator void* () const
	{
		return m_p;
	}

	T& 
	operator ++ ()
	{ 
		if (m_p) 
			m_p = m_p->m_pNext; 

		return *(T*) this;
	}

	T& 
	operator -- ()
	{ 
		if (m_p) 
			m_p = m_p->m_pPrev; 

		return *(T*) this;
	}

	T
	operator ++ (int) // post increment
	{ 
		T Old = *(T*) this;
		operator ++ ();
		return Old;
	}

	T
	operator -- (int) // post decrement
	{ 
		T Old = *(T*) this;
		operator -- ();
		return Old;
	}

	T& 
	operator += (size_t Count)
	{ 
		return Inc (Count); 
	}

	T& 
	operator -= (size_t Count)
	{ 
		return Inc (-Count); 
	}

	T
	operator + (size_t Count) const
	{ 
		return GetInc (Count); 
	}

	T
	operator - (size_t Count) const
	{ 
		return GetInc (-Count); 
	}

	T& 
	Inc (intptr_t Count)
	{	
		intptr_t i;

		if (Count > 0)
			for (i = 0; i < Count && m_p; i++)
				m_p = m_p->m_pNext;
		else
			for (i = 0; i > Count && m_p; i--)
				m_p = m_p->m_pPrev;

		return *(T*) this;
	}

	T 
	GetInc (intptr_t Count) const
	{ 
		return CIteratorBaseT (*this).Inc (Count); 
	}

	TListLink* 
	GetLink () const
	{ 
		return m_p; 
	}

	static 
	T
	FromLink (TListLink* p)
	{ 
		T It;
		It.m_p = p;
		return It;
	}
};

//.............................................................................

template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CIteratorT: public CIteratorBaseT <CIteratorT <T, TLink> >
{
public:
	CIteratorT ()
	{ 
	}

	CIteratorT (const CIteratorT& Src)
	{ 
		m_p = Src.m_p; 
	}

	template <
		typename T2, 
		typename TLink2
		>
	CIteratorT (const CIteratorT <T2, TLink2>& Src)
	{ 
		operator = (*Src); 
	}

	CIteratorT (T* p)
	{ 
		operator = (p); 
	}

	T*
	operator * () const
	{ 
		return GetObject (); 
	}

	T* 
	operator -> () const
	{ 
		return GetObject (); 
	}

	CIteratorT&
	operator = (T* p)
	{
		m_p = p ? TLink () (p) : NULL; 
		return *this;
	}

	bool 
	operator == (T* p)
	{ 
		return m_p == (p ? TLink () (p) : NULL); 
	}

	bool 
	operator != (T* p)
	{ 
		return !operator == (p); 
	}

	T* 
	GetObject () const
	{ 
		size_t Offset = (size_t) TLink () ((T*) 1) - 1;
		return m_p ? (T*) ((uchar_t*) m_p - Offset) : NULL; 
	}

	T*
	GetEntry () const
	{
		return GetObject ();
	}
};

//.............................................................................

template <
	typename T, 
	typename TIterator, 
	typename TDelete,
	typename TLink
	>
class CListBaseT
{
public:
	typedef TIterator CIterator;
	typedef TDelete CDelete;
	typedef TLink CLink;

protected:
	TListLink* m_pHead;
	TListLink* m_pTail;
	size_t m_Count;

public:
	CListBaseT ()
	{ 
		Construct ();
	}
	
	~CListBaseT ()
	{ 
		Clear (); 
	}

	bool 
	IsEmpty () const
	{ 
		return m_pHead == NULL; 
	}

	size_t 
	GetCount () const
	{ 
		return m_Count; 
	}

	CIterator 
	GetHead () const
	{ 
		return CIterator::FromLink (m_pHead); 
	}

	CIterator 
	GetTail () const
	{ 
		return CIterator::FromLink (m_pTail); 
	}

	T* 
	Remove (CIterator It)
	{ 
		TListLink* pLink = It.GetLink ();
		TListLink* pNext = pLink->m_pNext;
		TListLink* pPrev = pLink->m_pPrev;

		if (pPrev)
			pPrev->m_pNext = pNext;
		else
			m_pHead = pNext;
		
		if (pNext)
			pNext->m_pPrev = pPrev;
		else
			m_pTail = pPrev;

		m_Count--;
		return It.GetEntry ();
	}

	T* 
	RemoveHead ()
	{ 
		return m_pHead ? Remove (CIterator::FromLink (m_pHead)) : NULL; 
	}

	T* 
	RemoveTail ()
	{ 
		return m_pTail ? Remove (CIterator::FromLink (m_pTail)) : NULL; 
	}

	void 
	Clear ()
	{ 
		while (!IsEmpty ())
			TDelete () (RemoveHead ());
	}

	CIterator 
	InsertHead (T* p)
	{ 
		TListLink* pLink = TLink () (p);

		pLink->m_pPrev = NULL;
		pLink->m_pNext = m_pHead;

		if (m_pHead)
			m_pHead->m_pPrev = pLink;
		else
			m_pTail = pLink;

		m_pHead = pLink;
		m_Count++;

		return CIterator::FromLink (pLink);
	}

	CIterator 
	InsertTail (T* p)
	{ 
		TListLink* pLink = TLink () (p);

		pLink->m_pNext = NULL;
		pLink->m_pPrev = m_pTail;
		
		if (m_pTail)
			m_pTail->m_pNext = pLink;
		else
			m_pHead = pLink;

		m_pTail = pLink;
		m_Count++;

		return CIterator::FromLink (pLink);
	}

	CIterator 
	InsertBefore (
		T* p, 
		CIterator Before
		)
	{ 
		if (!Before)
			return InsertTail (p);

		TListLink* pLink = TLink () (p);
		TListLink* pBeforeLink = Before.GetLink ();
		TListLink* pPrev = pBeforeLink->m_pPrev;

		pLink->m_pNext = pBeforeLink;
		pLink->m_pPrev = pPrev;
		Before->m_pPrev = pLink;

		if (pPrev)
			pPrev->m_pNext = pLink;
		else
			m_pHead = pLink;

		m_Count++;

		return CIterator::FromLink (pLink);
	}

	CIterator 
	InsertAfter (
		T* p, 
		CIterator After
		)
	{ 
		if (!After)
			return InsertHead (p);

		TListLink* pLink = TLink () (p);
		TListLink* pAfterLink = After.GetLink ();
		TListLink* pNext = pAfterLink->m_pNext;

		pLink->m_pPrev = pAfterLink;
		pLink->m_pNext = pNext;
		After->m_pNext = pLink;

		if (pNext)
			pNext->m_pPrev = pLink;
		else
			m_pTail = pLink;

		m_Count++;

		return CIterator::FromLink (pLink);
	}

	void 
	InsertListHead (CListBaseT* pSrc)
	{
		if (pSrc->IsEmpty ())
			return;

		if (IsEmpty ())
		{
			TakeOver (pSrc);
			return;
		}

		m_pHead->m_pPrev = pSrc->m_pTail;
		pSrc->m_pTail->m_pNext = m_pHead;
		m_pHead = pSrc->m_pHead;
		m_Count += pSrc->m_Count;

		pSrc->m_pHead = NULL;
		pSrc->m_pTail = NULL;
		pSrc->m_Count = 0;
	}

	void 
	InsertListTail (CListBaseT* pSrc)
	{
		if (pSrc->IsEmpty ())
			return;

		if (IsEmpty ())
		{
			TakeOver (pSrc);
			return;
		}

		m_pTail->m_pNext = pSrc->m_pHead;
		pSrc->m_pHead->m_pPrev = m_pTail;
		m_pTail = pSrc->m_pTail;
		m_Count += pSrc->m_Count;

		pSrc->m_pHead = NULL;
		pSrc->m_pTail = NULL;
		pSrc->m_Count = 0;
	}

	void 
	MoveBefore (
		CIterator It, 
		CIterator Before
		)
	{ 
		T* p = *It;
		Remove (It);
		InsertBefore (p, Before);
	}

	void 
	MoveAfter (
		CIterator It, 
		CIterator After
		)
	{ 
		T* p = *It;
		Remove (It);
		InsertAfter (p, pAfter);
	}

	void 
	MoveToHead (CIterator It)
	{
		T* p = *It;
		Remove (It);
		InsertHead (p);
	}

	void 
	MoveToTail (CIterator It)
	{
		T* p = *It;
		Remove (It);
		InsertTail (p);
	}

	void 
	TakeOver (CListBaseT* pList)
	{
		Clear ();
		
		m_pHead = pList->m_pHead;
		m_pTail = pList->m_pTail;
		m_Count = pList->m_Count;

		pList->Construct ();
	}

protected:
	void
	Construct ()
	{
		m_pHead = NULL;
		m_pTail = NULL;
		m_Count = 0;
	}
};

//.............................................................................

template <
	typename T, 
	typename TDelete, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CListT: public CListBaseT <T, CIteratorT <T, TLink>, TDelete, TLink> 
{
public:
	void 
	Delete (CIterator It)
	{ 
		T* p = Remove (It);
		TDelete () (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CStdListT: public CListT <T, typename mem::CStdFactoryT <T>::CDelete, TLink>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CCppListT: public CListT <T, typename mem::CCppFactoryT <T>::CDelete, TLink>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CAuxListT: public CListT <T, rtl::CVoidT <T*>, TLink>
{
public:
	~CAuxListT ()
	{
		Construct ();
	}

	void
	Clear ()
	{
		Construct ();
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

