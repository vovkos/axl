// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_ESCAPEENCODING_H

#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

//.............................................................................

class CEscapeEncoding
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
		CString* string,
		const char* p, 
		size_t length = -1
		);

	static
	CString
	encode (
		const char* p, 
		size_t length = -1
		)
	{
		CString string;
		encode (&string, p, length);
		return string;
	}

	static
	size_t
	encode (
		CString* dstString,
		const CString& srcString
		)
	{
		return encode (dstString, srcString, srcString.getLength ());
	}

	static
	CString
	encode (const CString& string)
	{
		return encode (string, string.getLength ());
	}

	static
	size_t
	decode (
		CString* string,
		const char* p, 
		size_t length = -1
		);

	static
	CString
	decode (		
		const char* p, 
		size_t length = -1
		)
	{
		CString string;
		decode (&string, p, length);
		return string;
	}

	static
	size_t
	decode (
		CString* dstString,
		const CString& srcString
		)
	{
		return decode (dstString, srcString, srcString.getLength ());
	}

	static
	CString
	decode (const CString& string)
	{
		return decode (string, string.getLength ());
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

