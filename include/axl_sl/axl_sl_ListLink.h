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

#define _AXL_SL_LISTLINK_H

namespace axl {
namespace sl {

//..............................................................................

struct ListLink {
	template <
		typename T,
		typename Entry,
		typename Link,
		typename GetLink
	>
	friend class IteratorBase;

	template <
		typename T,
		typename GetLink,
		typename Iterator,
		typename ConstIterator
	>
	friend class ListBase;

protected:
	ListLink* m_next;
	ListLink* m_prev;

public:
	ListLink*
	getNext() {
		return m_next;
	}

	ListLink*
	getPrev() {
		return m_prev;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY ListLink g_nullListLink; // we can't use { 0 } here, but it's
                                        // guaranteed to be zero-initialized

//..............................................................................

} // namespace sl
} // namespace axl
