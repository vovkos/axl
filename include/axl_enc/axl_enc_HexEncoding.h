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

class GetHexChar_l {
public:
	char
	operator () (uchar_t x) {
		static char charTable[] = "0123456789abcdef";
		return charTable[x & 0xf];
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GetHexChar_u {
public:
	char
	operator () (uchar_t x) {
		static char charTable[] = "0123456789ABCDEF";
		return charTable[x & 0xf];
	}
};

//..............................................................................

enum HexEncodingFlag {
	HexEncodingFlag_UpperCase = 0x01,
	HexEncodingFlag_NoSpace   = 0x02,
	HexEncodingFlag_Multiline = 0x04,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HexEncoding {
public:
	static
	uchar_t
	getHexCharValue(char x) {
		return
			x >= '0' && x <= '9' ? x - '0' :
			x >= 'a' && x <= 'f' ? 10 + x - 'a' :
			x >= 'A' && x <= 'F' ? 10 + x - 'A' : 0;
	}

	static
	char
	getHexChar_l(uchar_t x) {
		return GetHexChar_l()(x);
	}

	static
	char
	getHexChar_u(uchar_t x) {
		return GetHexChar_u()(x);
	}

	template <typename T>
	static
	size_t
	encode(
		sl::StringBase<T>* string,
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
	) {
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
	decode(const sl::StringRef& source) {
		sl::Array<char> buffer;
		decode(&buffer, source);
		return buffer;
	}
};

namespace impl {

//..............................................................................

template <typename T>
class InsertNoSpace {
public:
	size_t
	operator () (
		T* p,
		size_t i
	) {
		return 0;
	}
};

template <typename T>
class InsertNoSpaceMultiline {
public:
	size_t
	operator () (
		T* p,
		size_t i
	) {
		if (i & 0x0f)
			return 0;

		*p = '\n';
		return 1;
	}
};

template <typename T>
class InsertSpace {
public:
	size_t
	operator () (
		T* p,
		size_t i
	) {
		*p = ' ';
		return 1;
	}
};

template <typename T>
class InsertSpaceMultiline {
public:
	size_t
	operator () (
		T* p,
		size_t i
	) {
		*p = (i & 0x0f) ? ' ' : '\n';
		return 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetHexChar,
	typename InsertSpace
>
void
hexEncode(
	T* dst,
	const uchar_t* src,
	size_t size
) {
	uchar_t x = src[0];
	*dst++ = GetHexChar()(x >> 4);
	*dst++ = GetHexChar()(x);

	for (size_t i = 1; i < size; i++) {
		uchar_t x = src[i];
		dst += InsertSpace()(dst, i);
		*dst++ = GetHexChar()(x >> 4);
		*dst++ = GetHexChar()(x);
	}
}

//..............................................................................

} // namespace impl

template <typename T>
size_t
HexEncoding::encode(
	sl::StringBase<T>* string,
	const void* p,
	size_t size,
	uint_t flags
) {
	if (!size) {
		string->clear();
		return 0;
	}

	const uchar_t* src = (const uchar_t*)p;

	size_t length;
	if (flags & HexEncodingFlag_NoSpace) {
		length = size * 2;
		if (flags & HexEncodingFlag_Multiline) {
			size_t lineCount = length / 16;
			if (lineCount & 0x0f)
				lineCount++;

			length += lineCount - 1;
		}

		T* dst = string->createBuffer(length);
		if (!dst)
			return -1;

		if (flags & HexEncodingFlag_Multiline)
			if (flags & HexEncodingFlag_UpperCase)
				impl::hexEncode<T, GetHexChar_u, impl::InsertNoSpaceMultiline<T> >(dst, src, size);
			else
				impl::hexEncode<T, GetHexChar_l, impl::InsertNoSpaceMultiline<T> >(dst, src, size);
		else
			if (flags & HexEncodingFlag_UpperCase)
				impl::hexEncode<T, GetHexChar_u, impl::InsertNoSpace<T> >(dst, src, size);
			else
				impl::hexEncode<T, GetHexChar_l, impl::InsertNoSpace<T> >(dst, src, size);
	} else {
		length = size * 3 - 1;
		T* dst = string->createBuffer(length);
		if (!dst)
			return -1;

		if (flags & HexEncodingFlag_Multiline)
			if (flags & HexEncodingFlag_UpperCase)
				impl::hexEncode<T, GetHexChar_u, impl::InsertSpaceMultiline<T> >(dst, src, size);
			else
				impl::hexEncode<T, GetHexChar_l, impl::InsertSpaceMultiline<T> >(dst, src, size);
		else
			if (flags & HexEncodingFlag_UpperCase)
				impl::hexEncode<T, GetHexChar_u, impl::InsertSpace<T> >(dst, src, size);
			else
				impl::hexEncode<T, GetHexChar_l, impl::InsertSpace<T> >(dst, src, size);
	}

	return length;
}

//..............................................................................

} // namespace enc
} // namespace axl
