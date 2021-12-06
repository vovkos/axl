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

#define _AXL_ENC_CHARCODEC_H

#include "axl_enc_Utf.h"
#include "axl_sl_Array.h"
#include "axl_sl_String.h"

namespace axl {
namespace enc {

//..............................................................................

enum CharCodecKind {
	CharCodecKind_Ascii = 0,
	CharCodecKind_Utf8,
	CharCodecKind_Utf16,
	CharCodecKind_Utf16_be,
	CharCodecKind_Utf32,
	CharCodecKind_Utf32_be,

	// later maybe add other important encodings

	CharCodecKind__Count,
};

//..............................................................................

class CharCodec {
public:
	typedef uint32_t DecoderState;

protected:
	size_t m_unitSize;

public:
	CharCodec() {
		m_unitSize = 1;
	}

	size_t
	getUnitSize() {
		return m_unitSize;
	}

	// calc encode buffer

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const utf8_t* p,
		size_t length
	) = 0;

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const utf16_t* p,
		size_t length
	) = 0;

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const utf32_t* p,
		size_t length
	) = 0;

	size_t
	calcRequiredBufferSizeToEncode_utf8(const sl::StringRef_utf8& string) {
		return calcRequiredBufferSizeToEncode_utf8(string.cp(), string.getLength());
	}

	size_t
	calcRequiredBufferSizeToEncode_utf16(const sl::StringRef_utf16& string) {
		return calcRequiredBufferSizeToEncode_utf16(string.cp(), string.getLength());
	}

	size_t
	calcRequiredBufferSizeToEncode_utf32(const sl::StringRef_utf32& string) {
		return calcRequiredBufferSizeToEncode_utf32(string.cp(), string.getLength());
	}

	// encode

	virtual
	EncodeLengthResult
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length
	) = 0;

	virtual
	EncodeLengthResult
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length
	) = 0;

	virtual
	EncodeLengthResult
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length
	) = 0;

	size_t
	encode_utf8(
		sl::Array<char>* buffer,
		const utf8_t* p,
		size_t length
	);

	size_t
	encode_utf16(
		sl::Array<char>* buffer,
		const utf16_t* p,
		size_t length
	);

	size_t
	encode_utf32(
		sl::Array<char>* buffer,
		const utf32_t* p,
		size_t length
	);

	size_t
	encode_utf8(
		sl::Array<char>* buffer,
		const sl::StringRef_utf8& string
	) {
		return encode_utf8(buffer, string.cp(), string.getLength());
	}

	size_t
	encode_utf16(
		sl::Array<char>* buffer,
		const sl::StringRef_utf16& string
	) {
		return encode_utf16(buffer, string.cp(), string.getLength());
	}

	size_t
	encode_utf32(
		sl::Array<char>* buffer,
		const sl::StringRef_utf32& string
	) {
		return encode_utf32(buffer, string.cp(), string.getLength());
	}

	sl::Array<char>
	encode_utf8(
		const utf8_t* p,
		size_t length
	) {
		sl::Array<char> buffer;
		encode_utf8(&buffer, p, length);
		return buffer;
	}

	sl::Array<char>
	encode_utf16(
		const utf16_t* p,
		size_t length
	) {
		sl::Array<char> buffer;
		encode_utf16(&buffer, p, length);
		return buffer;
	}

	sl::Array<char>
	encode_utf32(
		const utf32_t* p,
		size_t length
	) {
		sl::Array<char> buffer;
		encode_utf32(&buffer, p, length);
		return buffer;
	}

	sl::Array<char>
	encode_utf8(const sl::StringRef_utf8& string) {
		return encode_utf8(string.cp(), string.getLength());
	}

	sl::Array<char>
	encode_utf16(const sl::StringRef_utf16& string) {
		return encode_utf16(string.cp(), string.getLength());
	}

	sl::Array<char>
	encode_utf32(const sl::StringRef_utf32& string) {
		return encode_utf32(string.cp(), string.getLength());
	}

	// calc decode buffer

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		const void* p,
		size_t size
	) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		const void* p,
		size_t size
	) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		const void* p,
		size_t size
	) = 0;

	// decode

	virtual
	EncodeLengthResult
	decode_utf8(
		DecoderState* state,
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) = 0;

	virtual
	EncodeLengthResult
	decode_utf16(
		DecoderState* state,
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) = 0;

	virtual
	EncodeLengthResult
	decode_utf32(
		DecoderState* state,
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) = 0;

	virtual
	EncodeLengthResult
	decode_utf8(
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		DecoderState state = 0;
		return decode_utf8(&state, buffer, bufferLength, p, size);
	}

	virtual
	EncodeLengthResult
	decode_utf16(
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		DecoderState state = 0;
		return decode_utf16(&state, buffer, bufferLength, p, size);
	}

	virtual
	EncodeLengthResult
	decode_utf32(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		DecoderState state = 0;
		return decode_utf32(&state, buffer, bufferLength, p, size);
	}

	size_t
	decode_utf8(
		sl::String_utf8* string,
		const void* p,
		size_t size
	);

	size_t
	decode_utf16(
		sl::String_utf16* string,
		const void* p,
		size_t size
	);

	size_t
	decode_utf32(
		sl::String_utf32* string,
		const void* p,
		size_t size
	);

	sl::String_utf8
	decode_utf8(
		const void* p,
		size_t size
	) {
		sl::String_utf8 string;
		decode_utf8(&string, p, size);
		return string;
	}

	sl::String_utf16
	decode_utf16(
		const void* p,
		size_t size
	) {
		sl::String_utf16 string;
		decode_utf16(&string, p, size);
		return string;
	}

	sl::String_utf32
	decode_utf32(
		const void* p,
		size_t size
	) {
		sl::String_utf32 string;
		decode_utf32(&string, p, size);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CharCodec*
getCharCodec(CharCodecKind codecKind);

//..............................................................................

class AsciiCodec: public CharCodec {
public:
	AsciiCodec() {
		m_unitSize = 1;
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const utf8_t* p,
		size_t length
	) {
		return Convert<Ascii, Utf8>::calcRequiredLength(p, length);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const utf16_t* p,
		size_t length
	) {
		return Convert<Ascii, Utf16>::calcRequiredLength(p, length);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const utf32_t* p,
		size_t length
	) {
		return Convert<Ascii, Utf32>::calcRequiredLength(p, length);
	}

	virtual
	EncodeLengthResult
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length
	) {
		return Convert<Ascii, Utf8>::convert(
			(char*)buffer,
			bufferSize,
			p,
			length
		);
	}

	virtual
	EncodeLengthResult
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length
	) {
		return Convert<Ascii, Utf16>::convert(
			(char*)buffer,
			bufferSize,
			p,
			length
		);
	}

	virtual
	EncodeLengthResult
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length
	) {
		return Convert<Ascii, Utf32>::convert(
			(char*)buffer,
			bufferSize,
			p,
			length
		);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		const void* p,
		size_t size
	) {
		return Convert<Utf8, Ascii>::calcRequiredLength((char*)p, size);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		const void* p,
		size_t size
	) {
		return Convert<Utf16, Ascii>::calcRequiredLength((char*)p, size);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		const void* p,
		size_t size
	) {
		return Convert<Utf32, Ascii>::calcRequiredLength((char*)p, size);
	}

	virtual
	EncodeLengthResult
	decode_utf8(
		DecoderState* unused,
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		return Convert<Utf8, Ascii>::convert(
			buffer,
			bufferLength,
			(char*)p,
			size
		);
	}

	virtual
	EncodeLengthResult
	decode_utf16(
		DecoderState* unused,
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		return Convert<Utf16, Ascii>::convert(
			buffer,
			bufferLength,
			(char*)p,
			size
		);
	}

	virtual
	EncodeLengthResult
	decode_utf32(
		DecoderState* unused,
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		return Convert<Utf32, Ascii>::convert(
			buffer,
			bufferLength,
			(char*)p,
			size
		);
	}
};

