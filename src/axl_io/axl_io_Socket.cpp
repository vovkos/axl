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
#include "axl_io_Socket.h"

namespace axl {
namespace io {

//..............................................................................

bool
Socket::accept (
	Socket* socket,
	SockAddr* addr
	)
{
#if (_AXL_OS_WIN)
	SOCKET h = m_socket.accept (addr);
#else
	int h = m_socket.accept (addr);
#endif

	if (h == -1)
		return false;

	socket->m_socket.attach (h);
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
