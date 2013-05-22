// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
class CStringHashTableMapAT: public CStringHashTableMapT <TValue, char>
{
};

template <typename TValue>
class CStringHashTableMapWT: public CStringHashTableMapT <TValue, wchar_t>
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
class CStringHashTableMapIteratorAT: public CStringHashTableMapIteratorT <TValue, char>
{
public:
	CStringHashTableMapIteratorAT ()
	{ 
	}

	CStringHashTableMapIteratorAT (const CIteratorT <typename CStringHashTableMapIteratorAT::CEntry>& Src)
	{ 
		this->m_p = Src.GetLink (); 
	}

	CStringHashTableMapIteratorAT (typename CStringHashTableMapIteratorAT::CEntry* p)
	{ 
		operator = (p); 
	}
};

template <typename TValue>
class CStringHashTableMapIteratorWT: public CStringHashTableMapIteratorT <TValue, wchar_t>
{
public:
	CStringHashTableMapIteratorWT ()
	{ 
	}

	CStringHashTableMapIteratorWT (const CIteratorT <typename CStringHashTableMapIteratorWT::CEntry>& Src)
	{ 
		this->m_p = Src.GetLink (); 
	}

	CStringHashTableMapIteratorWT (typename CStringHashTableMapIteratorWT::CEntry* p)
	{ 
		operator = (p); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

