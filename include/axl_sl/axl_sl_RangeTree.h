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

#define _AXL_SL_RANGETREE_H

#include "axl_sl_RbTree.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Offset0,
	typename Value0,
	typename ValueArg0 = typename ArgType<Value0>::Type
>
class RangeTreeValue {
	template <
		typename Offset2,
		typename Value2,
		typename Eq,
		typename ValueArgs2
	>
	friend class RangeTree;

public:
	typedef Offset0 Offset;
	typedef Value0 Value;
	typedef ValueArg0 ValueArg;

protected:
	Offset m_endOffset;

public:
	Value m_value;

public:
	RangeTreeValue():
		m_endOffset(),
		m_value() {}

	Value& operator * () {
		return m_value;
	}

	const Value& operator * () const {
		return m_value;
	}

	Value* operator -> () {
		return &m_value;
	}

	const Value* operator -> () const {
		return &m_value;
	}

	Offset
	getEndOffset() const {
		return m_endOffset;
	}

	const Value&
	getValue() const {
		return m_value;
	}

	Value&
	getValue() {
		return m_value;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Offset, // integer type (size_t, uint64_t, intptr_t, etc)
	typename Value0,
	typename ValueEq = Eq<Value0>,
	typename ValueArg0 = typename ArgType<Value0>::Type
>
class RangeTree: public RbTree<
	Offset,
	RangeTreeValue<Offset, Value0, ValueArg0>,
	Lt<Offset>,
	std::tuple<Offset, ValueArg0>
> {
public:
	typedef Value0 Value;
	typedef ValueArg0 ValueArg;

	typedef RbTree<
		Offset,
		RangeTreeValue<Offset, Value, ValueArg>,
		Lt<Offset>,
		std::tuple<Offset, ValueArg>
	> Tree;

	typedef typename Tree::Iterator Iterator;
	typedef typename Tree::ConstIterator ConstIterator;

public:
	ConstIterator
	addRange(
		Offset offset,
		Offset length,
		ValueArg value
	) {
		ASSERT(length);
		Offset endOffset = offset + length;

		Iterator it = this->template find<RelOpKind_Le>(offset);
		if (!it || it->m_value.m_endOffset < offset) { // no intersections with preceding ranges, add a new one
			it = addPoint(offset, endOffset, value);
			normalize(it);
		} else if (ValueEq()(it->m_value.m_value, value)) { // intersection with the same value, reuse the old range
			if (it->m_value.m_endOffset >= endOffset) // the new range is completely covered by the old one, noop
				return it;

			it->m_value.m_endOffset = endOffset; // need to extend the old one
			normalize(it);
		} else if (it->getKey() == offset) { // intersection with a different value (same offset)
			Iterator prevIt = it.getPrev();
			if (prevIt && prevIt->m_value.m_endOffset == offset && ValueEq()(prevIt->m_value.m_value, value)) { // merge with the preceding range
				it = prevIt;
				it->m_value.m_endOffset = endOffset;
				normalize(it);
			} else if (it->m_value.m_endOffset <= endOffset) { // the whole old range is completely covered by the new one
				it->m_value.m_endOffset = endOffset;
				it->m_value.m_value = value;
				normalize(it);
			} else { // split the old range in two
				addPoint(endOffset, it->m_value.m_endOffset, it->m_value.m_value);
				it->m_value.m_endOffset = endOffset;
				it->m_value.m_value = value;
				// no need to normalize
			}
		} else { // intersection with a different value (different offsets)
			if (it->m_value.m_endOffset <= endOffset) { // the end of the old range is completely covered by the new one
				it->m_value.m_endOffset = offset; // head of the old range
				it = addPoint(offset, endOffset, value);
				normalize(it);
			} else { // split the old range in three
				addPoint(endOffset, it->m_value.m_endOffset, it->m_value.m_value); // tail of the old range
				it->m_value.m_endOffset = offset; // head of the old range
				it = addPoint(offset, endOffset, value);
				// no need to normalize
			}
		}

		return it;
	}

protected:
	Iterator
	addPoint(
		Offset offset,
		Offset endOffset,
		ValueArg value
	) {
		Iterator it = this->visit(offset);
		it->m_value.m_endOffset = endOffset;
		it->m_value.m_value = value;
		return it;
	}

	void
	normalize(Iterator it) {
		Iterator nextIt = it.getNext();
		while (nextIt) {
			if (nextIt->m_value.m_endOffset <= it->m_value.m_endOffset) // the old range is completely covered, erase
				this->erase(nextIt++);
			else { // spans beyond
				if (nextIt->getKey() > it->m_value.m_endOffset) // no intersection
					break;

				if (!ValueEq()(nextIt->m_value.m_value, it->m_value.m_value)) // different value, adjust the old range
					this->adjustKey(*nextIt, it->m_value.m_endOffset);
				else { // same value, combine ranges and erase the old one
					it->m_value.m_endOffset = nextIt->m_value.m_endOffset;
					this->erase(nextIt);
				}

				break;
			}
		}
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
