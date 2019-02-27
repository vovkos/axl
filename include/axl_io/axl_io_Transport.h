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

#define _AXL_IO_TRANSPORT_H

#include "axl_io_Pch.h"

namespace axl {
namespace io {

//..............................................................................

class Transport
{
public:
	virtual
	size_t
	transmit(
		const void* p,
		size_t size
		) = 0;

	virtual
	size_t
	receive(
		void* p,
		size_t size
		) = 0;
};

//..............................................................................

} // namespace io
} // namespace axl
