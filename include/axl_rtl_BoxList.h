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
	T m_Value;

public:
	CBoxListEntryT ()
	{
	}

	CBoxListEntryT (const T& Value)
	{
		m_Value = Value;
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
		return *GetObject (); 
	}

	T* 
	operator -> () const
	{ 
		return GetObject (); 
	}

	CBoxListEntryT <T>*
	GetEntry () const
	{
		return GetEntryFromLink (this->m_p);
	}

	static
	CBoxListEntryT <T>*
	GetEntryFromLink (const TListLink* p)
	{ 
		return (CBoxListEntryT <T>*) p;
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
		return p ? &GetEntryFromLink (p)->m_Value : NULL;
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
	typename mem::CStdFactoryT <CBoxListEntryT <T> >::CDelete
	>
{
public:
	typedef CListBaseT <
		CBoxListEntryT <T>, 
		CBoxIteratorT <T>,
		typename mem::CStdFactoryT <CBoxListEntryT <T> >::CDelete
		> CListBase;
	
	typedef CBoxListEntryT <T> CEntry;
	typedef CBoxIteratorT <T> CIterator;

public:
	T
	Remove (CIterator It)
	{ 
		T Value = *It;
		CEntry* pEntry = It.GetEntry ();
		CListBase::Remove (pEntry);
		AXL_MEM_DELETE (pEntry);
		return Value;
	}

	T
	RemoveHead ()
	{ 
		return this->m_pHead ? Remove (CIterator::FromLink (this->m_pHead)) : T ();
	}

	T 
	RemoveTail ()
	{ 
		return this->m_pTail ? Remove (CIterator::FromLink (this->m_pTail)) : T ();
	}

	CIterator 
	InsertHead (TInsertArg Value)
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		pEntry->m_Value = Value;
		return InsertHeadEntry (pEntry);
	}

	CIterator 
	InsertTail (TInsertArg Value)
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		pEntry->m_Value = Value;
		return InsertTailEntry (pEntry);
	}

	CIterator 
	InsertBefore (
		TInsertArg Value, 
		CIterator Before
		)
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		pEntry->m_Value = Value;
		return InsertBeforeEntry (pEntry, Before);
	}

	CIterator 
	InsertAfter (
		TInsertArg Value, 
		CIterator After
		)
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		pEntry->m_Value = Value;
		return InsertAfterEntry (pEntry, After);
	}

	// empty element insertion

	CIterator 
	InsertHead ()
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		return InsertHeadEntry (pEntry);
	}

	CIterator 
	InsertTail ()
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		return InsertTailEntry (pEntry);
	}

	CIterator 
	InsertBefore (CIterator Before)
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		return InsertBeforeEntry (pEntry, Before);
	}

	CIterator 
	InsertAfter (CIterator After)
	{ 
		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		return InsertAfterEntry (pEntry, After);
	}

	// direct access to entries

	CEntry*
	RemoveEntry (CIterator It)
	{ 
		return CListBase::Remove (It);
	}

	CEntry*
	RemoveHeadEntry ()
	{ 
		return CListBase::RemoveHead ();
	}

	CEntry*
	RemoveTailEntry ()
	{ 
		return CListBase::RemoveTail ();
	}

	CIterator 
	InsertHeadEntry (CEntry* pEntry)
	{ 
		return CListBase::InsertHead (pEntry);
	}

	CIterator 
	InsertTailEntry (CEntry* pEntry)
	{ 
		return CListBase::InsertTail (pEntry);
	}

	CIterator 
	InsertBeforeEntry (
		CEntry* pEntry, 
		CIterator Before
		)
	{ 
		return CListBase::InsertBefore (pEntry, Before);
	}

	CIterator 
	InsertAfterEntry (
		CEntry* pEntry, 
		CIterator After
		)
	{ 
		return CListBase::InsertAfter (pEntry, After);
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
	CConstBoxListT (const CBoxListT <T, TInsertArg>& List)
	{ 
		this->m_pList = List.GetList ();
	}

	template <typename TDelete>
	CConstBoxListT (const CListT <CBoxListEntryT <T>, TDelete>& List)
	{ 
		this->m_pList = List.GetList ();
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
