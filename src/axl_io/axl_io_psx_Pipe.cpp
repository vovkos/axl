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
#include "axl_io_psx_Pipe.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

bool
Pipe::create()
{
	close();

	int fd[2];
	int result = ::pipe(fd);
	if (result == -1)
		return err::failWithLastSystemError();

	m_readFile.attach(fd[0]);
	m_writeFile.attach(fd[1]);
	return true;
}

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
