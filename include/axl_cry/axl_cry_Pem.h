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

#define _AXL_CRY_PEM_H

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

//..............................................................................

bool
encodePem(
	sl::String* buffer,
	const sl::StringRef& name,
	const sl::StringRef& header,
	const void* p,
	size_t size
);

inline
bool
encodePem(
	sl::String* buffer,
	const sl::StringRef& name,
	const void* p,
	size_t size
) {
	return encodePem(buffer, name, sl::StringRef(), p, size);
}

inline
sl::String
encodePem(
	const sl::StringRef& name,
	const sl::StringRef& header,
	const void* p,
	size_t size
) {
	sl::String string;
	encodePem(&string, name, p, size);
	return string;
}

inline
sl::String
encodePem(
	const sl::StringRef& name,
	const void* p,
	size_t size
) {
	return encodePem(name, sl::StringRef(), p, size);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
decodePem(
	sl::String* name,
	sl::String* header,
	sl::Array<char>* buffer,
	const sl::StringRef& pem
);

inline
bool
decodePem(
	sl::String* name,
	sl::Array<char>* buffer,
	const sl::StringRef& pem
) {
	return decodePem(name, NULL, buffer, pem);
}

bool
decodePem(
	const sl::StringRef& expectedName,
	sl::String* header,
	sl::Array<char>* buffer,
	const sl::StringRef& pem
);

inline
bool
decodePem(
	const sl::StringRef& expectedName,
	sl::Array<char>* buffer,
	const sl::StringRef& pem
) {
	return decodePem(expectedName, NULL, buffer, pem);
}

//..............................................................................

} // namespace cry
} // namespace axl
