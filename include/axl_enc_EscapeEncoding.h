// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ENC_ESCAPEENCODING_H

#include "axl_enc_HexEncoding.h"

namespace axl {
namespace enc {

//.............................................................................

class EscapeEncoding
{
public:
	static
	char
	findEscapeChar (char x);

	static
	char
	findEscapeReplaceChar (char x);

	static
	size_t
	encode (
		rtl::String* string,
		const char* p, 
		size_t length = -1
		);

	static
	rtl::String
	encode (
		const char* p, 
		size_t length = -1
		)
	{
		rtl::String string;
		encode (&string, p, length);
		return string;
	}

	static
	size_t
	decode (
		rtl::String* string,
		const char* p, 
		size_t length = -1
		);

	static
	rtl::String
	decode (		
		const char* p, 
		size_t length = -1
		)
	{
		rtl::String string;
		decode (&string, p, length);
		return string;
	}
};

//.............................................................................

} // namespace enc
} // namespace axl
