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
	typename THandle,
	typename TClose	= CVoidT <THandle>,
	typename TGetInvalidHandle = CZeroT <THandle>
	>
class CHandleT
{
public:
	typedef THandle H;

protected:
	H m_h;

public:
	CHandleT ()
	{
		m_h = TGetInvalidHandle () ();
	}

	CHandleT (H h)
	{
		m_h = h;
	}

	~CHandleT ()
	{
		close ();
	}

	operator H () const
	{
		return m_h;
	}

	const CHandleT&
	operator = (H h)
	{
		attach (h);
		return *this;
	}

	bool
	isOpen () const
	{
		return m_h != TGetInvalidHandle () ();
	}

	void
	close ()
	{
		if (isOpen ())
		{
			TClose () (m_h);
			m_h = TGetInvalidHandle () ();
		}
	}

	void
	attach (H h)
	{
		close ();
		m_h = h;
	}

	H
	detach ()
	{
		H h = m_h;
		m_h = TGetInvalidHandle () ();
		return h;
	}

	H*
	getHandlePtr ()
	{
		return &m_h;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
