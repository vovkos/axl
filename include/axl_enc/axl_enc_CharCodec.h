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

enum CharCodecKind
{
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

class CharCodec
{
protected:
	size_t m_unitSize;

public:
	CharCodec()
	{
		m_unitSize = 1;
	}

	size_t
	getUnitSize()
	{
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
	calcRequiredBufferSizeToEncode_utf8(const sl::StringRef_utf8& string)
	{
		return calcRequiredBufferSizeToEncode_utf8(string.cp(), string.getLength());
	}

	size_t
	calcRequiredBufferSizeToEncode_utf16(const sl::StringRef_utf16& string)
	{
		return calcRequiredBufferSizeToEncode_utf16(string.cp(), string.getLength());
	}

	size_t
	calcRequiredBufferSizeToEncode_utf32(const sl::StringRef_utf32& string)
	{
		return calcRequiredBufferSizeToEncode_utf32(string.cp(), string.getLength());
	}

	// encode

	virtual
	size_t
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length,
		size_t* takenLength = NULL
		) = 0;

	virtual
	size_t
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length,
		size_t* takenLength = NULL
		) = 0;

	virtual
	size_t
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length,
		size_t* takenLength = NULL
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
		)
	{
		encode_utf8(buffer, string.cp(), string.getLength());
	}

	size_t
	encode_utf16(
		sl::Array<char>* buffer,
		const sl::StringRef_utf16& string
		)
	{
		encode_utf16(buffer, string.cp(), string.getLength());
	}

	size_t
	encode_utf32(
		sl::Array<char>* buffer,
		const sl::StringRef_utf32& string
		)
	{
		encode_utf32(buffer, string.cp(), string.getLength());
	}

	sl::Array<char>
	encode_utf8(
		const utf8_t* p,
		size_t length
		)
	{
		sl::Array<char> buffer;
		encode_utf8(&buffer, p, length);
		return buffer;
	}

	sl::Array<char>
	encode_utf16(
		const utf16_t* p,
		size_t length
		)
	{
		sl::Array<char> buffer;
		encode_utf16(&buffer, p, length);
		return buffer;
	}

	sl::Array<char>
	encode_utf32(
		const utf32_t* p,
		size_t length
		)
	{
		sl::Array<char> buffer;
		encode_utf32(&buffer, p, length);
		return buffer;
	}

	sl::Array<char>
	encode_utf8(const sl::StringRef_utf8& string)
	{
		return encode_utf8(string.cp(), string.getLength());
	}

	sl::Array<char>
	encode_utf16(const sl::StringRef_utf16& string)
	{
		return encode_utf16(string.cp(), string.getLength());
	}

	sl::Array<char>
	encode_utf32(const sl::StringRef_utf32& string)
	{
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

	// decode buffer

	virtual
	size_t
	decode_utf8(
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		) = 0;

	virtual
	size_t
	decode_utf16(
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		) = 0;

	virtual
	size_t
	decode_utf32(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		) = 0;

	virtual
	size_t
	decode_utf32(
		uchar_t* cplBuffer,
		utf32_t* textBuffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		) = 0;

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
		)
	{
		sl::String_utf8 string;
		decode_utf8(&string, p, size);
		return string;
	}

	sl::String_utf16
	decode_utf16(
		const void* p,
		size_t size
		)
	{
		sl::String_utf16 string;
		decode_utf16(&string, p, size);
		return string;
	}

	sl::String_utf32
	decode_utf32(
		const void* p,
		size_t size
		)
	{
		sl::String_utf32 string;
		decode_utf32(&string, p, size);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CharCodec*
getCharCodec(CharCodecKind codecKind);

//..............................................................................

class AsciiCodec: public CharCodec
{
public:
	AsciiCodec()
	{
		m_unitSize = 1;
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const utf8_t* p,
		size_t length
		)
	{
		return UtfToAsciiConvert<Utf8>::calcRequiredLength(p, length);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const utf16_t* p,
		size_t length
		)
	{
		return UtfToAsciiConvert<Utf16>::calcRequiredLength(p, length);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const utf32_t* p,
		size_t length
		)
	{
		return UtfToAsciiConvert<Utf32>::calcRequiredLength(p, length);
	}

	virtual
	size_t
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length,
		size_t* takenLength = NULL
		)
	{
		return UtfToAsciiConvert<Utf8>::convert(
			(char*)buffer,
			bufferSize,
			p,
			length,
			takenLength
			);
	}

	virtual
	size_t
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length,
		size_t* takenLength = NULL
		)
	{
		return UtfToAsciiConvert<Utf16>::convert(
			(char*)buffer,
			bufferSize,
			p,
			length,
			takenLength
			);
	}

	virtual
	size_t
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length,
		size_t* takenLength = NULL
		)
	{
		return UtfToAsciiConvert<Utf32>::convert(
			(char*)buffer,
			bufferSize,
			p,
			length,
			takenLength
			);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		const void* p,
		size_t size
		)
	{
		return AsciiToUtfConvert<Utf8>::calcRequiredLength((const char*) p, size);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		const void* p,
		size_t size
		)
	{
		return AsciiToUtfConvert<Utf16>::calcRequiredLength((const char*) p, size);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		const void* p,
		size_t size
		)
	{
		return AsciiToUtfConvert<Utf32>::calcRequiredLength((const char*) p, size);
	}

	virtual
	size_t
	decode_utf8(
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		return AsciiToUtfConvert<Utf8>::convert(
			buffer,
			bufferLength,
			(const char*) p,
			size,
			takenSize
			);
	}

	virtual
	size_t
	decode_utf16(
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		return AsciiToUtfConvert<Utf16>::convert(
			buffer,
			bufferLength,
			(const char*) p,
			size,
			takenSize
			);
	}

	virtual
	size_t
	decode_utf32(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		return AsciiToUtfConvert<Utf32>::convert(
			buffer,
			bufferLength,
			(const char*) p,
			size,
			takenSize
			);
	}

	virtual
	size_t
	decode_utf32(
		uchar_t* cplBuffer,
		utf32_t* textBuffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		return AsciiToUtfConvert<Utf32>::convert(
			cplBuffer,
			textBuffer,
			bufferLength,
			(const char*) p,
			size,
			takenSize
			);
	}};

