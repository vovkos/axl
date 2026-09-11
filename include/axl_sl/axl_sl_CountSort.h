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

#define _AXL_SL_COUNTSORT_H

#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

// stable distribution sort for dense integer keys in [0, limit]

// O(count + limit) with no comparisons -- great for sorting arrays of indexes!

// on return, indexes[] hold indexes of each key in the sorted array

template <
	typename T,
	typename GetKey = ImplicitCast<T, size_t>
>
void
countSort(
	T* dst,          // out: T[count] (can't alias with src!)
	const T* src,    // in:  T[count]
	size_t count,
	size_t* indexes, // out: size_t[limit + 1]
	size_t limit
) {
	memset(indexes, 0, (limit + 1) * sizeof(size_t));

	for (size_t i = 0; i < count; i++) {
		size_t key = GetKey()(src[i]);
		ASSERT(key <= limit);
		indexes[key]++; // count each occurence of the key
	}

	for (size_t i = 1; i <= limit; i++)
		indexes[i] += indexes[i - 1]; // compute prefix sum for the key

	for (intptr_t i = count - 1; i >= 0; i--) {
		size_t key = GetKey()(src[i]);
		size_t j = --indexes[key]; // compute index of the key
		dst[j] = src[i];
	}
}

//..............................................................................

} // namespace sl
} // namespace axl
