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
#include "axl_enc_CharCodec.h"

namespace axl {
namespace enc {

//..............................................................................

CharCodec*
getCharCodec(CharCodecKind codecKind) {
	static AsciiCodec         asciiCodec;
	static UtfCodec<Utf8>     utf8Codec;
	static UtfCodec<Utf16>    utf16Codec;
	static UtfCodec<Utf16_be> utf16Codec_be;
	static UtfCodec<Utf32>    utf32Codec;
	static UtfCodec<Utf32_be> utf32Codec_be;

	static CharCodec* codecTable[] = {
		&asciiCodec,
		&utf8Codec,
		&utf16Codec,
		&utf16Codec_be,
		&utf32Codec,
		&utf32Codec_be,
	};

	return (size_t)codecKind < countof(codecTable) ? codecTable[codecKind] : NULL;
}

//..............................................................................

size_t
CharCodec::encode_utf8(
	sl::Array<char>* buffer,
	const utf8_t* p,
	size_t length
) {
	buffer->clear();
	buffer->reserve(length * m_unitSize);

	const utf8_t* end = p + length;
	while (p < end) {
		char tmpBuffer[256];

		EncodeLengthResult result = encode_utf8(tmpBuffer, sizeof(tmpBuffer), p, end - p);
		if (!result.m_srcLength)
			break;

		size_t size = buffer->append(tmpBuffer, result.m_dstLength);
		if (size == -1)
			return -1;

		p += result.m_srcLength;
	}

	return buffer->getCount();
}

size_t
CharCodec::encode_utf16(
	sl::Array<char>* buffer,
	const utf16_t* p,
	size_t length
) {
	buffer->clear();
	buffer->reserve(length * m_unitSize);

	const utf16_t* end = p + length;
	while (p < end) {
		char tmpBuffer[256];

		EncodeLengthResult result = encode_utf16(tmpBuffer, sizeof(tmpBuffer), p, end - p);
		if (!result.m_srcLength)
			break;

		size_t size = buffer->append(tmpBuffer, result.m_dstLength);
		if (size == -1)
			return -1;

		p += result.m_srcLength;
	}

	return buffer->getCount();
}

size_t
CharCodec::encode_utf32(
	sl::Array<char>* buffer,
	const utf32_t* p,
	size_t length
) {
	buffer->clear();
	buffer->reserve(length * m_unitSize);

	const utf32_t* end = p + length;
	while (p < end) {
		char tmpBuffer[256];

		EncodeLengthResult result = encode_utf32(tmpBuffer, sizeof(tmpBuffer), p, end - p);
		if (!result.m_srcLength)
			break;

		size_t size = buffer->append(tmpBuffer, result.m_dstLength);
		if (size == -1)
			return -1;

		p += result.m_srcLength;
	}

	return buffer->getCount();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
CharCodec::decode_utf8(
	sl::String_utf8* string,
	const void* _p,
	size_t size
) {
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	string->clear();
	string->reserve(length);

	DecoderState state = 0;
	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end) {
		utf8_t tmpBuffer[256];

		EncodeLengthResult result = decode_utf8(&state, tmpBuffer, countof(tmpBuffer), p, end - p);
		if (!result.m_srcLength)
			break;

		size_t length = string->append(tmpBuffer, result.m_dstLength);
		if (length == -1)
			return -1;

		p += result.m_srcLength;
	}

	return string->getLength();
}

size_t
CharCodec::decode_utf16(
	sl::String_utf16* string,
	const void* _p,
	size_t size
) {
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	string->clear();
	string->reserve(length);

	DecoderState state = 0;
	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end) {
		utf16_t tmpBuffer[256];

		EncodeLengthResult result = decode_utf16(&state, tmpBuffer, countof(tmpBuffer), p, end - p);
		if (!result.m_srcLength)
			break;

		size_t length = string->append(tmpBuffer, result.m_dstLength);
		if (length == -1)
			return -1;

		p += result.m_srcLength;
	}

	return string->getLength();
}

size_t
CharCodec::decode_utf32(
	sl::String_utf32* string,
	const void* _p,
	size_t size
) {
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	string->clear();
	string->reserve(length);

	DecoderState state = 0;
	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end) {
		utf32_t tmpBuffer[256];

		EncodeLengthResult result = decode_utf32(&state, tmpBuffer, countof(tmpBuffer), p, end - p);
		if (!result.m_srcLength)
			break;

		size_t length = string->append(tmpBuffer, result.m_dstLength);
		if (length == -1)
			return -1;

		p += result.m_srcLength;
	}

	return string->getLength();
}

//..............................................................................

} // namespace enc
} // namespace axl
