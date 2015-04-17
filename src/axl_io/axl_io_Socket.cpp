#include "pch.h"
#include "axl_io_Socket.h"

namespace axl {
namespace io {

//.............................................................................

bool
Socket::accept (
	Socket* socket,
	SockAddr* addr
	)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	SOCKET h = m_socket.accept (addr);
#else 
	int h = m_socket.accept (addr);
#endif

	if (h == -1)
		return false;

	socket->m_socket.attach (h);
	return true;
}

//.............................................................................

} // namespace io
} // namespace axl
