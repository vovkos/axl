#include "pch.h"
#include "axl_mt_win_Thread.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

bool 
Thread::create (
	SECURITY_ATTRIBUTES* secAttr,
	size_t stackSize,
	PTHREAD_START_ROUTINE pfThreadProc,
	void* context,
	uint_t flags
	)
{
	close ();

	dword_t threadId;
	m_h = ::CreateThread (secAttr, stackSize, pfThreadProc, context, flags, &threadId);
	return err::complete (m_h != NULL);
}

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
