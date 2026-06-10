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
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
struct HashTableEntry: MapEntry<Key, Value, KeyValueArgs> {
	template <
		typename Key2,
		typename Value2,
		typename Hash,
		typename Eq,
		typename KeyValueArgs2
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

public:
	HashTableEntry(
		typename HashTableEntry::KeyArg key,
		Bucket* bucket
	):
		MapEntry<Key, Value, KeyValueArgs>(key) {
		m_bucket = bucket;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using HashTableIterator = Iterator<HashTableEntry<Key, Value, KeyValueArgs> >;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using ConstHashTableIterator = ConstIterator<HashTableEntry<Key, Value, KeyValueArgs> >;

//..............................................................................

template <
	typename Key0,
	typename Value0,
	typename Hash0,
	typename Eq0 = Eq<Key0>,
	typename KeyValueArgs = ArgTypes<Key0, Value0>
>
class HashTable {
public:
	enum Def {
		Def_InitialBucketCount = 32,
		Def_ResizeThreshold    = 75,
	};

	typedef Key0 Key;
	typedef Value0 Value;
	typedef Hash0 Hash;
	typedef Eq0 Eq;
	typedef HashTableEntry<Key0, Value0, KeyValueArgs> Entry;
	typedef typename Entry::KeyArg KeyArg;
	typedef typename Entry::ValueArg ValueArg;
	typedef typename Entry::Bucket Bucket;
	typedef sl::Iterator<Entry> Iterator;
	typedef sl::ConstIterator<Entry> ConstIterator;

protected:
	List<Entry> m_list;
	Array<Bucket> m_table;
	uint_t m_resizeThreshold;
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
	getBucketCount() const {
		return m_table.getCount();
	}

	bool
	setBucketCount(size_t bucketCount) {
		ASSERT(isPowerOf2(bucketCount));
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
				Bucket* newBucket = &newRwi[m_hash(entry->m_key) & (bucketCount - 1)];
				entry->m_bucket = newBucket;
				newBucket->insertTail(entry);
			}
		}

		sl::takeOver(&m_table, &newTable);
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
		size_t bucketCount = m_table.getCount();
		if (!bucketCount)
			return NULL;

		ASSERT(isPowerOf2(bucketCount));
		typename Bucket::ConstIterator it = m_table[m_hash(key) & (bucketCount - 1)].getHead();
		for (; it; it++)
			if (m_eq(key, it->m_key))
				return it;

		return NULL;
	}

	Iterator
	find(KeyArg key) {
		return (Entry*)((const HashTable*)this)->find(key).p();
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
		} else if ((uint64_t)(getCount() + 1) * 100 > (uint64_t)bucketCount * m_resizeThreshold) {
			bucketCount *= 2;
			bool result = setBucketCount(bucketCount);
			if (!result)
				return NULL;
		}

		ASSERT(isPowerOf2(bucketCount));
		Bucket* bucket = &m_table.rwi()[m_hash(key) & (bucketCount - 1)];
		typename Bucket::Iterator it = bucket->getHead();
		for (; it; it++)
			if (m_eq(key, it->m_key))
				return it;

		Entry* entry = new Entry(key, bucket);
		m_list.insertTail(entry);
		bucket->insertTail(entry);
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
	typename KeyValueArgs = ArgTypes<Key, Value>
>
class SimpleHashTable: public HashTable<
	Key,
	Value,
	sl::HashInt<Key>,
	sl::Eq<Key>,
	KeyValueArgs
> {
};

//..............................................................................

// hash table for ducktyped keys -- uses methods .hash () and .isEqual ()

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
class DuckTypeHashTable: public HashTable<
	Key,
	Value,
	sl::HashDuckType<Key>,
	sl::EqDuckType<Key>,
	KeyValueArgs
> {
};

//..............................................................................

// variation of hash table for ducktyped keys (key is a pointer to duck type)

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key*, Value>
>
class DuckTypePtrHashTable: public HashTable<
	Key*,
	Value,
	sl::HashDuckType<Key>,
	sl::EqDuckType<Key>,
	KeyValueArgs
> {
};

//..............................................................................

#define AXL_SL_BEGIN_HASH_TABLE(Class, Key, Value, Hash, Eq, ...) \
class Class { \
public: \
	typedef axl::sl::HashTable<Key, Value, Hash, Eq, ##__VA_ARGS__> MapBase; \
	typedef MapBase::ConstIterator ConstIterator; \
static \
	ConstIterator \
	find(MapBase::KeyArg key) { \
		return axl::sl::getSingleton<Map>()->find(key); \
	} \
	static \
	Value \
	findValue ( \
		MapBase::KeyArg key, \
		MapBase::ValueArg undefinedValue \
	) { \
		return axl::sl::getSingleton<Map>()->findValue(key, undefinedValue); \
	} \
protected: \
	class Map: public MapBase { \
	public: \
		Map() {

#define AXL_SL_HASH_TABLE_ENTRY(key, value) \
			add(key, value);

#define AXL_SL_END_HASH_TABLE() \
		} \
	}; \
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_SIMPLE_HASH_TABLE(Class, Key, Value, ...) \
	AXL_SL_BEGIN_HASH_TABLE( \
		Class, \
		Key, \
		Value, \
		axl::sl::HashInt<Key>, \
		axl::sl::Eq<Key>, \
		##__VA_ARGS__ \
	)

#define AXL_SL_END_SIMPLE_HASH_TABLE() \
	AXL_SL_END_HASH_TABLE()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_DUCK_TYPE_HASH_TABLE(Class, Key, Value, ...) \
	AXL_SL_BEGIN_HASH_TABLE( \
		Class, \
		Key, \
		Value, \
		axl::sl::HashDuckType<Key>, \
		axl::sl::EqDuckType<Key>, \
		##__VA_ARGS__ \
	)

#define AXL_SL_END_DUCK_TYPE_HASH_TABLE() \
	AXL_SL_END_HASH_TABLE()

//..............................................................................

} // namespace sl
} // namespace axl
