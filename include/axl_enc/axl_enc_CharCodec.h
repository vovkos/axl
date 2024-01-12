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

#include "axl_sl_Array.h"
#include "axl_sl_String.h"
#include "axl_enc_Latin1.h"

namespace axl {
namespace enc {

//..............................................................................

enum CharCodecKind {
	CharCodecKind_Latin1 = 0,
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
protected:
	enum {
		DecodeBufferLength = 256,
	};

public:
	// calc encode buffer

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	// encode

	virtual
	ConvertLengthResult
	encode_utf8_u(
		void* buffer,
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	encode_utf16_u(
		void* buffer,
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	encode_utf32_u(
		void* buffer,
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	size_t
	encode_utf8(
		sl::Array<char>* buffer,
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	);

	size_t
	encode_utf16(
		sl::Array<char>* buffer,
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	);

	size_t
	encode_utf32(
		sl::Array<char>* buffer,
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	);

	sl::Array<char>
	encode_utf8(
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) {
		sl::Array<char> buffer;
		encode_utf8(&buffer, string, replacement);
		return buffer;
	}

	sl::Array<char>
	encode_utf16(
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) {
		sl::Array<char> buffer;
		encode_utf16(&buffer, string, replacement);
		return buffer;
	}

	sl::Array<char>
	encode_utf32(
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) {
		sl::Array<char> buffer;
		encode_utf32(&buffer, string, replacement);
		return buffer;
	}

	// calc decode buffer

	virtual
	void
	advanceDecoderState(
		DecoderState* state,
		const void* p,
		size_t size
	) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		DecoderState* state,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		DecoderState* state,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		DecoderState* state,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	size_t
	calcRequiredBufferLengthToDecode_utf8(
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return calcRequiredBufferLengthToDecode_utf8(p, size, replacement);
	}

	size_t
	calcRequiredBufferLengthToDecode_utf16(
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return calcRequiredBufferLengthToDecode_utf16(p, size, replacement);
	}

	size_t
	calcRequiredBufferLengthToDecode_utf32(
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return calcRequiredBufferLengthToDecode_utf32(p, size, replacement);
	}

	// decode

	virtual
	ConvertLengthResult
	decode_utf8_u(
		DecoderState* state,
		utf8_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	decode_utf16_u(
		DecoderState* state,
		utf16_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	decode_utf32_u(
		DecoderState* state,
		utf32_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	ConvertLengthResult
	decode_utf8_u(
		utf8_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return decode_utf8_u(&state, buffer, p, size, replacement);
	}

	ConvertLengthResult
	decode_utf16_u(
		utf16_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return decode_utf16_u(&state, buffer, p, size, replacement);
	}

	ConvertLengthResult
	decode_utf32_u(
		utf32_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return decode_utf32_u(&state, buffer, p, size, replacement);
	}

	virtual
	ConvertLengthResult
	decode_utf8(
		DecoderState* state,
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	decode_utf16(
		DecoderState* state,
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	virtual
	ConvertLengthResult
	decode_utf32(
		DecoderState* state,
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) = 0;

	ConvertLengthResult
	decode_utf8(
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return decode_utf8(&state, buffer, bufferLength, p, size, replacement);
	}

	ConvertLengthResult
	decode_utf16(
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return decode_utf16(&state, buffer, bufferLength, p, size, replacement);
	}

	ConvertLengthResult
	decode_utf32(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		DecoderState state = 0;
		return decode_utf32(&state, buffer, bufferLength, p, size, replacement);
	}

	size_t
	decode_utf8(
		sl::String_utf8* string,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	);

	size_t
	decode_utf16(
		sl::String_utf16* string,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	);

	size_t
	decode_utf32(
		sl::String_utf32* string,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	);

	sl::String_utf8
	decode_utf8(
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		sl::String_utf8 string;
		decode_utf8(&string, p, size, replacement);
		return string;
	}

	sl::String_utf16
	decode_utf16(
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		sl::String_utf16 string;
		decode_utf16(&string, p, size, replacement);
		return string;
	}

	sl::String_utf32
	decode_utf32(
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		sl::String_utf32 string;
		decode_utf32(&string, p, size, replacement);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CharCodec*
getCharCodec(CharCodecKind codecKind);

//..............................................................................

template <typename T>
class StdCodec: public CharCodec {
public:
	typedef typename T::C C;

public:
	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) {
		return Convert<T, Utf8>::calcRequiredLength(
			string.cp(),
			string.getEnd(),
			replacement
		) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) {
		return Convert<T, Utf16>::calcRequiredLength(
			string.cp(),
			string.getEnd(),
			replacement
		) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) {
		return Convert<T, Utf32>::calcRequiredLength(
			string.cp(),
			string.getEnd(),
			replacement
		) * sizeof(C);
	}

	virtual
	ConvertLengthResult
	encode_utf8_u(
		void* buffer,
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<C, Utf8::C> result = Convert<T, Utf8>::convert_u(
			(C*)buffer,
			string.cp(),
			string.getEnd(),
			replacement
		);

		return ConvertLengthResult(
			(result.m_dst - (C*)buffer) * sizeof(C),
			result.m_src - string.cp()
		);
	}

	virtual
	ConvertLengthResult
	encode_utf16_u(
		void* buffer,
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<C, Utf16::C> result = Convert<T, Utf16>::convert_u(
			(C*)buffer,
			string.cp(),
			string.getEnd(),
			replacement
		);

		return ConvertLengthResult(
			(result.m_dst - (C*)buffer) * sizeof(C),
			result.m_src - string.cp()
		);
	}

	virtual
	ConvertLengthResult
	encode_utf32_u(
		void* buffer,
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<C, Utf32::C> result = Convert<T, Utf32>::convert_u(
			(C*)buffer,
			string.cp(),
			string.getEnd(),
			replacement
		);

		return ConvertLengthResult(
			(result.m_dst - (C*)buffer) * sizeof(C),
			result.m_src - string.cp()
		);
	}

	virtual
	ConvertLengthResult
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const sl::StringRef_utf8& string,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<C, Utf8::C> result = Convert<T, Utf8>::convert(
			(C*)buffer,
			(C*)buffer + bufferSize / sizeof(C),
			string.cp(),
			string.getEnd(),
			replacement
		);

		return ConvertLengthResult(
			(result.m_dst - (C*)buffer) * sizeof(C),
			result.m_src - string.cp()
		);
	}

	virtual
	ConvertLengthResult
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const sl::StringRef_utf16& string,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<C, Utf16::C> result = Convert<T, Utf16>::convert(
			(C*)buffer,
			(C*)buffer + bufferSize / sizeof(C),
			string.cp(),
			string.getEnd(),
			replacement
		);

		return ConvertLengthResult(
			(result.m_dst - (C*)buffer) * sizeof(C),
			result.m_src - string.cp()
		);
	}

	virtual
	ConvertLengthResult
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const sl::StringRef_utf32& string,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<C, Utf32::C> result = Convert<T, Utf32>::convert(
			(C*)buffer,
			(C*)buffer + bufferSize / sizeof(C),
			string.cp(),
			string.getEnd(),
			replacement
		);

		return ConvertLengthResult(
			(result.m_dst - (C*)buffer) * sizeof(C),
			result.m_src - string.cp()
		);
	}

	virtual
	void
	advanceDecoderState(
		DecoderState* state,
		const void* p,
		size_t size
	) {
		return Advance<typename T::Decoder>::advance(
			state,
			(C*)p,
			(C*)p + size / sizeof(C)
		);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		DecoderState* state,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		return Convert<Utf8, T>::calcRequiredLength(
			state,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		DecoderState* state,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		return Convert<Utf16, T>::calcRequiredLength(
			state,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		DecoderState* state,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		return Convert<Utf32, T>::calcRequiredLength(
			state,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);
	}

	virtual
	ConvertLengthResult
	decode_utf8_u(
		DecoderState* state,
		utf8_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<Utf8::C, C> result = Convert<Utf8, T>::convert_u(
			state,
			buffer,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);

		return ConvertLengthResult(
			result.m_dst - buffer,
			(result.m_src - (C*)p) * sizeof(C)
		);
	}

	virtual
	ConvertLengthResult
	decode_utf16_u(
		DecoderState* state,
		utf16_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<Utf16::C, C> result = Convert<Utf16, T>::convert_u(
			state,
			buffer,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);

		return ConvertLengthResult(
			result.m_dst - buffer,
			(result.m_src - (C*)p) * sizeof(C)
		);
	}

	virtual
	ConvertLengthResult
	decode_utf32_u(
		DecoderState* state,
		utf32_t* buffer,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<Utf32::C, C> result = Convert<Utf32, T>::convert_u(
			state,
			buffer,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);

		return ConvertLengthResult(
			result.m_dst - buffer,
			(result.m_src - (C*)p) * sizeof(C)
		);
	}

	virtual
	ConvertLengthResult
	decode_utf8(
		DecoderState* state,
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<Utf8::C, C> result = Convert<Utf8, T>::convert(
			state,
			buffer,
			buffer + bufferLength,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);

		return ConvertLengthResult(
			result.m_dst - buffer,
			(result.m_src - (C*)p) * sizeof(C)
		);
	}

	virtual
	ConvertLengthResult
	decode_utf16(
		DecoderState* state,
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<Utf16::C, C> result = Convert<Utf16, T>::convert(
			state,
			buffer,
			buffer + bufferLength,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);

		return ConvertLengthResult(
			result.m_dst - buffer,
			(result.m_src - (C*)p) * sizeof(C)
		);
	}

	virtual
	ConvertLengthResult
	decode_utf32(
		DecoderState* state,
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult<Utf32::C, C> result = Convert<Utf32, T>::convert(
			state,
			buffer,
			buffer + bufferLength,
			(C*)p,
			(C*)p + size / sizeof(C),
			replacement
		);

		return ConvertLengthResult(
			result.m_dst - buffer,
			(result.m_src - (C*)p) * sizeof(C)
		);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
