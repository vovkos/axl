// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_FUNC_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

// std functors

template <typename T>
class CVoidT
{
public:
	void 
	operator () (T x) const
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename A, 
	typename B
	>
class CCastT
{
public:
	B 
	operator () (A x) const
	{ 
		return (B) x; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename A, 
	typename B
	>
class CImplicitCastT
{
public:
	B 
	operator () (A x) const
	{ 
		return x; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename A, 
	typename B, 
	size_t Offset
	>
class COffsetT
{
public:
	B* 
	operator () (A* p) const
	{ 
		return (B*) ((uchar_t*) p + Offset); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CConstructT
{
public:
	void 
	operator () (T* p) const
	{ 
		new (p) T; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CDestructT
{
public:
	void 
	operator () (T* p) const
	{ 
		p->~T (); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CReleaseT
{
public:
	void 
	operator () (T* p) const
	{ 
		p->Release (); 
	}
};

//.............................................................................

// relational functors

template <typename T>
class CEqualT
{
public:
	bool 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a == b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CNotEqualT
{
public:
	bool 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a != b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CLessT
{
public:
	bool 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a < b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CGreaterT
{
public:
	bool 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a > b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CLessOrEqualT
{
public:
	bool 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a <= b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CGreaterOrEqualT
{
public:
	bool 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a >= b; 
	}
};

//.............................................................................

// comparison functor

template <typename T>
class CCmpT
{
public:
	int 
	operator () (
		const T& a, 
		const T& b
		) const
	{ 
		return a < b ? -1 : a > b ? 1 : 0; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CCmpBinT
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

template <typename T>
class CCmpStringT
{
};

template <typename T>
class CCmpStringIT
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CCmpStringT <char>
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
class CCmpStringIT <char>
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
class CCmpStringT <wchar_t>
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
class CCmpStringIT <wchar_t>
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

typedef CCmpStringT <char>     CCmpStringA;
typedef CCmpStringT <wchar_t>  CCmpStringW;
typedef CCmpStringT <tchar_t>  CCmpString;
typedef CCmpStringIT <char>    CCmpStringIA;
typedef CCmpStringIT <wchar_t> CCmpStringIW;
typedef CCmpStringIT <tchar_t> CCmpStringI;

//.............................................................................

// hash functors

size_t
djb2(
	const void* _p,
	size_t Size
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CHashDjb2T
{
public:
	size_t 
	operator () (const T& Key) const
	{ 
		return djb2 (&Key, sizeof (Key)); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CHashIdT
{
public:
	size_t 
	operator () (T Key) const
	{ 
		return (size_t) Key; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
struct CHashStringT 
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CHashStringT <char>
{
public:
	size_t 
	operator () (const char* p) const
	{ 
		return p ? djb2 (p, strlen (p)) : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CHashStringT <wchar_t>
{
public:
	size_t 
	operator () (const wchar_t* p) const
	{ 
		return p ? djb2 (p, wcslen (p) * sizeof (wchar_t)) : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CHashStringT <char>    CHashStringA;
typedef CHashStringT <wchar_t> CHashStringW;
typedef CHashStringT <tchar_t> CHashString;

//.............................................................................

// size functors

template <typename T>
class CGetSizeT
{
public:
	size_t
	operator () (const T& x)
	{
		return sizeof (T);
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
