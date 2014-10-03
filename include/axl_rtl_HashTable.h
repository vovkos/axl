// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HASHTABLE_H

#include "axl_rtl_Array.h"
#include "axl_rtl_List.h"
#include "axl_rtl_Func.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class HashTableEntryBase: public rtl::ListLink
{
public:
	class BucketLink
	{
	public:
		rtl::ListLink*
		operator () (T* entry)
		{
			return &entry->m_bucketLink;
		}
	};

	typedef rtl::AuxList <T, BucketLink> Bucket;

	rtl::ListLink m_bucketLink;
	Bucket* m_bucket;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Key>
class HashTableEntry: public HashTableEntryBase <HashTableEntry <Key> >
{
public:
	Key m_key;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

//.............................................................................

// too bad there are no templated typedefs in C++
// another solution would be:
//
//	template <
//		typename TKey,
//		typename TValue
//		>
//	class CHashTableMapIteratorT
//	{
//	public:
//		typedef CIteratorT <CHashTableMapEntryT <TKey, TValue> > T;
//	};
//
// but then it's too easy to forget to write ::T type suffix

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

	HashTableIterator (HashTableEntry <Key>* p)
	{
		this->m_p = p;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

	HashTableMapIterator (HashTableMapEntry <Key, Value>* p)
	{
		this->m_p = p;
	}
};

//.............................................................................

template <
	typename Key_0,
	typename Hash_0,
	typename Cmp_0 = Cmp <Key_0>,
	typename Entry_0 = HashTableEntry <Key_0>
	>
class HashTable
{
public:
	enum Def
	{
		Def_InitialBucketCount = 32,
		Def_ResizeThreshold    = 75,
	};

	typedef Key_0 Key;
	typedef Hash_0 Hash;
	typedef Cmp_0 Cmp;
	typedef Entry_0 Entry;
	typedef typename Entry::Bucket Bucket;

	typedef rtl::Iterator <Entry> Iterator;

protected:
	rtl::StdList <Entry> m_list;
	rtl::Array <Bucket> m_table;
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
		rtl::Array <Bucket> newTable;
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
	find (const Key& key) const
	{
		size_t bucketCount = m_table.getCount ();
		if (!bucketCount)
			return NULL;

		size_t hash = Hash () (key);
		const Bucket* bucket = &m_table [hash % bucketCount];

		typename Bucket::Iterator it = bucket->getHead ();
		for (; it; it++)
		{
			int cmp = Cmp () (key, it->m_key);
			if (cmp == 0)
				return it;
		}

		return NULL;
	}

	Iterator
	visit (const Key& key)
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
			int cmp = Cmp () (key, it->m_key);
			if (cmp == 0)
				return it;
		}

		Entry* entry = AXL_MEM_NEW (Entry);
		entry->m_key = key;
		entry->m_bucket = bucket;
		m_list.insertTail (entry);
		bucket->insertTail (entry);

	#if (_AXL_PTR_BITNESS == 64)
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
	eraseByKey (const Key& key)
	{
		Iterator it = find (key);
		if (!it)
			return false;

		erase (it);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value,
	typename Hash,
	typename Cmp = Cmp <Key>
	>
class HashTableMap: public HashTable <
	Key,
	Hash,
	Cmp,
	HashTableMapEntry <Key, Value>
	>
{
public:
	Value&
	operator [] (const Key& key)
	{
		return this->visit (key)->m_value;
	}
};

//.............................................................................

#define AXL_RTL_BEGIN_HASH_TABLE_MAP(Class, Key, Value, Hash, Cmp) \
class Class \
{ \
public: \
	typedef axl::rtl::HashTableMap <Key, Value, Hash, Cmp> MapBase; \
	typedef MapBase::Iterator Iterator; \
	static \
	Iterator \
	find (Key key) \
	{ \
		return axl::rtl::getSingleton <Map> ()->find (key); \
	} \
	Iterator \
	operator () (Key key) \
	{ \
		return find (key); \
	} \
protected: \
	class Map: public MapBase \
	{ \
	public: \
		Map () \
		{

#define AXL_RTL_HASH_TABLE_MAP_ENTRY(key, value) \
			visit (key)->m_value = value;

#define AXL_RTL_END_HASH_TABLE_MAP() \
		} \
	}; \
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_INT(Class, Value) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(Class, int, Value)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_CHAR(Class, Value) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(Class, char, Value)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(Class, Key, Value) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP ( \
		class, \
		Key, \
		Value, \
		axl::rtl::HashId <Key>, \
		axl::rtl::Cmp <Key> \
		)

//.............................................................................

} // namespace rtl
} // namespace axl

