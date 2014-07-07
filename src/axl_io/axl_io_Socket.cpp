#include "pch.h"
#include "axl_io_Socket.h"

namespace axl {
namespace io {

//.............................................................................

bool
CSocket::Accept (
	CSocket* pSocket,
	sockaddr* pAddr
	)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	SOCKET Socket = m_Socket.Accept (pAddr);
#else 
	int Socket = m_Socket.Accept (pAddr);
#endif

	if (Socket == -1)
		return false;

	pSocket->m_Socket.Attach (Socket);
	return true;
}

//.............................................................................

} // namespace io
} // namespace axl

