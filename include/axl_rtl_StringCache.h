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
class CStringCacheT
{
protected:
	rtl::CBoxListT <rtl::CStringT <C> > m_symbolList;
	rtl::CStringHashTableT <C> m_symbolMap;
	
public:
	const C*
	getString (const C* p)
	{
		rtl::CHashTableIteratorT <const C*> it = m_symbolMap.find (p);
		if (it)
			return it->m_key;

		rtl::CStringT <C>& string = *m_symbolList.insertTail (p);
		m_symbolMap.visit (string);
		return string;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CStringCacheT <char>    CStringCache;
typedef CStringCacheT <wchar_t> CStringCache_w;

//.............................................................................

} // namespace rtl
} // namespace axl

