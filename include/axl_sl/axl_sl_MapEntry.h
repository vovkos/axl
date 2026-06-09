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

#define _AXL_SL_MAPENTRY_H

#include "axl_sl_Iterator.h"
#include "axl_sl_ArgType.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key0,
	typename Value0,
	typename KeyValueArgs = ArgTypes<Key0, Value0>
>
struct MapEntry: ListLink {
	typedef Key0 Key;
	typedef Value0 Value;
	typedef typename KeyValueArgs::template Type<0> KeyArg;
	typedef typename KeyValueArgs::template Type<1> ValueArg;

protected:
	Key m_key;

public:
	Value m_value;

public:
	MapEntry(KeyArg key):
		m_key(key),
		m_value() {} // ensure default initialization

	KeyArg
	getKey() const {
		return m_key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using MapIterator = Iterator<MapEntry<Key, Value, KeyValueArgs> >;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using ConstMapIterator = ConstIterator<MapEntry<Key, Value, KeyValueArgs> >;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename Compare,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
class CompareMapEntryKey {
protected:
	Compare m_compare;

public:
	CompareMapEntryKey(Compare compare):
		m_compare(compare) {}

	bool
	operator () (
		const MapEntry<Key, Value, KeyValueArgs>& entry1,
		const MapEntry<Key, Value, KeyValueArgs>& entry2
	) const {
		return m_compare(entry1.getKey(), entry2.getKey());
	}

	bool
	operator () (
		const MapEntry<Key, Value, KeyValueArgs>* entry1,
		const MapEntry<Key, Value, KeyValueArgs>* entry2
	) const {
		return m_compare(entry1->getKey(), entry2->getKey());
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename Compare,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
class CompareMapEntryValue {
public:
	bool
	operator () (
		const MapEntry<Key, Value, KeyValueArgs>& entry1,
		const MapEntry<Key, Value, KeyValueArgs>& entry2
	) const {
		return Compare()(entry1.m_value, entry2.m_value);
	}

	bool
	operator () (
		const MapEntry<Key, Value, KeyValueArgs>* entry1,
		const MapEntry<Key, Value, KeyValueArgs>* entry2
	) const {
		return Compare()(entry1->m_value, entry2->m_value);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
