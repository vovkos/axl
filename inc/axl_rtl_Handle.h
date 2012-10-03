// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HANDLE_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename THandle,
	THandle hInvalid,
	typename TClose	
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
		m_h = hInvalid;
	}

	CHandleT (H h)
	{
		m_h = h;
	}

	~CHandleT ()
	{
		Close ();
	}

	operator H () const
	{ 
		return m_h; 
	}

	const CHandleT&
	operator = (H h)
	{
		Attach (h);
		return *this;
	}

	bool 
	IsOpen ()
	{
		return m_h != hInvalid;
	}

	void 
	Close ()
	{
		if (IsOpen ())
		{
			TClose () (m_h);
			m_h = hInvalid;
		}
	}

	void 
	Attach (H h)
	{
		Close ();
		m_h = h;
	}

	H 
	Detach ()
	{
		H h = m_h;
		m_h = hInvalid;
		return h;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
