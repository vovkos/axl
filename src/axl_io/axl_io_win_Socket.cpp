#include "pch.h"
#include "axl_io_win_Socket.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
CSocket::Open (
	int AddressFamily,
	int SockKind,
	int Protocol
	)
{
	Close ();
	m_h = ::socket (AddressFamily, SockKind, Protocol);
	return err::Complete (m_h != INVALID_SOCKET);
}

int
CSocket::GetError ()
{
	int Error;
	GetOption (SOL_SOCKET, SO_ERROR, &Error, sizeof (int));
	return Error;
}

bool
CSocket::SetBlockingMode (bool IsBlocking)
{
	ulong_t Value = !IsBlocking;
	int Result = ::ioctlsocket (m_h, FIONBIO, &Value);
	return err::Complete (m_h != -1);
}

size_t
CSocket::GetIncomingDataSize ()
{
	ulong_t Value;
	int Result = ::ioctlsocket (m_h, FIONREAD, &Value);
	if (Result == -1)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return Value;
}

bool
CSocket::GetAddress (sockaddr* pAddr)
{
	int Size = sizeof (sockaddr);
	int Result = ::getsockname (m_h, pAddr, &Size);
	return err::Complete (Result != SOCKET_ERROR);
}

bool
CSocket::GetPeerAddress (sockaddr* pAddr)
{
	int Size = sizeof (sockaddr);
	int Result = ::getpeername (m_h, pAddr, &Size);
	return err::Complete (Result != SOCKET_ERROR);
}

SOCKET
CSocket::Accept (sockaddr* pAddr)
{
	int Size = sizeof (sockaddr);
	SOCKET Socket = ::accept (m_h, pAddr, &Size);
	return err::Complete (Socket, INVALID_SOCKET);
}

size_t
CSocket::RecvFrom (
	void* p,
	size_t Size,
	sockaddr* pAddr
	)
{
	int SockAddrSize = sizeof (sockaddr);
	int Result = ::recvfrom (m_h, (char*) p, (int) Size, 0, pAddr, &SockAddrSize);
	return err::Complete (Result, SOCKET_ERROR);
}

bool
CSocket::CompleteAsyncRequest (
	int Result,
	int PendingResult
	)
{
	if (Result != SOCKET_ERROR)
		return true;

	dword_t Error = WSAGetLastError ();
	if (Error != PendingResult)
	{
		err::SetError (Error);
		return false;
	}

	return true;
}

bool
CSocket::WsaOpen (
	int AddressFamily,
	int SockKind,
	int Protocol,
	dword_t Flags
	)
{
	Close ();
	m_h = ::WSASocket (AddressFamily, SockKind, Protocol, NULL, 0, Flags);
	return err::Complete (m_h != INVALID_SOCKET);
}

SOCKET
CSocket::WsaAccept (sockaddr* pAddr)
{
	int Size = sizeof (sockaddr);
	SOCKET s = ::WSAAccept (m_h, pAddr, pAddr ? &Size : NULL, NULL, 0);
	return err::Complete (s, INVALID_SOCKET);
}

bool
CSocket::WsaSend (
	const void* p,
	dword_t Size,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = Size;

	int Result = WSASend (m_h, &Buf, 1, NULL, 0, pOverlapped, pfOnComplete);
	return CompleteAsyncRequest (Result, WSA_IO_PENDING);
}

bool
CSocket::WsaRecv (
	void* p,
	dword_t Size,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = Size;

	dword_t ActualSize;
	dword_t Flags;

	int Result = WSARecv (m_h, &Buf, 1, &ActualSize, &Flags, pOverlapped, pfOnComplete);
	return CompleteAsyncRequest (Result, WSA_IO_PENDING);
}

bool
CSocket::WsaSendTo (
	const void* p,
	dword_t Size,
	const sockaddr* pAddr,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = Size;

	int Result = WSASendTo (m_h, &Buf, 1, NULL, 0, pAddr, sizeof (sockaddr), pOverlapped, pfOnComplete);
	return CompleteAsyncRequest (Result, WSA_IO_PENDING);
}

bool
CSocket::WsaRecvFrom (
	void* p,
	dword_t Size,
	sockaddr* pAddr,
	int* pAddrSize,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = Size;

	dword_t ActualSize;
	dword_t Flags;

	int Result = WSARecvFrom (m_h, &Buf, 1, &ActualSize, &Flags, pAddr, pAddrSize, pOverlapped, pfOnComplete);
	return CompleteAsyncRequest (Result, WSA_IO_PENDING);
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

