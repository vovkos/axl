// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_STRINGHASHTABLE_H

#include "axl_rtl_HashTable.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename C>
class StringHashTable: public HashTable <
	const C*,
	rtl::HashStringBase <C>, 
	rtl::CmpStringBase <C>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringHashTable <char>    StringHashable;
typedef StringHashTable <wchar_t> StringHashable_w;

typedef StringHashable::Iterator   StringHashableIterator;
typedef StringHashable_w::Iterator StringHashableIterator_w;

//.............................................................................

template <
	typename Value,
	typename C = char
	>
class StringHashTableMap: public HashTableMap <
	const C*,
	Value,
	rtl::HashStringBase <C>, 
	rtl::CmpStringBase <C>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableMap_w: public StringHashTableMap <Value, wchar_t>
{
};

//.............................................................................

template <
	typename Value,
	typename C = char
	>
class StringHashTableMapIterator: public HashTableMapIterator <const C*, Value>
{
public:
	StringHashTableMapIterator ()
	{ 
	}

	StringHashTableMapIterator (const Iterator <typename StringHashTableMapIterator::Entry>& src)
	{ 
		this->m_p = src.getLink (); // thanks a lot gcc
	}

	StringHashTableMapIterator (typename StringHashTableMapIterator::Entry* p)
	{ 
		operator = (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableMapIterator_w: public StringHashTableMapIterator <Value, wchar_t>
{
public:
	StringHashTableMapIterator_w ()
	{ 
	}

	StringHashTableMapIterator_w (const Iterator <typename StringHashTableMapIterator_w::Entry>& src)
	{ 
		this->m_p = src.getLink (); 
	}

	StringHashTableMapIterator_w (typename StringHashTableMapIterator_w::Entry* p)
	{ 
		operator = (p); 
	}
};

//.............................................................................

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING(class, Value) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING_T (class, Value, char)

#define AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING_T(class, Value, C) \
	AXL_RTL_BEGIN_HASH_TABLE_MAP ( \
		class, \
		const C*, \
		Value, \
		rtl::HashStringBase <C>, \
		rtl::CmpStringBase <C> \
		)

//.............................................................................

} // namespace rtl
} // namespace axl

