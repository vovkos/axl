// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_FOREACH_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Action
	>
void
loopArray (
	T* p,
	size_t count,
	Action action
	)
{
	T* end = p + count;

	for (; p < end; p++)
		action (*p);
}

template <
	typename Iterator,
	typename Action
	>
void
loopIterator (
	Iterator it,
	Action action
	)
{
	for (; it; it++)
		action (*it);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Filter,
	typename Action
	>
size_t
filteredLoopArray (
	T* p,
	size_t count,
	Filter filter,
	Action action
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
	typename Iterator,
	typename Filter,
	typename Action
	>
size_t
filteredLoopIterator (
	Iterator it,
	Filter filter,
	Action action
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

//..............................................................................

} // namespace sl
} // namespace axl
