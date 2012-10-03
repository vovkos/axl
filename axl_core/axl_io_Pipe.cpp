#include "stdafx.h"
#include "axl_io_Pipe.h"

namespace axl {
namespace io {

//.............................................................................

bool CPipe::Open (
	const tchar_t* pName,
	SECURITY_DESCRIPTOR* pSd
	)
{
	return false;
}

void 
CPipe::Close ()
{
}

//.............................................................................

bool
CListenerPipe::Listen (
	const tchar_t* pName,
	SECURITY_DESCRIPTOR* pSd,
	size_t BackLog,
	exe::IFunction* pOnAccept
	)
{
	return false;
}

//.............................................................................

bool 
CConnectionPipe::Connect (
	const tchar_t* pAddress,
	ulong_t Timeout,
	exe::IFunction* pOnComplete
	)
{
	return false;
}

bool 
CConnectionPipe::Disconnect (
	ulong_t Timeout,
	exe::IFunction* pOnComplete
	)
{
	return false;
}

bool 
CConnectionPipe::SyncConnect (
	const tchar_t* pAddress,
	ulong_t Timeout
	)
{
	return false;
}

bool 
CConnectionPipe::SyncDisconnect (ulong_t Timeout)
{
	return false;
}

bool
CConnectionPipe::IsConnected ()
{
	return false;
}

bool
CConnectionPipe::Send (
	const void* p,
	size_t Size,
	exe::IFunction* pOnComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

bool
CConnectionPipe::Recv (
	void* p,
	size_t Size,
	exe::IFunction* pOnComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

//.............................................................................

} // namespace io
} // namespace axl

 