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
#include "axl_enc_Base32Encoding.h"

namespace axl {
namespace enc {

//..............................................................................

class InsertNoNl {
public:
	size_t
	operator () (
		char* p,
		size_t i
	) {
		return 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InsertNl {
public:
	size_t
	operator () (
		char* p,
		size_t i
	) {
		if (i & 0x3f)
			return 0;

		*p = '\n';
		return 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename GetBase32Char,
	typename InsertNl
>
static
void
encodeImpl(
	char* dst,
	size_t dstSize,
	const uchar_t* src,
	size_t srcSize
) {
	uint_t x = 0;
	size_t b = 0;

	char* dstEnd = dst + dstSize;
	const uchar_t* srcEnd = src + srcSize;

	for (size_t i = 0; src < srcEnd; src++) {
		x <<= 8;
		x |= *src;
		b += 8;

		while (b >= 5) {
			b -= 5;
			dst += InsertNl()(dst, i++);
			*dst++ = GetBase32Char()(x >> b);
		}
	}

	if (b) // can't be a newline here
		*dst++ = GetBase32Char()(x << (5 - b));

	while (dst < dstEnd)
		*dst++ = '=';

	ASSERT(dst == dstEnd);
}

//..............................................................................

size_t
Base32Encoding::encode(
	sl::String* string,
	const void* p,
	size_t size,
	uint_t flags
) {
	if (!size) {
		string->clear();
		return 0;
	}

	size_t bitCount = size * 8;
	size_t length = bitCount / 5;
	if (bitCount % 5)
		length++;

	if (!(flags & Base32EncodingFlag_NoPadding))
		length += 7 - ((length - 1) & 7);

	if (flags & Base32EncodingFlag_Multiline) {
		size_t lineCount = length / 64;
		if (lineCount & 0x3f)
			lineCount++;

		length += lineCount - 1;
	}

	char* dst = string->createBuffer(length);
	if (!dst)
		return -1;

	if (flags & Base32EncodingFlag_Multiline)
		encodeImpl<GetBase32Char, InsertNl>(dst, length, (uchar_t*)p, size);
	else
		encodeImpl<GetBase32Char, InsertNoNl>(dst, length, (uchar_t*)p, size);

	return length;
}

size_t
Base32Encoding::decode(
	sl::Array<char>* buffer,
	const sl::StringRef& string
) {
	static const uchar_t charMap['Z' - '2' + 1] = {
		26, 27, 28, 29, 30, 31,                             //  2 .. 7
		-1, -1, -1, -1, -1, -1, -1, -1, -1,                 //  8  9  :  ;  <  =  >  ?  @
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, //  A .. M
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, //  N .. Z
	};

	size_t sizeGuess = (string.getLength() * 5) / 8;
	buffer->reserve(sizeGuess);
	buffer->clear();

	uint_t x = 0;
	size_t i = 0;

	const char* p = string.cp();
	const char* end = string.getEnd();
	for (; p < end; p++) {
		char c = *p;
		if (c < '2' || c > 'Z')
			continue; // ignore padding & whitespace

		uchar_t y = charMap[c - '2'];
		if (y > 31)
			continue; // ignore padding & non-Base32 chars

		x <<= 5;
		x |= y;
		i += 5;

		if (i >= 8) {
			i -= 8;
			buffer->append(x >> i);
		}
	}

	return buffer->getCount();
}

//..............................................................................

} // namespace enc
} // namespace axl
