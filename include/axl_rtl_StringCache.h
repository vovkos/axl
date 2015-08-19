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
protected:
	rtl::BoxList <rtl::StringBase <C> > m_symbolList;
	rtl::StringHashTableBase <C> m_symbolMap;
	
public:
	const C*
	getString (const C* p)
	{
		rtl::HashTableIterator <const C*> it = m_symbolMap.find (p);
		if (it)
			return it->m_key;

		rtl::StringBase <C>& string = *m_symbolList.insertTail (p);
		m_symbolMap.visit (string);
		return string;
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

