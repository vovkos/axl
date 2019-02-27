//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_SL_LIST_H

#include "axl_sl_Iterator.h"
#include "axl_sl_Construct.h"
#include "axl_mem_New.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
struct ListData
{
	T* m_head;
	T* m_tail;
	size_t m_count;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink0,
	typename Iterator0,
	typename ConstIterator0,
	typename Delete0
	>
class ListBase: protected ListData<T>
{
public:
	typedef T Entry;
	typedef GetLink0 GetLink;
	typedef Iterator0 Iterator;
	typedef ConstIterator0 ConstIterator;
	typedef Delete0 Delete;
	typedef sl::ListData<T> ListData;

public:
	ListBase()
	{
		construct();
	}

	~ListBase()
	{
		clear();
	}

	bool
	isEmpty() const
	{
		return this->m_head == NULL;
	}

	const ListData*
	getListData() const
	{
		return this;
	}

	size_t
	getCount() const
	{
		return this->m_count;
	}

	Iterator
	getHead()
	{
		return this->m_head;
	}

	ConstIterator
	getHead() const
	{
		return this->m_head;
	}

	Iterator
	getTail()
	{
		return this->m_tail;
	}

	ConstIterator
	getTail() const
	{
		return this->m_tail;
	}

	T*
	remove(Iterator it)
	{
		ListLink* link = it.getLink();
		ListLink* next = link->m_next;
		ListLink* prev = link->m_prev;

		if (prev)
			prev->m_next = next;
		else
			this->m_head = Iterator::getEntryFromLink(next);

		if (next)
			next->m_prev = prev;
		else
			this->m_tail = Iterator::getEntryFromLink(prev);

		this->m_count--;
		return it.getEntry();
	}

	T*
	removeHead()
	{
		return this->m_head ? remove(this->m_head) : NULL;
	}

	T*
	removeTail()
	{
		return this->m_tail ? remove(this->m_tail) : NULL;
	}

	void
	clear()
	{
		if (!this->m_head)
			return;

		ListLink* link = GetLink() (this->m_head);
		while (link)
		{
			T* p = Iterator::getEntryFromLink(link);
			link = link->m_next;
			Delete() (p);
		}

		construct();
	}

	bool
	clearButHead()
	{
		if (isEmpty())
			return false;

		T* p = removeHead();
		clear();
		insertHead(p);
		return true;
	}

	bool
	clearButTail()
	{
		if (isEmpty())
			return false;

		T* p = removeTail();
		clear();
		insertTail(p);
		return true;
	}

	Iterator
	insertHead(T* p)
	{
		ListLink* link = GetLink() (p);
		ListLink* headLink = Iterator::getLinkFromEntry(this->m_head);

		link->m_prev = NULL;
		link->m_next = headLink;

		if (headLink)
			headLink->m_prev = link;
		else
			this->m_tail = p;

		this->m_head = p;
		this->m_count++;

		return p;
	}

	Iterator
	insertTail(T* p)
	{
		ListLink* link = GetLink() (p);
		ListLink* tailLink = Iterator::getLinkFromEntry(this->m_tail);

		link->m_next = NULL;
		link->m_prev = tailLink;

		if (tailLink)
			tailLink->m_next = link;
		else
			this->m_head = p;

		this->m_tail = p;
		this->m_count++;

		return p;
	}

	Iterator
	insertBefore(
		T* p,
		Iterator before
		)
	{
		if (!before)
			return insertTail(p);

		ListLink* link = GetLink() (p);
		ListLink* beforeLink = before.getLink();
		ListLink* prev = beforeLink->m_prev;

		link->m_next = beforeLink;
		link->m_prev = prev;
		beforeLink->m_prev = link;

		if (prev)
			prev->m_next = link;
		else
			this->m_head = p;

		this->m_count++;

		return Iterator::fromLink(link);
	}

	Iterator
	insertAfter(
		T* p,
		Iterator after
		)
	{
		if (!after)
			return insertHead(p);

		ListLink* link = GetLink() (p);
		ListLink* afterLink = after.getLink();
		ListLink* next = afterLink->m_next;

		link->m_prev = afterLink;
		link->m_next = next;
		afterLink->m_next = link;

		if (next)
			next->m_prev = link;
		else
			this->m_tail = p;

		this->m_count++;

		return Iterator::fromLink(link);
	}

