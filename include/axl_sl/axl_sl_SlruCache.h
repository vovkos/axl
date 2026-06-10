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

#define _AXL_SL_SLRUCACHE_H

#include "axl_sl_HashTable.h"

namespace axl {
namespace sl {

//..............................................................................

// SlruCacheEntry is stored directly in MapEntry::m_value

template <
	typename Key0,
	typename Value0,
	typename KeyValueArgs = ArgTypes<Key0, Value0>
>
struct SlruCacheEntry: ListLink {
	template <
		typename Key2,
		typename Value2,
		template <
			typename MapKey,
			typename MapValue,
			typename MapKeyValueArgs
		> class Map,
		bool IsProtectedLru,
		typename KeyValueArgs2
	>
	friend class SlruCache;

public:
	typedef Key0 Key;
	typedef Value0 Value;
	typedef typename std::tuple_element<0, KeyValueArgs>::type KeyArg;
	typedef typename std::tuple_element<1, KeyValueArgs>::type ValueArg;
	typedef sl::MapEntry<Key, SlruCacheEntry, std::tuple<KeyArg, const SlruCacheEntry&> > MapEntry;

	class GetMapLink {
	public:
		ListLink*
		operator () (SlruCacheEntry* entry) const {
			return containerof(entry, MapEntry, m_value);
		}

		const ListLink*
		operator () (const SlruCacheEntry* entry) const {
			return containerof(entry, MapEntry, m_value);
		}
	};

protected:
	enum Tier {
		Tier_Undefined,
		Tier_Probation,
		Tier_Protected,
	};

protected:
	Tier m_tier;

public:
	Value m_value;

public:
	SlruCacheEntry():
		m_value() {
		m_tier = Tier_Undefined;
	}

