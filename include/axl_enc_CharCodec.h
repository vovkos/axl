// This file is part of Ninja Scroll (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ENC_CHARCODEC_H

#include "axl_enc_Utf.h"
#include "axl_sl_Array.h"

namespace axl {
namespace enc {

//.............................................................................

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

//.............................................................................

class CharCodec
{
protected:
	size_t m_unitSize;

public:
	CharCodec ()
	{
		m_unitSize = 1;
	}

	size_t
	getUnitSize ()
	{
		return m_unitSize;
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf8 (
		const utf8_t* p,
		size_t length
		) = 0;

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf16 (
		const utf16_t* p,
		size_t length
		) = 0;

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf32 (
		const utf32_t* p,
		size_t length
		) = 0;

	size_t
	encodeFromUtf8 (
		sl::Array <char>* buffer,
		const utf8_t* p,
		size_t length
		);

	size_t
	encodeFromUtf16 (
		sl::Array <char>* buffer,
		const utf16_t* p,
		size_t length
		);

	size_t
	encodeFromUtf32 (
		sl::Array <char>* buffer,
		const utf32_t* p,
		size_t length
		);

	sl::Array <char>
	encodeFromUtf8 (
		const utf8_t* p,
		size_t length
		);

	sl::Array <char>
	encodeFromUtf16 (
		const utf16_t* p,
		size_t length
		);

	sl::Array <char>
	encodeFromUtf32 (
		const utf32_t* p,
		size_t length
		);

	virtual
	void
	encodeFromUtf8 (
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		) = 0;

	virtual
	void
	encodeFromUtf16 (
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		) = 0;

	virtual
	void
	encodeFromUtf32 (
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf8 (
		const void* p,
		size_t size
		) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf16 (
		const void* p,
		size_t size
		) = 0;

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf32 (
		const void* p,
		size_t size
		) = 0;

	size_t
	decodeToUtf8 (
		sl::Array <utf8_t>* buffer,
		const void* p,
		size_t size
		);

	size_t
	decodeToUtf16 (
		sl::Array <utf16_t>* buffer,
		const void* p,
		size_t size
		);

	size_t
	decodeToUtf32 (
		sl::Array <utf32_t>* buffer,
		const void* p,
		size_t size
		);

	sl::Array <utf8_t>
	decodeToUtf8 (
		const void* p,
		size_t size
		);

	sl::Array <utf16_t>
	decodeToUtf16 (
		const void* p,
		size_t size
		);

	sl::Array <utf32_t>
	decodeToUtf32 (
		const void* p,
		size_t size
		);

	virtual
	void
	decodeToUtf8 (
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		) = 0;

	virtual
	void
	decodeToUtf16 (
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		) = 0;

	virtual
	void
	decodeToUtf32 (
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CharCodec*
getCharCodec (CharCodecKind codecKind);

//.............................................................................

class AsciiCodec: public CharCodec
{
public:
	AsciiCodec ()
	{
		m_unitSize = 1;
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf8 (
		const utf8_t* p,
		size_t length
		)
	{
		return UtfToAsciiConvert <Utf8>::calcRequiredLength (p, length);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf16 (
		const utf16_t* p,
		size_t length
		)
	{
		return UtfToAsciiConvert <Utf16>::calcRequiredLength (p, length);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf32 (
		const utf32_t* p,
		size_t length
		)
	{
		return UtfToAsciiConvert <Utf32>::calcRequiredLength (p, length);
	}

	virtual
	void
	encodeFromUtf8 (
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		)
	{
		UtfToAsciiConvert <Utf8>::convert (
			(char*) buffer,
			bufferSize,
			p,
			length,
			takenBufferSize_o,
			takenLength_o
			);
	}

	virtual
	void
	encodeFromUtf16 (
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		)
	{
		UtfToAsciiConvert <Utf16>::convert (
			(char*) buffer,
			bufferSize,
			p,
			length,
			takenBufferSize_o,
			takenLength_o
			);
	}

	virtual
	void
	encodeFromUtf32 (
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		)
	{
		UtfToAsciiConvert <Utf32>::convert (
			(char*) buffer,
			bufferSize,
			p,
			length,
			takenBufferSize_o,
			takenLength_o
			);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf8 (
		const void* p,
		size_t size
		)
	{
		return AsciiToUtfConvert <Utf8>::calcRequiredLength ((const char*) p, size);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf16 (
		const void* p,
		size_t size
		)
	{
		return AsciiToUtfConvert <Utf16>::calcRequiredLength ((const char*) p, size);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf32 (
		const void* p,
		size_t size
		)
	{
		return AsciiToUtfConvert <Utf32>::calcRequiredLength ((const char*) p, size);
	}

	virtual
	void
	decodeToUtf8 (
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o,
		size_t* takenSize_o,
		size_t* expectedSize_o
		)
	{
		AsciiToUtfConvert <Utf8>::convert (
			buffer,
			bufferLength,
			(const char*) p,
			size,
			takenBufferLength_o,
			takenSize_o,
			expectedSize_o
			);
	}

	virtual
	void
	decodeToUtf16 (
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		)
	{
		AsciiToUtfConvert <Utf16>::convert (
			buffer,
			bufferLength,
			(const char*) p,
			size,
			takenBufferLength_o,
			takenSize_o,
			expectedSize_o
			);
	}

	virtual
	void
	decodeToUtf32 (
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		)
	{
		AsciiToUtfConvert <Utf32>::convert (
			buffer,
			bufferLength,
			(const char*) p,
			size,
			takenBufferLength_o,
			takenSize_o,
			expectedSize_o
			);
	}
};

//.............................................................................

template <typename T>
class UtfCodec: public CharCodec
{
public:
	typedef typename T::C C;

public:
	UtfCodec ()
	{
		m_unitSize = sizeof (C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf8 (
		const utf8_t* p,
		size_t length
		)
	{
		return UtfConvert <T, Utf8>::calcRequiredLength (p, length) * sizeof (C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf16 (
		const utf16_t* p,
		size_t length
		)
	{
		return UtfConvert <T, Utf16>::calcRequiredLength (p, length) * sizeof (C);
	}

	virtual
	size_t
	calcRequiredBufferSizeToEncodeFromUtf32 (
		const utf32_t* p,
		size_t length
		)
	{
		return UtfConvert <T, Utf32>::calcRequiredLength (p, length) * sizeof (C);
	}

	virtual
	void
	encodeFromUtf8 (
		void* buffer,
		size_t bufferSize,
		const utf8_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		)
	{
		size_t bufferLength = bufferSize / sizeof (C);
		size_t takenDstLength;

		UtfConvert <T, Utf8>::convert (
			(C*) buffer,
			bufferLength,
			p,
			length,
			&takenDstLength,
			takenLength_o
			);

		if (takenBufferSize_o)
			*takenBufferSize_o = takenDstLength * sizeof (C);
	}

	virtual
	void
	encodeFromUtf16 (
		void* buffer,
		size_t bufferSize,
		const utf16_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		)
	{
		size_t bufferLength = bufferSize / sizeof (C);
		size_t takenDstLength;

		UtfConvert <T, Utf16>::convert (
			(C*) buffer,
			bufferLength,
			p,
			length,
			&takenDstLength,
			takenLength_o
			);

		if (takenBufferSize_o)
			*takenBufferSize_o = takenDstLength * sizeof (C);
	}

	virtual
	void
	encodeFromUtf32 (
		void* buffer,
		size_t bufferSize,
		const utf32_t* p,
		size_t length,
		size_t* takenBufferSize_o = NULL,
		size_t* takenLength_o = NULL
		)
	{
		size_t bufferLength = bufferSize / sizeof (C);
		size_t takenDstLength;

		UtfConvert <T, Utf32>::convert (
			(C*) buffer,
			bufferLength,
			p,
			length,
			&takenDstLength,
			takenLength_o
			);

		if (takenBufferSize_o)
			*takenBufferSize_o = takenDstLength * sizeof (C);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf8 (
		const void* p,
		size_t size
		)
	{
		size_t length = size / sizeof (C);
		return UtfConvert <Utf8, T>::calcRequiredLength ((const C*) p, length);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf16 (
		const void* p,
		size_t size
		)
	{
		size_t length = size / sizeof (C);
		return UtfConvert <Utf16, T>::calcRequiredLength ((const C*) p, length);
	}

	virtual
	size_t
	calcRequiredBufferLengthToDecodeToUtf32 (
		const void* p,
		size_t size
		)
	{
		size_t length = size / sizeof (C);
		return UtfConvert <Utf32, T>::calcRequiredLength ((const C*) p, length);
	}

	virtual
	void
	decodeToUtf8 (
		utf8_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		)
	{
		size_t length = size / sizeof (C);
		ASSERT (length || !size); // don't pass less than one unit

		size_t takenLength;
		size_t expectedLength;

		UtfConvert <Utf8, T>::convert (
			buffer,
			bufferLength,
			(const C*) p,
			length,
			takenBufferLength_o,
			&takenLength,
			&expectedLength
			);

		if (takenSize_o)
			*takenSize_o = takenLength * sizeof (C);

		if (expectedSize_o)
			*expectedSize_o = expectedLength * sizeof (C);
	}

	virtual
	void
	decodeToUtf16 (
		utf16_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		)
	{
		size_t length = size / sizeof (C);
		ASSERT (length || !size); // don't pass less than one unit

		size_t takenLength;
		size_t expectedLength;

		UtfConvert <Utf16, T>::convert (
			buffer,
			bufferLength,
			(const C*) p,
			length,
			takenBufferLength_o,
			&takenLength,
			&expectedLength
			);

		if (takenSize_o)
			*takenSize_o = takenLength * sizeof (C);

		if (expectedSize_o)
			*expectedSize_o = expectedLength * sizeof (C);
	}

	virtual
	void
	decodeToUtf32 (
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		size_t* takenBufferLength_o = NULL,
		size_t* takenSize_o = NULL,
		size_t* expectedSize_o = NULL
		)
	{
		size_t length = size / sizeof (C);
		ASSERT (length || !size); // don't pass less than one unit

		size_t takenLength;
		size_t expectedLength;

		UtfConvert <Utf32, T>::convert (
			buffer,
			bufferLength,
			(const C*) p,
			length,
			takenBufferLength_o,
			&takenLength,
			&expectedLength
			);

		if (takenSize_o)
			*takenSize_o = takenLength * sizeof (C);

		if (expectedSize_o)
			*expectedSize_o = expectedLength * sizeof (C);
	}
};

//.............................................................................

class CodePointDecoder
{
protected:
	CharCodec* m_charCodec;

	char m_accumulator [8];
	size_t m_accumulatorCurrentSize;
	size_t m_accumulatorExpectedSize;

public:
	CodePointDecoder (
		CharCodecKind codecKind = CharCodecKind_Utf8,
		uint32_t state = 0
		)
	{
		setup (codecKind, state);
	}

	CharCodec*
	getCharCodec ()
	{
		return m_charCodec;
	}

	void
	setup (
		CharCodecKind codecKind,
		uint32_t state = 0
		)
	{
		setup (enc::getCharCodec (codecKind), state);
	}

	void
	setup (
		CharCodec* codec,
		uint32_t state = 0
		)
	{
		m_charCodec = codec;
		loadState (state);
	}

	void
	resetAccumulator ()
	{
		m_accumulatorCurrentSize = 0;
		m_accumulatorExpectedSize = 0;
	}

	void
	clear ()
	{
		m_charCodec = NULL;
		resetAccumulator ();
	}

	void
	loadState (uint32_t state);

	uint32_t
	saveState ();

	// Decode () returns taken size or -1 if code point is incomplete yet

	size_t
	decode (
		utf32_t* codePoint,
		const void* p,
		size_t size
		);
};

//.............................................................................

} // namespace enc
} // namespace axl
