#include "pch.h"
#include "axl_io_win_Sock.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
CSock::GetLocalAddress (SOCKADDR* pAddr)
{
	ASSERT (IsOpen ());

	int Size = sizeof (SOCKADDR);
	int Result = ::getsockname (m_h, pAddr, &Size);
	return err::Complete (Result != SOCKET_ERROR);
}

bool
CSock::GetPeerAddress (SOCKADDR* pAddr)
{
	ASSERT (IsOpen ());

	int Size = sizeof (SOCKADDR);
	int Result = ::getpeername (m_h, pAddr, &Size);
	return err::Complete (Result != SOCKET_ERROR);
}


SOCKET
CSock::Accept (SOCKADDR* pAddr)
{
	ASSERT (IsOpen ());

	int Size = sizeof (SOCKADDR);

	SOCKET s = ::WSAAccept (m_h, pAddr, pAddr ? &Size : NULL, NULL, 0);
	return err::Complete (s, (SOCKET) -1);
}

bool
CSock::CompleteAsyncOp (
	int Result,
	dword_t PendingCode
	)
{
	if (Result == SOCKET_ERROR)
	{
		dword_t Error = WSAGetLastError ();
		return err::Complete (Error == PendingCode);
	}

	return true;
}

bool
CSock::Send (
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
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

bool
CSock::Recv (
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

	static dword_t _ActualSize;
	static dword_t _Flags;

	int Result = WSARecv (m_h, &Buf, 1, &_ActualSize, &_Flags, pOverlapped, pfOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

bool
CSock::SendTo (
	const void* p,
	dword_t Size,
	const SOCKADDR* pAddr,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = Size;

	int Result = WSASendTo (m_h, &Buf, 1, NULL, 0, pAddr, sizeof (SOCKADDR), pOverlapped, pfOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

bool
CSock::RecvFrom (
	const void* p,
	dword_t Size,
	SOCKADDR* pAddr,
	int* pAddrSize,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = Size;

	static dword_t _ActualSize;
	static dword_t _Flags;

	int Result = WSARecvFrom (m_h, &Buf, 1, &_ActualSize, &_Flags, pAddr, pAddrSize, pOverlapped, pfOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

