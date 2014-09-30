// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	TListLink* m_next;
	TListLink* m_prev;
};

struct TList
{
	TListLink* m_head;
	TListLink* m_tail;
	size_t m_count;
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
		return next ();
	}

	T& 
	operator -- ()
	{ 
		return prev ();
	}

	T
	operator ++ (int) // post increment
	{ 
		T old = *(T*) this;
		next ();
		return old;
	}

	T
	operator -- (int) // post decrement
	{ 
		T old = *(T*) this;
		prev ();
		return old;
	}

	T& 
	operator += (size_t count)
	{ 
		return inc (count); 
	}

	T& 
	operator -= (size_t count)
	{ 
		return inc (-count); 
	}

	T
	operator + (size_t count) const
	{ 
		return getInc (count); 
	}

	T
	operator - (size_t count) const
	{ 
		return getInc (-count); 
	}

	T& 
	next ()
	{	
		if (m_p) 
			m_p = m_p->m_next; 

		return *(T*) this;
	}

	T& 
	prev ()
	{	
		if (m_p) 
			m_p = m_p->m_prev; 

		return *(T*) this;
	}

	T& 
	inc (intptr_t count)
	{	
		intptr_t i;

		if (count > 0)
			for (i = 0; i < count && m_p; i++)
				m_p = m_p->m_next;
		else
			for (i = 0; i > count && m_p; i--)
				m_p = m_p->m_prev;

		return *(T*) this;
	}

	T 
	getNext () const
	{ 
		return CIteratorBaseT (*this).next (); 
	}

	T 
	getPrev () const
	{ 
		return CIteratorBaseT (*this).prev (); 
	}

	T 
	getInc (intptr_t count) const
	{ 
		return CIteratorBaseT (*this).inc (count); 
	}

	const TListLink* 
	getLink () const
	{ 
		return m_p; 
	}

	static 
	T
	fromLink (const TListLink* p)
	{ 
		T it;
		it.m_p = p;
		return it;
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
	CIteratorT (const CIteratorT <T2, TLink2>& src)
	{ 
		operator = (*src); 
	}

	CIteratorT (T* p)
	{ 
		operator = (p); 
	}

	T*
	operator * () const
	{ 
		return getObject (); 
	}

	T* 
	operator -> () const
	{ 
		return getObject (); 
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
	getEntry () const
	{
		return getEntryFromLink (this->m_p); 
	}

	static
	T*
	getEntryFromLink (const TListLink* p)
	{ 
		return getObjectFromLink (p);
	}

	T* 
	getObject () const
	{ 
		return getObjectFromLink (this->m_p); 
	}

	static 
	T*
	getObjectFromLink (const TListLink* p)
	{ 
		size_t offset = (size_t) CLink () ((T*) 1) - 1;
		return p ? (T*) ((uchar_t*) p - offset) : NULL; 
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
	typedef TDelete COperatorDelete;
	typedef typename CIterator::CLink CLink;

public:
	CListBaseT ()
	{ 
		construct ();
	}
	
	~CListBaseT ()
	{ 
		clear (); 
	}

	bool 
	isEmpty () const
	{ 
		return m_head == NULL; 
	}

	const TList*
	getList () const
	{
		return this;
	}

	size_t 
	getCount () const
	{ 
		return m_count; 
	}

	CIterator 
	getHead () const
	{ 
		return CIterator::fromLink (m_head); 
	}

	CIterator 
	getTail () const
	{ 
		return CIterator::fromLink (m_tail); 
	}

	T* 
	remove (CIterator it)
	{ 
		TListLink* link = (TListLink*) it.getLink ();
		TListLink* next = link->m_next;
		TListLink* prev = link->m_prev;

		if (prev)
			prev->m_next = next;
		else
			m_head = next;
		
		if (next)
			next->m_prev = prev;
		else
			m_tail = prev;

		m_count--;
		return it.getEntry ();
	}

	T* 
	removeHead ()
	{ 
		return m_head ? remove (CIterator::fromLink (m_head)) : NULL; 
	}

	T* 
	removeTail ()
	{ 
		return m_tail ? remove (CIterator::fromLink (m_tail)) : NULL; 
	}

	void 
	clear ()
	{ 
		TListLink* link = m_head;
		while (link)
		{
			T* p = CIterator::getEntryFromLink (link);
			link = link->m_next;
			COperatorDelete () (p);
		}

		construct ();
	}

	bool 
	clearButHead ()
	{ 
		if (isEmpty ())
			return false;

		T* p = removeHead ();
		clear ();
		insertHead (p);
		return true;
	}

	bool 
	clearButTail ()
	{ 
		if (isEmpty ())
			return false;

		T* p = removeTail ();
		clear ();
		insertTail (p);
		return true;
	}

	CIterator 
	insertHead (T* p)
	{ 
		TListLink* link = CLink () (p);

		link->m_prev = NULL;
		link->m_next = m_head;

		if (m_head)
			m_head->m_prev = link;
		else
			m_tail = link;

		m_head = link;
		m_count++;

		return CIterator::fromLink (link);
	}

	CIterator 
	insertTail (T* p)
	{ 
		TListLink* link = CLink () (p);

		link->m_next = NULL;
		link->m_prev = m_tail;
		
		if (m_tail)
			m_tail->m_next = link;
		else
			m_head = link;

		m_tail = link;
		m_count++;

		return CIterator::fromLink (link);
	}

	CIterator 
	insertBefore (
		T* p, 
		CIterator before
		)
	{ 
		if (!before)
			return insertTail (p);

		TListLink* link = CLink () (p);
		TListLink* beforeLink = (TListLink*) before.getLink ();
		TListLink* prev = beforeLink->m_prev;

		link->m_next = beforeLink;
		link->m_prev = prev;
		beforeLink->m_prev = link;

		if (prev)
			prev->m_next = link;
		else
			m_head = link;

		m_count++;

		return CIterator::fromLink (link);
	}

	CIterator 
	insertAfter (
		T* p, 
		CIterator after
		)
	{ 
		if (!after)
			return insertHead (p);

		TListLink* link = CLink () (p);
		TListLink* afterLink = (TListLink*) after.getLink ();
		TListLink* next = afterLink->m_next;

		link->m_prev = afterLink;
		link->m_next = next;
		afterLink->m_next = link;

		if (next)
			next->m_prev = link;
		else
			m_tail = link;

		m_count++;

		return CIterator::fromLink (link);
	}

	void 
	insertListHead (CListBaseT* src)
	{
		if (src->isEmpty ())
			return;

		if (isEmpty ())
		{
			takeOver (src);
			return;
		}

		m_head->m_prev = src->m_tail;
		src->m_tail->m_next = m_head;
		m_head = src->m_head;
		m_count += src->m_count;

		src->m_head = NULL;
		src->m_tail = NULL;
		src->m_count = 0;
	}

	void 
	insertListTail (CListBaseT* src)
	{
		if (src->isEmpty ())
			return;

		if (isEmpty ())
		{
			takeOver (src);
			return;
		}

		m_tail->m_next = src->m_head;
		src->m_head->m_prev = m_tail;
		m_tail = src->m_tail;
		m_count += src->m_count;

		src->m_head = NULL;
		src->m_tail = NULL;
		src->m_count = 0;
	}

	void 
	moveBefore (
		CIterator it, 
		CIterator before
		)
	{ 
		T* p = *it;
		remove (it);
		insertBefore (p, before);
	}

	void 
	moveAfter (
		CIterator it, 
		CIterator after
		)
	{ 
		T* p = *it;
		remove (it);
		insertAfter (p, after);
	}

	void 
	moveToHead (CIterator it)
	{
		T* p = *it;
		remove (it);
		insertHead (p);
	}

	void 
	moveToTail (CIterator it)
	{
		T* p = *it;
		remove (it);
		insertTail (p);
	}

	void 
	takeOver (CListBaseT* list)
	{
		clear ();
		
		m_head = list->m_head;
		m_tail = list->m_tail;
		m_count = list->m_count;

		list->construct ();
	}

protected:
	void
	construct ()
	{
		m_head = NULL;
		m_tail = NULL;
		m_count = 0;
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
	const TList* m_list;

public:
	CConstListBaseT ()
	{ 
		m_list = NULL;
	}

	bool 
	isEmpty () const
	{ 
		return m_list ? m_list->m_head == NULL : true;  
	}

	size_t 
	getCount () const
	{ 
		return m_list ? m_list->m_count : 0; 
	}

	CIterator 
	getHead () const
	{ 
		return m_list ? CIterator::fromLink (m_list->m_head) : CIterator ();  
	}

	CIterator 
	getTail () const
	{ 
		return m_list ? CIterator::fromLink (m_list->m_tail) : CIterator (); 
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
	erase (typename CListT::CIterator it)
	{ 
		T* p = this->remove (it); 
		typename CListT::COperatorDelete () (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CStdListT: public CListT <T, typename mem::CStdFactoryT <T>::COperatorDelete, TLink>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T, 
	typename TLink = CImplicitCastT <T*, TListLink*> 
	>
class CCppListT: public CListT <T, typename mem::CCppFactoryT <T>::COperatorDelete, TLink>
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
	CAuxListT ()
	{
		this->construct (); 
	}

	void
	clear ()
	{
		this->construct (); 
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
	CConstListT (const CListT <T, TDelete, TLink>& list)
	{ 
		this->m_list = list.getList (); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

