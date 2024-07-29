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
#include "axl_enc_Base32Encoding_nj.h"

namespace axl {
namespace enc {

//..............................................................................

class Base32Writer {
protected:
	char* m_p;
	char* m_end;
	size_t m_index;
	size_t m_hyphenDistance;

public:
	Base32Writer(
		char* p,
		size_t length,
		size_t hyphenDistance
	);

	void
	write(uchar_t x);

	void
	addPadding();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Base32Writer::Base32Writer(
	char* p,
	size_t length,
	size_t hyphenDistance
) {
	m_p = p;
	m_end = p + length;
	m_index = 0;
	m_hyphenDistance = hyphenDistance;
}

void
Base32Writer::write(uchar_t x) {
	static const char charTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	if (m_index && m_index % m_hyphenDistance == 0) {
		ASSERT(m_p < m_end);
		*m_p++ = '-';
	}

	ASSERT(m_p < m_end);
	*m_p++ = charTable[x & 0x1f];
	m_index++;
}

void
Base32Writer::addPadding() {
	static const char paddingTable[] = "0189";

	while (m_p < m_end)
		*m_p++ = paddingTable[rand() % lengthof(paddingTable)];
}

//..............................................................................

size_t
Base32Encoding_nj::encode(
	sl::String* string,
	const void* p,
	size_t size,
	size_t hyphenDistance
) {
	if (!size) {
		string->clear();
		return 0;
	}

	size_t bitCount = size * 8;
	size_t length = bitCount / 5;
	if (bitCount % 5)
		length++;

	if (hyphenDistance != -1) {
		size_t leftover = length % hyphenDistance;
		if (leftover)
			length += hyphenDistance - leftover;

		size_t hyphenCount = (length - 1) / hyphenDistance;
		length += hyphenCount;
	}

	char* dst = string->createBuffer(length);
	if (!dst)
		return -1;

	Base32Writer writer(dst, length, hyphenDistance);

	uchar_t x = 0;
	size_t i = 0;

	const uchar_t* src = (const uchar_t*)p;
	const uchar_t* srcEnd = src + size;
	for (; src < srcEnd; src++) {
		uchar_t y = *src;

		switch (i) {
		case 0:
			writer.write(y);
			x = y >> 5;
			i = 2;
			break;

		case 1:
			writer.write(x | (y << 4));
			writer.write(y >> 1);
			x = y >> 6;
			i = 3;
			break;

		case 2:
			writer.write(x | (y << 3));
			writer.write(y >> 2);
			x = y >> 7;
			i = 4;
			break;

		case 3:
			writer.write(x | (y << 2));
			writer.write(y >> 3);
			x = 0;
			i = 0;
			break;

		case 4:
			writer.write(x | (y << 1));
			x = y >> 4;
			i = 1;
			break;

		default:
			ASSERT(false);
		}
	}

	if (i)
		writer.write(x);

	writer.addPadding();
	return length;
}

size_t
Base32Encoding_nj::decode(
	sl::Array<char>* buffer,
	const sl::StringRef& string
) {
	size_t sizeGuess = (string.getLength() * 5) / 8;
	buffer->reserve(sizeGuess);
	buffer->clear();

	uchar_t x = 0;
	size_t i = 0;

	const char* p = string.cp();
	const char* end = string.getEnd();
	for (; p < end; p++) {
		char c = *p;

		uchar_t y;

		if (c >= '2' && c <= '7')
			y = c - '2' + 26;
		else if (c >= 'a' && c <= 'z')
			y = c - 'a';
		else if (c >= 'A' && c <= 'Z')
			y = c - 'A';
		else
			continue; // ignore padding, hyphens and other chars

		x |= y << i;

		if (i < 3) {
			i += 5;
		} else if (i == 3) {
			buffer->append(x);
			x = 0;
			i = 0;
		} else {
			buffer->append(x);
			x = y >> (8 - i);
			i -= 3;
		}
	}

	return buffer->getCount();
}

//..............................................................................

} // namespace enc
} // namespace axl
