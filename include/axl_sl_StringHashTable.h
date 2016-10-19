// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_STRINGHASHTABLE_H

#include "axl_sl_HashTable.h"
#include "axl_sl_String.h"

namespace axl {
namespace sl {

//..............................................................................

typedef DuckTypeHashTable <String>       StringHashTable;
typedef DuckTypeHashTable <String_w>     StringHashTable_w;
typedef DuckTypeHashTable <String_utf8>  StringHashTable_utf8;
typedef DuckTypeHashTable <String_utf16> StringHashTable_utf16;
typedef DuckTypeHashTable <String_utf32> StringHashTable_utf32;

typedef StringHashTable::Iterator        StringHashTableIterator;
typedef StringHashTable_w::Iterator      StringHashTableIterator_w;
typedef StringHashTable_utf8::Iterator   StringHashTableIterator_utf8;
typedef StringHashTable_utf16::Iterator  StringHashTableIterator_utf16;
typedef StringHashTable_utf32::Iterator  StringHashTableIterator_utf32;

//..............................................................................

template <typename Value>
class StringHashTableMap: public DuckTypeHashTableMap <String, Value>
{
};

template <typename Value>
class StringHashTableMap_w: public DuckTypeHashTableMap <String_w, Value>
{
};

template <typename Value>
class StringHashTableMap_utf8: public DuckTypeHashTableMap <String_utf8, Value>
{
};

template <typename Value>
class StringHashTableMap_utf16: public DuckTypeHashTableMap <String_utf16, Value>
{
};

template <typename Value>
class StringHashTableMap_utf32: public DuckTypeHashTableMap <String_utf32, Value>
{
};

//..............................................................................

template <typename Value>
class StringHashTableMapIterator: public HashTableMapIterator <String, Value>
{
public:
	StringHashTableMapIterator ()
	{
	}

	StringHashTableMapIterator (const Iterator <typename StringHashTableMapIterator::Entry>& src)
	{
		this->m_p = src.getEntry ();
	}

	StringHashTableMapIterator (typename StringHashTableMapIterator::Entry* p)
	{
		operator = (p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableMapIterator_w: public HashTableMapIterator <String_w, Value>
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableMapIterator_utf8: public HashTableMapIterator <String_utf8, Value>
{
public:
	StringHashTableMapIterator_utf8 ()
	{
	}

	StringHashTableMapIterator_utf8 (const Iterator <typename StringHashTableMapIterator_utf8::Entry>& src)
	{
		this->m_p = src.getLink ();
	}

	StringHashTableMapIterator_utf8 (typename StringHashTableMapIterator_utf8::Entry* p)
	{
		operator = (p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableMapIterator_utf16: public HashTableMapIterator <String_utf16, Value>
{
public:
	StringHashTableMapIterator_utf16 ()
	{
	}

	StringHashTableMapIterator_utf16 (const Iterator <typename StringHashTableMapIterator_utf16::Entry>& src)
	{
		this->m_p = src.getLink ();
	}

	StringHashTableMapIterator_utf16 (typename StringHashTableMapIterator_utf16::Entry* p)
	{
		operator = (p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableMapIterator_utf32: public HashTableMapIterator <String_utf32, Value>
{
public:
	StringHashTableMapIterator_utf32 ()
	{
	}

	StringHashTableMapIterator_utf32 (const Iterator <typename StringHashTableMapIterator_utf32::Entry>& src)
	{
		this->m_p = src.getLink ();
	}

	StringHashTableMapIterator_utf32 (typename StringHashTableMapIterator_utf32::Entry* p)
	{
		operator = (p);
	}
};

//..............................................................................

#define AXL_SL_BEGIN_STRING_HASH_TABLE_MAP(Class, Value) \
	AXL_SL_BEGIN_DUCK_TYPE_HASH_TABLE_MAP (Class, axl::sl::String, Value)

#define AXL_SL_END_STRING_HASH_TABLE_MAP() \
	AXL_SL_END_DUCK_TYPE_HASH_TABLE_MAP ()

//..............................................................................

} // namespace sl
} // namespace axl

