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

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
struct SlruCacheEntry: MapEntry<Key, Value, KeyValueArgs> {
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

protected:
	enum Tier {
		Tier_Probation = 0,
		Tier_Protected,
	};

	class GetTierLink {
	public:
		ListLink*
		operator () (SlruCacheEntry* entry) const {
			return &entry->m_tierLink;
		}

		const ListLink*
		operator () (const SlruCacheEntry* entry) const {
			return &entry->m_tierLink;
		}
	};

protected:
	ListLink m_tierLink;
	ListLink* m_mapLink;
	Tier m_tier;

public:
	SlruCacheEntry(
		KeyArg key,
		ListLink* mapLink
	):
		MapEntry<Key, Value, KeyValueArgs>(key) {
		m_tier = Tier_Probation;
		m_mapLink = mapLink;
	}
};

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
	typedef Map0<Key, Entry*, std::tuple<KeyArg, Entry*> > Map;
	typedef sl::Iterator<Entry> Iterator;
	typedef sl::ConstIterator<Entry> ConstIterator;

#if (_AXL_TEST)
	typedef AuxList<Entry, typename Entry::GetTierLink> TierList;
#endif

	enum Def {
		Def_ProbationCapacity = 1 * 1024, // 25%
		Def_ProtectedCapacity = 3 * 1024, // 75%
	};

protected:
	Map m_map;
	List<Entry> m_list;
	AuxList<Entry, typename Entry::GetTierLink> m_probationList;
	AuxList<Entry, typename Entry::GetTierLink> m_protectedList;
	size_t m_probationCapacity;
	size_t m_protectedCapacity;

public:
	explicit SlruCache() {
		m_probationCapacity = Def_ProbationCapacity;
		m_protectedCapacity = Def_ProtectedCapacity;
	}

	explicit SlruCache(
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
		m_list.clear();
	}

	bool
	isEmpty() const {
		return m_list.isEmpty();
	}

	Iterator
	getHead() {
		return m_list.getHead();
	}

	ConstIterator
	getHead() const {
		return m_list.getHead();
	}

	Iterator
	getTail() {
		return m_list.getTail();
	}

	ConstIterator
	getTail() const {
		return m_list.getTail();
	}

	size_t
	getCount() const {
		return m_list.getCount();
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
		return mapIt ? mapIt->m_value : NULL;
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
		if (mapIt->m_value) {
			Entry* entry = mapIt->m_value;
			if (entry->m_tier == Entry::Tier_Probation)
				promote(entry);
			else if (IsProtectedLru) {
				ASSERT(entry->m_tier == Entry::Tier_Protected);
				m_protectedList.remove(entry);
				m_protectedList.insertHead(entry);
			}

			return entry;
		}

		Entry* entry = new Entry(key, mapIt.p());
		mapIt->m_value = entry;
		m_list.insertTail(entry);
		m_probationList.insertHead(entry);
		evict();
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

		m_map.erase((typename Map::Entry*)entry->m_mapLink);
		m_list.erase(entry);
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
	promote(Entry* entry) {
		ASSERT(entry->m_tier == Entry::Tier_Probation);
		entry->m_tier = Entry::Tier_Protected;
		m_probationList.remove(entry);
		m_protectedList.insertHead(entry);
		demote();
	}

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
			m_map.erase((typename Map::Entry*)entry->m_mapLink);
			m_list.erase(entry);
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .



//..............................................................................

} // namespace sl
} // namespace axl
