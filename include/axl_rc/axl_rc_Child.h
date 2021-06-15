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

#define _AXL_RC_CHILD_H

#include "axl_rc_RefCount.h"

namespace axl {
namespace rc {

//..............................................................................

// creatable child ref-counted object

template <
	typename T,
	size_t extra = 0
	>
class Child
{
	AXL_DISABLE_COPY(Child)

protected:
	char m_buffer[sizeof(T) + extra];

public:
	Child(RefCount* parent)
	{
		memset(m_buffer, 0, sizeof(m_buffer));
		T* p = AXL_RC_NEW_INPLACE(T, m_buffer, parent, 0);
		p->addRef();
	}

	~Child()
	{
		p()->release();
	}

	operator T* ()
	{
		return p();
	}

	T*
	operator & ()
	{
		return p();
	}

	T*
	operator -> ()
	{
		return p();
	}

	T* p()
	{
		return (T*)m_buffer;
	}
};

//..............................................................................

} // namespace rc
} // namespace axl
