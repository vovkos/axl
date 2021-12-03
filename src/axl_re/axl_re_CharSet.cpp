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

#include "pch.h"
#include "axl_re_CharSet.h"

namespace axl {
namespace re {

//..............................................................................

void
CharSet::add(utf32_t c) {
	ASSERT(c >= 0);

	Iterator it = m_map.find(c, sl::BinTreeFindRelOp_Le);
	if (!it || it->m_value + 1 < c)
		it = m_map.visit(c);
	else if (it->m_value >= c)
		return;

	Iterator nextIt = it.getNext();

	if (!nextIt || nextIt->getKey() != c + 1)
		it->m_value = c;
	else { // merge with the next
		it->m_value = nextIt->m_value;
		m_map.erase(nextIt);
	}
}

void
CharSet::add(
	utf32_t from,
	utf32_t to
) {
	ASSERT(from >= 0 && from <= to);

	Iterator it = m_map.find(from, sl::BinTreeFindRelOp_Le);
	if (!it || it->m_value + 1 < from)
		it = m_map.visit(from);
	else if (it->m_value >= to)
		return;

	// delete all overlapped nodes

	for (;;) {
		Iterator nextIt = it.getNext();
		if (!nextIt || nextIt->getKey() > to + 1)
			break;

		if (nextIt->m_value > to)
			to = nextIt->m_value;

		m_map.erase(nextIt);
	}

	ASSERT(it->m_value < to);
	it->m_value = to;
}

void
CharSet::add(const CharSet& src) {
	ConstIterator it = src.m_map.getHead();
	for (; it; it++)
		add(it->getKey(), it->m_value);
}

void
CharSet::invert() {
	sl::RbTree<utf32_t, utf32_t> origin;
	sl::takeOver(&origin, &m_map);

	ConstIterator it = origin.getHead();
	if (!it)
		return;

	utf32_t c = it->getKey();

	if (c > 0)
		add(0, c - 1);

	c = it->m_value;

	for (it++; it; it++) {
		add(c + 1, it->getKey() - 1);
		c = it->m_value;
	}

	if (c < 0x7fffffff)
		add(c + 1, 0x7fffffff);
}

void
CharSet::copy(const CharSet& src) {
	m_map.clear();

	ConstIterator it = src.m_map.getHead();
	for (; it; it++)
		m_map.add(it->getKey(), it->m_value);
}

void
CharSet::getString(sl::String* string) const {
	string->clear();

	ConstIterator it = m_map.getHead();
	for (; it != 0; it++)
		appendCharRangeString(string, it->getKey(), it->m_value);
}

//..............................................................................

void
appendCharString(
	sl::String* string,
	utf32_t c
) {
	if (enc::isPrintable(c) && c < 0x80)
		string->append(c);
	else if (c <= 0xffff)
		string->appendFormat("\\u%04x", c);
	else
		string->appendFormat("\\U%08x", c);
}

void
appendCharRangeString(
	sl::String* string,
	utf32_t from,
	utf32_t to
) {
	if (from == to)
		appendCharString(string, from);
	else {
		appendCharString(string, from);
		*string += '-';
		appendCharString(string, to);
	}
}

void
getCharRangeString(
	sl::String* string,
	utf32_t from,
	utf32_t to
) {
	string->clear();
	if (from == to) {
		*string = '\'';
		appendCharRangeString(string, from, to);
		*string += '\'';
	} else {
		*string = '[';
		appendCharRangeString(string, from, to);
		*string += ']';
	}
}

//..............................................................................

} // namespace re
} // namespace axl
