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

#define _AXL_SL_SWAP_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
void
swap(
	T* a,
	T* b
	)
{
	T t = *a;
	*a = *b;
	*b = t;
}

template <typename T>
class Swap
{
public:
	void
	operator () (
		T* a,
		T* b
		) const
	{
		swap(a, b);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
