// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HEXENCODING_H

#include "axl_rtl_String.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

//.............................................................................

enum EHexEncode
{
	EHexEncode_UpperCase = 1,
	EHexEncode_NoSpace   = 2,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CHexEncoding
{
public:
	static 
	char
	GetHexChar_l (uchar_t x)
	{
		static char CharTable [] = "0123456789abcdef";
		return CharTable [x & 0xf];
	}

	static 
	char
	GetHexChar_u (uchar_t x)
	{
		static char CharTable [] = "0123456789ABCDEF";	
		return CharTable [x & 0xf];
	}

	static 
	uchar_t
	GetHexCharValue (char x)
	{
		return 
			x >= '0' && x <= '9' ? x - '0' :
			x >= 'a' && x <= 'f' ? 10 + x - 'a' :
			x >= 'A' && x <= 'F' ? 10 + x - 'A' : 0;
	}

	static
	size_t
	Encode (
		CString* pString,
		const void* p, 
		size_t Size,
		uint_t Flags = 0
		);

	static
	CString
	Encode (
		const void* p, 
		size_t Size,
		uint_t Flags = 0
		)
	{
		CString String;
		Encode (&String, p, Size, Flags);
		return String;
	}

	static
	size_t
	Decode (
		CArrayT <uchar_t>* pBuffer,
		const char* p, 
		size_t Length = -1
		);

	static
	CArrayT <uchar_t>
	Decode (
		const char* p, 
		size_t Length = -1
		)
	{
		CArrayT <uchar_t> Buffer;
		Decode (&Buffer, p, Length);
		return Buffer;
	}

	static
	size_t
	Decode (
		CArrayT <uchar_t>* pBuffer,
		const CString& String
		)
	{
		return Decode (pBuffer, String, String.GetLength ());
	}

	static
	CArrayT <uchar_t>
	Decode (const CString& String)
	{
		return Decode (String, String.GetLength ());
	}

protected:
	static
	void
	Encode_l (
		char* pDst,
		const uchar_t* pSrc, 
		const uchar_t* pSrcEnd
		);

	static
	void
	Encode_u (
		char* pDst,
		const uchar_t* pSrc, 
		const uchar_t* pSrcEnd
		);

	static
	void
	Encode_nsl (
		char* pDst,
		const uchar_t* pSrc, 
		const uchar_t* pSrcEnd
		);

	static
	void
	Encode_nsu (
		char* pDst,
		const uchar_t* pSrc, 
		const uchar_t* pSrcEnd
		);
};

//.............................................................................

} // namespace rtl
} // namespace axl
