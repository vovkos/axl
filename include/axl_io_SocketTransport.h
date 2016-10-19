// This Socket is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKETTRANSPORT_H

#include "axl_io_Transport.h"

namespace axl {
namespace io {

//..............................................................................

class Socketransport:
	public Transport,
	public sl::Handle <SOCKET>
{
public:
	virtual
	size_t
	transmit (
		const void* p,
		size_t size
		)
	{
		int result = send (m_h, (const char*) p, size, 0);
		return err::complete (result, SOCKET_ERROR);
	}

	virtual
	size_t
	receive (
		void* p,
		size_t size
		)
	{
		int result = recv (m_h, (char*) p, size, 0);
		return err::complete (result, SOCKET_ERROR);
	}
};

//..............................................................................

} // namespace io
} // namespace axl

