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

#define _AXL_SL_PTRITERATOR_H

#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	bool IsReverse = false
>
class PtrIterator {
protected:
	T* m_p;

public:
	PtrIterator(T* p = NULL) {
		m_p = p;
	}

	operator T* () const {
		return m_p;
	}

	T&
	operator [] (intptr_t i) const {
		return *inc(m_p, i);
	}

	bool
	operator == (T* p) {
		return m_p == p;
	}

	bool
	operator != (T* p) {
		return m_p != p;
	}

	bool
	operator < (T* p) {
		return IsReverse ? m_p > p : m_p < p;
	}

	bool
	operator <= (T* p) {
		return IsReverse ? m_p >= p : m_p <= p;
	}

	bool
	operator > (T* p) {
		return IsReverse ? m_p < p : m_p > p;
	}

	bool
	operator >= (T* p) {
		return IsReverse ? m_p <= p : m_p >= p;
	}

	PtrIterator&
	operator ++ () {
		return next();
	}

	PtrIterator&
	operator -- () {
		return prev();
	}

	PtrIterator
	operator ++ (int) { // post increment
		PtrIterator old = *this;
		inc(1);
		return old;
	}

	PtrIterator
	operator -- (int) { // post decrement
		PtrIterator old = *this;
		inc(-1);
		return old;
	}

	PtrIterator&
	operator += (intptr_t count) {
		return inc(count);
	}

	PtrIterator&
	operator -= (intptr_t count) {
		return inc(-count);
	}

	PtrIterator
	operator + (intptr_t count) const {
		return getInc(count);
	}

	PtrIterator
	operator - (intptr_t count) const {
		return getInc(-count);
	}

	intptr_t
	operator - (T* p) const {
		return sub(m_p, p);
	}

	PtrIterator
	getNext() const {
		return PtrIterator(inc(m_p, 1));
	}

	PtrIterator
	getPrev() const {
		return PtrIterator(inc(m_p, -1));
	}

	PtrIterator
	getInc(intptr_t count) const {
		return PtrIterator(inc(m_p, count));
	}

	PtrIterator&
	next() {
		return inc(1);
	}

	PtrIterator&
	prev() {
		return inc(-1);
	}

	PtrIterator&
	inc(intptr_t count) {
		m_p = inc(m_p, count);
		return *this;
	}

	static
	T*
	inc(
		T* p,
		intptr_t count
	) {
		return IsReverse ? p - count : p + count;
	}

	static
	size_t
	sub(
		T* p1,
		T* p2
	) {
		return IsReverse ? p2 - p1 : p1 - p2;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class PtrReverseIterator: PtrIterator<T, true> {
public:
	PtrReverseIterator(T* p = NULL):
		PtrIterator(p) {}
};

//..............................................................................

} // namespace sl
} // namespace axl
