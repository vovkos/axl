// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
		handle_t m_handle;

	public:
		T m_value;

		handle_t getHandle ()
		{
			return m_handle;
		}
	};

	typedef CIteratorT <CEntry> CListIterator;
	typedef CHashTableMapIteratorT <handle_t, CListIterator> CMapIterator;
	
protected:
	CStdListT <CEntry> m_list;
	CHashTableMapT <handle_t, CListIterator, CHashIdT <handle_t> > m_hashTable;
	uintptr_t m_seed;

public:
	CHandleTableT ()
	{
		m_seed = 1;
	}

	CHandleTableT (uintptr_t seed)
	{
		m_seed = seed ? seed : 1; // never 0
	}

	bool
	isEmpty () const
	{
		return m_list.isEmpty ();
	}

	CConstListT <CEntry> 
	getList () const
	{
		return m_list;
	}

	bool
	create (uintptr_t seed = 1)
	{
		clear ();
		m_seed = seed ? seed : 1; // never 0
	}

	void
	clear ()
	{
		m_list.clear ();
		m_hashTable.clear ();
	}

	CMapIterator
	find (handle_t handle) const
	{
		return m_hashTable.find (handle);
	}
	
	bool
	find (
		handle_t handle,
		T* value
		) const
	{
		CMapIterator it = m_hashTable.find (handle);
		if (!it)
			return false;

		*value = it->m_value->m_value;
		return true;			
	}

	handle_t
	add (T value)
	{
		ASSERT (m_seed);
		handle_t handle = (handle_t) m_seed;

		CEntry* entry = AXL_MEM_NEW (CEntry);
		entry->m_handle = handle;
		entry->m_value = value;
		
		CListIterator it = m_list.insertTail (entry);
		m_hashTable [handle] = it;

		m_seed++;
		if (!m_seed)
			m_seed = 1; // never 0

		return handle;
	}

	void
	remove (CMapIterator it)
	{
		m_list.erase (it->m_value);
		m_hashTable.erase (it);
	}

	bool
	remove (handle_t handle)
	{
		CMapIterator it = m_hashTable.find (handle);
		if (!it)
			return false;

		remove (it);
		return true;			
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

