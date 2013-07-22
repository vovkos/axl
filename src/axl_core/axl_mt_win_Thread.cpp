#include "pch.h"
#include "axl_mt_win_Thread.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

bool 
CThread::Create (
	SECURITY_ATTRIBUTES* pSecAttr,
	size_t StackSize,
	PTHREAD_START_ROUTINE pfThreadProc,
	void* pContext,
	uint_t Flags
	)
{
	Close ();

	dword_t ThreadId;
	m_h = ::CreateThread (pSecAttr, StackSize, pfThreadProc, pContext, Flags, &ThreadId);
	return err::Complete (m_h != NULL);
}

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
