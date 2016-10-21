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

#include "pch.h"
#include "axl_iok_Iterator.h"

namespace axl {
namespace iok {

//..............................................................................

Object
Iterator::next ()
{
	io_object_t p = ::IOIteratorNext (m_p);

	Object object;
	object.attach (p);
	return object;
}

//..............................................................................

} // namespace iok
} // namespace axl
