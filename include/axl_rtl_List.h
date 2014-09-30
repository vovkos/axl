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

struct ListLink
{
	ListLink* m_next;
	ListLink* m_prev;
};

struct ListData
{
	ListLink* m_head;
	ListLink* m_tail;
	size_t m_count;
};

//.............................................................................

template <typename T>
class IteratorBase
{
protected:
	const ListLink* m_p;

public:
	IteratorBase ()
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
		return IteratorBase (*this).next (); 
	}

	T 
	getPrev () const
	{ 
		return IteratorBase (*this).prev (); 
	}

	T 
	getInc (intptr_t count) const
	{ 
		return IteratorBase (*this).inc (count); 
	}

	const ListLink* 
	getLink () const
	{ 
		return m_p; 
	}

	static 
	T
	fromLink (const ListLink* p)
	{ 
		T it;
		it.m_p = p;
		return it;
	}
};

//.............................................................................

template <
	typename T, 
	typename Link = ImplicitCast <T*, ListLink*> 
	>
class Iterator: public IteratorBase <Iterator <T, Link> >
{
public:
	typedef Link Link;

public:
	Iterator ()
	{ 
	}

	template <
		typename T2, 
		typename Link2
		>
	Iterator (const Iterator <T2, Link2>& src)
	{ 
		operator = (*src); 
	}

	Iterator (T* p)
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

	Iterator&
	operator = (T* p)
	{
		this->m_p = p ? Link () (p) : NULL; // thanks a lot gcc
		return *this;
	}

	bool 
	operator == (T* p) const
	{ 
		return this->m_p == (p ? Link () (p) : NULL); 
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
	getEntryFromLink (const ListLink* p)
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
	getObjectFromLink (const ListLink* p)
	{ 
		size_t offset = (size_t) Link () ((T*) 1) - 1;
		return p ? (T*) ((uchar_t*) p - offset) : NULL; 
	}
};

//.............................................................................

template <
	typename T, 
	typename Iterator, 
	typename Delete
	>
class ListBase: protected ListData
{
public:
	typedef Iterator Iterator;
	typedef Delete OperatorDelete;
	typedef typename Iterator::Link Link;

public:
	ListBase ()
	{ 
		construct ();
	}
	
	~ListBase ()
	{ 
		clear (); 
	}

	bool 
	isEmpty () const
	{ 
		return m_head == NULL; 
	}

	const ListData*
	getListData () const
	{
		return this;
	}

	size_t 
	getCount () const
	{ 
		return m_count; 
	}

	Iterator 
	getHead () const
	{ 
		return Iterator::fromLink (m_head); 
	}

	Iterator 
	getTail () const
	{ 
		return Iterator::fromLink (m_tail); 
	}

	T* 
	remove (Iterator it)
	{ 
		ListLink* link = (ListLink*) it.getLink ();
		ListLink* next = link->m_next;
		ListLink* prev = link->m_prev;

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
		return m_head ? remove (Iterator::fromLink (m_head)) : NULL; 
	}

	T* 
	removeTail ()
	{ 
		return m_tail ? remove (Iterator::fromLink (m_tail)) : NULL; 
	}

