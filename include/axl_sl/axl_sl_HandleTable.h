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

template <typename T>
using HandleTableIterator = HashTableIterator<uintptr_t, T>;

template <typename T>
using ConstHandleTableIterator = ConstHashTableIterator<uintptr_t, T>;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
class HandleTableBase: public HashTable<
	Key,
	Value,
	HashInt<Key>,
	Eq<Key>,
	KeyValueArgs
> {
protected:
	Key m_seed;

public:
	HandleTableBase(typename HandleTableBase::KeyArg seed = 1) {
		m_seed = seed;
	}

	void
	reset(typename HandleTableBase::KeyArg seed = 1) {
		this->clear();
		m_seed = seed;
	}

	typename HandleTableBase::KeyArg
	add(typename HandleTableBase::ValueArg value) {
		return add(m_seed, value);
	}

	typename HandleTableBase::KeyArg
	add(
		typename HandleTableBase::KeyArg key,
		typename HandleTableBase::ValueArg value
	) {
		for (;;) {
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
	typename KeyValueArgs = ArgTypes<uintptr_t, T>
>
class HandleTable: public HandleTableBase<uintptr_t, T, KeyValueArgs> {
public:
	HandleTable(uintptr_t seed = 1):
		HandleTableBase<uintptr_t, T, KeyValueArgs>(seed) {}
};

//..............................................................................

} // namespace sl
} // namespace axl
