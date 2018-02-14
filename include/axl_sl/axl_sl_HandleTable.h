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
#include "axl_sl_ArgType.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value
	>
class HandleTableIteratorBase: public Iterator <HashTableEntry <Key, Value> >
{
public:
	HandleTableIteratorBase ()
	{
	}

	HandleTableIteratorBase (const Iterator <HashTableEntry <Key, Value> >& src)
	{
		this->m_p = src.getEntry ();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HandleTableIterator: public HandleTableIteratorBase <uintptr_t, T>
{
public:
	HandleTableIterator ()
	{
	}

	HandleTableIterator (const HandleTableIteratorBase <uintptr_t, T>& src)
	{
		this->m_p = src.getEntry ();
	}
};

//..............................................................................

template <
	typename Key,
	typename Value,
	typename ValueArg = typename ArgType <Value>::Type
	>
class HandleTableBase: public HashTable <
	Key, 
	Value, 
	HashId <Key>,
	Eq <Key>,
	Key, 
	Value
	>
{
protected:
	Key m_seed;

public:
	HandleTableBase (Key seed = 1)
	{
		m_seed = seed;
	}

	void
	reset (Key seed = 1)
	{
		clear ();
		m_seed = seed;
	}

	Key
	add (ValueArg value)
	{
		return add (m_seed, value);
	}

	Key
	add (
		Key key,
		ValueArg value
		)
	{
		for (;;)
		{
			if (!key)
				key = 1; // never 0

			if (!find (key))
				break;

			key++;
		}

		ASSERT (key);
		Iterator it = visit (key);
		it->m_value = value;
		m_seed = key + 1;
		return key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType <T>::Type
	>
class HandleTable: public HandleTableBase <uintptr_t, T, Arg>
{
public:
	HandleTable (uintptr_t seed = 1):
		HandleTableBase (seed)
	{
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
