// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HANDLETABLE_H

#include "axl_rtl_List.h"
#include "axl_rtl_HashTable.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class CHandleTableT 
{
public:
	class CEntry: public rtl::TListLink
	{
		friend class CHandleTableT;

	protected:
		handle_t m_Handle;

	public:
		T m_Value;

		handle_t GetHandle ()
		{
			return m_Handle;
		}
	};

	typedef CIteratorT <CEntry> CListIterator;
	typedef CHashTableMapIteratorT <handle_t, CListIterator> CMapIterator;
	
protected:
	CStdListT <CEntry> m_List;
	CHashTableMapT <handle_t, CListIterator, CHashIdT <handle_t> > m_HashTable;
	uintptr_t m_Seed;

public:
	CHandleTableT ()
	{
		m_Seed = 1;
	}

	CHandleTableT (uintptr_t Seed)
	{
		m_Seed = Seed ? Seed : 1; // never 0
	}

	bool
	IsEmpty () const
	{
		return m_List.IsEmpty ();
	}

	CConstListT <CEntry> 
	GetList () const
	{
		return m_List;
	}

	bool
	Create (uintptr_t Seed = 1)
	{
		Clear ();
		m_Seed = Seed ? Seed : 1; // never 0
	}

	void
	Clear ()
	{
		m_List.Clear ();
		m_HashTable.Clear ();
	}

	CMapIterator
	Find (handle_t Handle) const
	{
		return m_HashTable.Find (Handle);
	}
	
	bool
	Find (
		handle_t Handle,
		T* pValue
		) const
	{
		CMapIterator It = m_HashTable.Find (Handle);
		if (!It)
			return false;

		*pValue = It->m_Value->m_Value;
		return true;			
	}

	handle_t
	Add (T Value)
	{
		ASSERT (m_Seed);
		handle_t Handle = (handle_t) m_Seed;

		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		pEntry->m_Handle = Handle;
		pEntry->m_Value = Value;
		
		CListIterator It = m_List.InsertTail (pEntry);
		m_HashTable [Handle] = It;

		m_Seed++;
		if (!m_Seed)
			m_Seed = 1; // never 0

		return Handle;
	}

	void
	Remove (CMapIterator It)
	{
		m_List.Delete (It->m_Value);
		m_HashTable.Delete (It);
	}

	bool
	Remove (handle_t Handle)
	{
		CMapIterator It = m_HashTable.Find (Handle);
		if (!It)
			return false;

		Remove (It);
		return true;			
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

