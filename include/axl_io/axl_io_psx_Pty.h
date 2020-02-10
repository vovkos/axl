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

#pragma once

#define _AXL_IO_PSX_PTY_H

#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class Pty: public File
{
public:
	bool
	open(uint_t openFlags = O_RDWR);

	bool
	grant()
	{
		int result = ::grantpt(m_h);
		return err::complete(result == 0);
	}

	bool
	unlock()
	{
		int result = ::unlockpt(m_h);
		return err::complete(result == 0);
	}

	bool
	getSlaveFileName(sl::String* fileName);

	sl::String
	getSlaveFileName()
	{
		sl::String fileName;
		getSlaveFileName(&fileName);
		return fileName;
	}
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
