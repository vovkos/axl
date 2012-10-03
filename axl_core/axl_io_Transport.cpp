#include "stdafx.h"
#include "axl_io_Transport.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

void 
OnSyncSendRecvComplete (
	mt::CEvent* pEvent,
	err::CError* pError,
	size_t* pActualSize,
	
	const err::CError& Error,
	size_t ActualSize
	)
{
	if (pError)
		*pError = Error,

	*pActualSize = ActualSize;
	pEvent->Signal ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
ITransport::SyncSend (
	const void* p,
	size_t Size
	)
{
	mt::CEvent Event;
	err::CError Error;
	size_t ActualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_3 <mt::CEvent*, err::CError*, size_t*>,
		COnRecvCompleteArg
		> OnComplete (OnSyncSendRecvComplete, &Event, &Error, &ActualSize);

	Send (p, Size, &OnComplete);
	Event.Wait ();

	if (Error)
	{
		SetError (Error);
		return -1;
	}

	return ActualSize;
}

size_t
ITransport::SyncRecv (
	void* p,
	size_t Size
	)
{
	mt::CEvent Event;
	err::CError Error;
	size_t ActualSize;
	
	exe::CFunctionT <
		exe::CArgSeqT_3 <mt::CEvent*, err::CError*, size_t*>,
		COnRecvCompleteArg
		> OnComplete (OnSyncSendRecvComplete, &Event, &Error, &ActualSize);

	Recv (p, Size, &OnComplete);
	Event.Wait ();

	if (Error)
	{
		SetError (Error);
		return -1;
	}

	return ActualSize;
}

//.............................................................................

} // namespace io
} // namespace axl

