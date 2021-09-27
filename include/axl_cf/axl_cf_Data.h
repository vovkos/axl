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

#define _AXL_CF_DATA_H

#include "axl_cf_Type.h"
#include "axl_sl_String.h"

namespace axl {
namespace cf {

//..............................................................................

class Data: public TypeBase<CFDataRef> {
public:
	Data() {}

	Data(const Data& src):
		TypeBase<CFDataRef>(src) {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Data(Data&& src):
		TypeBase<CFDataRef>(std::move(src)) {}
#endif

	Data(
		CFDataRef p,
		bool isAttach = false
	):
		TypeBase<CFDataRef>(p, isAttach) {}

	Data(
		const void* p,
		size_t size
	) {
		create(p, size);
	}

	Data&
	operator = (const Data& src) {
		copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Data&
	operator = (Data&& src) {
		move(std::move(src));
		return *this;
	}
#endif

	Data&
	operator = (CFDataRef p) {
		copy(p);
		return *this;
	}

	size_t
	getLength() const {
		ASSERT(m_p);
		return ::CFDataGetLength(m_p);
	}

	const uchar_t*
	getBytePtr() const {
		ASSERT(m_p);
		return ::CFDataGetBytePtr(m_p);
	}

	bool
	create(CFDataRef src);

	bool
	create(
		const void* p,
		size_t size
	);
};

//..............................................................................

} // namespace cf
} // namespace axl