//..............................................................................

template <typename T>
class UtfCodec: public CharCodec
{
public:
	typedef typename T::C C;

public:
	UtfCodec()
	{
		m_unitSize = sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf8(
		const utf8_t* p,
		size_t length
		)
	{
		return UtfConvert<T, Utf8>::calcRequiredLength(p, length) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf16(
		const utf16_t* p,
		size_t length
		)
	{
		return UtfConvert<T, Utf16>::calcRequiredLength(p, length) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncode_utf32(
		const utf32_t* p,
		size_t length
		)
	{
		return UtfConvert<T, Utf32>::calcRequiredLength(p, length) * sizeof(C);
	}

	virtual
	size_t
	encode_utf8(
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length,
		size_t* takenLength = NULL
		)
	{
		return UtfConvert<T, Utf8>::convert(
			(C*)buffer,
			bufferSize / sizeof(C),
			p,
			length,
			takenLength
			) * sizeof(C);
	}

	virtual
	size_t
	encode_utf16(
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length,
		size_t* takenLength = NULL
		)
	{
		return UtfConvert<T, Utf16>::convert(
			(C*)buffer,
			bufferSize / sizeof(C),
			p,
			length,
			takenLength
			) * sizeof(C);
	}

	virtual
	size_t
	encode_utf32(
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length,
		size_t* takenLength = NULL
		)
	{
		return UtfConvert<T, Utf32>::convert(
			(C*)buffer,
			bufferSize / sizeof(C),
			p,
			length,
			takenLength
			) * sizeof(C);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf8(
		const void* p,
		size_t size
		)
	{
		return UtfConvert<Utf8, T>::calcRequiredLength(
			(const C*) p,
			size / sizeof(C)
			);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf16(
		const void* p,
		size_t size
		)
	{
		return UtfConvert<Utf16, T>::calcRequiredLength(
			(const C*) p,
			size / sizeof(C)
			);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecode_utf32(
		const void* p,
		size_t size
		)
	{
		return UtfConvert<Utf32, T>::calcRequiredLength(
			(const C*) p,
			size / sizeof(C)
			);
	}

	virtual
	size_t
	decode_utf8(
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		size_t takenLength;
		size_t takenBufferLength = UtfConvert<Utf8, T>::convert(
			buffer,
			bufferLength,
			(const C*) p,
			size / sizeof(C),
			&takenLength
			);

		if (takenSize)
			*takenSize = takenLength * sizeof(C);

		return takenBufferLength;
	}

	virtual
	size_t
	decode_utf16(
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		size_t takenLength;
		size_t takenBufferLength = UtfConvert<Utf16, T>::convert(
			buffer,
			bufferLength,
			(const C*) p,
			size / sizeof(C),
			&takenLength
			);

		if (takenSize)
			*takenSize = takenLength * sizeof(C);

		return takenBufferLength;
	}

	virtual
	size_t
	decode_utf32(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		size_t takenLength;
		size_t takenBufferLength = UtfConvert<Utf32, T>::convert(
			buffer,
			bufferLength,
			(const C*) p,
			size / sizeof(C),
			&takenLength
			);

		if (takenSize)
			*takenSize = takenLength * sizeof(C);

		return takenBufferLength;
	}

	virtual
	size_t
	decode_utf32(
		uchar_t* cplBuffer,
		utf32_t* textBuffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize = NULL
		)
	{
		size_t takenLength;
		size_t takenBufferLength = UtfConvert<Utf32, T>::convert(
			cplBuffer,
			textBuffer,
			bufferLength,
			(const C*) p,
			size / sizeof(C),
			&takenLength
			);

		if (takenSize)
			*takenSize = takenLength * sizeof(C);

		return takenBufferLength;
	}
};

//..............................................................................

class CodePointDecoder
{
protected:
	CharCodec* m_charCodec;

	size_t m_accumulatorCount;

	union
	{
		char m_accumulator[4];
		uint32_t m_accumulator_i32;
	};

public:
	CodePointDecoder(
		CharCodecKind codecKind = CharCodecKind_Utf8,
		uint32_t state = 0
		)
	{
		setup(codecKind, state);
	}

	CharCodec*
	getCharCodec()
	{
		return m_charCodec;
	}

	size_t
	getAccumulatorCount()
	{
		return m_accumulatorCount;
	}

	const char*
	getAccumulator()
	{
		return m_accumulator;
	}

	uint32_t
	getAccumulator_i32()
	{
		return m_accumulator_i32;
	}

	void
	setup(
		CharCodecKind codecKind,
		uint32_t state = 0
		)
	{
		setup(enc::getCharCodec(codecKind), state);
	}

	void
	setup(
		CharCodec* codec,
		uint32_t state = 0
		)
	{
		m_charCodec = codec;
		loadState(state);
	}

	void
	clear()
	{
		m_charCodec = NULL;
		resetAccumulator();
	}

	void
	resetAccumulator()
	{
		m_accumulatorCount = 0;
	}

	void
	loadState(uint32_t state);

	uint32_t
	saveState();

	size_t
	decode(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize
		);

	size_t
	decode(
		uchar_t* cplBuffer,
		utf32_t* textBuffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize
		);

protected:
	size_t
	decodeImpl(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize
		);

	size_t
	decodeImpl(
		uchar_t* cplBuffer,
		utf32_t* textBuffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenSize
		);
};

//..............................................................................

} // namespace enc
} // namespace axl
