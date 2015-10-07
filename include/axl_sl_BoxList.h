// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_BOXLIST_H

#include "axl_sl_List.h"

namespace axl {
namespace sl {

//.............................................................................

template <typename T>
class BoxListEntry: public ListLink
{
public:
	T m_value;

public:
	BoxListEntry ()
	{
	}

	BoxListEntry (const T& value)
	{
		m_value = value;
	}
};

//.............................................................................

template <typename T>
class BoxIterator: public IteratorBase <
	BoxIterator <T>,
	BoxListEntry <T>,
	ImplicitCast <BoxListEntry <T>*, ListLink*>
	>
{
public:
	BoxIterator ()
	{
	}

	BoxIterator (BoxListEntry <T>* p)
	{
		this->m_p = p;
	}

	T&
	operator * () const
	{
		return r ();
	}

	T*
	operator -> () const
	{
		return p ();
	}

	T&
	r () const
	{
		ASSERT (this->m_p);
		return *p ();
	}

	T*
	p () const
	{
		return this->m_p ? &this->m_p->m_value : NULL;
	}
};

//.............................................................................

template <
	typename T,
	typename InsertArg = T
	>
class BoxList: public ListBase <
	BoxListEntry <T>,
	BoxIterator <T>,
	typename mem::StdDelete <BoxListEntry <T> >
	>
{
public:
	typedef ListBase <
		BoxListEntry <T>,
		BoxIterator <T>,
		typename mem::StdDelete <BoxListEntry <T> >
		> BaseType;

	typedef typename BaseType::Entry Entry;
	typedef typename BaseType::Iterator Iterator;

public:
	template <typename L>
	void
	copy (const L& list)
	{
		clear ();

		typename L::Iterator it = list.getHead ();
		for (; it; it++)
			insertTail (*it);
	}

	T
	remove (Iterator it)
	{
		T value = *it;
		Entry* entry = it.getEntry ();
		BaseType::remove (entry);
		AXL_MEM_DELETE (entry);
		return value;
	}

	T
	removeHead ()
	{
		return this->m_head ? remove (this->m_head) : T ();
	}

	T
	removeTail ()
	{
		return this->m_tail ? remove (this->m_tail) : T ();
	}

	Iterator
	insertHead (InsertArg value)
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_value = value;
		return insertHeadEntry (entry);
	}

	Iterator
	insertTail (InsertArg value)
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_value = value;
		return insertTailEntry (entry);
	}

	Iterator
	insertBefore (
		InsertArg value,
		Iterator before
		)
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_value = value;
		return insertBeforeEntry (entry, before);
	}

	Iterator
	insertAfter (
		InsertArg value,
		Iterator after
		)
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_value = value;
		return insertAfterEntry (entry, after);
	}

	// empty element insertion

	Iterator
	insertHead ()
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		return insertHeadEntry (entry);
	}

	Iterator
	insertTail ()
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		return insertTailEntry (entry);
	}

	Iterator
	insertBefore (Iterator before)
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		return insertBeforeEntry (entry, before);
	}

	Iterator
	insertAfter (Iterator after)
	{
		Entry* entry = AXL_MEM_NEW (Entry);
		return insertAfterEntry (entry, after);
	}

	// direct access to entries

	Entry*
	removeEntry (Iterator it)
	{
		return BaseType::remove (it);
	}

	Entry*
	removeHeadEntry ()
	{
		return BaseType::removeHead ();
	}

	Entry*
	removeTailEntry ()
	{
		return BaseType::removeTail ();
	}

	Iterator
	insertHeadEntry (Entry* entry)
	{
		return BaseType::insertHead (entry);
	}

	Iterator
	insertTailEntry (Entry* entry)
	{
		return BaseType::insertTail (entry);
	}

	Iterator
	insertBeforeEntry (
		Entry* entry,
		Iterator before
		)
	{
		return BaseType::insertBefore (entry, before);
	}

	Iterator
	insertAfterEntry (
		Entry* entry,
		Iterator after
		)
	{
		return BaseType::insertAfter (entry, after);
	}
};

//.............................................................................

template <typename T>
class ConstBoxList: public ConstListBase <T, BoxIterator <T> >
{
public:
	ConstBoxList ()
	{
	}

	template <typename InsertArg>
	ConstBoxList (const BoxList <T, InsertArg>& list)
	{
		this->m_listData = list.getListData ();
	}

	template <typename Delete>
	ConstBoxList (const List <BoxListEntry <T>, Delete>& list)
	{
		this->m_listData = list.getListData ();
	}
};

//.............................................................................

} // namespace sl
} // namespace axl
