// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_FOREACH_H

#include "axl_rtl_Func.h"
#include "axl_rtl_List.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename T,
	typename TAction
	>
void
loopArray (
	T* p,
	size_t count,
	TAction action
	)
{
	T* end = p + count;

	for (; p < end; p++)
		action (*p);
}

template <
	typename TIterator,
	typename TAction
	>
void
loopIterator (
	TIterator it,
	TAction action
	)
{
	for (; it; it++)
		action (*it);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TFilter,
	typename TAction
	>
size_t
filteredLoopArray (
	T* p,
	size_t count,
	TFilter filter,
	TAction action
	)
{
	T* end = p + count;

	size_t filterPassCount = 0;

	for (; p < end; p++)
		if (filter (*p))
		{
			action (*p);
			filterPassCount++;
		}

	return filterPassCount;
}

template <
	typename TIterator,
	typename TFilter,
	typename TAction
	>
size_t
filteredLoopIterator (
	TIterator it,
	TFilter filter,
	TAction action
	)
{
	size_t filterPassCount = 0;

	for (; it; it++)
		if (filter (*it))
		{
			action (*it);
			filterPassCount++;
		}

	return filterPassCount;
}

//.............................................................................

} // namespace rtl
} // namespace axl