	void
	insertListHead(ListBase* src)
	{
		if (src->isEmpty())
			return;

		if (isEmpty())
		{
			sl::takeOver(this, src);
			return;
		}

		ListLink* headLink = GetLink() (this->m_head);
		ListLink* srcTailLink = GetLink() (src->m_tail);

		headLink->m_prev = srcTailLink;
		srcTailLink->m_next = headLink;
		this->m_head = src->m_head;
		this->m_count += src->m_count;

		src->m_head = NULL;
		src->m_tail = NULL;
		src->m_count = 0;
	}

	void
	insertListTail(ListBase* src)
	{
		if (src->isEmpty())
			return;

		if (isEmpty())
		{
			sl::takeOver(this, src);
			return;
		}

		ListLink* tailLink = GetLink() (this->m_tail);
		ListLink* srcHeadLink = GetLink() (src->m_head);

		tailLink->m_next = srcHeadLink;
		srcHeadLink->m_prev = tailLink;
		this->m_tail = src->m_tail;
		this->m_count += src->m_count;

		src->m_head = NULL;
		src->m_tail = NULL;
		src->m_count = 0;
	}

	void
	moveBefore(
		Iterator it,
		Iterator before
		)
	{
		T* p = it.getEntry();
		remove(it);
		insertBefore(p, before);
	}

	void
	moveAfter(
		Iterator it,
		Iterator after
		)
	{
		T* p = it.getEntry();
		remove(it);
		insertAfter(p, after);
	}

	void
	moveToHead(Iterator it)
	{
		T* p = it.getEntry();
		remove(it);
		insertHead(p);
	}

	void
	moveToTail(Iterator it)
	{
		T* p = it.getEntry();
		remove(it);
		insertTail(p);
	}

protected:
	void
	construct()
	{
		this->m_head = NULL;
		this->m_tail = NULL;
		this->m_count = 0;
	}
};

//..............................................................................

template <
	typename T,
	typename GetLink0,
	typename Iterator0
	>
class ConstListBase
{
public:
	typedef T Entry;
	typedef GetLink0 GetLink;
	typedef Iterator0 Iterator;
	typedef Iterator0 ConstIterator;
	typedef sl::ListData<T> ListData;

protected:
	const ListData* m_listData;

public:
	ConstListBase()
	{
		m_listData = NULL;
	}

	bool
	isEmpty() const
	{
		return m_listData ? m_listData->m_head == NULL : true;
	}

	size_t
	getCount() const
	{
		return m_listData ? m_listData->m_count : 0;
	}

	ConstIterator
	getHead() const
	{
		return m_listData ? m_listData->m_head : NULL;
	}

	ConstIterator
	getTail() const
	{
		return m_listData ? m_listData->m_tail : NULL;
	}
};

//..............................................................................

template <
	typename T,
	typename GetLink = ImplicitPtrCast<T, ListLink>,
	typename Delete = mem::StdDelete<T>
	>
class List: public ListBase<
	T,
	GetLink,
	Iterator<T, GetLink>,
	ConstIterator<T, GetLink>,
	Delete
	>
{
public:
	void
	erase(typename List::Iterator it)
	{
		T* p = this->remove(it);
		typename List::Delete() (p);
	}

	void
	eraseHead()
	{
		T* p = this->removeHead();
		p ? typename List::Delete() (p) : (void) 0;
	}

	void
	eraseTail()
	{
		T* p = this->removeTail();
		p ? typename List::Delete() (p) : (void) 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitPtrCast<T, ListLink>
	>
class CppList: public List<T, GetLink, mem::CppDelete<T> >
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitPtrCast<T, ListLink>
	>
class AuxList: public List<T, GetLink, sl::Void<T*> >
{
public:
	void
	clear()
	{
		this->construct();
	}
};

//..............................................................................

template <
	typename T,
	typename GetLink = ImplicitPtrCast<T, ListLink>
	>
class ConstList: public ConstListBase<
	T,
	GetLink,
	ConstIterator<T, GetLink>
	>
{
public:
	ConstList()
	{
	}

	template <typename Delete>
	ConstList(const List<T, GetLink, Delete>& list)
	{
		this->m_listData = list.getListData();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
