// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_LOOP_H

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
LoopArray (
	T* p,
	size_t Count,
	TAction Action
	)
{
	T* pEnd = p + Count;

	for (; p < pEnd; p++)
		Action (*p);
}

template <
	typename TIterator,
	typename TAction
	>
void
LoopIterator (
	TIterator It,
	TAction Action
	)
{
	for (; It; It++)
		Action (*It);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TFilter,
	typename TAction
	>
size_t
FilteredLoopArray (
	T* p,
	size_t Count,
	TFilter Filter,
	TAction Action
	)
{
	T* pEnd = p + Count;

	size_t FilterPassCount = 0;

	for (; p < pEnd; p++)
		if (Filter (*p))
		{
			Action (*p);
			FilterPassCount++;
		}

	return FilterPassCount;
}

template <
	typename TIterator,
	typename TFilter,
	typename TAction
	>
size_t
FilteredLoopIterator (
	TIterator It,
	TFilter Filter,
	TAction Action
	)
{
	size_t FilterPassCount = 0;

	for (; It; It++)
		if (Filter (*It))
		{
			Action (*It);
			FilterPassCount++;
		}

	return FilterPassCount;
}

//.............................................................................

} // namespace rtl
} // namespace axl
