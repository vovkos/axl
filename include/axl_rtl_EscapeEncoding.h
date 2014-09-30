// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_ESCAPEENCODING_H

#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

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
		String* string,
		const char* p, 
		size_t length = -1
		);

	static
	String
	encode (
		const char* p, 
		size_t length = -1
		)
	{
		String string;
		encode (&string, p, length);
		return string;
	}

	static
	size_t
	encode (
		String* dstString,
		const String& srcString
		)
	{
		return encode (dstString, srcString, srcString.getLength ());
	}

	static
	String
	encode (const String& string)
	{
		return encode (string, string.getLength ());
	}

	static
	size_t
	decode (
		String* string,
		const char* p, 
		size_t length = -1
		);

	static
	String
	decode (		
		const char* p, 
		size_t length = -1
		)
	{
		String string;
		decode (&string, p, length);
		return string;
	}

	static
	size_t
	decode (
		String* dstString,
		const String& srcString
		)
	{
		return decode (dstString, srcString, srcString.getLength ());
	}

	static
	String
	decode (const String& string)
	{
		return decode (string, string.getLength ());
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

