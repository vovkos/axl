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

#define _AXL_PY_BUFFER_H

#include "axl_py_Error.h"

namespace axl {
namespace py {

//..............................................................................

class Buffer: public Py_buffer {
public:
	Buffer() {
		memset(this, 0, sizeof(Py_buffer));
	}

	~Buffer() {
		::PyBuffer_Release(this);
	}

	bool
	isContiguous(char order = 'C') const {
		return ::PyBuffer_IsContiguous(this, order) != 0;
	}
};

//..............................................................................

} // namespace lua
} // namespace axl
