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

#define _AXL_CF_TYPE_H

#include "axl_cf_Pch.h"

namespace axl {
namespace cf {

//..............................................................................

sl::String
getStringFromCfString(CFStringRef cfString);

sl::String
cfTypeToString(
	CFTypeRef cfType,
	bool isVerbose = false
);

sl::String
getTypeIdDescription(CFTypeID typeId);

inline
CFTypeID
getStringTypeId() {
	return ::CFStringGetTypeID();
}

inline
CFTypeID
getBooleanTypeId() {
	return ::CFBooleanGetTypeID();
}

inline
CFTypeID
getNumberTypeId() {
	return ::CFNumberGetTypeID();
}

//..............................................................................

template <typename T>
class TypeBase {
protected:
	T m_p;

public:
	TypeBase() {
		m_p = NULL;
	}

	TypeBase(const TypeBase& src) {
		if (src.m_p)
			::CFRetain(src.m_p);

		m_p = src.m_p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	TypeBase(TypeBase&& src) {
		m_p = src.m_p;
		src.m_p = NULL;
	}
#endif

	TypeBase(
		T p,
		bool isAttach = false
	) {
		if (p && !isAttach)
			::CFRetain(p);

		m_p = p;
	}

	~TypeBase() {
		clear();
	}

	operator T() const {
		return m_p;
	}

	TypeBase&
	operator = (const TypeBase& src) {
		copy(src.m_p);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	TypeBase&
	operator = (TypeBase&& src) {
		move(std::move(src));
		return *this;
	}
#endif

	TypeBase&
	operator = (T p) {
		copy(p);
		return *this;
	}

	bool
	isNull() const {
		return !m_p;
	}

	T*
	p() {
		ASSERT(!m_p);
		return &m_p;
	}

	void
	attach(T p) {
		clear();
		m_p = p;
	}

	T
	detach() {
		T p = m_p;
		m_p = NULL;
		return p;
	}

	void
	clear() {
		if (m_p)
			::CFRelease(m_p);

		m_p = NULL;
	}

	void
	copy(T p) {
		if (m_p == p)
			return;

		if (p)
			::CFRetain(p);

		if (m_p)
			::CFRelease(m_p);

		m_p = p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(TypeBase&& src) {
		if (m_p)
			::CFRelease(m_p);

		m_p = src.m_p;
		src.m_p = NULL;
	}
#endif

	intptr_t
	getRetainCount() const {
		ASSERT(m_p);
		return ::CFGetRetainCount(m_p);
	}

	void
	retain() {
		ASSERT(m_p);
		::CFRetain(m_p);
	}

	void
	release() {
		ASSERT(m_p);
		::CFRelease(m_p);
	}

	bool
	isEqual(T type) const {
		ASSERT(m_p);
		return ::CFEqual(m_p, type);
	}

	CFHashCode
	getHash() const {
		ASSERT(m_p);
		return ::CFHash(m_p);
	}

	CFTypeID
	getTypeId() const {
		ASSERT(m_p);
		return ::CFGetTypeID(m_p);
	}

	sl::String
	getDescription() const {
		ASSERT(m_p);
		CFStringRef cfString = ::CFCopyDescription(m_p);
		sl::String string = getStringFromCfString(cfString);
		::CFRelease(cfString);
		return string;
	}

	sl::String
	getTypeIdDescription() const {
		return getTypeIdDescription(getTypeId());
	}

	void
	show() const {
		ASSERT(m_p);
		::CFShow(m_p);
	}

	sl::String
	toString() {
		ASSERT(m_p);
		return cfTypeToString(m_p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef TypeBase<CFTypeRef> Type;

//..............................................................................

} // namespace cf
} // namespace axl
