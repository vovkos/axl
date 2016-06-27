// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_TRANSPORT_H

namespace axl {
namespace io {

//.............................................................................

class Transport
{
public:
	virtual
	size_t
	transmit (
		const void* p,
		size_t size
		) = 0;

	virtual
	size_t
	receive (
		void* p,
		size_t size
		) = 0;
};

//.............................................................................

} // namespace io
} // namespace axl

