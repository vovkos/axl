// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	rtl::CBoxListT <rtl::CStringT <C> > m_SymbolList;
	rtl::CStringHashTableT <C> m_SymbolMap;
	
public:
	const C*
	GetString (const C* p)
	{
		rtl::CHashTableIteratorT <const C*> It = m_SymbolMap.Find (p);
		if (It)
			return It->m_Key;

		rtl::CStringT <C>& String = *m_SymbolList.InsertTail (p);
		m_SymbolMap.Goto (String);
		return String;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CStringCacheT <char>    CStringCache;
typedef CStringCacheT <wchar_t> CStringCache_w;

//.............................................................................

} // namespace rtl
} // namespace axl

