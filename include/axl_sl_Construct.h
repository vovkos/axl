// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_SL_CONSTRUCT_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//.............................................................................

template <typename T>
void 
construct (T* p)
{
	new (p) T;
}

template <
	typename T,
	typename Arg
	>
void 
construct (
	T* p,
	Arg arg
	)
{
	new (p) T (arg);
}

template <
	typename T,
	typename Arg1,
	typename Arg2
	>
void 
construct (
	T* p,
	Arg1 arg1,
	Arg2 arg2
	)
{
	new (p) T (arg1, arg2);
}

template <
	typename T,
	typename Arg1,
	typename Arg2,
	typename Arg3
	>
void 
construct (
	T* p,
	Arg1 arg1,
	Arg2 arg2,
	Arg3 arg3
	)
{
	new (p) T (arg1, arg2, arg3);
}

template <
	typename T,
	typename Arg1,
	typename Arg2,
	typename Arg3,
	typename Arg4
	>
void 
construct (
	T* p,
	Arg1 arg1,
	Arg2 arg2,
	Arg3 arg3,
	Arg4 arg4
	)
{
	new (p) T (arg1, arg2, arg3, arg4);
}

template <typename T>
void 
destruct (T* p)
{
	p->~T ();
}

//.............................................................................

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

} // namespace sl
} // namespace axl
