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

#define _AXL_IO_SHMTREADER_H

#include "axl_io_ShmtBase.h"

namespace axl {
namespace io {

//..............................................................................

class ShmtReader: public ShmtBase {
public:
	size_t
	read(sl::Array<char>* buffer);

	sl::Array<char>
	read() {
		sl::Array<char> buffer;
		read(&buffer);
		return buffer;
	}
};

//..............................................................................

} // namespace io
} // namespace axl
