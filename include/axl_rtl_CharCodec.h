// This file is part of Ninja Scroll (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_CHARCODEC_H

#include "axl_rtl_Utf.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

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

	void
	encodeFromUtf8 (
		rtl::Array <char>* buffer,
		const utf8_t* p,
		size_t length
		);

	void
	encodeFromUtf16 (
		rtl::Array <char>* buffer,
		const utf16_t* p,
		size_t length
		);

	void
	encodeFromUtf32 (
		rtl::Array <char>* buffer,
		const utf32_t* p,
		size_t length
		);

	rtl::Array <char>
	encodeFromUtf8 (
		const utf8_t* p,
		size_t length
		);

	rtl::Array <char>
	encodeFromUtf16 (
		const utf16_t* p,
		size_t length
		);

	rtl::Array <char>
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

	void
	decodeToUtf8 (
		rtl::Array <utf8_t>* buffer,
		const void* p,
		size_t size
		);

	void
	decodeToUtf16 (
		rtl::Array <utf16_t>* buffer,
		const void* p,
		size_t size
		);

	void
	decodeToUtf32 (
		rtl::Array <utf32_t>* buffer,
		const void* p,
		size_t size
		);

	rtl::Array <utf8_t>
	decodeToUtf8 (
		const void* p,
		size_t size
		);

	rtl::Array <utf16_t>
	decodeToUtf16 (
		const void* p,
		size_t size
		);

	rtl::Array <utf32_t>
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
		rtl::UtfToAsciiConvert <rtl::Utf8>::convert (
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
		rtl::UtfToAsciiConvert <rtl::Utf16>::convert (
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
		rtl::UtfToAsciiConvert <rtl::Utf32>::convert (
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
		rtl::AsciiToUtfConvert <rtl::Utf8>::convert (
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
		rtl::AsciiToUtfConvert <rtl::Utf16>::convert (
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
		rtl::AsciiToUtfConvert <rtl::Utf32>::convert (
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

		rtl::UtfConvert <T, rtl::Utf8>::convert (
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

		rtl::UtfConvert <T, rtl::Utf16>::convert (
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

		rtl::UtfConvert <T, rtl::Utf32>::convert (
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

		rtl::UtfConvert <rtl::Utf8, T>::convert (
			buffer,
			bufferLength,
			(C*) p,
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

		rtl::UtfConvert <rtl::Utf16, T>::convert (
			buffer,
			bufferLength,
			(C*) p,
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

		rtl::UtfConvert <rtl::Utf32, T>::convert (
			buffer,
			bufferLength,
			(C*) p,
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
		setup (rtl::getCharCodec (codecKind), state);
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

} // namespace rtl
} // namespace axl
