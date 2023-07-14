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

#define _AXL_SL_BOYERMOORESKIPTABLES_H

#include "axl_sl_Array.h"

namespace axl {
namespace sl {

//..............................................................................

// skip tables: Boyer-Moore-Horspool (bad-skip only) vs Boyer-Moore (good-skip)

class BoyerMooreSkipTableRoot {
protected:
	sl::Array<size_t> m_table;

public:
	void
	clear() {
		m_table.clear();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename C>
class BoyerMooreBadSkipTableBase: public BoyerMooreSkipTableRoot {
public:
	bool
	build(
		const C* pattern,
		size_t patternLength,
		size_t tableSize
	) {
		bool result = m_table.setCount(tableSize);
		if (!result)
			return false;

		for (size_t i = 0; i < tableSize; i++)
			m_table[i] = patternLength;

		intptr_t last = patternLength - 1;
		for (intptr_t i = 0, j = last; i < last; i++, j--)
			m_table[(size_t)pattern[i] % tableSize] = j;

		return true;
	}

	size_t
	getSkip(C c) const {
		return m_table[(size_t)c % m_table.getCount()];
	}

	size_t
	getSkip(
		C c,
		size_t patternPos // unused
	) const {
		return getSkip(c);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename C>
class BoyerMooreGoodSkipTableBase: public BoyerMooreSkipTableRoot {
public:
	bool
	build(
		const C* pattern,
		size_t patternLength
	) {
		bool result = m_table.setCount(patternLength);
		if (!result)
			return false;

		intptr_t lastPrefixPos = patternLength - 1;
		for (intptr_t i = patternLength - 1; i >= 0; i--) {
			if (isPrefix(pattern, patternLength, i + 1))
				lastPrefixPos = i + 1;

			m_table[i] = lastPrefixPos + patternLength - 1 - i;
		}

		for (intptr_t i = 0; i < patternLength - 1; i++) {
			size_t suffixLength = calcSuffixLength(pattern, patternLength, i);
			if (pattern[i - suffixLength] != pattern[patternLength - 1 - suffixLength])
				m_table[patternLength - 1 - suffixLength] = patternLength - 1 - i + suffixLength;
		}

		return true;
	}

	size_t
	getSkip(size_t patternPos) const {
		return m_table[patternPos];
	}

protected:
	static
	bool
	isPrefix(
		const C* pattern,
		size_t patternLength,
		intptr_t pos
	) {
		intptr_t suffixLength = patternLength - pos;
		for (intptr_t i = 0, j = pos; i < suffixLength; i++, j++)
			if (pattern[i] != pattern[j])
				return false;

		return true;
	}

	static
	size_t
	calcSuffixLength(
		const C* pattern,
		size_t patternLength,
		intptr_t pos
	) {
		intptr_t i = 0;
		intptr_t j = pos;
		intptr_t k = patternLength - 1;

		while (i < pos && pattern[j] == pattern[k]) {
			i++;
			j--;
			k--;
		}

		return i;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename C>
class BoyerMooreSkipTablesBase {
protected:
	BoyerMooreBadSkipTableBase<C> m_badSkipTable;
	BoyerMooreGoodSkipTableBase<C> m_goodSkipTable;

public:
	void
	clear() {
		m_badSkipTable.clear();
		m_goodSkipTable.clear();
	}

	bool
	build(
		const C* pattern,
		size_t patternLength,
		size_t badSkipTableSize
	) {
		return
			m_badSkipTable.build(pattern, patternLength, badSkipTableSize) &&
			m_goodSkipTable.build(pattern, patternLength);
	}

	size_t
	getSkip(
		C c,
		size_t patternPos
	) const {
		size_t badSkip = m_badSkipTable.getSkip(c);
		size_t goodSkip = m_goodSkipTable.getSkip(patternPos);
		return AXL_MAX(badSkip, goodSkip);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
