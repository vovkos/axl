//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_io_win_Iocp.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
Iocp::create(dword_t threadCount)
{
	close();

	m_h = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, threadCount);
	return err::complete(m_h != NULL);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
