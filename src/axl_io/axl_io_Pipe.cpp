#include "pch.h"
#include "axl_io_Pipe.h"

namespace axl {
namespace io {

//.............................................................................

bool CPipe::Open (
	const char* pName,
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
	const char* pName,
	SECURITY_DESCRIPTOR* pSd,
	size_t BackLog,
	const exe::CFunction& OnAccept
	)
{
	return false;
}

//.............................................................................

bool 
CConnectionPipe::Connect (
	const char* pAddress,
	uint_t Timeout,
	const exe::CFunction& OnComplete
	)
{
	return false;
}

bool 
CConnectionPipe::Disconnect (
	uint_t Timeout,
	const exe::CFunction& OnComplete
	)
{
	return false;
}

bool 
CConnectionPipe::SyncConnect (
	const char* pAddress,
	uint_t Timeout
	)
{
	return false;
}

bool 
CConnectionPipe::SyncDisconnect (uint_t Timeout)
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
	const exe::CFunction& OnComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

bool
CConnectionPipe::Recv (
	void* p,
	size_t Size,
	const exe::CFunction& OnComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

//.............................................................................

} // namespace io
} // namespace axl

 