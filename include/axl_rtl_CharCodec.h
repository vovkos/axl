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

enum ECharCodec
{
	ECharCodec_Ascii = 0,
	ECharCodec_Utf8,
	ECharCodec_Utf16,
	ECharCodec_Utf16_be,
	ECharCodec_Utf32,
	ECharCodec_Utf32_be,

	// later maybe add other important encodings

	ECharCodec__Count,
};

//.............................................................................

class CCharCodec
{
protected:
	size_t m_unitSize;

public:
	CCharCodec ()
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
		rtl::CArrayT <char>* buffer,
		const utf8_t* p,
		size_t length
		);

	void
	encodeFromUtf16 (
		rtl::CArrayT <char>* buffer,
		const utf16_t* p,
		size_t length
		);

	void
	encodeFromUtf32 (
		rtl::CArrayT <char>* buffer,
		const utf32_t* p,
		size_t length
		);

	rtl::CArrayT <char>
	encodeFromUtf8 (
		const utf8_t* p,
		size_t length
		);

	rtl::CArrayT <char>
	encodeFromUtf16 (
		const utf16_t* p,
		size_t length
		);

	rtl::CArrayT <char>
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
		rtl::CArrayT <utf8_t>* buffer,
		const void* p,
		size_t size
		);

	void
	decodeToUtf16 (
		rtl::CArrayT <utf16_t>* buffer,
		const void* p,
		size_t size
		);

	void
	decodeToUtf32 (
		rtl::CArrayT <utf32_t>* buffer,
		const void* p,
		size_t size
		);

	rtl::CArrayT <utf8_t>
	decodeToUtf8 (
		const void* p,
		size_t size
		);

	rtl::CArrayT <utf16_t>
	decodeToUtf16 (
		const void* p,
		size_t size
		);

	rtl::CArrayT <utf32_t>
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

CCharCodec*
getCharCodec (ECharCodec codecKind);

//.............................................................................

class CAsciiCodec: public CCharCodec
{
public:
	CAsciiCodec ()
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
		rtl::CUtfToAsciiConvertT <rtl::CUtf8>::convert (
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
		rtl::CUtfToAsciiConvertT <rtl::CUtf16>::convert (
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
		rtl::CUtfToAsciiConvertT <rtl::CUtf32>::convert (
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
		rtl::CAsciiToUtfConvertT <rtl::CUtf8>::convert (
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
		rtl::CAsciiToUtfConvertT <rtl::CUtf16>::convert (
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
		rtl::CAsciiToUtfConvertT <rtl::CUtf32>::convert (
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
class CUtfCodecT: public CCharCodec
{
public:
	typedef typename T::C C;

public:
	CUtfCodecT ()
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

		rtl::CUtfConvertT <T, rtl::CUtf8>::convert (
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

		rtl::CUtfConvertT <T, rtl::CUtf16>::convert (
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

		rtl::CUtfConvertT <T, rtl::CUtf32>::convert (
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

		rtl::CUtfConvertT <rtl::CUtf8, T>::convert (
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

		rtl::CUtfConvertT <rtl::CUtf16, T>::convert (
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

		rtl::CUtfConvertT <rtl::CUtf32, T>::convert (
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

class CCodePointDecoder
{
protected:
	CCharCodec* m_charCodec;

	char m_accumulator [8];
	size_t m_accumulatorCurrentSize;
	size_t m_accumulatorExpectedSize;

public:
	CCodePointDecoder (
		ECharCodec codecKind = ECharCodec_Utf8,
		uint32_t state = 0
		)
	{
		setup (codecKind, state);
	}

	CCharCodec*
	getCharCodec ()
	{
		return m_charCodec;
	}

	void
	setup (
		ECharCodec codecKind,
		uint32_t state = 0
		)
	{
		setup (rtl::getCharCodec (codecKind), state);
	}

	void
	setup (
		CCharCodec* codec,
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
