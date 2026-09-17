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

// parser errors

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// a parser error must be rendered eagerly -- yajl builds the description from
// parser state that is gone by the time anyone asks. a generator error is just
// a status code, so it keeps the code and renders on demand.

AXL_SL_DEFINE_GUID(
	g_yajlGenErrorGuid,
	0x64d630e0, 0x649c, 0x4d0a, 0xa9, 0x9a, 0x9e, 0x9c, 0xf8, 0x3c, 0xda, 0xcc
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class YajlGenErrorProvider: public err::ErrorProvider {
public:
	static
	sl::StringRef
	getErrorDescription(yajl_gen_status status);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return getErrorDescription((yajl_gen_status)error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
registerYajlGenErrorProvider();

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class YajlGenError: public err::Error {
public:
	YajlGenError() {}

	YajlGenError(yajl_gen_status status) {
		create(status);
	}

	size_t
	create(yajl_gen_status status) {
		return Error::create(g_yajlGenErrorGuid, status);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
setYajlGenError(yajl_gen_status status) {
	return err::setError(YajlGenError(status));
}

inline
bool
failWithYajlGenError(yajl_gen_status status) {
	setYajlGenError(status);
	return false;
}

//..............................................................................

} // namespace json
} // namespace axl
