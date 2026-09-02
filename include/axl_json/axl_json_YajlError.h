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

#define _AXL_JSON_YAJLERROR_H

#include "axl_json_YajlPch.h"

namespace axl {
namespace json {

//..............................................................................

sl::String
getYajlErrorString(
	yajl_handle handle,
	const void* p,
	size_t size,
	bool isVerbose = false
);

size_t
setYajlError(
	yajl_handle handle,
	const void* p,
	size_t size
);

inline
bool
failWithYajlError(
	yajl_handle handle,
	const void* p,
	size_t size
) {
	setYajlError(handle, p, size);
	return false;
}

//..............................................................................

} // namespace json
} // namespace axl
