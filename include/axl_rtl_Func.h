// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_FUNC_H

#include "axl_mem_Factory.h"

namespace axl {
namespace rtl {

//.............................................................................

// consts

template <typename T>
class Void
{
public:
	void 
	operator () (T x) const
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class True
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
class False
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
class Zero
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
class MinusOne
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
class ImplicitCast
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
class ExplicitCast
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
	size_t offset
	>
class Offset
{
public:
	B* 
	operator () (A* p) const
	{ 
		return (B*) ((uchar_t*) p + offset); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename C, 
	typename B
	>
class OffsetOfClass 
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
class Construct
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
class Destruct
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
class New
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
class Delete
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
class CppNew
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
class CppDelete
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
class Release
{
public:
	void 
	operator () (T* p) const
	{ 
		p->release (); 
	}
};


//.............................................................................

// arithmetic functors (sometimes it's necessary to redefine TArg to "const T&")

template <
	typename T,
	typename Arg = T
	>
class Minus
{
public:
	T
	operator () (Arg a) const
	{ 
		return -a; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Not
{
public:
	T
	operator () (Arg a) const
	{ 
		return ~a; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Add
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a + b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Sub
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a - b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Mul
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a * b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Div
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a / b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Mod
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a % b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Shl
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a << b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Shr
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a >> b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class And
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a & b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Xor
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a ^ b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Or
{
public:
	T
	operator () (
		Arg a,
		Arg b
		) const
	{ 
		return a | b; 
	}
};

template <
	typename T,
	typename Arg = T
	>
class Min
{
public:
	T
	operator () (
		Arg a, 
		Arg b
		) const
	{
		return a < b ? a : b;
	}
};

template <
	typename T,
	typename Arg = T
	>
class Max
{
public:
	T
	operator () (
		Arg a, 
		Arg b
		) const
	{
		return a > b ? a : b;
	}
};

//.............................................................................

// relational functors

template <
	typename T,
	typename Arg = T
	>
class Eq
{
public:
	bool 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a == b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class Ne
{
public:
	bool 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a != b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class Lt
{
public:
	bool 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a < b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class Gt
{
public:
	bool 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a > b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class Le
{
public:
	bool 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a <= b; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class Ge
{
public:
	bool 
	operator () (
		Arg a, 
		Arg b
		) const
	{ 
		return a >= b; 
	}
};

//.............................................................................

// assign functors

template <
	typename T,
	typename Arg = T
	>
class Assign
{
public:
	void
	operator () (
		T& a, 
		Arg b
		) const
	{ 
		a = b;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Func,
	typename T,
	typename Arg = T
	>
class OpAssign
{
public:
	void
	operator () (
		T& a, 
		Arg b
		) const
	{ 
		a = Func () (a, b);
	}
};

//.............................................................................

// comparison functor

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

//.............................................................................

// hash functors

size_t
djb2 (
	const void* p,
	size_t size
	);

uint16_t
crc16(
	const void* p, 
	size_t size
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashDjb2
{
public:
	size_t 
	operator () (const T& key) const
	{ 
		return djb2 (&key, sizeof (key)); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashCrc16
{
public:
	size_t 
	operator () (const T& key) const
	{ 
		return crc16 (&key, sizeof (key)); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashId
{
public:
	size_t 
	operator () (T key) const
	{ 
		return (size_t) key; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashStringBase 
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashStringBase <char>
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
class HashStringBase <wchar_t>
{
public:
	size_t 
	operator () (const wchar_t* p) const
	{ 
		return p ? djb2 (p, wcslen (p) * sizeof (wchar_t)) : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef HashStringBase <char>    HashString;
typedef HashStringBase <wchar_t> HashString_w;
typedef HashStringBase <utf8_t>  HashString_utf8;
typedef HashStringBase <utf16_t> HashString_utf16;
typedef HashStringBase <utf32_t> HashString_utf32;

//.............................................................................

// size functors

template <typename T>
class GetSize
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
	typename Func,
	typename Result,
	typename Arg,
	typename ArgSave = Arg
	>
class BinClosure_1 
{
protected:
	ArgSave m_a;
	
public:
	BinClosure_1 (Arg a)
	{
		m_a = a;
	}

	Result
	operator () (Arg b) const
	{ 
		return Func () (m_a, b); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Func,
	typename Result,
	typename Arg,
	typename ArgSave = Arg
	>
class BinClosure_2
{
protected:
	ArgSave m_b;
	
public:
	BinClosure_2 (Arg b)
	{
		m_b = b;
	}

	Result
	operator () (Arg a) const
	{ 
		return Func () (a, m_b); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class AssignClosure_L
{
protected:
	T* m_p;
	
public:
	AssignClosure_L (T& a)
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
	typename Arg = T
	>
class AssignClosure_R
{
protected:
	T m_b;
	
public:
	AssignClosure_R (Arg b)
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
swap (
	T& a, 
	T& b
	)
{ 
	T t = a;
	a = b; 
	b = t;
}

template <typename T>
class Swap
{
public:
	void
	operator () (
		T& a, 
		T& b
		) const
	{ 
		swap (a, b);
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