	void 
	clear ()
	{ 
		ListLink* link = m_head;
		while (link)
		{
			T* p = Iterator::getEntryFromLink (link);
			link = link->m_next;
			OperatorDelete () (p);
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

	Iterator 
	insertHead (T* p)
	{ 
		ListLink* link = Link () (p);

		link->m_prev = NULL;
		link->m_next = m_head;

		if (m_head)
			m_head->m_prev = link;
		else
			m_tail = link;

		m_head = link;
		m_count++;

		return Iterator::fromLink (link);
	}

	Iterator 
	insertTail (T* p)
	{ 
		ListLink* link = Link () (p);

		link->m_next = NULL;
		link->m_prev = m_tail;
		
		if (m_tail)
			m_tail->m_next = link;
		else
			m_head = link;

		m_tail = link;
		m_count++;

		return Iterator::fromLink (link);
	}

	Iterator 
	insertBefore (
		T* p, 
		Iterator before
		)
	{ 
		if (!before)
			return insertTail (p);

		ListLink* link = Link () (p);
		ListLink* beforeLink = (ListLink*) before.getLink ();
		ListLink* prev = beforeLink->m_prev;

		link->m_next = beforeLink;
		link->m_prev = prev;
		beforeLink->m_prev = link;

		if (prev)
			prev->m_next = link;
		else
			m_head = link;

		m_count++;

		return Iterator::fromLink (link);
	}

	Iterator 
	insertAfter (
		T* p, 
		Iterator after
		)
	{ 
		if (!after)
			return insertHead (p);

		ListLink* link = Link () (p);
		ListLink* afterLink = (ListLink*) after.getLink ();
		ListLink* next = afterLink->m_next;

		link->m_prev = afterLink;
		link->m_next = next;
		afterLink->m_next = link;

		if (next)
			next->m_prev = link;
		else
			m_tail = link;

		m_count++;

		return Iterator::fromLink (link);
	}

	void 
	insertListHead (ListBase* src)
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
	insertListTail (ListBase* src)
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
		Iterator it, 
		Iterator before
		)
	{ 
		T* p = *it;
		remove (it);
		insertBefore (p, before);
	}

	void 
	moveAfter (
		Iterator it, 
		Iterator after
		)
	{ 
		T* p = *it;
		remove (it);
		insertAfter (p, after);
	}

	void 
	moveToHead (Iterator it)
	{
		T* p = *it;
		remove (it);
		insertHead (p);
	}

	void 
	moveToTail (Iterator it)
	{
		T* p = *it;
		remove (it);
		insertTail (p);
	}

	void 
	takeOver (ListBase* list)
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
	typename Iterator
	>
class ConstListBase
{
public:
	typedef Iterator Iterator;
	typedef typename Iterator::Link Link;

protected:
	const ListData* m_listData;

public:
	ConstListBase ()
	{ 
		m_listData = NULL;
	}

	bool 
	isEmpty () const
	{ 
		return m_listData ? m_listData->m_head == NULL : true;  
	}

	size_t 
	getCount () const
	{ 
		return m_listData ? m_listData->m_count : 0; 
	}

	Iterator 
	getHead () const
	{ 
		return m_listData ? Iterator::fromLink (m_listData->m_head) : Iterator ();  
	}

	Iterator 
	getTail () const
	{ 
		return m_listData ? Iterator::fromLink (m_listData->m_tail) : Iterator (); 
	}
};

//.............................................................................

template <
	typename T, 
	typename Delete, 
	typename Link = ImplicitCast <T*, ListLink*> 
	>
class List: public ListBase <T, Iterator <T, Link>, Delete> 
{
public:
	void 
	erase (typename List::Iterator it)
	{ 
		T* p = this->remove (it); 
		typename List::OperatorDelete () (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


template <
	typename T, 
	typename Link = ImplicitCast <T*, ListLink*> 
	>
class StdList: public List <T, typename mem::StdFactory <T>::OperatorDelete, Link>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T, 
	typename Link = ImplicitCast <T*, ListLink*> 
	>
class CppList: public List <T, typename mem::CppFactory <T>::OperatorDelete, Link>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T, 
	typename Link = ImplicitCast <T*, ListLink*> 
	>
class AuxList: public List <T, rtl::Void <T*>,  Link>
{
public:
	AuxList ()
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
	typename Link = ImplicitCast <T*, ListLink*> 
	>
class ConstList: public ConstListBase <T, Iterator <T, Link> > 
{
public:
	ConstList ()
	{ 
	}

	template <typename Delete>
	ConstList (const List <T, Delete, Link>& list)
	{ 
		this->m_listData = list.getListData (); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

