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

#include "axl_sl_HashTable.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value
	>
class HandleTableIteratorBase: public Iterator<HashTableEntry<Key, Value> >
{
public:
	HandleTableIteratorBase()
	{
	}

	HandleTableIteratorBase(const Iterator<HashTableEntry<Key, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <
	typename Key,
	typename Value
	>
class ConstHandleTableIteratorBase: public ConstIterator<HashTableEntry<Key, Value> >
{
public:
	ConstHandleTableIteratorBase()
	{
	}

	ConstHandleTableIteratorBase(const ConstIterator<HashTableEntry<Key, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HandleTableIterator: public HandleTableIteratorBase<uintptr_t, T>
{
public:
	HandleTableIterator()
	{
	}

	HandleTableIterator(const Iterator<HashTableEntry<uintptr_t, T> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename T>
class ConstHandleTableIterator: public ConstHandleTableIteratorBase<uintptr_t, T>
{
public:
	ConstHandleTableIterator()
	{
	}

	ConstHandleTableIterator(const ConstIterator<HashTableEntry<uintptr_t, T> >& src)
	{
		this->m_p = src.getEntry();
	}
};

//..............................................................................

template <
	typename Key,
	typename Value,
	typename ValueArg = typename ArgType<Value>::Type
	>
class HandleTableBase: public HashTable<
	Key,
	Value,
	HashId<Key>,
	Eq<Key>,
	Key,
	ValueArg
	>
{
protected:
	Key m_seed;

public:
	HandleTableBase(Key seed = 1)
	{
		m_seed = seed;
	}

	void
	reset(Key seed = 1)
	{
		this->clear();
		m_seed = seed;
	}

	Key
	add(ValueArg value)
	{
		return add(m_seed, value);
	}

	Key
	add(
		Key key,
		ValueArg value
		)
	{
		for (;;)
		{
			if (!key)
				key = 1; // never 0

			if (!this->find(key))
				break;

			key++;
		}

		ASSERT(key);
		typename HandleTableBase::Iterator it = this->visit(key);
		it->m_value = value;
		m_seed = key + 1;
		return key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
	>
class HandleTable: public HandleTableBase<uintptr_t, T, Arg>
{
public:
	HandleTable(uintptr_t seed = 1):
		HandleTableBase<uintptr_t, T, Arg> (seed)
	{
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
