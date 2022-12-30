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

#define _AXL_IOK_OBJECT_H

#include "axl_iok_Pch.h"
#include "axl_cf_String.h"

namespace axl {
namespace iok {

//..............................................................................

static
inline
sl::String
getSuperClass(const sl::StringRef& className) {
	cf::String cfSuperClassName;
	cfSuperClassName.attach(::IOObjectCopySuperclassForClass(cf::String(className)));
	return !cfSuperClassName.isNull() ? cfSuperClassName.getString() : sl::String();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class ObjectBase {
protected:
	T m_p;

public:
	ObjectBase() {
		m_p = MACH_PORT_NULL;
	}

	ObjectBase(const ObjectBase& src) {
		if (src.m_p)
			::IOObjectRetain(src.m_p);

		m_p = src.m_p;
	}

	ObjectBase(
		T p,
		bool isAttach = false
	) {
		if (p && !isAttach)
			::IOObjectRetain(p);

		m_p = p;
	}

	operator T() const {
		return m_p;
	}

	ObjectBase&
	operator = (const ObjectBase& src) {
		copy(src.m_p);
		return *this;
	}

	ObjectBase&
	operator = (T p) {
		copy(p);
		return *this;
	}

	T
	p() const {
		return m_p;
	}

	void
	attach(T p) {
		clear();
		m_p = p;
	}

	T
	detach() {
		T p = m_p;
		m_p = MACH_PORT_NULL;
		return p;
	}

	void
	clear() {
		if (m_p)
			::IOObjectRelease(m_p);

		m_p = MACH_PORT_NULL;
	}

	void
	copy(T p) {
		if (m_p == p)
			return;

		if (p)
			::IOObjectRetain(p);

		if (m_p)
			::IOObjectRelease(m_p);

		m_p = p;
	}

	void
	retain() {
		::IOObjectRetain(m_p);
	}

	void
	release() {
		::IOObjectRelease(m_p);
	}

	sl::String
	getClass() const;

	sl::String
	getSuperClass() const {
		return iok::getSuperClass(getClass());
	}

	bool
	conformsTo(const char* className) const {
		ASSERT(m_p);
		return ::IOObjectConformsTo(m_p, className) != 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
sl::String
ObjectBase<T>::getClass() const {
	ASSERT(m_p);
	io_name_t buffer;
	buffer[0] = 0;
	::IOObjectGetClass(m_p, buffer);
	return buffer;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectBase<io_object_t> Object;

//..............................................................................

} // namespace iok
} // namespace axl
