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
#include "axl_sl_Hash.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class HashTableEntryBase: public ListLink
{
public:
	class BucketLink
	{
	public:
		ListLink*
		operator () (T* entry)
		{
			return &entry->m_bucketLink;
		}
	};

	typedef AuxList <T, BucketLink> Bucket;

	ListLink m_bucketLink;
	Bucket* m_bucket;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Key>
class HashTableEntry: public HashTableEntryBase <HashTableEntry <Key> >
{
public:
	Key m_key;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
	>
class HashTableMapEntry: public HashTableEntryBase <HashTableMapEntry <Key, Value> >
{
public:
	Key m_key;
	Value m_value;
};

//..............................................................................

// too bad there are no templated typedefs in C++
// another solution would be:
//
//	template <
//		typename Key,
//		typename Value
//		>
//	class HashTableMapIterator
//	{
//	public:
//		typedef Iterator <HashTableMapEntry <Key, Value> > T;
//	};
//
// but then it's too easy to forget to write ::T type suffix

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Key>
class HashTableIterator: public Iterator <HashTableEntry <Key> >
{
public:
	HashTableIterator ()
	{
	}

	HashTableIterator (const Iterator <HashTableEntry <Key> >& src)
	{
		this->m_p = src.getEntry ();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
	>
class HashTableMapIterator: public Iterator <HashTableMapEntry <Key, Value> >
{
public:
	HashTableMapIterator ()
	{
	}

	HashTableMapIterator (const Iterator <HashTableMapEntry <Key, Value> >& src)
	{
		this->m_p = src.getEntry ();
	}
};

//..............................................................................

template <
	typename Key,
	typename Hash,
	typename Eq = Eq <Key>,
	typename KeyArg = typename ArgType <Key>::Type,
	typename Entry = HashTableEntry <Key>
	>
class HashTable
{
public:
	enum Def
	{
		Def_InitialBucketCount = 32,
		Def_ResizeThreshold    = 75,
	};

	typedef typename Entry::Bucket Bucket;
	typedef sl::Iterator <Entry> Iterator;

protected:
	StdList <Entry> m_list;
	Array <Bucket> m_table;
	size_t m_resizeThreshold;

public:
	HashTable ()
	{
		m_resizeThreshold = Def_ResizeThreshold;
	}

	void
	clear ()
	{
		m_table.clear ();
		m_list.clear ();
	}

	bool
	isEmpty () const
	{
		return m_list.isEmpty ();
	}

	Iterator
	getHead () const
	{
		return m_list.getHead ();
	}

	Iterator
	getTail () const
	{
		return m_list.getHead ();
	}

	size_t
	getCount () const
	{
		return m_list.getCount ();
	}

	size_t
	getBucketCount () const
	{
		return m_table.getCount ();
	}

	bool
	setBucketCount (size_t bucketCount)
	{
		Array <Bucket> newTable;
		bool result = newTable.setCount (bucketCount);
		if (!result)
			return false;

		size_t oldBucketCount = m_table.getCount ();
		for (size_t i = 0; i < oldBucketCount; i++)
		{
			Bucket* oldBucket = &m_table [i];
			while (!oldBucket->isEmpty ())
			{
				Entry* entry = oldBucket->removeHead ();
				size_t hash = Hash () (entry->m_key);

				Bucket* newBucket = &newTable [hash % bucketCount];
				entry->m_bucket = newBucket;
				newBucket->insertTail (entry);
			}
		}

		m_table = newTable;
		return true;
	}

	size_t
	getResizeThreshold () const
	{
		return m_resizeThreshold;
	}

	void
	setResizeThreshold (size_t resizeThreshold)
	{
		m_resizeThreshold = resizeThreshold;
	}

	Iterator
	find (KeyArg key) const
	{
		size_t bucketCount = m_table.getCount ();
		if (!bucketCount)
			return NULL;

		size_t hash = Hash () (key);
		const Bucket* bucket = &m_table [hash % bucketCount];

		typename Bucket::Iterator it = bucket->getHead ();
		for (; it; it++)
		{
			bool isEqual = Eq () (key, it->m_key);
			if (isEqual)
				return it;
		}

		return NULL;
	}

	Iterator
	visit (KeyArg key)
	{
		size_t bucketCount = m_table.getCount ();
		if (!bucketCount)
		{
			bucketCount = Def_InitialBucketCount;

			bool result = m_table.setCount (bucketCount);
			if (!result)
				return NULL;
		}

		size_t hash = Hash () (key);
		Bucket* bucket = &m_table [hash % bucketCount];

		typename Bucket::Iterator it = bucket->getHead ();
		for (; it; it++)
		{
			bool isEqual = Eq () (key, it->m_key);
			if (isEqual)
				return it;
		}

		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_key = key;
		entry->m_bucket = bucket;
		m_list.insertTail (entry);
		bucket->insertTail (entry);

	#if (AXL_PTR_BITS == 64)
		size_t loadFactor = getCount () * 100 / bucketCount;
	#else
		size_t loadFactor = (size_t) ((uint64_t) getCount () * 100 / bucketCount);
	#endif

		if (loadFactor > m_resizeThreshold)
			setBucketCount (bucketCount * 2);

		return entry;
	}

	void
	erase (Iterator it)
	{
		Entry* entry = *it;
		entry->m_bucket->remove (entry);
		m_list.remove (entry);
		AXL_MEM_DELETE (entry);
	}

	bool
	eraseKey (KeyArg key)
	{
		Iterator it = find (key);
		if (!it)
			return false;

		erase (it);
		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename Hash,
	typename Eq = Eq <Key>,
	typename KeyArg = typename ArgType <Key>::Type,
	typename ValueArg = typename ArgType <Value>::Type
	>
class HashTableMap: public HashTable <
	Key,
	Hash,
	Eq,
	KeyArg,
	HashTableMapEntry <Key, Value>
	>
{
public:
	typedef HashTable <
		Key,
		Hash,
		Eq,
		KeyArg,
		HashTableMapEntry <Key, Value>
		> BaseType;

	typedef typename BaseType::Iterator Iterator;

public:
	Value&
	operator [] (KeyArg key)
	{
		return this->visit (key)->m_value;
	}

	Value
	findValue (
		KeyArg key,
		ValueArg undefinedValue
		) const
	{
		Iterator it = this->find (key);
		return it ? it->m_value : undefinedValue;
	}

	Iterator
	add (
		KeyArg key,
		ValueArg value
		)
	{
		Iterator it = this->visit (key);
		it->m_value = value;
		return it;
	}
};

//..............................................................................

// hash table for simple types (char, int, void* etc)

template <
	typename Key,
	typename KeyArg = typename ArgType <Key>::Type
	>
class SimpleHashTable: public HashTable <
	Key,
	sl::HashId <Key>,
	sl::Eq <Key>,
	KeyArg
	>
{
};

template <
	typename Key,
	typename Value,
	typename KeyArg = typename ArgType <Key>::Type,
	typename ValueArg = typename ArgType <Value>::Type
	>
class SimpleHashTableMap: public HashTableMap <
	Key,
	Value,
	sl::HashId <Key>,
	sl::Eq <Key>,
	KeyArg,
	ValueArg
	>
{
};

//..............................................................................

// hash table for ducktyped keys -- uses methods .hash ()

template <
	typename Key,
	typename KeyArg = typename ArgType <Key>::Type
	>
class DuckTypeHashTable: public HashTable <
	Key,
	sl::HashDuckType <Key>,
	sl::EqDuckType <Key>,
	KeyArg
	>
{
};

template <
	typename Key,
	typename Value,
	typename KeyArg = typename ArgType <Key>::Type,
	typename ValueArg = typename ArgType <Value>::Type
	>
class DuckTypeHashTableMap: public HashTableMap <
	Key,
	Value,
	sl::HashDuckType <Key>,
	sl::EqDuckType <Key>,
	KeyArg,
	ValueArg
	>
{
};

//..............................................................................

// variant of hash table for ducktyped keys (key is a pointer to duck type)

template <typename Key>
class DuckTypePtrHashTable: public HashTable <
	Key*,
	sl::HashDuckType <Key>,
	sl::EqDuckType <Key>,
	Key*
	>
{
};

template <
	typename Key,
	typename Value,
	typename ValueArg = typename ArgType <Value>::Type
	>
class DuckTypePtrHashTableMap: public HashTableMap <
	Key*,
	Value,
	sl::HashDuckType <Key>,
	sl::EqDuckType <Key>,
	Key*,
	ValueArg
	>
{
};

//..............................................................................

#define AXL_SL_BEGIN_HASH_TABLE_MAP_EX(Class, Key, Value, Hash, Eq, KeyArg, ValueArg) \
class Class \
{ \
public: \
	typedef axl::sl::HashTableMap <Key, Value, Hash, Eq, KeyArg, ValueArg> MapBase; \
	typedef MapBase::Iterator Iterator; \
	static \
	Iterator \
	find (KeyArg key) \
	{ \
		return axl::sl::getSingleton <Map> ()->find (key); \
	} \
	static \
	Value \
	findValue ( \
		KeyArg key, \
		ValueArg undefinedValue \
		) \
	{ \
		return axl::sl::getSingleton <Map> ()->findValue (key, undefinedValue); \
	} \
protected: \
	class Map: public MapBase \
	{ \
	public: \
		Map () \
		{

#define AXL_SL_HASH_TABLE_MAP_ENTRY(key, value) \
			visit (key)->m_value = value;

#define AXL_SL_END_HASH_TABLE_MAP() \
		} \
	}; \
};

#define AXL_SL_BEGIN_HASH_TABLE_MAP(Class, Key, Value, Hash) \
	AXL_SL_BEGIN_HASH_TABLE_MAP_EX ( \
		Class, \
		Key, \
		Value, \
		Hash, \
		axl::sl::Eq <Key>, \
		axl::sl::ArgType <Key>::Type, \
		axl::sl::ArgType <Value>::Type \
		)

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_SIMPLE_HASH_TABLE_MAP(Class, Key, Value) \
	AXL_SL_BEGIN_HASH_TABLE_MAP ( \
		Class, \
		Key, \
		Value, \
		axl::sl::HashId <Key> \
		)

#define AXL_SL_END_SIMPLE_HASH_TABLE_MAP() \
	AXL_SL_END_HASH_TABLE_MAP ()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_DUCK_TYPE_HASH_TABLE_MAP(Class, Key, Value) \
	AXL_SL_BEGIN_HASH_TABLE_MAP_EX ( \
		Class, \
		Key, \
		Value, \
		axl::sl::HashDuckType <Key>, \
		axl::sl::Eq <Key>, \
		axl::sl::ArgType <Key>::Type, \
		axl::sl::ArgType <Value>::Type \
		)

#define AXL_SL_END_DUCK_TYPE_HASH_TABLE_MAP() \
	AXL_SL_END_HASH_TABLE_MAP ()

//..............................................................................

} // namespace sl
} // namespace axl
