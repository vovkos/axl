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
	int Socket = m_Socket.Accept (pAddr);
	if (Socket == -1)
		return false;

	pSocket->m_Socket.Attach (Socket);
	return true;
}

//.............................................................................

} // namespace io
} // namespace axl

