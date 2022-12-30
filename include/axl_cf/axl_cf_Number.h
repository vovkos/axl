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

#define _AXL_CF_NUMBER_H

#include "axl_cf_Type.h"

namespace axl {
namespace cf {

//..............................................................................

class Number: public TypeBase<CFNumberRef> {
public:
	Number() {}

	Number(const Number& src):
		TypeBase<CFNumberRef>(src) {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Number(Number&& src):
		TypeBase<CFNumberRef>(std::move(src)) {}
#endif

	Number(
		CFNumberType type,
		const void* p
	) {
		create(type, p);
	}

	Number(
		CFNumberRef p,
		bool isAttach = false
	):
		TypeBase<CFNumberRef>(p, isAttach) {}

	Number(char x) {
		create(kCFNumberCharType, &x);
	}

	Number(short x) {
		create(kCFNumberShortType, &x);
	}

	Number(int x) {
		create(kCFNumberIntType, &x);
	}

	Number(long x) {
		create(kCFNumberLongType, &x);
	}

	Number(long long x) {
		create(kCFNumberLongLongType, &x);
	}

	Number(float x) {
		create(kCFNumberFloatType, &x);
	}

	Number(double x) {
		create(kCFNumberDoubleType, &x);
	}

	Number&
	operator = (const Number& src) {
		copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Number&
	operator = (Number&& src) {
		move(std::move(src));
		return *this;
	}
#endif

	Number&
	operator = (CFNumberRef p) {
		copy(p);
		return *this;
	}

	Number&
	operator = (char x) {
		create(kCFNumberCharType, &x);
		return *this;
	}

	Number&
	operator = (short x) {
		create(kCFNumberShortType, &x);
		return *this;
	}

	Number&
	operator = (int x) {
		create(kCFNumberIntType, &x);
		return *this;
	}

	Number&
	operator = (long x) {
		create(kCFNumberLongType, &x);
		return *this;
	}

	Number&
	operator = (long long x) {
		create(kCFNumberLongLongType, &x);
		return *this;
	}

	Number&
	operator = (float x) {
		create(kCFNumberFloatType, &x);
		return *this;
	}

	Number&
	operator = (double x) {
		create(kCFNumberDoubleType, &x);
		return *this;
	}

	bool
	create(
		CFNumberType type,
		const void* p
	);

	bool
	create(char x) {
		return create(kCFNumberCharType, &x);
	}

	bool
	create(short x) {
		return create(kCFNumberShortType, &x);
	}

	bool
	create(int x) {
		return create(kCFNumberIntType, &x);
	}

	bool
	create(long x) {
		return create(kCFNumberLongType, &x);
	}

	bool
	create(long long x) {
		return create(kCFNumberLongLongType, &x);
	}

	bool
	create(float x) {
		return create(kCFNumberFloatType, &x);
	}

	bool
	create(double x) {
		return create(kCFNumberDoubleType, &x);
	}

	bool
	isFloat() const {
		ASSERT(m_p);
		return ::CFNumberIsFloatType(m_p);
	}

	CFComparisonResult
	compare(CFNumberRef p2) const {
		ASSERT(m_p);
		return ::CFNumberCompare(m_p, p2, NULL);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
