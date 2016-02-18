#include "pch.h"
#include "axl_io_Transport.h"
#include "axl_sys_Event.h"

namespace axl {
namespace io {

//.............................................................................

void
onSyncSendRecvComplete (
	sys::Event* event,
	err::Error* error_o,
	size_t* actualSize,

	const err::Error& error,
	size_t actualSize
	)
{
	if (error)
		*error_o = error,

	*actualSize = actualSize;
	event->signal ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
Asyncransport::syncSend (
	const void* p,
	size_t size
	)
{
	sys::Event event;
	err::Error error;
	size_t actualSize;

	exe::Function <
		exe::ArgSeq_3 <sys::Event*, err::Error*, size_t*>,
		OnRecvCompleteArg
		> onComplete (onSyncSendRecvComplete, &event, &error, &actualSize);

	send (p, size, &onComplete);
	event.wait ();

	if (error)
	{
		setError (error);
		return -1;
	}

	return actualSize;
}

size_t
Asyncransport::syncRecv (
	void* p,
	size_t size
	)
{
	sys::Event event;
	err::Error error;
	size_t actualSize;

	exe::Function <
		exe::ArgSeq_3 <sys::Event*, err::Error*, size_t*>,
		OnRecvCompleteArg
		> onComplete (onSyncSendRecvComplete, &event, &error, &actualSize);

	recv (p, size, &onComplete);
	event.wait ();

	if (error)
	{
		setError (error);
		return -1;
	}

	return actualSize;
}

//.............................................................................

} // namespace io
} // namespace axl

