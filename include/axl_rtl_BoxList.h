// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BOXLIST_H

#include "axl_rtl_List.h"

namespace axl {
namespace rtl {

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
class BoxIterator: public IteratorBase <BoxIterator <T> >
{
public:
	typedef ImplicitCast <BoxListEntry <T>*, ListLink*> Link;

public:
	BoxIterator ()
	{
	}

	BoxIterator (BoxListEntry <T>* p)
	{
		this->m_p = p; // thanks a lot gcc
	}

	T&
	operator * () const
	{ 
		return *getObject (); 
	}

	T* 
	operator -> () const
	{ 
		return getObject (); 
	}

	BoxListEntry <T>*
	getEntry () const
	{
		return getEntryFromLink (this->m_p);
	}

	static
	BoxListEntry <T>*
	getEntryFromLink (const ListLink* p)
	{ 
		return (BoxListEntry <T>*) p;
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
		return p ? &getEntryFromLink (p)->m_value : NULL;
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
	typename mem::StdFactory <BoxListEntry <T> >::OperatorDelete
	>
{
public:
	typedef ListBase <
		BoxListEntry <T>, 
		BoxIterator <T>,
		typename mem::StdFactory <BoxListEntry <T> >::OperatorDelete
		> ListBase;
	
	typedef BoxListEntry <T> Entry;
	typedef BoxIterator <T> Iterator;

public:
	T
	remove (Iterator it)
	{ 
		T value = *it;
		Entry* entry = it.getEntry ();
		ListBase::remove (entry);
		AXL_MEM_DELETE (entry);
		return value;
	}

	T
	removeHead ()
	{ 
		return this->m_head ? remove (Iterator::fromLink (this->m_head)) : T ();
	}

	T 
	removeTail ()
	{ 
		return this->m_tail ? remove (Iterator::fromLink (this->m_tail)) : T ();
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
		return ListBase::remove (it);
	}

	Entry*
	removeHeadEntry ()
	{ 
		return ListBase::removeHead ();
	}

	Entry*
	removeTailEntry ()
	{ 
		return ListBase::removeTail ();
	}

	Iterator 
	insertHeadEntry (Entry* entry)
	{ 
		return ListBase::insertHead (entry);
	}

	Iterator 
	insertTailEntry (Entry* entry)
	{ 
		return ListBase::insertTail (entry);
	}

	Iterator 
	insertBeforeEntry (
		Entry* entry, 
		Iterator before
		)
	{ 
		return ListBase::insertBefore (entry, before);
	}

	Iterator 
	insertAfterEntry (
		Entry* entry, 
		Iterator after
		)
	{ 
		return ListBase::insertAfter (entry, after);
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

} // namespace rtl
} // namespace axl
