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

#define _AXL_SL_HASHTABLE_H

#include "axl_sl_Array.h"
#include "axl_sl_List.h"
#include "axl_sl_MapEntry.h"
#include "axl_sl_Hash.h"
#include "axl_sl_Operator.h"
#include "axl_sl_Singleton.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value
>
struct HashTableEntry: MapEntry<Key, Value> {
	template <
		typename Key2,
		typename Value2,
		typename Hash,
		typename Eq,
		typename KeyArg,
		typename ValueArg
	>
	friend class HashTable;

protected:
	class GetBucketLink {
	public:
		ListLink*
		operator () (HashTableEntry* entry) const {
			return &entry->m_bucketLink;
		}

		const ListLink*
		operator () (const HashTableEntry* entry) const {
			return &entry->m_bucketLink;
		}
	};

	typedef AuxList<HashTableEntry, GetBucketLink> Bucket;

protected:
	ListLink m_bucketLink;
	Bucket* m_bucket;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
>
class HashTableIterator: public Iterator<HashTableEntry<Key, Value> > {
public:
	HashTableIterator() {}

	HashTableIterator(const Iterator<HashTableEntry<Key, Value> >& src) {
		this->m_p = src.getEntry();
	}
};

template <
	typename Key,
	typename Value
>
class ConstHashTableIterator: public ConstIterator<HashTableEntry<Key, Value> > {
public:
	ConstHashTableIterator() {}

	ConstHashTableIterator(const Iterator<HashTableEntry<Key, Value> >& src) {
		this->m_p = src.getEntry();
	}

	ConstHashTableIterator(const ConstIterator<HashTableEntry<Key, Value> >& src) {
		this->m_p = src.getEntry();
	}
};

//..............................................................................

template <
	typename Key,
	typename Value,
	typename Hash,
	typename Eq = Eq<Key>,
	typename KeyArg = typename ArgType<Key>::Type,
	typename ValueArg = typename ArgType<Value>::Type
>
class HashTable {
public:
	enum Def {
		Def_InitialBucketCount = 32,
		Def_ResizeThreshold    = 75,
	};

	typedef HashTableEntry<Key, Value> Entry;
	typedef sl::Iterator<Entry> Iterator;
	typedef sl::ConstIterator<Entry> ConstIterator;
	typedef typename Entry::Bucket Bucket;

protected:
	List<Entry> m_list;
	Array<Bucket> m_table;
	size_t m_resizeThreshold;
	Hash m_hash;
	Eq m_eq;

public:
	explicit HashTable(
		const Hash& hash = Hash(),
		const Eq& eq = Eq()
	) {
		m_resizeThreshold = Def_ResizeThreshold;
		m_hash = hash;
		m_eq = eq;
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
		m_table.clear();
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
		return m_list.getHead();
	}

	ConstIterator
	getTail() const {
		return m_list.getHead();
	}

	size_t
	getCount() const {
		return m_list.getCount();
	}

	size_t
	getBucketCount() const {
		return m_table.getCount();
	}

	bool
	setBucketCount(size_t bucketCount) {
		Array<Bucket> newTable;
		bool result = newTable.setCount(bucketCount);
		if (!result)
			return false;

		typename Array<Bucket>::Rwi oldRwi = m_table;
		typename Array<Bucket>::Rwi newRwi = newTable;
		size_t oldBucketCount = m_table.getCount();
		for (size_t i = 0; i < oldBucketCount; i++) {
			Bucket* oldBucket = &oldRwi[i];
			while (!oldBucket->isEmpty()) {
				Entry* entry = oldBucket->removeHead();
				size_t hash = m_hash(entry->m_key);

				Bucket* newBucket = &newRwi[hash % bucketCount];
				entry->m_bucket = newBucket;
				newBucket->insertTail(entry);
			}
		}

		m_table = newTable;
		return true;
	}

	size_t
	getResizeThreshold() const {
		return m_resizeThreshold;
	}

	void
	setResizeThreshold(size_t resizeThreshold) {
		m_resizeThreshold = resizeThreshold;
	}

	ConstIterator
	find(KeyArg key) const {
		size_t bucketCount = m_table.getCount();
		if (!bucketCount)
			return NULL;

		size_t hash = m_hash(key);
		typename Bucket::ConstIterator it = m_table[hash % bucketCount].getHead();
		for (; it; it++) {
			bool isEqual = m_eq(key, it->m_key);
			if (isEqual)
				return it;
		}

		return NULL;
	}

