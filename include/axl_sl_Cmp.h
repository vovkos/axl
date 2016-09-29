// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_SL_CMP_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//.............................................................................

template <
	typename T,
	typename Arg = T
	>
class Cmp
{
public:
	int 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a < b ? -1 : a > b ? 1 : 0; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CmpBin
{
public:
	int 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return memcmp (&a, &b, sizeof (T));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename C
	>
class CmpPtr
{
public:
	int 
	operator () (
		const T* a, 
		const T* b
		) const
	{ 
		return C () (*a, *b);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CmpStringBase
{
};

template <typename T>
class CmpStringBase_i
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpStringBase <char>
{
public:
	int
	operator () (
		const char* p1,
		const char* p2
		) const
	{ 
		return p1 ? p2 ? strcmp (p1, p2) : 1 : p2 ? -1 : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpStringBase_i <char>
{
public:
	int
	operator () (
		const char* p1,
		const char* p2
		) const
	{ 
		return p1 ? p2 ? _stricmp (p1, p2) : 1 : p2 ? -1 : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpStringBase <wchar_t>
{
public:
	int
	operator () (
		const wchar_t* p1,
		const wchar_t* p2
		) const
	{ 
		return p1 ? p2 ? wcscmp (p1, p2) : 1 : p2 ? -1 : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpStringBase_i <wchar_t>
{
public:
	int
	operator () (
		const wchar_t* p1,
		const wchar_t* p2
		) const
	{ 
		return p1 ? p2 ? _wcsicmp (p1, p2) : 1 : p2 ? -1 : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CmpStringBase <char>      CmpString;
typedef CmpStringBase <wchar_t>   CmpString_w;
typedef CmpStringBase_i <char>    CmpString_i;
typedef CmpStringBase_i <wchar_t> CmpString_wi;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CmpDuckType
{
public:
	int 
	operator () (
		const T* a,
		const T* b
		)
	{
		return a->cmp (*b);
	}

	int 
	operator () (
		const T& a,
		const T& b
		)
	{
		return a.cmp (b);
	}
};

//.............................................................................

} // namespace sl
} // namespace axl
