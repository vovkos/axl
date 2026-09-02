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
#include "axl_json_YajlError.h"

namespace axl {
namespace json {

//..............................................................................

sl::String
getYajlErrorString(
	yajl_handle handle,
	const void* p,
	size_t size,
	bool isVerbose
) {
	uchar_t* errorString = ::yajl_get_error(handle, isVerbose, (const uchar_t*)p, size);
	if (!errorString)
		return sl::String();

	sl::String string((const char*)errorString);
	string.trimRight();
	::yajl_free_error(handle, errorString);
	return string;
}

size_t
setYajlError(
	yajl_handle handle,
	const void* p,
	size_t size
) {
	uchar_t* errorString = ::yajl_get_error(handle, false, (const uchar_t*)p, size);
	if (!errorString) // yajl only fails to render the description when out of memory
		return err::setError(err::SystemErrorCode_InsufficientResources);

	size_t result = err::setError(sl::StringRef((const char*)errorString).getRightTimmedString());
	::yajl_free_error(handle, errorString);
	return result;
}

//..............................................................................

} // namespace json
} // namespace axl
