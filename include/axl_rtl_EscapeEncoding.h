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
	FindEscapeChar (char x);

	static
	char
	FindEscapeReplaceChar (char x);

	static
	size_t
	Encode (
		CString* pString,
		const char* p, 
		size_t Length = -1
		);

	static
	CString
	Encode (
		const char* p, 
		size_t Length = -1
		)
	{
		CString String;
		Encode (&String, p, Length);
		return String;
	}

	static
	size_t
	Encode (
		CString* pString,
		const CString& String
		)
	{
		return Encode (pString, String, String.GetLength ());
	}

	static
	CString
	Encode (const CString& String)
	{
		return Encode (String, String.GetLength ());
	}

	static
	size_t
	Decode (
		CString* pString,
		const char* p, 
		size_t Length = -1
		);

	static
	CString
	Decode (		
		const char* p, 
		size_t Length = -1
		)
	{
		CString String;
		Decode (&String, p, Length);
		return String;
	}

	static
	size_t
	Decode (
		CString* pString,
		const CString& String
		)
	{
		return Decode (pString, String, String.GetLength ());
	}

	static
	CString
	Decode (const CString& String)
	{
		return Decode (String, String.GetLength ());
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

