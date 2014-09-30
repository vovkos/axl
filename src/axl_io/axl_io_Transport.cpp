#include "pch.h"
#include "axl_io_Transport.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

void 
onSyncSendRecvComplete (
	mt::CEvent* event,
	err::CError* error,
	size_t* actualSize,
	
	const err::CError& error,
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
CAsyncTransport::syncSend (
	const void* p,
	size_t size
	)
{
	mt::CEvent event;
	err::CError error;
	size_t actualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_3 <mt::CEvent*, err::CError*, size_t*>,
		COnRecvCompleteArg
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
CAsyncTransport::syncRecv (
	void* p,
	size_t size
	)
{
	mt::CEvent event;
	err::CError error;
	size_t actualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_3 <mt::CEvent*, err::CError*, size_t*>,
		COnRecvCompleteArg
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

