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

#define _AXL_SL_FENWICKTREE_H

#include "axl_sl_Array.h"
#include "axl_sl_BitIdx.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename ValueArg0 = ArgType<T>
>
class FenwickTree {
public:
	typedef T Value;
	typedef ValueArg0 ValueArg;

protected:
	Array<T> m_array; // m_array[i] = sum(i + 1 - getLoBit(i + 1) .. i)

public:
	size_t
	getCount() const {
		return m_array.getCount();
	}

	bool
	isEmpty() const {
		return m_array.isEmpty();
	}

	void
	clear() {
		m_array.clear();
	}

	bool
	reserve(size_t count) {
		return m_array.reserve(count);
	}

	size_t
	append(ValueArg value) {
		size_t i = m_array.getCount();
		bool result = m_array.setCount(i + 1);
		if (!result)
			return -1;

		Value sum = value;
		size_t j = i + 1; // 1-based index
		size_t base = j - getLoBit(j); // 1-based left end of the affected range
		for (j--; j != base; j &= j - 1)
			sum += m_array[j - 1];

		m_array.rwi()[i] = sum;
		return i;
	}

	void
	inc(
		size_t i,
		ValueArg delta
	) {
		size_t count = m_array.getCount();
		typename Array<T>::Rwi rwi = m_array.rwi();
		for (; i < count; i += getLoBit(i + 1))
			rwi[i] += delta;
	}

	void
	dec(
		size_t i,
		ValueArg delta
	) {
		inc(i, -delta);
	}

	// calculates sum(0 .. i)

	Value
	calcStat() const {
		return calcStat<Value>();
	}

	Value
	calcStat(size_t index) const {
		return calcStat<Value>(index);
	}

	template <typename Partial>
	Partial
	calcStat() const {
		return isEmpty() ? Partial(Value()) : calcStat<Partial>(m_array.getCount() - 1);
	}

	template <typename Partial>
	Partial
	calcStat(size_t i) const {
		Partial stat = m_array[i];
		size_t j = i + 1; // 1-based index
		for (j &= j - 1; j; j &= j - 1)
			stat += m_array[j - 1];

		return stat;
	}

	// finds smallest i so that targetStat <= sum(0 .. i); returns -1 if not found
	// sum must be monotonic non-decreasing for this to work

	size_t
	findByStat(
		ValueArg targetStat,
		Value* remainder = NULL
	) const {
		return findByStat<Value, ValueArg>(targetStat, remainder);
	}

	// this overload can operate on a specific field of a multi-field value

	template <
		typename Partial,
		typename PartialArg = ArgType<Partial>
	>
	size_t
	findByStat(
		PartialArg targetStat,
		Partial* remainder = NULL
	) const {
		if (m_array.isEmpty())
			return -1;

		size_t count = m_array.getCount();
		size_t base = 0; // 0-based start of search range
		Partial stat = targetStat;

		for (size_t bit = getHiBit(count); bit; bit >>= 1) {
			size_t i = base + bit - 1;
			if (i >= count)
				continue;

			Partial left = m_array[i];
			if (left < stat) { // target beyond i, go right
				stat -= left;
				base += bit;
			}
		}

		if (base >= count)
			return -1;

		if (remainder)
			*remainder = stat;

		return base;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
