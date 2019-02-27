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

#define _AXL_IOK_ITERATOR_H

#include "axl_iok_Object.h"

namespace axl {
namespace iok {

//..............................................................................

class Iterator: public ObjectBase<io_iterator_t>
{
public:
	Iterator()
	{
	}

	Iterator(const Iterator& src)
	{
		copy(src);
	}

	Iterator(
		io_iterator_t p,
		bool isAttach = false
		)
	{
		isAttach ? copy(p) : attach(p);
	}

	bool
	isValid()
	{
		return ::IOIteratorIsValid(m_p);
	}

	Object
	next();

	void
	reset()
	{
		::IOIteratorReset(m_p);
	}
};

//..............................................................................

} // namespace iok
} // namespace axl
