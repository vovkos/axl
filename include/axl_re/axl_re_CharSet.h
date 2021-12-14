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

#define _AXL_RE_CHARSET_H

#include "axl_re_Pch.h"

namespace axl {
namespace re {

//..............................................................................

class CharSet {
public:
	typedef sl::RbTreeIterator<utf32_t, utf32_t> Iterator;
	typedef sl::ConstRbTreeIterator<utf32_t, utf32_t> ConstIterator;

protected:
	sl::RbTree<utf32_t, utf32_t> m_map;

public:
	bool
	isEmpty() const {
		return m_map.isEmpty();
	}

	void
	clear() {
		m_map.clear();
	}

	size_t
	getCount() const {
		return m_map.getCount();
	}

	ConstIterator
	getHead() const {
		return m_map.getHead();
	}

	ConstIterator
	getTail() const {
		return m_map.getTail();
	}

	bool
	isSet(utf32_t c) const {
		ConstIterator it = m_map.find<sl::RelOpKind_Le>(c);
		return it && c <= it->m_value;
	}

	void
	add(utf32_t c);

	void
	add(
		utf32_t from,
		utf32_t to
	);

	void
	add(const CharSet& src);

	void
	invert();

	void
	copy(const CharSet& src);

	void
	getString(sl::String* string) const;

	sl::String
	getString() const {
		sl::String string;
		getString(&string);
		return string;
	}
};

void
appendCharString(
	sl::String* string,
	utf32_t c
);

inline
void
getCharString(
	sl::String* string,
	utf32_t c
) {
	string->clear();
	appendCharString(string, c);
}

void
appendCharRangeString(
	sl::String* string,
	utf32_t from,
	utf32_t to
);

void
getCharRangeString(
	sl::String* string,
	utf32_t from,
	utf32_t to
);

//..............................................................................

} // namespace re
} // namespace axl
