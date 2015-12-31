// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IOK_ITERATOR_H

#include "axl_iok_Object.h"

namespace axl {
namespace iok {

//.............................................................................

class Iterator: public ObjectBase <io_iterator_t>
{
public:
	Iterator ()
	{
	}

	Iterator (const Iterator& src)
	{
		copy (src);
	}

	Iterator (
		io_iterator_t p,
		bool isAttach = false
		)
	{
		isAttach ? copy (p) : attach (p);
	}

	bool
	isValid ()
	{
		return ::IOIteratorIsValid (m_p);
	}

	Object
	next ();

	void
	reset ()
	{
		::IOIteratorReset (m_p);
	}
};

//.............................................................................

} // namespace iok
} // namespace axl

