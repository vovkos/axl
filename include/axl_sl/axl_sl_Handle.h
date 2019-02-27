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

#define _AXL_SL_HANDLE_H

#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Close = Void<T>,
	typename GetInvalidHandle = Zero<T>
	>
class Handle
{
public:
	typedef T H;

protected:
	T m_h;

public:
	Handle()
	{
		m_h = GetInvalidHandle() ();
	}

	Handle(T h)
	{
		m_h = h;
	}

	~Handle()
	{
		close();
	}

	static
	T
	getInvalidHandle()
	{
		return GetInvalidHandle() ();
	}

	operator T() const
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
		attach(h);
		return *this;
	}

	bool
	isOpen() const
	{
		return m_h != GetInvalidHandle() ();
	}

	void
	close()
	{
		if (isOpen())
		{
			Close() (m_h);
			m_h = GetInvalidHandle() ();
		}
	}

	void
	attach(T h)
	{
		close();
		m_h = h;
	}

	T
	detach()
	{
		T h = m_h;
		m_h = GetInvalidHandle() ();
		return h;
	}

	T*
	p()
	{
		return &m_h;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
