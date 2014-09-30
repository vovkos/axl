#include "pch.h"
#include "axl_io_Pipe.h"

namespace axl {
namespace io {

//.............................................................................

bool Pipe::open (
	const char* name,
	SECURITY_DESCRIPTOR* sd
	)
{
	return false;
}

void 
Pipe::close ()
{
}

//.............................................................................

bool
ListenerPipe::listen (
	const char* name,
	SECURITY_DESCRIPTOR* sd,
	size_t backLog,
	const exe::Function& onAccept
	)
{
	return false;
}

//.............................................................................

bool 
ConnectionPipe::connect (
	const char* address,
	uint_t timeout,
	const exe::Function& onComplete
	)
{
	return false;
}

bool 
ConnectionPipe::disconnect (
	uint_t timeout,
	const exe::Function& onComplete
	)
{
	return false;
}

bool 
ConnectionPipe::syncConnect (
	const char* address,
	uint_t timeout
	)
{
	return false;
}

bool 
ConnectionPipe::syncDisconnect (uint_t timeout)
{
	return false;
}

bool
ConnectionPipe::isConnected ()
{
	return false;
}

bool
ConnectionPipe::send (
	const void* p,
	size_t size,
	const exe::Function& onComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

bool
ConnectionPipe::recv (
	void* p,
	size_t size,
	const exe::Function& onComplete // void OnComplete (size_t ActualSize);
	)
{
	return false;
}

//.............................................................................

} // namespace io
} // namespace axl

 