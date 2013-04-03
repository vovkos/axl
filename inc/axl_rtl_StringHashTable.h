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

typedef CStringHashTableA::CIterator CStringHashTableIteratorA;
typedef CStringHashTableW::CIterator CStringHashTableIteratorW;
typedef CStringHashTable::CIterator CStringHashTableIterator;

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

template <
	typename TValue,
	typename C = tchar_t
	>
class CStringHashTableMapIteratorT: public CHashTableMapIteratorT <
	const C*,
	TValue
	>
{
public:
	CStringHashTableMapIteratorT ()
	{ 
	}

	CStringHashTableMapIteratorT (const CStringHashTableMapIteratorT& Src)
	{ 
		m_p = Src.m_p; 
	}

	CStringHashTableMapIteratorT (const CIteratorT <CHashTableMapEntryT <const C*, TValue> >& Src)
	{ 
		m_p = Src.GetLink (); 
	}

	CStringHashTableMapIteratorT (CEntry* p)
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

	CStringHashTableMapIteratorAT (const CStringHashTableMapIteratorT& Src)
	{ 
		m_p = Src.m_p; 
	}

	CStringHashTableMapIteratorAT (const CIteratorT <CHashTableMapEntryT <const char*, TValue> >& Src)
	{ 
		m_p = Src.GetLink (); 
	}

	CStringHashTableMapIteratorAT (CEntry* p)
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

	CStringHashTableMapIteratorWT (const CStringHashTableMapIteratorT& Src)
	{ 
		m_p = Src.m_p; 
	}

	CStringHashTableMapIteratorWT (const CIteratorT <CHashTableMapEntryT <const wchar_t*, TValue> >& Src)
	{ 
		m_p = Src.GetLink (); 
	}

	CStringHashTableMapIteratorWT (CEntry* p)
	{ 
		operator = (p); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

