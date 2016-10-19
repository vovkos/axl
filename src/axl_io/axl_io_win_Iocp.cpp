#include "pch.h"
#include "axl_io_win_Iocp.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
Iocp::create (dword_t threadCount)
{
	close ();

	m_h = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE, NULL, 0, threadCount);
	return err::complete (m_h != NULL);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl

