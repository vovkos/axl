#include "pch.h"
#include "axl_io_Transport.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

void 
onSyncSendRecvComplete (
	mt::Event* event,
	err::Error* error,
	size_t* actualSize,
	
	const err::Error& error,
	size_t actualSize
	)
{
	if (error)
		*error = error,

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
	mt::Event event;
	err::Error error;
	size_t actualSize;
	
	exe::Function <
		exe::ArgSeq_3 <mt::Event*, err::Error*, size_t*>,
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
	mt::Event event;
	err::Error error;
	size_t actualSize;
	
	exe::Function <
		exe::ArgSeq_3 <mt::Event*, err::Error*, size_t*>,
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

