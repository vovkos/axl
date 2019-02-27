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

#include "axl_g_Pch.h"

namespace axl {
namespace iok {

//..............................................................................

template <typename T>
class ObjectBase
{
protected:
	T m_p;

public:
	ObjectBase()
	{
		m_p = MACH_PORT_NULL;
	}

	ObjectBase(const ObjectBase& src)
	{
		if (src.m_p)
			::IOObjectRetain(src.m_p);

		m_p = src.m_p;
	}

	ObjectBase(
		T p,
		bool isAttach = false
		)
	{
		if (p && !isAttach)
			::IOObjectRetain(p);

		m_p = p;
	}

	operator T() const
	{
		return m_p;
	}

	ObjectBase&
	operator = (const ObjectBase& src)
	{
		copy(src.m_p);
		return *this;
	}

	ObjectBase&
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
		m_p = MACH_PORT_NULL;
		return p;
	}

	void
	clear()
	{
		if (m_p)
			::IOObjectRelease(m_p);

		m_p = MACH_PORT_NULL;
	}

	void
	copy(T p)
	{
		if (m_p == p)
			return;

		if (p)
			::IOObjectRetain(p);

		if (m_p)
			::IOObjectRelease(m_p);

		m_p = p;
	}

	void
	retain()
	{
		::IOObjectRetain(m_p);
	}

	void
	release()
	{
		::IOObjectRelease(m_p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectBase<io_object_t> Object;

//..............................................................................

} // namespace iok
} // namespace axl
