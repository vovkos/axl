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

#define _AXL_CF_STRING_H

#include "axl_cf_Type.h"
#include "axl_sl_String.h"

namespace axl {
namespace cf {

//..............................................................................

class String: public TypeBase<CFStringRef> {
public:
	String() {}

	String(const String& src):
		TypeBase<CFStringRef>(src) {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	String(String&& src) {
		m_p = src.m_p;
		src.m_p = NULL;
	}
#endif

	String(
		CFStringRef p,
		bool isAttach = false
	):
		TypeBase<CFStringRef>(p, isAttach) {}

	String(
		const char* p,
		size_t length = -1
	) {
		create(p, length);
	}

	String(const sl::StringRef& string) {
		create(string);
	}

	String&
	operator = (const String& src) {
		copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	String&
	operator = (String&& src) {
		move(std::move(src));
		return *this;
	}
#endif

	String&
	operator = (CFStringRef p) {
		copy(p);
		return *this;
	}

	String&
	operator = (const sl::StringRef& string) {
		create(string);
		return *this;
	}

	String&
	operator = (const char* p) {
		create(p);
		return *this;
	}

	bool
	create(CFStringRef src);

	bool
	create(
		const char* p,
		size_t length = -1
	);

	bool
	create(const sl::StringRef& string) {
		return create(string.cp(), string.getLength());
	}

	size_t
	getLength() const {
		ASSERT(m_p);
		return ::CFStringGetLength(m_p);
	}

	CFComparisonResult
	compare(
		CFStringRef p2,
		CFStringCompareFlags flags = 0
	) const {
		ASSERT(m_p);
		return ::CFStringCompare(m_p, p2, flags);
	}

	sl::String
	getString() const {
		ASSERT(m_p);
		return getStringFromCfString(m_p);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
