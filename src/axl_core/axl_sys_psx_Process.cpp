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
#include "axl_sys_psx_Process.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

sl::String
getProcessImageName(uint_t pid)
{
	char buffer[256]; // enough
	sprintf(buffer, "/proc/%d/comm", pid);
	io::File file;
	bool result = file.open(buffer, io::FileFlag_ReadOnly);
	if (!result)
		return sl::String();

	size_t size = file.read(buffer, sizeof(buffer) - 1);
	if (size == -1)
		return sl::String();

	sl::String string(buffer, size);
	string.trimRight();
	return string;
}

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
