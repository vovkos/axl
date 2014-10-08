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

template <typename T>
struct ListData
{
	T* m_head;
	T* m_tail;
	size_t m_count;
};

//.............................................................................

template <
	typename T,
	typename Entry_0,
	typename GetLink_0
	>
class IteratorBase
{
public:
	typedef Entry_0 Entry;
	typedef GetLink_0 GetLink;

protected:
	Entry* m_p;

public:
	IteratorBase ()
	{
		m_p = NULL;
	}

	// operator bool () might seem more logical, but then '+' and '-' will not work

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
			m_p = getEntryFromLink (getLink ()->m_next);

		return *(T*) this;
	}

	T&
	prev ()
	{
		if (m_p)
			m_p = getEntryFromLink (getLink ()->m_prev);

		return *(T*) this;
	}

	T&
	inc (intptr_t count)
	{
		ListLink* link = getLink ();

		if (count > 0)
			for (intptr_t i = 0; i < count && link; i++)
				link = link->m_next;
		else
			for (intptr_t i = 0; i > count && link; i--)
				link = link->m_prev;

		m_p = getEntryFromLink (link);
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

	Entry*
	getEntry () const
	{
		return m_p;
	}

	ListLink*
	getLink () const
	{
		return m_p ? GetLink () (m_p) : NULL;
	}

	static
	Entry*
	getEntryFromLink (ListLink* p)
	{
		return p ? (Entry*) ((char*) p  + 1 - (size_t) GetLink () ((Entry*) 1)) : NULL;
	}

	static
	ListLink*
	getLinkFromEntry (Entry* p)
	{
		return p ? GetLink () (p) : NULL;
	}

	static
	T
	fromLink (ListLink* p)
	{
		T it;
		it.m_p = getEntryFromLink (p);
		return it;
	}
};

//.............................................................................

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class Iterator: public IteratorBase <Iterator <T, GetLink>, T, GetLink>
{
public:
	Iterator ()
	{
	}

	template <
		typename T2,
		typename GetLink2
		>
	Iterator (const Iterator <T2, GetLink2>& src)
	{
		operator = (*src);
	}

	Iterator (T* p)
	{
		this->m_p = p;
	}

	T*
	operator * () const
	{
		return this->m_p;
	}

	T*
	operator -> () const
	{
		return this->m_p;
	}

	Iterator&
	operator = (T* p)
	{
		this->m_p = p;
		return *this;
	}

	bool
	operator == (T* p) const
	{
		return this->m_p == p;
	}

	bool
	operator != (T* p) const
	{
		return !operator == (p);
	}

	T* p () const
	{
		return this->m_p;
	}
};

//.............................................................................

template <
	typename T,
	typename Iterator_0,
	typename Delete_0
	>
class ListBase: protected ListData <typename Iterator_0::Entry>
{
public:
	typedef Iterator_0 Iterator;
	typedef Delete_0 Delete;
	typedef typename Iterator::Entry Entry;
	typedef typename Iterator::GetLink GetLink;
	typedef ListData <Entry> ListData;

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
		return m_head;
	}

	Iterator
	getTail () const
	{
		return m_tail;
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
			m_head = Iterator::getEntryFromLink (next);

		if (next)
			next->m_prev = prev;
		else
			m_tail = Iterator::getEntryFromLink (prev);

		m_count--;
		return it.getEntry ();
	}

	T*
	removeHead ()
	{
		return m_head ? remove (m_head) : NULL;
	}

	T*
	removeTail ()
	{
		return m_tail ? remove (m_tail) : NULL;
	}

	void
	clear ()
	{
		if (!m_head)
			return;

		ListLink* link = GetLink () (m_head);
		while (link)
		{
			T* p = Iterator::getEntryFromLink (link);
			link = link->m_next;
			Delete () (p);
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
		ListLink* link = GetLink () (p);
		ListLink* headLink = Iterator::getLinkFromEntry (m_head);

		link->m_prev = NULL;
		link->m_next = headLink;

		if (headLink)
			headLink->m_prev = link;
		else
			m_tail = p;

		m_head = p;
		m_count++;

		return p;
	}

	Iterator
	insertTail (T* p)
	{
		ListLink* link = GetLink () (p);
		ListLink* tailLink = Iterator::getLinkFromEntry (m_tail);

		link->m_next = NULL;
		link->m_prev = tailLink;

		if (tailLink)
			tailLink->m_next = link;
		else
			m_head = p;

		m_tail = p;
		m_count++;

		return p;
	}

	Iterator
	insertBefore (
		T* p,
		Iterator before
		)
	{
		if (!before)
			return insertTail (p);

		ListLink* link = GetLink () (p);
		ListLink* beforeLink = (ListLink*) before.getLink ();
		ListLink* prev = beforeLink->m_prev;

		link->m_next = beforeLink;
		link->m_prev = prev;
		beforeLink->m_prev = link;

		if (prev)
			prev->m_next = link;
		else
			m_head = p;

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

		ListLink* link = GetLink () (p);
		ListLink* afterLink = (ListLink*) after.getLink ();
		ListLink* next = afterLink->m_next;

		link->m_prev = afterLink;
		link->m_next = next;
		afterLink->m_next = link;

		if (next)
			next->m_prev = link;
		else
			m_tail = p;

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

		ListLink* headLink = GetLink () (m_head);
		ListLink* srcTailLink = GetLink () (src->m_tail);

		headLink->m_prev = srcTailLink;
		srcTailLink->m_next = headLink;
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

		ListLink* tailLink = GetLink () (m_tail);
		ListLink* srcHeadLink = GetLink () (src->m_head);

		tailLink->m_next = srcHeadLink;
		srcHeadLink->m_prev = tailLink;
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
	typename Iterator_0
	>
class ConstListBase
{
public:
	typedef Iterator_0 Iterator;
	typedef typename Iterator::Entry Entry;
	typedef typename Iterator::GetLink GetLink;
	typedef ListData <Entry> ListData;

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
		return m_listData ? m_listData->m_head : NULL;
	}

	Iterator
	getTail () const
	{
		return m_listData ? m_listData->m_tail : NULL;
	}
};

//.............................................................................

template <
	typename T,
	typename Delete,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class List: public ListBase <T, Iterator <T, GetLink>, Delete>
{
public:
	void
	erase (typename List::Iterator it)
	{
		T* p = this->remove (it);
		typename List::Delete () (p);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class StdList: public List <T, typename mem::StdFactory <T>::Delete, GetLink>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class CppList: public List <T, typename mem::CppFactory <T>::Delete, GetLink>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class AuxList: public List <T, rtl::Void <T*>,  GetLink>
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
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class ConstList: public ConstListBase <T, Iterator <T, GetLink> >
{
public:
	ConstList ()
	{
	}

	template <typename Delete>
	ConstList (const List <T, Delete, GetLink>& list)
	{
		this->m_listData = list.getListData ();
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

