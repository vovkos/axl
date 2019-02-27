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

#define _AXL_SL_CMP_H

#include "axl_sl_ArgType.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		return memcmp(&a, &b, sizeof(T));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		return C() (*a, *b);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CmpSzBase
{
};

template <typename T>
class CmpSzBase_i
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpSzBase<char>
{
public:
	int
	operator () (
		const char* p1,
		const char* p2
		) const
	{
		return p1 ? p2 ? strcmp(p1, p2) : 1 : p2 ? -1 : 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpSzBase_i<char>
{
public:
	int
	operator () (
		const char* p1,
		const char* p2
		) const
	{
		return p1 ? p2 ? _stricmp(p1, p2) : 1 : p2 ? -1 : 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpSzBase<wchar_t>
{
public:
	int
	operator () (
		const wchar_t* p1,
		const wchar_t* p2
		) const
	{
		return p1 ? p2 ? wcscmp(p1, p2) : 1 : p2 ? -1 : 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CmpSzBase_i<wchar_t>
{
public:
	int
	operator () (
		const wchar_t* p1,
		const wchar_t* p2
		) const
	{
		return p1 ? p2 ? _wcsicmp(p1, p2) : 1 : p2 ? -1 : 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CmpSzBase<char>      CmpSz;
typedef CmpSzBase<wchar_t>   CmpSz_w;
typedef CmpSzBase_i<char>    CmpSz_i;
typedef CmpSzBase_i<wchar_t> CmpSz_wi;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
	>
class CmpDuckType
{
public:
	int
	operator () (
		Arg a,
		Arg b
		) const
	{
		return a.cmp(b);
	}

	int
	operator () (
		const T* a,
		const T* b
		) const
	{
		return a->cmp(*b);
	}
};

template <typename T>
class CmpDuckTypePtr
{
public:
	int
	operator () (
		const T& a,
		const T& b
		) const
	{
		return a.cmp(&b);
	}

	int
	operator () (
		const T* a,
		const T* b
		) const
	{
		return a->cmp(b);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