	Iterator
	find(KeyArg key) {
		return m_table.ensureExclusive() ?
			(Entry*)((const HashTable*)this)->find(key).getEntry() : // a simple const-cast
			NULL;
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
		size_t bucketCount = m_table.getCount();
		if (!bucketCount) {
			bucketCount = Def_InitialBucketCount;

			bool result = m_table.setCount(bucketCount);
			if (!result)
				return NULL;
		}

		size_t hash = m_hash(key);
		Bucket* bucket = &m_table.rwi()[hash % bucketCount];
		typename Bucket::Iterator it = bucket->getHead();
		for (; it; it++) {
			bool isEqual = m_eq(key, it->m_key);
			if (isEqual)
				return it;
		}

		Entry* entry = new (mem::ZeroInit) Entry;
		entry->m_key = key;
		entry->m_bucket = bucket;
		m_list.insertTail(entry);
		bucket->insertTail(entry);

	#if (AXL_PTR_BITS == 64)
		size_t loadFactor = getCount() * 100 / bucketCount;
	#else
		size_t loadFactor = (size_t)((uint64_t)getCount() * 100 / bucketCount);
	#endif

		if (loadFactor > m_resizeThreshold)
			setBucketCount(bucketCount * 2);

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

	Iterator
	add(
		KeyArg key,
		ValueArg value,
		bool* isNew
	) {
		size_t prevCount = this->getCount();

		Iterator it = this->visit(key);
		it->m_value = value;

		if (isNew)
			*isNew = this->getCount() > prevCount;

		return it;
	}

	Iterator
	addIfNotExists(
		KeyArg key,
		ValueArg value
	) {
		size_t prevCount = this->getCount();

		Iterator it = this->visit(key);

		if (this->getCount() == prevCount)
			return NULL;

		it->m_value = value;
		return it;
	}

	void
	erase(Iterator it) {
		it->m_bucket->remove(*it);
		m_list.erase(it);
	}

	bool
	eraseKey(KeyArg key) {
		Iterator it = find(key);
		if (!it)
			return false;

		erase(it);
		return true;
	}

	bool
	sortByKey() {
		return sortByKey(Lt<Key, KeyArg>());
	}

	template <typename Compare>
	bool
	sortByKey(Compare compare) {
		return m_list.sort(CompareMapEntryKey<Key, Value, Compare>(compare));
	}

	bool
	sortByValue() {
		return sortByValue(Lt<Value, ValueArg>());
	}

	template <typename Compare>
	bool
	sortByValue(Compare compare) {
		return m_list.sort(CompareMapEntryValue<Key, Value, Compare>(compare));
	}
};

//..............................................................................

// hash table for simple types (char, int, void* etc)

template <
	typename Key,
	typename Value,
	typename KeyArg = typename ArgType<Key>::Type,
	typename ValueArg = typename ArgType<Value>::Type
>
class SimpleHashTable: public HashTable<
	Key,
	Value,
	sl::HashId<Key>,
	sl::Eq<Key>,
	KeyArg,
	ValueArg
> {
};

//..............................................................................

// hash table for ducktyped keys -- uses methods .hash () and .isEqual ()

template <
	typename Key,
	typename Value,
	typename KeyArg = typename ArgType<Key>::Type,
	typename ValueArg = typename ArgType<Value>::Type
>
class DuckTypeHashTable: public HashTable<
	Key,
	Value,
	sl::HashDuckType<Key>,
	sl::EqDuckType<Key>,
	KeyArg,
	ValueArg
> {
};

//..............................................................................

// variation of hash table for ducktyped keys (key is a pointer to duck type)

template <
	typename Key,
	typename Value,
	typename ValueArg = typename ArgType<Value>::Type
>
class DuckTypePtrHashTable: public HashTable<
	Key*,
	Value,
	sl::HashDuckType<Key>,
	sl::EqDuckType<Key>,
	Key*,
	ValueArg
> {
};

//..............................................................................

#define AXL_SL_BEGIN_HASH_TABLE_EX(Class, Key, Value, Hash, Eq, KeyArg, ValueArg) \
class Class { \
public: \
	typedef axl::sl::HashTable<Key, Value, Hash, Eq, KeyArg, ValueArg> MapBase; \
	typedef MapBase::Iterator Iterator; \
	static \
	Iterator \
	find(KeyArg key) { \
		return axl::sl::getSingleton<Map>()->find(key); \
	} \
	static \
	Value \
	findValue ( \
		KeyArg key, \
		ValueArg undefinedValue \
	) { \
		return axl::sl::getSingleton<Map>()->findValue(key, undefinedValue); \
	} \
protected: \
	class Map: public MapBase { \
	public: \
		Map() {

#define AXL_SL_HASH_TABLE_ENTRY(key, value) \
			visit(key)->m_value = value;

#define AXL_SL_END_HASH_TABLE() \
		} \
	}; \
};

#define AXL_SL_BEGIN_HASH_TABLE(Class, Key, Value, Hash) \
	AXL_SL_BEGIN_HASH_TABLE_EX( \
		Class, \
		Key, \
		Value, \
		Hash, \
		axl::sl::Eq<Key>, \
		axl::sl::ArgType<Key>::Type, \
		axl::sl::ArgType<Value>::Type \
	)

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_SIMPLE_HASH_TABLE(Class, Key, Value) \
	AXL_SL_BEGIN_HASH_TABLE( \
		Class, \
		Key, \
		Value, \
		axl::sl::HashId<Key> \
	)

#define AXL_SL_END_SIMPLE_HASH_TABLE() \
	AXL_SL_END_HASH_TABLE()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_DUCK_TYPE_HASH_TABLE(Class, Key, Value) \
	AXL_SL_BEGIN_HASH_TABLE_EX( \
		Class, \
		Key, \
		Value, \
		axl::sl::HashDuckType<Key>, \
		axl::sl::EqDuckType<Key>, \
		axl::sl::ArgType<Key>::Type, \
		axl::sl::ArgType<Value>::Type \
	)

#define AXL_SL_END_DUCK_TYPE_HASH_TABLE() \
	AXL_SL_END_HASH_TABLE()

//..............................................................................

} // namespace sl
} // namespace axl
