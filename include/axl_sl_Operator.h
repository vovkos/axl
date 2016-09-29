// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_SL_OPERATOR_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//.............................................................................

// cast operators

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

template <typename T>
class Void
{
public:
	void 
	operator () (T x) const
	{
	}
};

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

// sizeof & offsetof operators

template <typename T>
class SizeOf
{
public:
	size_t
	operator () (const T* x) const
	{
		return sizeof (T);
	}

	size_t
	operator () (const T& x) const
	{
		return sizeof (T);
	}
};

template <
	typename C, 
	typename B
	>
class OffsetOf
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

// offset (pointer increment) operator

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

//.............................................................................

// no-op (identity)

template <typename T>
class NoOp
{
public:
	T
	operator () (T a) const
	{ 
		return a; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// arithmetic operators (sometimes it's necessary to redefine Arg to "const T&")

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

// relational operators

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

} // namespace sl
} // namespace axl
