#include "pch.h"
#include "axl_io_win_Iocp.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool 
CIocp::Create (dword_t ThreadCount)
{
	Close ();

	m_h = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE, NULL, 0, ThreadCount);
	return err::Complete (m_h != NULL);
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

