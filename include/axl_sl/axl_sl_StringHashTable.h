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

#define _AXL_SL_STRINGHASHTABLE_H

#include "axl_sl_HashTable.h"
#include "axl_sl_String.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class EqStringIgnoreCaseBase_pcu
{
public:
	bool
	operator () (
		const sl::StringRefBase<T>& string1,
		const sl::StringRefBase<T>& string2
		) const
	{
		return string1.isEqualIgnoreCase(string2, CaseConvertMethod_PerCodeUnit);
	}
};

template <typename T>
class EqStringIgnoreCaseBase_pcp
{
public:
	bool
	operator () (
		const sl::StringRefBase<T>& string1,
		const sl::StringRefBase<T>& string2
		) const
	{
		return string1.isEqualIgnoreCase(string2, CaseConvertMethod_PerCodePoint);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef EqStringIgnoreCaseBase_pcu<char>    EqStringIgnoreCase_pcu;
typedef EqStringIgnoreCaseBase_pcu<wchar_t> EqStringIgnoreCase_pcu_w;
typedef EqStringIgnoreCaseBase_pcu<utf8_t>  EqStringIgnoreCase_pcu_utf8;
typedef EqStringIgnoreCaseBase_pcu<utf16_t> EqStringIgnoreCase_pcu_utf16;
typedef EqStringIgnoreCaseBase_pcu<utf32_t> EqStringIgnoreCase_pcu_utf32;

typedef EqStringIgnoreCaseBase_pcp<char>    EqStringIgnoreCase_pcp;
typedef EqStringIgnoreCaseBase_pcp<wchar_t> EqStringIgnoreCase_pcp_w;
typedef EqStringIgnoreCaseBase_pcp<utf8_t>  EqStringIgnoreCase_pcp_utf8;
typedef EqStringIgnoreCaseBase_pcp<utf16_t> EqStringIgnoreCase_pcp_utf16;
typedef EqStringIgnoreCaseBase_pcp<utf32_t> EqStringIgnoreCase_pcp_utf32;

//..............................................................................

template <typename T>
class HashStringIgnoreCaseBase_pcu
{
public:
	size_t
	operator () (const sl::StringRefBase<T>& string) const
	{
		return string.hashIgnoreCase(CaseConvertMethod_PerCodeUnit);
	}
};

template <typename T>
class HashStringIgnoreCaseBase_pcp
{
public:
	size_t
	operator () (const sl::StringRefBase<T>& string) const
	{
		return string.hashIgnoreCase(CaseConvertMethod_PerCodePoint);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef HashStringIgnoreCaseBase_pcu<char>    HashStringIgnoreCase_pcu;
typedef HashStringIgnoreCaseBase_pcu<wchar_t> HashStringIgnoreCase_pcu_w;
typedef HashStringIgnoreCaseBase_pcu<utf8_t>  HashStringIgnoreCase_pcu_utf8;
typedef HashStringIgnoreCaseBase_pcu<utf16_t> HashStringIgnoreCase_pcu_utf16;
typedef HashStringIgnoreCaseBase_pcu<utf32_t> HashStringIgnoreCase_pcu_utf32;

typedef HashStringIgnoreCaseBase_pcp<char>    HashStringIgnoreCase_pcp;
typedef HashStringIgnoreCaseBase_pcp<wchar_t> HashStringIgnoreCase_pcp_w;
typedef HashStringIgnoreCaseBase_pcp<utf8_t>  HashStringIgnoreCase_pcp_utf8;
typedef HashStringIgnoreCaseBase_pcp<utf16_t> HashStringIgnoreCase_pcp_utf16;
typedef HashStringIgnoreCaseBase_pcp<utf32_t> HashStringIgnoreCase_pcp_utf32;

//..............................................................................

template <typename Value>
class StringHashTable: public DuckTypeHashTable<String, Value>
{
};

template <typename Value>
class StringHashTable_w: public DuckTypeHashTable<String_w, Value>
{
};

template <typename Value>
class StringHashTable_utf8: public DuckTypeHashTable<String_utf8, Value>
{
};

template <typename Value>
class StringHashTable_utf16: public DuckTypeHashTable<String_utf16, Value>
{
};

template <typename Value>
class StringHashTable_utf32: public DuckTypeHashTable<String_utf32, Value>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableIgnoreCase_pcu: public HashTable<
	String,
	Value,
	HashStringIgnoreCase_pcu,
	EqStringIgnoreCase_pcu
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcu_w: public HashTable<
	String_w,
	Value,
	HashStringIgnoreCase_pcu_w,
	EqStringIgnoreCase_pcu_w
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcu_utf8: public HashTable<
	String_utf8,
	Value,
	HashStringIgnoreCase_pcu_utf8,
	EqStringIgnoreCase_pcu_utf8
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcu_utf16: public HashTable<
	String_utf16,
	Value,
	HashStringIgnoreCase_pcu_utf16,
	EqStringIgnoreCase_pcu_utf16
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcu_utf32: public HashTable<
	String_utf32,
	Value,
	HashStringIgnoreCase_pcu_utf32,
	EqStringIgnoreCase_pcu_utf32
	>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Value>
class StringHashTableIgnoreCase_pcp: public HashTable<
	String,
	Value,
	HashStringIgnoreCase_pcp,
	EqStringIgnoreCase_pcp
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcp_w: public HashTable<
	String_w,
	Value,
	HashStringIgnoreCase_pcp_w,
	EqStringIgnoreCase_pcp_w
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcp_utf8: public HashTable<
	String_utf8,
	Value,
	HashStringIgnoreCase_pcp_utf8,
	EqStringIgnoreCase_pcp_utf8
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcp_utf16: public HashTable<
	String_utf16,
	Value,
	HashStringIgnoreCase_pcp_utf16,
	EqStringIgnoreCase_pcp_utf16
	>
{
};

template <typename Value>
class StringHashTableIgnoreCase_pcp_utf32: public HashTable<
	String_utf32,
	Value,
	HashStringIgnoreCase_pcp_utf32,
	EqStringIgnoreCase_pcp_utf32
	>
{
};

//..............................................................................

template <typename Value>
class StringHashTableIterator: public Iterator<HashTableEntry<String, Value> >
{
public:
	StringHashTableIterator()
	{
	}

	StringHashTableIterator(const Iterator<HashTableEntry<String, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class ConstStringHashTableIterator: public ConstIterator<HashTableEntry<String, Value> >
{
public:
	ConstStringHashTableIterator()
	{
	}

	ConstStringHashTableIterator(const ConstIterator<HashTableEntry<String, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class StringHashTableIterator_w: public Iterator<HashTableEntry<String_w, Value> >
{
public:
	StringHashTableIterator_w()
	{
	}

	StringHashTableIterator_w(const Iterator<HashTableEntry<String_w, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class ConstStringHashTableIterator_w: public ConstIterator<HashTableEntry<String_w, Value> >
{
public:
	ConstStringHashTableIterator_w()
	{
	}

	ConstStringHashTableIterator_w(const ConstIterator<HashTableEntry<String_w, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class StringHashTableIterator_utf8: public Iterator<HashTableEntry<String_utf8, Value> >
{
public:
	StringHashTableIterator_utf8()
	{
	}

	StringHashTableIterator_utf8(const Iterator<HashTableEntry<String_utf8, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class ConstStringHashTableIterator_utf8: public ConstIterator<HashTableEntry<String_utf8, Value> >
{
public:
	ConstStringHashTableIterator_utf8()
	{
	}

	ConstStringHashTableIterator_utf8(const ConstIterator<HashTableEntry<String_utf8, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class StringHashTableIterator_utf16: public Iterator<HashTableEntry<String_utf16, Value> >
{
public:
	StringHashTableIterator_utf16()
	{
	}

	StringHashTableIterator_utf16(const Iterator<HashTableEntry<String_utf16, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class ConstStringHashTableIterator_utf16: public ConstIterator<HashTableEntry<String_utf16, Value> >
{
public:
	ConstStringHashTableIterator_utf16()
	{
	}

	ConstStringHashTableIterator_utf16(const ConstIterator<HashTableEntry<String_utf16, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class StringHashTableIterator_utf32: public Iterator<HashTableEntry<String_utf32, Value> >
{
public:
	StringHashTableIterator_utf32()
	{
	}

	StringHashTableIterator_utf32(const Iterator<HashTableEntry<String_utf32, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

template <typename Value>
class ConstStringHashTableIterator_utf32: public ConstIterator<HashTableEntry<String_utf32, Value> >
{
public:
	ConstStringHashTableIterator_utf32()
	{
	}

	ConstStringHashTableIterator_utf32(const ConstIterator<HashTableEntry<String_utf32, Value> >& src)
	{
		this->m_p = src.getEntry();
	}
};

//..............................................................................

#define AXL_SL_BEGIN_STRING_HASH_TABLE(Class, Value) \
	AXL_SL_BEGIN_DUCK_TYPE_HASH_TABLE(Class, axl::sl::String, Value)

#define AXL_SL_END_STRING_HASH_TABLE() \
	AXL_SL_END_DUCK_TYPE_HASH_TABLE()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_STRING_HASH_TABLE_IGNORE_CASE_PCU(Class, Value) \
	AXL_SL_HASH_TABLE_EX ( \
		Class, \
		axl::sl::String, \
		Value, \
		HashStringIgnoreCase_pcu, \
		EqStringIgnoreCase_pcu, \
		axl::sl::StringRef, \
		axl::sl::ArgType<Value>::Type \
		)

#define AXL_SL_END_STRING_HASH_TABLE_IGNORE_CASE_PCU() \
	AXL_SL_END_HASH_TABLE()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_BEGIN_STRING_HASH_TABLE_IGNORE_CASE_PCP(Class, Value) \
	AXL_SL_HASH_TABLE_EX ( \
		Class, \
		axl::sl::String, \
		Value, \
		HashStringIgnoreCase_pcp, \
		EqStringIgnoreCase_pcp, \
		axl::sl::StringRef, \
		axl::sl::ArgType<Value>::Type \
		)

#define AXL_SL_END_STRING_HASH_TABLE_IGNORE_CASE_PCP() \
	AXL_SL_END_HASH_TABLE()

//..............................................................................

} // namespace sl
} // namespace axl
