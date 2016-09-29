// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_SL_SWAP_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//.............................................................................

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

} // namespace sl
} // namespace axl
