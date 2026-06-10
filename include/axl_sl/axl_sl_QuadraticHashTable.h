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

#define _AXL_SL_QUADRATICHASHTABLE_H

#include "axl_sl_Array.h"
#include "axl_sl_List.h"
#include "axl_sl_Hash.h"
#include "axl_sl_Operator.h"
#include "axl_sl_ArgType.h"
#include "axl_sl_BitIdx.h"
#include "axl_sl_MapEntry.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using QuadraticHashTableEntry = MapEntry<Key, Value, KeyValueArgs>;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using ConstQuadraticHashTableIterator = ConstMapIterator<Key, Value, KeyValueArgs>;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using QuadraticHashTableIterator = MapIterator<Key, Value, KeyValueArgs>;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key0,
	Key0 EmptyKey,
	Key0 TombstoneKey,
	typename Value0,
	typename Hash0,
	typename Eq0 = Eq<Key0>,
	typename KeyValueArgs = ArgTypes<Key0, Value0>
>
class QuadraticHashTable {
public:
	enum {
		Def_InitialCapacity = 32,
		Def_ResizeThreshold = 75,
	};

	typedef Key0 Key;
	typedef Value0 Value;
	typedef Hash0 Hash;
	typedef Eq0 Eq;
	typedef MapEntry<Key, Value, KeyValueArgs> Entry;
	typedef typename Entry::KeyArg KeyArg;
	typedef typename Entry::ValueArg ValueArg;
	typedef sl::Iterator<Entry> Iterator;
	typedef sl::ConstIterator<Entry> ConstIterator;

protected:
	struct Slot {
		Key m_key;
		Entry* m_entry;
	};

	List<Entry> m_list;
	sl::Array<Slot> m_slotArray;
	size_t m_tombstoneCount;
	uint_t m_resizeThreshold;
	Hash m_hash;
	Eq m_eq;

public:
	explicit QuadraticHashTable(
		const Hash& hash = Hash(),
		const Eq& eq = Eq()
	) {
		m_hash = hash;
		m_eq = eq;
		m_tombstoneCount = 0;
		m_resizeThreshold = Def_ResizeThreshold;
	}

	const Value&
	operator [] (KeyArg key) const {
		return this->find(key)->m_value;
	}

	Value&
	operator [] (KeyArg key) {
		return this->visit(key)->m_value;
	}

	bool isEmpty() const {
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

	size_t getCount() const {
		return m_list.getCount();
	}

	size_t
	getCapacity() const {
		return m_slotArray.getCount();
	}

	uint_t
	getResizeThreshold() const {
		return m_resizeThreshold;
	}

	void
	setResizeThreshold(uint_t resizeThreshold) {
		m_resizeThreshold = resizeThreshold;
	}

	void
	clear() {
		typename sl::Array<Slot>::Rwi rwi = m_slotArray;
		size_t capacity = m_slotArray.getCount();
		for (size_t i = 0; i < capacity; i++)
			rwi[i].m_key = EmptyKey;

		m_list.clear();
		m_tombstoneCount = 0;
	}

	ConstIterator
	find(KeyArg key) const {
		size_t i = findSlot(key);
		return i != -1 ? m_slotArray[i].m_entry : NULL;
	}

	Iterator
	find(KeyArg key) {
		return (Entry*)((const QuadraticHashTable*)this)->find(key).p();
	}

	Iterator
	visit(KeyArg key) {
		size_t capacity = m_slotArray.getCount();
		size_t count = m_list.getCount();
		if ((count + m_tombstoneCount + 1) * 100 > capacity * m_resizeThreshold) {
			if (!capacity)
				capacity = Def_InitialCapacity;
			else if (m_tombstoneCount <= count / 2) // if it's mostly tombstones, just rehash
				capacity *= 2;

			if (!setCapacity(capacity))
				return NULL;
		}

		typename sl::Array<Slot>::Rwi rwi = m_slotArray;
		size_t mask = capacity - 1;
		size_t i = m_hash(key) & mask;
		size_t step = 1;
		Slot* tombstone = NULL; // first reusable tombstone on the probe chain
		for (;;) {
			Key slotKey = rwi[i].m_key;
			if (m_eq(slotKey, key))
				return rwi[i].m_entry;

			if (m_eq(slotKey, EmptyKey)) {
				Entry* entry = new Entry(key);
				m_list.insertTail(entry);

				if (tombstone) {
					tombstone->m_key = key;
					tombstone->m_entry = entry;
					m_tombstoneCount--;
				} else {
					rwi[i].m_key = key;
					rwi[i].m_entry = entry;
				}

				return entry;
			} else if (m_eq(slotKey, TombstoneKey) && !tombstone)
				tombstone = &rwi[i];

			i = (i + step) & mask; // triangular-number probe
			step++;
		}
	}

	Iterator
	add(KeyArg key, ValueArg value) {
		Iterator it = visit(key);
		it->m_value = value;
		return it;
	}

	void
	erase(Iterator it) {
		size_t i = findSlot(it->getKey());
		ASSERT(i != -1 && m_slotArray[i].m_entry == *it);

		eraseSlot(i);
		m_list.erase(it);
	}

	bool
	eraseKey(KeyArg key) {
		size_t i = findSlot(key);
		if (i == -1)
			return false;

		Entry* entry = m_slotArray[i].m_entry;
		eraseSlot(i);
		m_list.erase(entry);
		return true;
	}

	bool
	setCapacity(size_t capacity) {
		ASSERT(isPowerOf2(capacity));
		bool result = m_slotArray.setCount(capacity);
		if (!result)
			return false;

		typename sl::Array<Slot>::Rwi rwi = m_slotArray;
		for (size_t i = 0; i < capacity; i++)
			rwi[i].m_key = EmptyKey;

		Iterator it = m_list.getHead();
		for (; it; it++)
			addEntry(*it);

		m_tombstoneCount = 0;
		return true;
	}

protected:
	size_t
	findSlot(KeyArg key) const {
		if (m_list.isEmpty())
			return -1;

		ASSERT(!m_eq(key, EmptyKey) && !m_eq(key, TombstoneKey));
		size_t mask = m_slotArray.getCount() - 1;
		size_t i = m_hash(key) & mask;
		size_t step = 1;
		for (;;) {
			Key slotKey = m_slotArray[i].m_key;
			if (m_eq(slotKey, key))
				return i;
			else if (m_eq(slotKey, EmptyKey))
				return -1;

			i = (i + step) & mask; // triangular-number probe
			step++;
		}
	}

	void
	eraseSlot(size_t i) {
		typename sl::Array<Slot>::Rwi rwi = m_slotArray;
		rwi[i].m_key = TombstoneKey;
		rwi[i].m_entry = NULL;
		m_tombstoneCount++;
	}

	void
	addEntry(Entry* entry) {
		ASSERT(!m_slotArray.isEmpty());
		typename sl::Array<Slot>::Rwi rwi = m_slotArray;
		size_t mask = m_slotArray.getCount() - 1;
		size_t i = m_hash(entry->getKey()) & mask;
		size_t step = 1;
		for (;;) {
			Key slotKey = rwi[i].m_key;
			if (m_eq(slotKey, EmptyKey)) {
				rwi[i].m_key = entry->getKey();
				rwi[i].m_entry = entry;
				return;
			}

			i = (i + step) & mask; // triangular-number probe
			step++;
		}
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