//..............................................................................

template <typename T>
class UtfCodec: public CharCodec {
public:
	typedef typename T::C C;

public:
	UtfCodec() {
		m_unitSize = sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const utf8_t* p,
		size_t length
	) {
		return Convert<T, Utf8>::calcRequiredLength(p, length) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const utf16_t* p,
		size_t length
	) {
		return Convert<T, Utf16>::calcRequiredLength(p, length) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const utf32_t* p,
		size_t length
	) {
		return Convert<T, Utf32>::calcRequiredLength(p, length) * sizeof(C);
	}

	virtual
	EncodeLengthResult
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length
	) {
		EncodeLengthResult result = Convert<T, Utf8>::convert(
			(C*)buffer,
			bufferSize / sizeof(C),
			p,
			length
		);

		result.m_dstLength *= sizeof(C);
		return result;
	}

	virtual
	EncodeLengthResult
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length
	) {
		EncodeLengthResult result = Convert<T, Utf16>::convert(
			(C*)buffer,
			bufferSize / sizeof(C),
			p,
			length
		);

		result.m_dstLength *= sizeof(C);
		return result;
	}

	virtual
	EncodeLengthResult
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length
	) {
		EncodeLengthResult result = Convert<T, Utf32>::convert(
			(C*)buffer,
			bufferSize / sizeof(C),
			p,
			length
		);

		result.m_dstLength *= sizeof(C);
		return result;
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		const void* p,
		size_t size
	) {
		return Convert<Utf8, T>::calcRequiredLength((C*)p, size / sizeof(C));
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		const void* p,
		size_t size
	) {
		return Convert<Utf16, T>::calcRequiredLength((C*)p, size / sizeof(C));
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		const void* p,
		size_t size
	) {
		return Convert<Utf32, T>::calcRequiredLength((C*)p, size / sizeof(C));
	}

	virtual
	EncodeLengthResult
	decode_utf8(
		DecoderState* state,
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		EncodeLengthResult result = Convert<Utf8, T>::convert(
			state,
			buffer,
			bufferLength,
			(C*)p,
			size / sizeof(C)
		);

		result.m_srcLength *= sizeof(C);
		return result;
	}

	virtual
	EncodeLengthResult
	decode_utf16(
		DecoderState* state,
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		EncodeLengthResult result = Convert<Utf16, T>::convert(
			state,
			buffer,
			bufferLength,
			(C*) p,
			size / sizeof(C)
		);

		result.m_srcLength *= sizeof(C);
		return result;
	}

	virtual
	EncodeLengthResult
	decode_utf32(
		DecoderState* state,
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size
	) {
		EncodeLengthResult result = Convert<Utf32, T>::convert(
			state,
			buffer,
			bufferLength,
			(C*) p,
			size / sizeof(C)
		);

		result.m_srcLength *= sizeof(C);
		return result;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
