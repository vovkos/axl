// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_STRINGHASHTABLE_H

#include "axl_rtl_HashTable.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename C>
class CStringHashTableT: public CHashTableT <
	const C*,
	rtl::CHashStringT <C>, 
	rtl::CCmpStringT <C>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CStringHashTableT <char>    CStringHashTable;
typedef CStringHashTableT <wchar_t> CStringHashTable_w;

typedef CStringHashTable::CIterator   CStringHashTableIterator;
typedef CStringHashTable_w::CIterator CStringHashTableIterator_w;

//.............................................................................

template <
	typename TValue,
	typename C = char
	>
class CStringHashTableMapT: public CHashTableMapT <
	const C*,
	TValue,
	rtl::CHashStringT <C>, 
	rtl::CCmpStringT <C>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename TValue>
class CStringHashTableMapT_w: public CStringHashTableMapT <TValue, wchar_t>
{
};

//.............................................................................

template <
	typename TValue,
	typename C = char
	>
class CStringHashTableMapIteratorT: public CHashTableMapIteratorT <const C*, TValue>
{
public:
	CStringHashTableMapIteratorT ()
	{ 
	}

	CStringHashTableMapIteratorT (const CIteratorT <typename CStringHashTableMapIteratorT::CEntry>& Src)
	{ 
		this->m_p = Src.GetLink (); // thanks a lot gcc
	}

	CStringHashTableMapIteratorT (typename CStringHashTableMapIteratorT::CEntry* p)
	{ 
		operator = (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename TValue>
class CStringHashTableMapIteratorT_w: public CStringHashTableMapIteratorT <TValue, wchar_t>
{
public:
	CStringHashTableMapIteratorT_w ()
	{ 
	}

	CStringHashTableMapIteratorT_w (const CIteratorT <typename CStringHashTableMapIteratorT_w::CEntry>& Src)
	{ 
		this->m_p = Src.GetLink (); 
	}

	CStringHashTableMapIteratorT_w (typename CStringHashTableMapIteratorT_w::CEntry* p)
	{ 
		operator = (p); 
	}
};

//.............................................................................

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING(Class, TValue) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING_T (Class, TValue, char)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING_T(Class, TValue, C) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP ( \
		Class, \
		const C*, \
		TValue, \
		rtl::CHashStringT <C>, \
		rtl::CCmpStringT <C> \
		)

//.............................................................................

} // namespace rtl
} // namespace axl

