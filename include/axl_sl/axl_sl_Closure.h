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

#define _AXL_SL_CLOSURE_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Func,
	typename Result,
	typename Arg,
	typename ArgSave = Arg
	>
class BinClosure_1
{
protected:
	ArgSave m_a;

public:
	BinClosure_1(Arg a)
	{
		m_a = a;
	}

	Result
	operator () (Arg b) const
	{
		return Func() (m_a, b);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Func,
	typename Result,
	typename Arg,
	typename ArgSave = Arg
	>
class BinClosure_2
{
protected:
	ArgSave m_b;

public:
	BinClosure_2(Arg b)
	{
		m_b = b;
	}

	Result
	operator () (Arg a) const
	{
		return Func() (a, m_b);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class AssignClosure_L
{
protected:
	T* m_p;

public:
	AssignClosure_L(T& a)
	{
		m_p = &a;
	}

	void
	operator () (T& b) const
	{
		*m_p = b;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = T
	>
class AssignClosure_R
{
protected:
	T m_b;

public:
	AssignClosure_R(Arg b)
	{
		m_b = b;
	}

	void
	operator () (T& a) const
	{
		a = m_b;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
