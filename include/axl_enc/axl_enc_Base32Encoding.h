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

#define _AXL_ENC_BASE32ENCODING_H

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace enc {

//..............................................................................

class Base32Encoding
{
public:
	static
	size_t
	encode(
		sl::String* string,
		const void* p,
		size_t size,
		size_t hyphenDistance = 5
		);

	static
	sl::String
	encode(
		const void* p,
		size_t size,
		size_t hyphenDistance = 5
		)
	{
		sl::String string;
		encode(&string, p, size, hyphenDistance);
		return string;
	}

	static
	size_t
	decode(
		sl::Array<char>* buffer,
		const sl::StringRef& string
		);

	static
	sl::Array<char>
	decode(const sl::StringRef& string)
	{
		sl::Array<char> buffer;
		decode(&buffer, string);
		return buffer;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
