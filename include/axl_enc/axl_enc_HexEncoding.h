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

#define _AXL_ENC_HEXENCODING_H

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace enc {

//..............................................................................

enum HexEncodingFlag
{
	HexEncodingFlag_UpperCase = 1,
	HexEncodingFlag_NoSpace   = 2,
	HexEncodingFlag_Multiline = 4,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HexEncoding
{
protected:
	class GetHexChar_l
	{
	public:
		char
		operator () (uchar_t x)
		{
			static char charTable[] = "0123456789abcdef";
			return charTable[x & 0xf];
		}
	};

	class GetHexChar_u
	{
	public:
		char
		operator () (uchar_t x)
		{
			static char charTable[] = "0123456789ABCDEF";
			return charTable[x & 0xf];
		}
	};

	class InsertNoSpace
	{
	public:
		size_t
		operator () (
			char* p,
			size_t i
			)
		{
			return 0;
		}
	};

	class InsertSpace
	{
	public:
		size_t
		operator () (
			char* p,
			size_t i
			)
		{
			*p = ' ';
			return 1;
		}
	};

	class InsertSpaceMultiline
	{
	public:
		size_t
		operator () (
			char* p,
			size_t i
			)
		{
			*p = (i & 0xf) ? ' ' : '\n';
			return 1;
		}
	};

public:
	static
	char
	getHexChar_l(uchar_t x)
	{
		return GetHexChar_l()(x);
	}

	static
	char
	getHexChar_u(uchar_t x)
	{
		return GetHexChar_u()(x);
	}

	static
	uchar_t
	getHexCharValue(char x)
	{
		return
			x >= '0' && x <= '9' ? x - '0' :
			x >= 'a' && x <= 'f' ? 10 + x - 'a' :
			x >= 'A' && x <= 'F' ? 10 + x - 'A' : 0;
	}

	static
	size_t
	encode(
		sl::String* string,
		const void* p,
		size_t size,
		uint_t flags = 0
		);

	static
	sl::String
	encode(
		const void* p,
		size_t size,
		uint_t flags = 0
		)
	{
		sl::String string;
		encode(&string, p, size, flags);
		return string;
	}

	static
	size_t
	decode(
		sl::Array<char>* buffer,
		const sl::StringRef& source
		);

	static
	sl::Array<char>
	decode(const sl::StringRef& source)
	{
		sl::Array<char> buffer;
		decode(&buffer, source);
		return buffer;
	}

protected:
	template <
		typename GetHexChar,
		typename InsertSpace
		>
	static
	void
	encodeImpl(
		char* dst,
		const uchar_t* src,
		size_t size
		)
	{
		uchar_t x = src[0];
		*dst++ = GetHexChar()(x >> 4);
		*dst++ = GetHexChar()(x);

		for (size_t i = 1; i < size; i++)
		{
			dst += InsertSpace()(dst, i);

			uchar_t x = src[i];
			*dst++ = GetHexChar()(x >> 4);
			*dst++ = GetHexChar()(x);
		}
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
