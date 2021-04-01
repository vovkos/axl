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

#include "pch.h"
#include "axl_enc_Base64Encoding.h"

namespace axl {
namespace enc {

//..............................................................................

class InsertNoNl
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InsertNl
{
public:
	size_t
	operator () (
		char* p,
		size_t i
		)
	{
		if (i & 0x3f)
			return 0;

		*p = '\n';
		return 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename GetBase64Char,
	typename InsertNl
	>
static
void
encodeImpl(
	char* dst,
	size_t dstSize,
	const uchar_t* src,
	size_t srcSize
	)
{
	uint_t x = 0;
	size_t b = 0;

	char* dstEnd = dst + dstSize;
	const uchar_t* srcEnd = src + srcSize;

	for (size_t i = 0; src < srcEnd; src++)
	{
		x <<= 8;
		x |= *src;
		b += 8;

		while (b >= 6)
		{
			b -= 6;
			dst += InsertNl()(dst, i++);
			*dst++ = GetBase64Char()(x >> b);
		}
	}

	if (b) // can't be a newline here
		*dst++ = GetBase64Char()(x << (6 - b));

	while (dst < dstEnd)
		*dst++ = '=';

	ASSERT(dst == dstEnd);
}

//..............................................................................

size_t
Base64Encoding::encode(
	sl::String* string,
	const void* p,
	size_t size,
	uint_t flags
	)
{
	if (!size)
	{
		string->clear();
		return 0;
	}

	size_t bitCount = size * 8;
	size_t length = bitCount / 6;
	if (bitCount % 6)
		length++;

	if (!(flags & Base64EncodingFlag_NoPadding))
		length += 3 - ((length - 1) & 3);

	if (flags & Base64EncodingFlag_Multiline)
	{
		size_t lineCount = length / 64;
		if (lineCount & 0x3f)
			lineCount++;

		length += lineCount - 1;
	}

	char* dst = string->createBuffer(length);
	if (!dst)
		return -1;

	if (flags & Base64EncodingFlag_Multiline)
	{
		if (flags & Base64EncodingFlag_UrlChars)
			encodeImpl<GetBase64UrlChar, InsertNl>(dst, length, (uchar_t*)p, size);
		else
			encodeImpl<GetBase64Char, InsertNl>(dst, length, (uchar_t*)p, size);
	}
	else
	{
		if (flags & Base64EncodingFlag_UrlChars)
			encodeImpl<GetBase64UrlChar, InsertNoNl>(dst, length, (uchar_t*)p, size);
		else
			encodeImpl<GetBase64Char, InsertNoNl>(dst, length, (uchar_t*)p, size);
	}

	return length;
}

size_t
Base64Encoding::decode(
	sl::Array<char>* buffer,
	const sl::StringRef& string
	)
{
	static const uchar_t charMap['z' - '+' + 1] =
	{
		62, 63, 62,                                         //  +  ,  -
		-1,                                                 //  .
		63,                                                 //  /
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61,             //  0 .. 9
		-1, -1, -1, -1, -1, -1, -1,                         //  :  ;  <  =  >  ?  @
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, //  A .. M
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, //  N .. Z
		-1, -1, -1, -1,                                     //  [ \ ] ^
		63,                                                 //  _
		-1,                                                 //  `
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, //  a .. m
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, //  n .. z
	};

	size_t sizeGuess = (string.getLength() * 6) / 8;
	buffer->reserve(sizeGuess);
	buffer->clear();

	uint_t x = 0;
	size_t i = 0;

	const char* p = string.cp();
	const char* end = string.getEnd();
	for (; p < end; p++)
	{
		char c = *p;
		if (c < '+' || c > 'z')
			continue; // ignore padding & whitespace

		uchar_t y = charMap[c - '+'];
		if (y > 63)
			continue; // ignore padding & non-base64 chars

		x <<= 6;
		x |= y;
		i += 6;

		if (i >= 8)
		{
			i -= 8;
			buffer->append(x >> i);
		}
	}

	return buffer->getCount();
}

//..............................................................................

} // namespace enc
} // namespace axl
