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

#define _AXL_CF_ARRAY_H

#include "axl_cf_Type.h"

namespace axl {
namespace cf {

//..............................................................................

class Array: public TypeBase<CFArrayRef> {
public:
	Array() {}

	Array(const Array& src):
		TypeBase<CFArrayRef>(src) {}

	Array(Array&& src):
		TypeBase<CFArrayRef>(std::move(src)) {}

	Array(
		CFArrayRef p,
		bool isAttach = false
	):
		TypeBase<CFArrayRef>(p, isAttach) {}

	Array&
	operator = (const Array& src) {
		copy(src);
		return *this;
	}

	Array&
	operator = (Array&& src) {
		move(std::move(src));
		return *this;
	}

	Array&
	operator = (CFArrayRef p) {
		copy(p);
		return *this;
	}

	size_t
	getCount() const {
		ASSERT(m_p);
		return ::CFArrayGetCount(m_p);
	}

	const void*
	getValue(size_t index) const {
		ASSERT(m_p);
		return ::CFArrayGetValueAtIndex(m_p, index);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
