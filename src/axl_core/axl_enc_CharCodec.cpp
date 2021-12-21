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
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace enc {

//..............................................................................

CharCodec*
getCharCodec(CharCodecKind codecKind) {
	static StdCodec<Ascii>     asciiCodec;
	static StdCodec<Utf8>      utf8Codec;
	static StdCodec<Utf16s>    utf16Codec;
	static StdCodec<Utf16s_be> utf16Codec_be;
	static StdCodec<Utf32s>    utf32Codec;
	static StdCodec<Utf32s_be> utf32Codec_be;

	static CharCodec* codecTable[CharCodecKind__Count] = {
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
	const sl::StringRef_utf8& string,
	utf32_t replacement
) {
	buffer->clear();

	const utf8_t* p = string.cp();
 	const utf8_t* end = string.getEnd();
	while (p < end) {
		char tmpBuffer[256];
		ConvertLengthResult result = encode_utf8(
			tmpBuffer,
			sizeof(tmpBuffer),
			sl::StringRef_utf8(p, end - p),
			replacement
		);

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
	const sl::StringRef_utf16& string,
	utf32_t replacement
) {
	buffer->clear();

	const utf16_t* p = string.cp();
	const utf16_t* end = string.getEnd();
	while (p < end) {
		char tmpBuffer[256];
		ConvertLengthResult result = encode_utf16(
			tmpBuffer,
			sizeof(tmpBuffer),
			sl::StringRef_utf16(p, end - p),
			replacement
		);

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
	const sl::StringRef_utf32& string,
	utf32_t replacement
) {
	buffer->clear();

	const utf32_t* p = string.cp();
	const utf32_t* end = string.getEnd();
	while (p < end) {
		char tmpBuffer[256];
		ConvertLengthResult result = encode_utf32(
			tmpBuffer,
			sizeof(tmpBuffer),
			sl::StringRef_utf32(p, end - p),
			replacement
		);

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
	const void* p0,
	size_t size,
	utf32_t replacement
) {
	string->clear();

	DecoderState state = 0;
	const char* p = (const char*)p0;
	const char* end = p + size;
	while (p < end) {
		utf8_t tmpBuffer[256];
		ConvertLengthResult result = decode_utf8(
			&state,
			tmpBuffer,
			countof(tmpBuffer),
			p,
			end - p,
			replacement
		);

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
	const void* p0,
	size_t size,
	utf32_t replacement
) {
	string->clear();

	DecoderState state = 0;
	const char* p = (const char*)p0;
	const char* end = p + size;
	while (p < end) {
		utf16_t buffer[DecodeBufferLength];
		ConvertLengthResult result = decode_utf16(
			&state,
			buffer,
			countof(buffer),
			p,
			end - p,
			replacement
		);

		if (!result.m_srcLength)
			break;

		size_t length = string->append(buffer, result.m_dstLength);
		if (length == -1)
			return -1;

		p += result.m_srcLength;
	}

	return string->getLength();
}

size_t
CharCodec::decode_utf32(
	sl::String_utf32* string,
	const void* p0,
	size_t size,
	utf32_t replacement
) {
	string->clear();

	DecoderState state = 0;
	const char* p = (const char*)p0;
	const char* end = p + size;
	while (p < end) {
		utf32_t buffer[DecodeBufferLength];
		ConvertLengthResult result = decode_utf32(
			&state,
			buffer,
			countof(buffer),
			p,
			end - p,
			replacement
		);

		if (!result.m_srcLength)
			break;

		size_t length = string->append(buffer, result.m_dstLength);
		if (length == -1)
			return -1;

		p += result.m_srcLength;
	}

	return string->getLength();
}

//..............................................................................

} // namespace enc
} // namespace axl
