#include "pch.h"
#include "axl_io_Pipe.h"

namespace axl {
namespace io {

//.............................................................................

bool CPipe::open (
	const char* name,
	SECURITY_DESCRIPTOR* sd
	)
{
	return false;
}

void 
CPipe::close ()
{
}

//.............................................................................

bool
CListenerPipe::listen (
	const char* name,
	SECURITY_DESCRIPTOR* sd,
	size_t backLog,
	const exe::CFunction& onAccept
	)
{
	return false;
}

//.............................................................................

bool 
CConnectionPipe::connect (
	const char* address,
	uint_t timeout,
	const exe::CFunction& onComplete
	)
{
	return false;
}

bool 
CConnectionPipe::disconnect (
	uint_t timeout,
	const exe::CFunction& onComplete
	)
{
	return false;
}

bool 
CConnectionPipe::syncConnect (
	const char* address,
	uint_t timeout
	)
{
	return false;
}

bool 
CConnectionPipe::syncDisconnect (uint_t timeout)
{
	return false;
}

bool
CConnectionPipe::isConnected ()
{
	return false;
}

bool
CConnectionPipe::send (
	const void* p,
	size_t size,
	const exe::CFunction& onComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

bool
CConnectionPipe::recv (
	void* p,
	size_t size,
	const exe::CFunction& onComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

//.............................................................................

} // namespace io
} // namespace axl

 