// This Socket is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SOCKETTRANSPORT_H

#include "axl_io_Transport.h"
#include "axl_rtl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

class CSocketTransport: 
	public ITransport,
	public rtl::CHandleT <SOCKET>
{
public:
	AXL_OBJ_CLASS_0 (CSocketTransport, ITransport)
		
public:
	virtual
	size_t
	Transmit (
		const void* p,
		size_t Size
		)
	{
		int Result = send (m_h, (const char*) p, Size, 0);
		return err::Complete (Result, SOCKET_ERROR);
	}

	virtual
	size_t
	Receive (
		void* p,
		size_t Size
		)
	{
		int Result = recv (m_h, (char*) p, Size, 0);
		return err::Complete (Result, SOCKET_ERROR);
	}
};

//.............................................................................

} // namespace io
} // namespace axl

