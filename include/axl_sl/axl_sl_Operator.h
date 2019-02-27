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

#define _AXL_SL_OPERATOR_H

#include "axl_sl_ArgType.h"

namespace axl {
namespace sl {

//..............................................................................

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
		return (B)x;
	}
};

template <
	typename A,
	typename B
	>
class ImplicitPtrCast
{
public:
	B*
	operator () (A* x) const
	{
		return x;
	}

	const B*
	operator () (const A* x) const
	{
		return x;
	}
};

template <
	typename A,
	typename B
	>
class ExplicitPtrCast
{
public:
	B*
	operator () (A* x) const
	{
		return (B*)x;
	}

	const B*
	operator () (const A* x) const
	{
		return (const B*) x;
	}
};

//..............................................................................

// sizeof & offsetof operators

template <typename T>
class SizeOf
{
public:
	size_t
	operator () (const T* x) const
	{
		return sizeof(T);
	}

	size_t
	operator () (const T& x) const
	{
		return sizeof(T);
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
		return (size_t)p - 1;
	}
};

//..............................................................................

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
		return (B*)((uchar_t*)p + offset);
	}
};

//..............................................................................

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// arithmetic operators (sometimes it's necessary to redefine Arg to "const T&")

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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
	typename Arg = typename ArgType<T>::Type
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

//..............................................................................

// relational operators

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
	>
class EqDuckType
{
public:
	bool
	operator () (
		Arg a,
		Arg b
		) const
	{
		return a.isEqual(b);
	}

	bool
	operator () (
		const T* a,
		const T* b
		) const
	{
		return a->isEqual(*b);
	}
};

template <typename T>
class EqDuckTypePtr
{
public:
	bool
	operator () (
		const T& a,
		const T& b
		) const
	{
		return a.isEqual(b);
	}

	bool
	operator () (
		const T* a,
		const T* b
		) const
	{
		return a->isEqual(b);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

//..............................................................................

// assign functors

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Func,
	typename T,
	typename Arg = typename ArgType<T>::Type
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
		a = Func() (a, b);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
