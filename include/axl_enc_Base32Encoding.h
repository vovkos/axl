// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

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
	encode (
		sl::String* string,
		const void* p,
		size_t size,
		size_t hyphenDistance = 5
		);

	static
	sl::String
	encode (
		const void* p,
		size_t size,
		size_t hyphenDistance = 5
		)
	{
		sl::String string;
		encode (&string, p, size, hyphenDistance);
		return string;
	}

	static
	size_t
	decode (
		sl::Array <char>* buffer,
		const sl::StringRef& string
		);

	static
	sl::Array <char>
	decode (const sl::StringRef& string)
	{
		sl::Array <char> buffer;
		decode (&buffer, string);
		return buffer;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
