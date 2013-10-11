// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_FUNC_H

#include "axl_mem_Factory.h"

namespace axl {
namespace rtl {

//.............................................................................

// consts

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

template <typename T>
class CTrueT
{
public:
	T
	operator () () const
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CFalseT
{
public:
	T
	operator () () const
	{
		return false;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CZeroT
{
public:
	T
	operator () () const
	{
		return 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CMinusOneT
{
public:
	T
	operator () () const
	{
		return (T) -1;
	}
};

//.............................................................................

// casts

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
	typename B
	>
class CExplicitCastT
{
public:
	B 
	operator () (A x) const
	{ 
		return (B) x; 
	}
};

//.............................................................................

// offsets

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

template <
	typename C, 
	typename B
	>
class COffsetOfClassT 
{
public:
	size_t 
	operator () () const
	{
		B* p = (C*) 1;
		return (size_t) p - 1;
	}
};

//.............................................................................

// construct / destruct
 
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

//.............................................................................

// new / delete

template <typename T>
class CNewT
{
public:
	void 
	operator () () const
	{ 
		return AXL_MEM_NEW (T); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CDeleteT
{
public:
	void 
	operator () (T* p) const
	{ 
		AXL_MEM_DELETE (p); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CCppNewT
{
public:
	void 
	operator () () const
	{ 
		return new T; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CCppDeleteT
{
public:
	void 
	operator () (T* p) const
	{ 
		delete p; 
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

// arithmetic functors (sometimes it's necessary to redefine TArg to "const T&")

template <
	typename T,
	typename TArg = T
	>
class CMinusT
{
public:
	T
	operator () (TArg a) const
	{ 
		return -a; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CNotT
{
public:
	T
	operator () (TArg a) const
	{ 
		return ~a; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CAddT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a + b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CSubT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a - b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CMulT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a * b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CDivT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a / b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CModT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a % b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CShlT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a << b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CShrT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a >> b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CAndT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a & b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CXorT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a ^ b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class COrT
{
public:
	T
	operator () (
		TArg a,
		TArg b
		) const
	{ 
		return a | b; 
	}
};

template <
	typename T,
	typename TArg = T
	>
class CMinT
{
public:
	T
	operator () (
		TArg a, 
		TArg b
		) const
	{
		return a < b ? a : b;
	}
};

template <
	typename T,
	typename TArg = T
	>
class CMaxT
{
public:
	T
	operator () (
		TArg a, 
		TArg b
		) const
	{
		return a > b ? a : b;
	}
};

//.............................................................................

// relational functors

template <
	typename T,
	typename TArg = T
	>
class CEqT
{
public:
	bool 
	operator () (
		TArg a, 
		TArg b
		) const
	{ 
		return a == b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CNeT
{
public:
	bool 
	operator () (
		TArg a, 
		TArg b
		) const
	{ 
		return a != b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CLtT
{
public:
	bool 
	operator () (
		TArg a, 
		TArg b
		) const
	{ 
		return a < b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CGtT
{
public:
	bool 
	operator () (
		TArg a, 
		TArg b
		) const
	{ 
		return a > b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CLeT
{
public:
	bool 
	operator () (
		TArg a, 
		TArg b
		) const
	{ 
		return a <= b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CGeT
{
public:
	bool 
	operator () (
		TArg a, 
		TArg b
		) const
	{ 
		return a >= b; 
	}
};

//.............................................................................

// assign functors

template <
	typename T,
	typename TArg = T
	>
class CAssignT
{
public:
	void
	operator () (
		T& a, 
		TArg b
		) const
	{ 
		a = b;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TFunc,
	typename T,
	typename TArg = T
	>
class COpAssignT
{
public:
	void
	operator () (
		T& a, 
		TArg b
		) const
	{ 
		a = TFunc () (a, b);
	}
};

//.............................................................................

// comparison functor

template <
	typename T,
	typename TArg = T
	>
class CCmpT
{
public:
	int 
	operator () (
		TArg a, 
		TArg b
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
class CCmpStringT_i
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
class CCmpStringT_i <char>
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
class CCmpStringT_i <wchar_t>
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

typedef CCmpStringT <char>      CCmpString;
typedef CCmpStringT <wchar_t>   CCmpString_w;
typedef CCmpStringT_i <char>    CCmpString_i;
typedef CCmpStringT_i <wchar_t> CCmpString_wi;

//.............................................................................

// hash functors

size_t
djb2 (
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
class CHashStringT 
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

typedef CHashStringT <char>    CHashString;
typedef CHashStringT <wchar_t> CHashString_w;

//.............................................................................

// size functors

template <typename T>
class CGetSizeT
{
public:
	size_t
	operator () (const T& x) const
	{
		return sizeof (T);
	}
};

//.............................................................................

// closure functors

template <
	typename TFunc,
	typename TResult,
	typename TArg,
	typename TArgSave = TArg
	>
class CBinClosureT_1 
{
protected:
	TArgSave m_a;
	
public:
	CBinClosureT_1 (TArg a)
	{
		m_a = a;
	}

	TResult
	operator () (TArg b) const
	{ 
		return TFunc () (m_a, b); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TFunc,
	typename TResult,
	typename TArg,
	typename TArgSave = TArg
	>
class CBinClosureT_2
{
protected:
	TArgSave m_b;
	
public:
	CBinClosureT_2 (TArg b)
	{
		m_b = b;
	}

	TResult
	operator () (TArg a) const
	{ 
		return TFunc () (a, m_b); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CAssignClosureT_L
{
protected:
	T* m_p;
	
public:
	CAssignClosureT_L (T& a)
	{
		m_p = &a;
	}

	void
	operator () (T& b) const
	{ 
		*m_p = b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TArg = T
	>
class CAssignClosureT_R
{
protected:
	T m_b;
	
public:
	CAssignClosureT_R (TArg b)
	{
		m_b = b;
	}

	void
	operator () (T& a) const
	{ 
		a = m_b; 
	}
};

//.............................................................................

// swap function & functor

template <typename T>
void
Swap (
	T& a, 
	T& b
	)
{ 
	T t = a;
	a = b; 
	b = t;
}

template <typename T>
class CSwapT
{
public:
	void
	operator () (
		T& a, 
		T& b
		) const
	{ 
		Swap (a, b);
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
