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
#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Arg0 = ArgType<T>
>
class FenwickTree {
public:
	typedef T Stat;
	typedef Arg0 StatArg;

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
	append(StatArg value) {
		size_t i = m_array.getCount();
		bool result = m_array.setCount(i + 1);
		if (!result)
			return -1;

		Stat sum = value;
		size_t j = i + 1; // 1-based index
		size_t base = j - getLoBit(j); // 1-based left end of the affected range
		for (j--; j != base; j &= j - 1)
			sum += m_array[j - 1];

		m_array.rwi()[i] = sum;
		return i;
	}

	// grand total

	Stat
	calcFullStat() const {
		return calcFullSubStat<Stat>();
	}

	template <typename SubStat>
	SubStat
	calcFullSubStat() const {
		return isEmpty() ?
			SubStat() :
			calcPrefixSubStat<SubStat>(m_array.getCount() - 1);
	}

	// inclusive prefix: sum(0 .. i)

	Stat
	calcPrefixStat(size_t index) const {
		return calcPrefixSubStat<Stat>(index);
	}

	template <typename SubStat>
	SubStat
	calcPrefixSubStat(size_t i) const {
		SubStat stat = m_array[i];
		size_t j = i + 1; // 1-based index
		for (j &= j - 1; j; j &= j - 1)
			stat += m_array[j - 1];

		return stat;
	}

	// the stat of exactly this element -- the inverse of what append() summed in

	Stat
	calcSelfStat(size_t index) const {
		return calcSelfSubStat<Stat>(index);
	}

	template <typename SubStat>
	SubStat
	calcSelfSubStat(size_t i) const {
		SubStat stat = m_array[i];
		size_t j = i + 1; // 1-based index
		size_t base = j - getLoBit(j); // 1-based left end of the affected range
		for (j--; j != base; j &= j - 1)
			stat -= m_array[j - 1];

		return stat;
	}

	// findPrefixStatGe -- first element whose prefix is >= arg, -1 if past the end

	size_t
	findPrefixStatGe(
		StatArg targetStat,
		Stat* offset = NULL
	) const {
		return findPrefixStatGeImpl<Stat, StatArg>(targetStat, offset);
	}

	// this descends on a single field of a multi-field stat

	template <typename SubStat>
	size_t
	findPrefixSubStatGe(
		const SubStat& targetStat,
		SubStat* offset = NULL
	) const {
		return findPrefixStatGeImpl<SubStat, const SubStat&>(targetStat, offset);
	}

	// findPrefixStatEq(arg): the element whose prefix is exactly arg, -1 if none

	size_t
	findPrefixStatEq(StatArg targetStat) const {
		return findPrefixStatEqImpl<Stat, StatArg>(targetStat);
	}

	template <typename SubStat>
	size_t
	findPrefixSubStatEq(const SubStat& targetStat) const {
		return findPrefixStatEqImpl<SubStat, const SubStat&>(targetStat);
	}

	// modification

	void
	addStat(
		size_t i,
		StatArg delta
	) {
		modifyStat<AddAssign<Stat, StatArg>, StatArg>(i, delta);
	}

	template <typename SubStat>
	void
	addSubStat(
		size_t i,
		const SubStat& delta
	) {
		modifyStat<AddAssign<Stat, const SubStat&>, const SubStat&>(i, delta);
	}

	void
	subStat(
		size_t i,
		StatArg delta
	) {
		modifyStat<SubAssign<Stat, StatArg>, StatArg>(i, delta);
	}

	template <typename SubStat>
	void
	subSubStat(
		size_t i,
		const SubStat& delta
	) {
		modifyStat<SubAssign<Stat, const SubStat&>, const SubStat&>(i, delta);
	}

protected:
	template <
		typename Stat2,
		typename StatArg2
	>
	size_t
	findPrefixStatGeImpl(
		StatArg2 targetStat,
		Stat2* offset
	) const {
		if (m_array.isEmpty())
			return -1;

		size_t count = m_array.getCount();
		size_t base = 0; // 0-based start of search range
		Stat2 stat = targetStat;

		for (size_t bit = getHiBit(count); bit; bit >>= 1) {
			size_t i = base + bit - 1;
			if (i >= count)
				continue;

			Stat2 left = m_array[i];
			if (left < stat) { // target beyond i, go right
				stat -= left;
				base += bit;
			}
		}

		if (base >= count) // targetStat is past the grand total
			return -1;

		if (offset)
			*offset = stat;

		return base;
	}

	template <
		typename Stat2,
		typename StatArg2
	>
	size_t
	findPrefixStatEqImpl(StatArg2 targetStat) const {
		Stat2 offset;
		size_t i = findPrefixStatGeImpl<Stat2, StatArg2>(targetStat, &offset);
		return i != -1 && offset == calcSelfSubStat<Stat2>(i) ? i : -1;
	}

	template <
		typename Op,
		typename StatArg2
	>
	void
	modifyStat(
		size_t i,
		StatArg2 delta
	) {
		size_t count = m_array.getCount();
		typename Array<T>::Rwi rwi = m_array.rwi();
		for (; i < count; i += getLoBit(i + 1))
			Op()(rwi[i], delta);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
