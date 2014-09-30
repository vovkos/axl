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
class CBoxListEntryT: public TListLink
{
public:
	T m_value;

public:
	CBoxListEntryT ()
	{
	}

	CBoxListEntryT (const T& value)
	{
		m_value = value;
	}
};

//.............................................................................

template <typename T>
class CBoxIteratorT: public CIteratorBaseT <CBoxIteratorT <T> >
{
public:
	typedef CImplicitCastT <CBoxListEntryT <T>*, TListLink*> CLink;

public:
	CBoxIteratorT ()
	{
	}

	CBoxIteratorT (CBoxListEntryT <T>* p)
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

	CBoxListEntryT <T>*
	getEntry () const
	{
		return getEntryFromLink (this->m_p);
	}

	static
	CBoxListEntryT <T>*
	getEntryFromLink (const TListLink* p)
	{ 
		return (CBoxListEntryT <T>*) p;
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
		return p ? &getEntryFromLink (p)->m_value : NULL;
	}
};

//.............................................................................

template <
	typename T,
	typename TInsertArg = T
	>
class CBoxListT: public CListBaseT <
	CBoxListEntryT <T>, 
	CBoxIteratorT <T>,
	typename mem::CStdFactoryT <CBoxListEntryT <T> >::COperatorDelete
	>
{
public:
	typedef CListBaseT <
		CBoxListEntryT <T>, 
		CBoxIteratorT <T>,
		typename mem::CStdFactoryT <CBoxListEntryT <T> >::COperatorDelete
		> CListBase;
	
	typedef CBoxListEntryT <T> CEntry;
	typedef CBoxIteratorT <T> CIterator;

public:
	T
	remove (CIterator it)
	{ 
		T value = *it;
		CEntry* entry = it.getEntry ();
		CListBase::remove (entry);
		AXL_MEM_DELETE (entry);
		return value;
	}

	T
	removeHead ()
	{ 
		return this->m_head ? remove (CIterator::fromLink (this->m_head)) : T ();
	}

	T 
	removeTail ()
	{ 
		return this->m_tail ? remove (CIterator::fromLink (this->m_tail)) : T ();
	}

	CIterator 
	insertHead (TInsertArg value)
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		entry->m_value = value;
		return insertHeadEntry (entry);
	}

	CIterator 
	insertTail (TInsertArg value)
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		entry->m_value = value;
		return insertTailEntry (entry);
	}

	CIterator 
	insertBefore (
		TInsertArg value, 
		CIterator before
		)
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		entry->m_value = value;
		return insertBeforeEntry (entry, before);
	}

	CIterator 
	insertAfter (
		TInsertArg value, 
		CIterator after
		)
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		entry->m_value = value;
		return insertAfterEntry (entry, after);
	}

	// empty element insertion

	CIterator 
	insertHead ()
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		return insertHeadEntry (entry);
	}

	CIterator 
	insertTail ()
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		return insertTailEntry (entry);
	}

	CIterator 
	insertBefore (CIterator before)
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		return insertBeforeEntry (entry, before);
	}

	CIterator 
	insertAfter (CIterator after)
	{ 
		CEntry* entry = AXL_MEM_NEW (CEntry);
		return insertAfterEntry (entry, after);
	}

	// direct access to entries

	CEntry*
	removeEntry (CIterator it)
	{ 
		return CListBase::remove (it);
	}

	CEntry*
	removeHeadEntry ()
	{ 
		return CListBase::removeHead ();
	}

	CEntry*
	removeTailEntry ()
	{ 
		return CListBase::removeTail ();
	}

	CIterator 
	insertHeadEntry (CEntry* entry)
	{ 
		return CListBase::insertHead (entry);
	}

	CIterator 
	insertTailEntry (CEntry* entry)
	{ 
		return CListBase::insertTail (entry);
	}

	CIterator 
	insertBeforeEntry (
		CEntry* entry, 
		CIterator before
		)
	{ 
		return CListBase::insertBefore (entry, before);
	}

	CIterator 
	insertAfterEntry (
		CEntry* entry, 
		CIterator after
		)
	{ 
		return CListBase::insertAfter (entry, after);
	}
};

//.............................................................................

template <typename T>
class CConstBoxListT: public CConstListBaseT <T, CBoxIteratorT <T> > 
{
public:
	CConstBoxListT ()
	{ 
	}

	template <typename TInsertArg>
	CConstBoxListT (const CBoxListT <T, TInsertArg>& list)
	{ 
		this->m_list = list.getList ();
	}

	template <typename TDelete>
	CConstBoxListT (const CListT <CBoxListEntryT <T>, TDelete>& list)
	{ 
		this->m_list = list.getList ();
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
