// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ENC_BASE32ENCODING_H

#include "axl_rtl_String.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace enc {

//.............................................................................

class Base32Encoding
{
public:
	static
	size_t
	encode (
		rtl::String* string,
		const void* p, 
		size_t size,
		size_t hyphenDistance = 5
		);

	static
	rtl::String
	encode (
		const void* p, 
		size_t size,
		size_t hyphenDistance = 5
		)
	{
		rtl::String string;
		encode (&string, p, size, hyphenDistance);
		return string;
	}

	static
	size_t
	decode (
		rtl::Array <char>* buffer,
		const char* p, 
		size_t length = -1
		);

	static
	rtl::Array <char>
	decode (
		const char* p, 
		size_t length = -1
		)
	{
		rtl::Array <char> buffer;
		decode (&buffer, p, length);
		return buffer;
	}

	static
	size_t
	decode (
		rtl::Array <char>* buffer,
		const rtl::String& string
		)
	{
		return decode (buffer, string, string.getLength ());
	}

	static
	rtl::Array <char>
	decode (const rtl::String& string)
	{
		return decode (string, string.getLength ());
	}
};

//.............................................................................

} // namespace enc
} // namespace axl
