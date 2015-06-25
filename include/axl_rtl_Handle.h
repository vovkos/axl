// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HANDLE_H

#include "axl_rtl_Func.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename T_0,
	typename Close	= Void <T_0>,
	typename GetInvalidHandle = Zero <T_0>
	>
class Handle
{
public:
	typedef T_0 T;

protected:
	T m_h;

public:
	Handle ()
	{
		m_h = GetInvalidHandle () ();
	}

	Handle (T h)
	{
		m_h = h;
	}

	~Handle ()
	{
		close ();
	}

	static
	T
	getInvalidHandle ()
	{
		return GetInvalidHandle () ();
	}

	static
	void
	closeHandle (T h)
	{
		if (h != GetInvalidHandle () ())
			Close () (h);
	}

	operator T () const
	{
		return m_h;
	}

	T operator -> () const
	{
		return m_h;
	}

	const Handle&
	operator = (T h)
	{
		attach (h);
		return *this;
	}

	bool
	isOpen () const
	{
		return m_h != GetInvalidHandle () ();
	}

	void
	close ()
	{
		if (isOpen ())
		{
			Close () (m_h);
			m_h = GetInvalidHandle () ();
		}
	}

	void
	attach (T h)
	{
		close ();
		m_h = h;
	}

	T
	detach ()
	{
		T h = m_h;
		m_h = GetInvalidHandle () ();
		return h;
	}

	T*
	getHandlePtr ()
	{
		return &m_h;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
