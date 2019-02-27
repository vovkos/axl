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
getStringTypeId()
{
	return ::CFStringGetTypeID();
}

inline
CFTypeID
getBooleanTypeId()
{
	return ::CFBooleanGetTypeID();
}

inline
CFTypeID
getNumberTypeId()
{
	return ::CFNumberGetTypeID();
}

//..............................................................................

template <typename T>
class TypeRefBase
{
protected:
	T m_p;

public:
	TypeRefBase()
	{
		m_p = NULL;
	}

	TypeRefBase(const TypeRefBase& src)
	{
		if (src.m_p)
			::CFRetain(src.m_p);

		m_p = src.m_p;
	}

	TypeRefBase(
		T p,
		bool isAttach = false
		)
	{
		if (p && !isAttach)
			::CFRetain(p);

		m_p = p;
	}

	~TypeRefBase()
	{
		clear();
	}

	operator T() const
	{
		return m_p;
	}

	TypeRefBase&
	operator = (const TypeRefBase& src)
	{
		copy(src.m_p);
		return *this;
	}

	TypeRefBase&
	operator = (T p)
	{
		copy(p);
		return *this;
	}

	T
	p() const
	{
		return m_p;
	}

	void
	attach(T p)
	{
		clear();
		m_p = p;
	}

	T
	detach()
	{
		T p = m_p;
		m_p = NULL;
		return p;
	}

	void
	clear()
	{
		if (m_p)
			::CFRelease(m_p);

		m_p = NULL;
	}

	void
	copy(T p)
	{
		if (m_p == p)
			return;

		if (p)
			::CFRetain(p);

		if (m_p)
			::CFRelease(m_p);

		m_p = p;
	}

	intptr_t
	getRetainCount() const
	{
		return ::CFGetRetainCount(m_p);
	}

	void
	retain()
	{
		::CFRetain(m_p);
	}

	void
	release()
	{
		::CFRelease(m_p);
	}

	bool
	isEqual(T type) const
	{
		return ::CFEqual(m_p, type);
	}

	CFHashCode
	getHash() const
	{
		return ::CFHash(m_p);
	}

	CFTypeID
	getTypeId() const
	{
		return ::CFGetTypeID(m_p);
	}

	sl::String
	getDescription() const
	{
		CFStringRef cfString = ::CFCopyDescription(m_p);
		sl::String string = getStringFromCfString(cfString);
		::CFRelease(cfString);
		return string;
	}

	sl::String
	getTypeIdDescription() const
	{
		return getTypeIdDescription(getTypeId());
	}

	void
	show() const
	{
		::CFShow(m_p);
	}

	sl::String
	toString()
	{
		return cfTypeToString(m_p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef TypeRefBase<CFTypeRef> TypeRef;

//..............................................................................

} // namespace cf
} // namespace axl
