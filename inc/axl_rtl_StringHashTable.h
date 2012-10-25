// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_STRINGHASHTABLE_H

#include "axl_rtl_HashTable.h"

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

typedef CStringHashTableT <char>    CStringHashTableA;
typedef CStringHashTableT <wchar_t> CStringHashTableW;
typedef CStringHashTableT <tchar_t> CStringHashTable;

//.............................................................................

template <
	typename TValue,
	typename C = tchar_t
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
class CStringHashTableMapAT: public CStringHashTableMapT <TValue, char>
{
};

template <typename TValue>
class CStringHashTableMapWT: public CStringHashTableMapT <TValue, wchar_t>
{
};

//.............................................................................

} // namespace rtl
} // namespace axl

