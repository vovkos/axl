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

#define _AXL_SL_ROBINHASHTABLE_H

#include "axl_sl_Array.h"
#include "axl_sl_List.h"
#include "axl_sl_MapEntry.h"
#include "axl_sl_Hash.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using RobinHashTableEntry = MapEntry<Key, Value, KeyValueArgs>;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using ConstRobinHashTableIterator = ConstMapIterator<Key, Value, KeyValueArgs>;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using RobinHashTableIterator = MapIterator<Key, Value, KeyValueArgs>;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename Hash,
	typename Eq = Eq<Key>,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
class RobinHashTable {
public:
	enum {
		Def_InitialCapacity = 32,
		Def_ResizeThreshold = 75, // percent
	};

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
	Array<Slot> m_slotArray;
	Array<uint8_t> m_ctrlArray;  // 0 - empty; otherwise, PSL + 1
	uint_t m_resizeThreshold;
	Hash m_hash;
	Eq m_eq;

public:
	explicit RobinHashTable(
		const Hash& hash = Hash(),
		const Eq& eq = Eq()
	) {
		m_hash = hash;
		m_eq = eq;
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

	void
	clear() {
		m_list.clear();
		m_slotArray.clear();
		m_ctrlArray.clear();
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

	size_t
	getCapacity() const {
		return m_ctrlArray.getCount();
	}

	bool
	setCapacity(size_t capacity) {
		ASSERT(isPowerOf2(capacity));

		bool result =
			m_slotArray.setCount(capacity) &&
			m_ctrlArray.setCount(capacity);

		if (!result)
			return false;

		Slot* slotArray = m_slotArray.p();
		for (size_t i = 0; i < capacity; i++)
			slotArray[i].m_entry = NULL;

		memset(m_ctrlArray.p(), 0, capacity * sizeof(uint8_t));

		Iterator it = m_list.getHead();
		for (; it; it++)
			addEntry(it.p());

		return true;
	}

	uint_t
	getResizeThreshold() const {
		return m_resizeThreshold;
	}

	void
	setResizeThreshold(uint_t resizeThreshold) {
		m_resizeThreshold = resizeThreshold;
	}

	ConstIterator
	find(KeyArg key) const {
		size_t i = findSlot(key);
		return i != -1 ? m_slotArray[i].m_entry : NULL;
	}

	Iterator
	find(KeyArg key) {
		return (Entry*)((const RobinHashTable*)this)->find(key).p();
	}

	Iterator
	visit(KeyArg key) {
		size_t capacity = m_ctrlArray.getCount();
		if ((getCount() + 1) * 100 > capacity * m_resizeThreshold) {
			capacity = capacity ? capacity * 2 : Def_InitialCapacity;
			bool result = setCapacity(capacity);
			if (!result)
				return NULL;
		}

		ASSERT(!m_ctrlArray.isEmpty());
		sl::Array<Slot>::Rwi slotRwi = m_slotArray;
		sl::Array<uint8_t>::Rwi ctrlRwi = m_ctrlArray;
		size_t mask = capacity - 1;
		size_t i = m_hash(key) & mask;
		uint_t psl = 1;
		Slot slot;
		Entry* entry = NULL;

		for (;;) {
			uint8_t ctrl = ctrlRwi[i];
			if (!ctrl) {
				if (!entry) {
					slot.m_key = key;
					slot.m_entry = entry = new Entry(key);
					m_list.insertTail(entry);
				}

				slotRwi[i] = slot;
				ctrlRwi[i] = psl;
				return entry;
			}

			ASSERT(slotRwi[i].m_entry);
			if (!entry && m_eq(slotRwi[i].m_key, key))
				return slotRwi[i].m_entry;

			if (ctrl < psl) {
				if (!entry) {
					slot.m_key = key;
					slot.m_entry = entry = new Entry(key);
					m_list.insertTail(entry);
				}

				ctrlRwi[i] = psl;
				psl = ctrl;
				Slot swapSlot = slotRwi[i];
				slotRwi[i] = slot;
				slot = swapSlot;
			}

			i = (i + 1) & mask;
			psl = (psl + 1) - (((psl + 1) & 0x100) >> 8); // increment with saturation
		}

		return entry;
	}

	Iterator
	add(
		KeyArg key,
		ValueArg value
	) {
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

protected:
	size_t
	findSlot(KeyArg key) const {
		if (m_list.isEmpty())
			return (size_t)-1;

		size_t mask = m_ctrlArray.getCount() - 1;
		size_t i = m_hash(key) & mask;
		uint_t psl = 1;
		for (;;) {
			uint8_t ctrl = m_ctrlArray[i];
			if (ctrl && m_eq(m_slotArray[i].m_key, key)) {
				ASSERT(m_slotArray[i].m_entry);
				return i;
			}

			if (ctrl < psl)
				return (size_t)-1;

			i = (i + 1) & mask;
			psl = (psl + 1) - (((psl + 1) & 0x100) >> 8); // increment with saturation
		}
	}

	void
	addEntry(Entry* entry) {
		ASSERT(!m_ctrlArray.isEmpty());
		sl::Array<Slot>::Rwi slotRwi = m_slotArray;
		sl::Array<uint8_t>::Rwi ctrlRwi = m_ctrlArray;
		size_t mask = m_ctrlArray.getCount() - 1;
		size_t i = m_hash(entry->getKey()) & mask;
		uint_t psl = 1;
		Slot slot = { entry->getKey(), entry };

		for (;;) {
			uint8_t ctrl = ctrlRwi[i];
			if (!ctrl) {
				slotRwi[i] = slot;
				ctrlRwi[i] = psl;
				return;
			}

			if (ctrl < psl) {
				ctrlRwi[i] = psl;
				psl = ctrl;
				Slot swapSlot = slotRwi[i];
				slotRwi[i] = slot;
				slot = swapSlot;
			}

			i = (i + 1) & mask;
			psl = (psl + 1) - (((psl + 1) & 0x100) >> 8); // increment with saturation
		}
	}

	void
	eraseSlot(size_t i) {
		sl::Array<Slot>::Rwi slotRwi = m_slotArray;
		sl::Array<uint8_t>::Rwi ctrlRwi = m_ctrlArray;
		size_t mask = m_ctrlArray.getCount() - 1;

		for (;;) {
			size_t nextIdx = (i + 1) & mask;
			uint8_t nextCtrl = ctrlRwi[nextIdx];
			if (nextCtrl <= 1) {
				ctrlRwi[i] = 0;
				slotRwi[i].m_entry = NULL;
				return;
			}

			slotRwi[i] = slotRwi[nextIdx];
			ctrlRwi[i] = nextCtrl == 0xff ? 0xff : nextCtrl - 1;
			i = nextIdx;
		}
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
