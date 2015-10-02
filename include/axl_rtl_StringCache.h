// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_STRINGCACHE_H

#include "axl_rtl_String.h"
#include "axl_rtl_BoxList.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename C>
class StringCacheBase
{
public:
	typedef StringBase <C> String;
	typedef StringHashTableMapBase <const String*, C> StringMap;
	typedef StringHashTableMapIterator <const String*, C> StringMapIterator;

protected:
	BoxList <String> m_stringList;
	StringHashTableMapBase <const String*, C> m_stringMap;
	
public:
	void
	clear ()
	{
		m_stringList.clear ();
		m_stringMap.clear ();
	}

	String
	getString (const C* srcString)
	{
		StringMapIterator it = m_stringMap.find (srcString);
		if (it)
			return *it->m_value;

		String* dstString = m_stringList.insertTail (srcString).p ();
		it = m_stringMap.visit (*dstString);
		it->m_value = dstString;
		return *dstString;
	}

	String
	getString (const String& srcString)
	{
		StringMapIterator it = m_stringMap.visit (srcString);
		if (it->m_value)
			return *it->m_value;

		String* dstString = m_stringList.insertTail (srcString).p ();
		it->m_value = dstString;
		return *dstString;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringCacheBase <char>    StringCache;
typedef StringCacheBase <wchar_t> StringCache_w;
typedef StringCacheBase <utf8_t>  StringCache_utf8;
typedef StringCacheBase <utf16_t> StringCache_utf16;
typedef StringCacheBase <utf32_t> StringCache_utf32;

//.............................................................................

} // namespace rtl
} // namespace axl