	KeyArg
	getKey() const {
		return containerof(this, MapEntry, m_value)->getKey();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using SlruCacheIterator = Iterator<
	SlruCacheEntry<Key, Value, KeyValueArgs>,
	typename SlruCacheEntry<Key, Value, KeyValueArgs>::GetMapLink
>;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using ConstSlruCacheIterator = ConstIterator<
	SlruCacheEntry<Key, Value, KeyValueArgs>,
	typename SlruCacheEntry<Key, Value, KeyValueArgs>::GetMapLink
>;

//..............................................................................

template <
	typename Key0,
	typename Value0,
	template <
		typename MapKey,
		typename MapValue,
		typename MapKeyValueArgs
	> class Map0,
	bool IsProtectedLru = false, // true - LRU, false - FIFO
	typename KeyValueArgs = ArgTypes<Key0, Value0>
>
class SlruCache {
public:
	typedef Key0 Key;
	typedef Value0 Value;
	typedef SlruCacheEntry<Key, Value, KeyValueArgs> Entry;
	typedef typename Entry::KeyArg KeyArg;
	typedef typename Entry::ValueArg ValueArg;
	typedef Map0<Key, Entry, std::tuple<KeyArg, const Entry&> > Map;
	typedef SlruCacheIterator<Key, Value, KeyValueArgs> Iterator;
	typedef ConstSlruCacheIterator<Key, Value, KeyValueArgs> ConstIterator;

#if (_AXL_TEST)
	typedef AuxList<Entry> TierList;
#endif

	enum Def {
		Def_ProbationCapacity = 1 * 1024, // 25%
		Def_ProtectedCapacity = 3 * 1024, // 75%
	};

protected:
	Map m_map;
	AuxList<Entry> m_probationList;
	AuxList<Entry> m_protectedList;
	size_t m_probationCapacity;
	size_t m_protectedCapacity;

public:
	SlruCache() {
		m_probationCapacity = Def_ProbationCapacity;
		m_protectedCapacity = Def_ProtectedCapacity;
	}

	SlruCache(
		size_t probationCapacity,
		size_t protectedCapacity
	) {
		ASSERT(probationCapacity && protectedCapacity);
		m_probationCapacity = probationCapacity;
		m_protectedCapacity = protectedCapacity;
	}

	const Value&
	operator [] (KeyArg key) const {
		return this->find(key)->m_value;
	}

	Value&
	operator [] (KeyArg key) {
		return this->visit(key)->m_value;
	}

	void
	clear() {
		m_probationList.clear();
		m_protectedList.clear();
		m_map.clear();
	}

	bool
	isEmpty() const {
		return m_map.isEmpty();
	}

	Iterator
	getHead() {
		typename Map::Iterator it = m_map.getHead();
		return it ? &it->m_value : NULL;
	}

	ConstIterator
	getHead() const {
		typename Map::ConstIterator it = m_map.getHead();
		return it ? &it->m_value : NULL;
	}

	Iterator
	getTail() {
		typename Map::Iterator it = m_map.getTail();
		return it ? &it->m_value : NULL;
	}

	ConstIterator
	getTail() const {
		typename Map::ConstIterator it = m_map.getTail();
		return it ? &it->m_value : NULL;
	}

	size_t
	getCount() const {
		return m_map.getCount();
	}

#if (_AXL_TEST)
	const TierList&
	getProbationList() const {
		return m_probationList;
	}

	const TierList&
	getProtectedList() const {
		return m_protectedList;
	}
#endif

	size_t
	getProbationCapacity() const {
		return m_probationCapacity;
	}

	size_t
	getProtectedCapacity() const {
		return m_protectedCapacity;
	}

	void
	setCapacity(
		size_t probationCapacity,
		size_t protectedCapacity
	) {
		ASSERT(probationCapacity && protectedCapacity);
		m_probationCapacity = probationCapacity;
		m_protectedCapacity = protectedCapacity;
		demote();
		evict();
	}

	ConstIterator
	find(KeyArg key) const {
		typename Map::ConstIterator mapIt = m_map.find(key);
		return mapIt ? &mapIt->m_value : NULL;
	}

	Iterator
	find(KeyArg key) {
		return (Entry*)((const SlruCache*)this)->find(key).p();
	}

	Value
	findValue(
		KeyArg key,
		ValueArg undefinedValue
	) const {
		ConstIterator it = this->find(key);
		return it ? it->m_value : undefinedValue;
	}

	Iterator
	visit(KeyArg key) {
		typename Map::Iterator mapIt = m_map.visit(key);
		Entry* entry = &mapIt->m_value;

		switch (entry->m_tier) {
		case Entry::Tier_Undefined:
			entry->m_tier = Entry::Tier_Probation;
			m_probationList.insertHead(entry);
			evict();
			break;

		case Entry::Tier_Probation:
			entry->m_tier = Entry::Tier_Protected;
			m_probationList.remove(entry);
			m_protectedList.insertHead(entry);
			demote();
			break;

		default:
			if (IsProtectedLru) {
				ASSERT(mapIt->m_value.m_tier == Entry::Tier_Protected);
				m_protectedList.remove(entry);
				m_protectedList.insertHead(entry);
			}
		}

		return entry;
	}

	Iterator
	add(
		KeyArg key,
		ValueArg value
	) {
		Iterator it = this->visit(key);
		it->m_value = value;
		return it;
	}

	void
	erase(Iterator it) {
		Entry* entry = *it;
		if (entry->m_tier == Entry::Tier_Protected)
			m_protectedList.remove(entry);
		else {
			ASSERT(entry->m_tier == Entry::Tier_Probation);
			m_probationList.remove(entry);
		}

		m_map.erase(containerof(entry, typename Map::Entry, m_value));
	}

	bool
	eraseKey(KeyArg key) {
		Iterator it = find(key);
		if (!it)
			return false;

		erase(it);
		return true;
	}

protected:
	void
	demote() {
		while (m_protectedList.getCount() > m_protectedCapacity) {
			Entry* entry = m_protectedList.removeTail();
			ASSERT(entry->m_tier == Entry::Tier_Protected);
			entry->m_tier = Entry::Tier_Probation;
			m_probationList.insertHead(entry);
		}
	}

	void
	evict() {
		while (m_probationList.getCount() > m_probationCapacity) {
			Entry* entry = m_probationList.removeTail();
			ASSERT(entry->m_tier == Entry::Tier_Probation);
			m_map.erase(containerof(entry, typename Map::Entry, m_value));
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .



//..............................................................................

} // namespace sl
} // namespace axl
