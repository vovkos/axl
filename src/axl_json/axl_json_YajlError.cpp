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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
registerYajlGenErrorProvider() {
	err::getErrorMgr()->registerProvider(
		g_yajlGenErrorGuid,
		sl::getSimpleSingleton<YajlGenErrorProvider>()
	);
}

sl::StringRef
YajlGenErrorProvider::getErrorDescription(yajl_gen_status status) {
	static const char* stringTable[] = {
		"OK",                                                  // yajl_gen_status_ok
		"JSON keys must be strings",                           // yajl_gen_keys_must_be_strings
		"maximum JSON generation depth exceeded",              // yajl_max_depth_exceeded
		"JSON generator is in error state",                    // yajl_gen_in_error_state
		"JSON generation is already complete",                 // yajl_gen_generation_complete
		"invalid JSON number",                                 // yajl_gen_invalid_number
		"no JSON generator buffer (print callback is in use)", // yajl_gen_no_buf
		"invalid UTF-8 in a JSON string",                      // yajl_gen_invalid_string
	};

	size_t i = (size_t)status;
	return i < countof(stringTable) ?
		sl::StringRef(stringTable[i]) :
		sl::formatString("unknown JSON generator error: %d", status);
}

//..............................................................................

} // namespace json
} // namespace axl
