#include "stdafx.h"
#include "axl_win_Sock.h"

namespace axl {
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
	if (s == INVALID_SOCKET)
	{
		err::SetLastSystemError ();
		return INVALID_SOCKET;
	}

	return s;
}

bool
CSock::CompleteAsyncOp (
	int Result,
	ulong_t PendingCode
	)
{
	if (Result == SOCKET_ERROR)
	{
		Result = WSAGetLastError ();
		if (Result == PendingCode)
			return true;
		
		return err::FailWithSystemError (Result);
	}

	return true;
}

bool
CSock::Send (
	const void* p,
	ulong_t Size,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = (ulong_t) Size;

	int Result = WSASend (m_h, &Buf, 1, NULL, 0, pOverlapped, pfnOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

bool
CSock::Recv (
	const void* p,
	ulong_t Size,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = (ulong_t) Size;

	static ulong_t _ActualSize;
	static ulong_t _Flags;

	int Result = WSARecv (m_h, &Buf, 1, &_ActualSize, &_Flags, pOverlapped, pfnOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

bool
CSock::SendTo (
	const void* p,
	ulong_t Size,
	const SOCKADDR* pAddr,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = (ulong_t) Size;

	int Result = WSASendTo (m_h, &Buf, 1, NULL, 0, pAddr, sizeof (SOCKADDR), pOverlapped, pfnOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

bool
CSock::RecvFrom (
	const void* p,
	ulong_t Size,
	SOCKADDR* pAddr,
	int* pAddrSize,
	WSAOVERLAPPED* pOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
	)
{
	ASSERT (IsOpen ());

	WSABUF Buf;
	Buf.buf = (char*) p;
	Buf.len = (ulong_t) Size;

	static ulong_t _ActualSize;
	static ulong_t _Flags;

	int Result = WSARecvFrom (m_h, &Buf, 1, &_ActualSize, &_Flags, pAddr, pAddrSize, pOverlapped, pfnOnComplete);
	return CompleteAsyncOp (Result, WSA_IO_PENDING);
}

//.............................................................................

} // namespace win
} // namespace axl

