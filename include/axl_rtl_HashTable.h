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
class CHashTableEntryBaseT: public rtl::TListLink
{
public:
	class CBucketLink
	{
	public:
		rtl::TListLink* 
		operator () (T* entry)
		{
			return &entry->m_bucketLink;
		}
	};

	typedef rtl::CAuxListT <T, CBucketLink> CBucket;

	rtl::TListLink m_bucketLink;
	CBucket* m_bucket;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename TKey>
class CHashTableEntryT: public CHashTableEntryBaseT <CHashTableEntryT <TKey> >
{
public:
	TKey m_key;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey,
	typename TValue
	>
class CHashTableMapEntryT: public CHashTableEntryBaseT <CHashTableMapEntryT <TKey, TValue> >
{
public:
	TKey m_key;
	TValue m_value;
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

template <typename TKey>
class CHashTableIteratorT: public CIteratorT <CHashTableEntryT <TKey> >
{
public:
	typedef CHashTableEntryT <TKey> CEntry;

	CHashTableIteratorT ()
	{ 
	}

	CHashTableIteratorT (const CIteratorT <CHashTableEntryT <TKey> >& src)
	{ 
		this->m_p = src.getLink (); // thanks a lot gcc
	}

	CHashTableIteratorT (CEntry* p)
	{ 
		operator = (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey,
	typename TValue
	>
class CHashTableMapIteratorT: public CIteratorT <CHashTableMapEntryT <TKey, TValue> >
{
public:
	typedef CHashTableMapEntryT <TKey, TValue> CEntry;

	CHashTableMapIteratorT ()
	{ 
	}

	CHashTableMapIteratorT (const CIteratorT <CHashTableMapEntryT <TKey, TValue> >& src)
	{ 
		this->m_p = src.getLink ();
	}

	CHashTableMapIteratorT (CEntry* p)
	{ 
		operator = (p); 
	}
};

//.............................................................................

template <
	typename TKey,
	typename THash,
	typename TCmp = CCmpT <TKey>,
	typename TEntry = CHashTableEntryT <TKey>
	>
class CHashTableT
{
public:
	enum EDef
	{
		EDef_InitialBucketCount = 32,
		EDef_ResizeThreshold    = 75,
	};

	typedef TKey CKey;
	typedef THash CHash;
	typedef TCmp CCmp;
	typedef TEntry CEntry;
	typedef typename CEntry::CBucket CBucket;

	typedef rtl::CIteratorT <CEntry> CIterator;
	
protected:
	rtl::CStdListT <CEntry> m_list;
	rtl::CArrayT <CBucket> m_table;
	size_t m_resizeThreshold;

public:
	CHashTableT ()
	{
		m_resizeThreshold = EDef_ResizeThreshold;
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

	CIterator 
	getHead () const
	{ 
		return m_list.getHead (); 
	}	

	CIterator 
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
		rtl::CArrayT <CBucket> newTable;
		bool result = newTable.setCount (bucketCount);
		if (!result)
			return false;

		size_t oldBucketCount = m_table.getCount ();
		for (size_t i = 0; i < oldBucketCount; i++)
		{
			CBucket* oldBucket = &m_table [i];
			while (!oldBucket->isEmpty ())
			{
				CEntry* entry = oldBucket->removeHead ();
				size_t hash = CHash () (entry->m_key);
				
				CBucket* newBucket = &newTable [hash % bucketCount];
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

	CIterator 
	find (const CKey& key) const
	{ 
		size_t bucketCount = m_table.getCount ();
		if (!bucketCount)
			return NULL;

		size_t hash = CHash () (key);	
		const CBucket* bucket = &m_table [hash % bucketCount];

		typename CBucket::CIterator it = bucket->getHead ();
		for (; it; it++)
		{
			int cmp = CCmp () (key, it->m_key);
			if (cmp == 0)
				return it;
		}

		return NULL;
	}

	CIterator
	visit (const CKey& key)
	{ 
		size_t bucketCount = m_table.getCount ();
		if (!bucketCount)
		{
			bucketCount = EDef_InitialBucketCount;
			
			bool result = m_table.setCount (bucketCount);
			if (!result)
				return NULL;
		}

		size_t hash = CHash () (key);
		CBucket* bucket = &m_table [hash % bucketCount];

		typename CBucket::CIterator it = bucket->getHead ();
		for (; it; it++)
		{
			int cmp = CCmp () (key, it->m_key);
			if (cmp == 0)
				return it;
		}

		CEntry* entry = AXL_MEM_NEW (CEntry);
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
	erase (CIterator it)
	{
		CEntry* entry = *it;
		entry->m_bucket->remove (entry);
		m_list.remove (entry);
		AXL_MEM_DELETE (entry);
	}

	bool 
	eraseByKey (const CKey& key)
	{ 
		CIterator it = find (key);
		if (!it)
			return false;

		erase (it);
		return true; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey, 
	typename TValue, 
	typename THash,
	typename TCmp = CCmpT <TKey>
	>
class CHashTableMapT: public CHashTableT <
	TKey,
	THash, 
	TCmp,
	CHashTableMapEntryT <TKey, TValue>
	>
{
public:
	TValue& 
	operator [] (const TKey& key)
	{
		return this->visit (key)->m_value;
	}
};

//.............................................................................

#define AXL_RTL_BEGIN_HASH_TABLE_MAP(class, TKey, TValue, THash, TCmp) \
class class \
{ \
public: \
	typedef axl::rtl::CHashTableMapT <TKey, TValue, THash, TCmp> CMapBase; \
	typedef CMapBase::CIterator CIterator; \
	static \
	CIterator \
	find (TKey key) \
	{ \
		return axl::rtl::getSingleton <CMap> ()->find (key); \
	} \
	CIterator \
	operator () (TKey key) \
	{ \
		return find (key); \
	} \
protected: \
	class CMap: public CMapBase \
	{ \
	public: \
		CMap () \
		{

#define AXL_RTL_HASH_TABLE_MAP_ENTRY(key, value) \
			visit (key)->m_value = value;

#define AXL_RTL_END_HASH_TABLE_MAP() \
		} \
	}; \
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_INT(class, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(class, int, TValue)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_CHAR(class, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(class, char, TValue)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(class, TKey, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP ( \
		class, \
		TKey, \
		TValue, \
		axl::rtl::CHashIdT <TKey>, \
		axl::rtl::CCmpT <TKey> \
		)

//.............................................................................

} // namespace rtl
} // namespace axl

