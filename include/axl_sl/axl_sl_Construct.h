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

#define _AXL_SL_CONSTRUCT_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
void
construct(T* p)
{
	new(p)T;
}

template <
	typename T,
	typename Arg
	>
void
construct(
	T* p,
	Arg arg
	)
{
	new(p)T(arg);
}

template <
	typename T,
	typename Arg1,
	typename Arg2
	>
void
construct(
	T* p,
	Arg1 arg1,
	Arg2 arg2
	)
{
	new(p)T(arg1, arg2);
}

template <
	typename T,
	typename Arg1,
	typename Arg2,
	typename Arg3
	>
void
construct(
	T* p,
	Arg1 arg1,
	Arg2 arg2,
	Arg3 arg3
	)
{
	new(p)T(arg1, arg2, arg3);
}

template <
	typename T,
	typename Arg1,
	typename Arg2,
	typename Arg3,
	typename Arg4
	>
void
construct(
	T* p,
	Arg1 arg1,
	Arg2 arg2,
	Arg3 arg3,
	Arg4 arg4
	)
{
	new(p)T(arg1, arg2, arg3, arg4);
}

template <typename T>
void
destruct(T* p)
{
	p->~T();
}

// be careful to only use takeOver where its applicable

template <typename T>
void
takeOver(
	T* dst,
	T* src
	)
{
	if (dst == src)
		return;

	dst->~T();
	memcpy(dst, src, sizeof(T));
	new(src)T;
}

//..............................................................................

template <typename T>
class Construct
{
public:
	void
	operator () (T* p) const
	{
		new(p)T;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class Destruct
{
public:
	void
	operator () (T* p) const
	{
		p->~T();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
