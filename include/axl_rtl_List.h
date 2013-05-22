// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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

struct TList
{
	TListLink* m_pHead;
	TListLink* m_pTail;
	size_t m_Count;
};

//.............................................................................

template <typename T>
class CIteratorBaseT
{
protected:
	const TListLink* m_p;

public:
	CIteratorBaseT ()
	{ 
		m_p = NULL; 
	}

	operator const void* () const
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

	const TListLink* 
	GetLink () const
	{ 
		return m_p; 
	}

	static 
	T
	FromLink (const TListLink* p)
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
	typedef TLink CLink;

public:
	CIteratorT ()
	{ 
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
		this->m_p = p ? CLink () (p) : NULL; // thanks a lot gcc
		return *this;
	}

	bool 
	operator == (T* p) const
	{ 
		return this->m_p == (p ? CLink () (p) : NULL); 
	}

	bool 
	operator != (T* p) const
	{ 
		return !operator == (p); 
	}

	T*
	GetEntry () const
	{
		return GetEntryFromLink (this->m_p); 
	}

	static
	T*
	GetEntryFromLink (const TListLink* p)
	{ 
		return GetObjectFromLink (p);
	}

	T* 
	GetObject () const
	{ 
		return GetObjectFromLink (this->m_p); 
	}

	static 
	T*
	GetObjectFromLink (const TListLink* p)
	{ 
		size_t Offset = (size_t) CLink () ((T*) 1) - 1;
		return p ? (T*) ((uchar_t*) p - Offset) : NULL; 
	}
};

//.............................................................................

template <
	typename T, 
	typename TIterator, 
	typename TDelete
	>
class CListBaseT: protected TList
{
public:
	typedef TIterator CIterator;
	typedef TDelete CDelete;
	typedef typename CIterator::CLink CLink;

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

	const TList*
	GetList () const
	{
		return this;
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
		TListLink* pLink = (TListLink*) It.GetLink ();
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
		TListLink* pLink = m_pHead;
		while (pLink)
		{
			T* p = CIterator::GetEntryFromLink (pLink);
			pLink = pLink->m_pNext;
			CDelete () (p);
		}

		Construct ();
	}

	bool 
	ClearButHead ()
	{ 
		if (IsEmpty ())
			return false;

		T* p = RemoveHead ();
		Clear ();
		InsertHead (p);
		return true;
	}

	bool 
	ClearButTail ()
	{ 
		if (IsEmpty ())
			return false;

		T* p = RemoveTail ();
		Clear ();
		InsertTail (p);
		return true;
	}

	CIterator 
	InsertHead (T* p)
	{ 
		TListLink* pLink = CLink () (p);

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
		TListLink* pLink = CLink () (p);

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

		TListLink* pLink = CLink () (p);
		TListLink* pBeforeLink = (TListLink*) Before.GetLink ();
		TListLink* pPrev = pBeforeLink->m_pPrev;

		pLink->m_pNext = pBeforeLink;
		pLink->m_pPrev = pPrev;
		pBeforeLink->m_pPrev = pLink;

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

		TListLink* pLink = CLink () (p);
		TListLink* pAfterLink = (TListLink*) After.GetLink ();
		TListLink* pNext = pAfterLink->m_pNext;

		pLink->m_pPrev = pAfterLink;
		pLink->m_pNext = pNext;
		pAfterLink->m_pNext = pLink;

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
		InsertAfter (p, After);
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
	typename TIterator
	>
class CConstListBaseT
{
public:
	typedef TIterator CIterator;
	typedef typename CIterator::CLink CLink;

protected:
	const TList* m_pList;

public:
	CConstListBaseT ()
	{ 
		m_pList = NULL;
	}

	bool 
	IsEmpty () const
	{ 
		return m_pList ? m_pList->m_pHead == NULL : true;  
	}

	size_t 
	GetCount () const
	{ 
		return m_pList ? m_pList->m_Count : 0; 
	}

	CIterator 
	GetHead () const
	{ 
		return m_pList ? CIterator::FromLink (m_pList->m_pHead) : CIterator ();  
	}

	CIterator 
	GetTail () const
	{ 
		return m_pList ? CIterator::FromLink (m_pList->m_pTail) : CIterator (); 
	}
};

//.............................................................................

template <
	typename T, 
	typename TDelete, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CListT: public CListBaseT <T, CIteratorT <T, TLink>, TDelete> 
{
public:
	void 
	Delete (typename CListT::CIterator It)
	{ 
		T* p = this->Remove (It); 
		typename CListT::CDelete () (p); 
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
class CAuxListT: public CListT <T, rtl::CVoidT <T*>,  TLink>
{
public:
	~CAuxListT ()
	{
		this->Construct (); 
	}

	void
	Clear ()
	{
		this->Construct (); 
	}
};

//.............................................................................

template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CConstListT: public CConstListBaseT <T, CIteratorT <T, TLink> > 
{
public:
	CConstListT ()
	{ 
	}

	template <typename TDelete>
	CConstListT (const CListT <T, TDelete, TLink>& List)
	{ 
		this->m_pList = List.GetList (); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

