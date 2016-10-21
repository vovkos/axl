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

#define _AXL_SL_HANDLETABLE_H

#include "axl_sl_List.h"
#include "axl_sl_HashTable.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class HandleTable
{
public:
	class Entry: public sl::ListLink
	{
		friend class HandleTable;

	protected:
		handle_t m_handle;

	public:
		T m_value;

		handle_t getHandle ()
		{
			return m_handle;
		}
	};

	typedef Iterator <Entry> ListIterator;
	typedef HashTableMapIterator <handle_t, ListIterator> MapIterator;

protected:
	StdList <Entry> m_list;
	HashTableMap <handle_t, ListIterator, HashId <handle_t> > m_hashTable;
	uintptr_t m_seed;

public:
	HandleTable ()
	{
		m_seed = 1;
	}

	HandleTable (uintptr_t seed)
	{
		m_seed = seed ? seed : 1; // never 0
	}

	bool
	isEmpty () const
	{
		return m_list.isEmpty ();
	}

	ConstList <Entry>
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

	MapIterator
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
		MapIterator it = m_hashTable.find (handle);
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

		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_handle = handle;
		entry->m_value = value;

		ListIterator it = m_list.insertTail (entry);
		m_hashTable [handle] = it;

		m_seed++;
		if (!m_seed)
			m_seed = 1; // never 0

		return handle;
	}

	void
	remove (MapIterator it)
	{
		m_list.erase (it->m_value);
		m_hashTable.erase (it);
	}

	bool
	remove (handle_t handle)
	{
		MapIterator it = m_hashTable.find (handle);
		if (!it)
			return false;

		remove (it);
		return true;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
