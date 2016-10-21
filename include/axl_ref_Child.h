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

#define _AXL_REF_CHILD_H

#include "axl_ref_RefCount.h"

namespace axl {
namespace ref {

//..............................................................................

// creatable child ref-counted object

template <
	typename T,
	size_t extra = 0
	>
class Child
{
	AXL_DISABLE_COPY (Child)

protected:
	char m_buffer [sizeof (T) + extra];

public:
	Child (RefCount* parent)
	{
		memset (m_buffer, 0, sizeof (m_buffer));
		T* p = AXL_REF_INPLACE_NEW (m_buffer, 0);
		p->addRef ();
		p->addWeakRef ();
	}

	~Child ()
	{
		p ()->release ();
	}

	operator T* ()
	{
		return p ();
	}

	T*
	operator & ()
	{
		return p ();
	}

	T*
	operator -> ()
	{
		return p ();
	}

	T* p ()
	{
		return (Object*) m_buffer;
	}

protected:
	static
	void
	free (void* p)
	{
		((Object*) p)->m_parent->weakRelease ();
	}
};

//..............................................................................

} // namespace ref
} // namespace axl
