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
#include "axl_io_psx_Pty.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

bool
Pty::open(uint_t openFlags)
{
	close();

	m_h = ::posix_openpt(openFlags);
	return err::complete(m_h != -1);
}

bool
Pty::getSlaveFileName(sl::String* fileName)
{
	char buffer[256];
	int result = ::ptsname_r(m_h, buffer, sizeof(buffer));
	if (result != 0)	
		return err::failWithLastSystemError();

	*fileName = buffer;
	return true;
}

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
