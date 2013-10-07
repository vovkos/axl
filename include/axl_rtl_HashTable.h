// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
		operator () (T* pEntry)
		{
			return &pEntry->m_BucketLink;
		}
	};

	typedef rtl::CAuxListT <T, CBucketLink> CBucket;

	rtl::TListLink m_BucketLink;
	CBucket* m_pBucket;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename TKey>
class CHashTableEntryT: public CHashTableEntryBaseT <CHashTableEntryT <TKey> >
{
public:
	TKey m_Key;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey,
	typename TValue
	>
class CHashTableMapEntryT: public CHashTableEntryBaseT <CHashTableMapEntryT <TKey, TValue> >
{
public:
	TKey m_Key;
	TValue m_Value;
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

	CHashTableIteratorT (const CIteratorT <CHashTableEntryT <TKey> >& Src)
	{ 
		this->m_p = Src.GetLink (); // thanks a lot gcc
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

	CHashTableMapIteratorT (const CIteratorT <CHashTableMapEntryT <TKey, TValue> >& Src)
	{ 
		this->m_p = Src.GetLink ();
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
	rtl::CStdListT <CEntry> m_List;
	rtl::CArrayT <CBucket> m_Table;
	size_t m_ResizeThreshold;

public:
	CHashTableT ()
	{
		m_ResizeThreshold = EDef_ResizeThreshold;
	}

	void 
	Clear ()
	{
		m_Table.Clear ();
		m_List.Clear ();
	}

	bool 
	IsEmpty () const
	{ 
		return m_List.IsEmpty (); 
	}

	CIterator 
	GetHead () const
	{ 
		return m_List.GetHead (); 
	}	

	CIterator 
	GetTail () const
	{ 
		return m_List.GetHead (); 
	}	

	size_t 
	GetCount () const
	{ 
		return m_List.GetCount (); 
	}	

	size_t 
	GetBucketCount () const
	{ 
		return m_Table.GetCount (); 
	}

	bool 
	SetBucketCount (size_t BucketCount)
	{
		rtl::CArrayT <CBucket> NewTable;
		bool Result = NewTable.SetCount (BucketCount);
		if (!Result)
			return false;

		size_t OldBucketCount = m_Table.GetCount ();
		for (size_t i = 0; i < OldBucketCount; i++)
		{
			CBucket* pOldBucket = &m_Table [i];
			while (!pOldBucket->IsEmpty ())
			{
				CEntry* pEntry = pOldBucket->RemoveHead ();
				size_t Hash = CHash () (pEntry->m_Key);
				
				CBucket* pNewBucket = &NewTable [Hash % BucketCount];
				pEntry->m_pBucket = pNewBucket;
				pNewBucket->InsertTail (pEntry);
			}
		}

		m_Table = NewTable;
		return true;
	}

	size_t 
	GetResizeThreshold () const
	{ 
		return m_ResizeThreshold; 
	}	

	void 
	SetResizeThreshold (size_t ResizeThreshold)
	{ 
		m_ResizeThreshold = ResizeThreshold; 
	}

	CIterator 
	Find (const CKey& Key) const
	{ 
		size_t BucketCount = m_Table.GetCount ();
		if (!BucketCount)
			return NULL;

		size_t Hash = CHash () (Key);	
		const CBucket* pBucket = &m_Table [Hash % BucketCount];

		typename CBucket::CIterator It = pBucket->GetHead ();
		for (; It; It++)
		{
			int Cmp = CCmp () (Key, It->m_Key);
			if (Cmp == 0)
				return It;
		}

		return NULL;
	}

	CIterator
	Goto (const CKey& Key)
	{ 
		size_t BucketCount = m_Table.GetCount ();
		if (!BucketCount)
		{
			BucketCount = EDef_InitialBucketCount;
			
			bool Result = m_Table.SetCount (BucketCount);
			if (!Result)
				return NULL;
		}

		size_t Hash = CHash () (Key);
		CBucket* pBucket = &m_Table [Hash % BucketCount];

		typename CBucket::CIterator It = pBucket->GetHead ();
		for (; It; It++)
		{
			int Cmp = CCmp () (Key, It->m_Key);
			if (Cmp == 0)
				return It;
		}

		CEntry* pEntry = AXL_MEM_NEW (CEntry);
		pEntry->m_Key = Key;
		pEntry->m_pBucket = pBucket;
		m_List.InsertTail (pEntry);
		pBucket->InsertTail (pEntry);

	#if (_AXL_PTR_BITNESS == 64)
		size_t LoadFactor = GetCount () * 100 / BucketCount;
	#else
		size_t LoadFactor = (size_t) ((uint64_t) GetCount () * 100 / BucketCount);
	#endif

		if (LoadFactor > m_ResizeThreshold)
			SetBucketCount (BucketCount * 2);

		return pEntry;
	}

	void 
	Delete (CIterator It)
	{
		CEntry* pEntry = *It;
		pEntry->m_pBucket->Remove (pEntry);
		m_List.Remove (pEntry);
		AXL_MEM_DELETE (pEntry);
	}

	bool 
	DeleteByKey (const CKey& Key)
	{ 
		CIterator It = Find (Key);
		if (!It)
			return false;

		Delete (It);
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
	operator [] (const TKey& Key)
	{
		return this->Goto (Key)->m_Value;
	}
};

//.............................................................................

#define AXL_RTL_BEGIN_HASH_TABLE_MAP(Class, TKey, TValue, THash, TCmp) \
class Class \
{ \
public: \
	typedef axl::rtl::CHashTableMapT <TKey, TValue, THash, TCmp> CMapBase; \
	typedef CMapBase::CIterator CIterator; \
	static \
	CIterator \
	Find (TKey Key) \
	{ \
		return axl::rtl::GetSingleton <CMap> ()->Find (Key); \
	} \
	CIterator \
	operator () (TKey Key) \
	{ \
		return Find (Key); \
	} \
protected: \
	class CMap: public CMapBase \
	{ \
	public: \
		CMap () \
		{

#define AXL_RTL_HASH_TABLE_MAP_ENTRY(Key, Value) \
			Goto (Key)->m_Value = Value;

#define AXL_RTL_END_HASH_TABLE_MAP() \
		} \
	}; \
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_INT(Class, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(Class, int, TValue)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_CHAR(Class, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(Class, char, TValue)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_INT_T(Class, TKey, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP ( \
		Class, \
		TKey, \
		TValue, \
		axl::rtl::CHashIdT <TKey>, \
		axl::rtl::CCmpT <TKey> \
		)

//.............................................................................

} // namespace rtl
} // namespace axl

