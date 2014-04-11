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
	size_t m_UnitSize;

public:
	CCharCodec ()
	{
		m_UnitSize = 1;
	}

	size_t
	GetUnitSize ()
	{
		return m_UnitSize;
	}

	void
	EncodeFromUtf8 (
		rtl::CArrayT <char>* pBuffer,
		const utf8_t* p,
		size_t Length
		);

	void
	EncodeFromUtf16 (
		rtl::CArrayT <char>* pBuffer,
		const utf16_t* p,
		size_t Length
		);

	void
	EncodeFromUtf32 (
		rtl::CArrayT <char>* pBuffer,
		const utf32_t* p,
		size_t Length
		);

	rtl::CArrayT <char>
	EncodeFromUtf8 (
		const utf8_t* p,
		size_t Length
		);

	rtl::CArrayT <char>
	EncodeFromUtf16 (
		const utf16_t* p,
		size_t Length
		);

	rtl::CArrayT <char>
	EncodeFromUtf32 (
		const utf32_t* p,
		size_t Length
		);

	virtual
	void
	EncodeFromUtf8 (
		void* pBuffer,
		size_t BufferSize,
		const utf8_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		) = 0;

	virtual
	void
	EncodeFromUtf16 (
		void* pBuffer,
		size_t BufferSize,
		const utf16_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		) = 0;

	virtual
	void
	EncodeFromUtf32 (
		void* pBuffer,
		size_t BufferSize,
		const utf32_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		) = 0;

	void
	DecodeToUtf8 (
		rtl::CArrayT <utf8_t>* pBuffer,
		const void* p,
		size_t Size
		);

	void
	DecodeToUtf16 (
		rtl::CArrayT <utf16_t>* pBuffer,
		const void* p,
		size_t Size
		);

	void
	DecodeToUtf32 (
		rtl::CArrayT <utf32_t>* pBuffer,
		const void* p,
		size_t Size
		);

	rtl::CArrayT <utf8_t>
	DecodeToUtf8 (
		const void* p,
		size_t Size
		);

	rtl::CArrayT <utf16_t>
	DecodeToUtf16 (
		const void* p,
		size_t Size
		);

	rtl::CArrayT <utf32_t>
	DecodeToUtf32 (
		const void* p,
		size_t Size
		);

	virtual
	void
	DecodeToUtf8 (
		utf8_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		) = 0;

	virtual
	void
	DecodeToUtf16 (
		utf16_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		) = 0;

	virtual
	void
	DecodeToUtf32 (
		utf32_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CCharCodec* 
GetCharCodec (ECharCodec CodecKind);

//.............................................................................

class CAsciiCodec: public CCharCodec
{
public:
	CAsciiCodec ()
	{
		m_UnitSize = 1;
	}

	virtual
	void
	EncodeFromUtf8 (
		void* pBuffer,
		size_t BufferSize,
		const utf8_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		)
	{
		rtl::CUtfToAsciiConvertT <rtl::CUtf8>::Convert (
			(char*) pBuffer, 
			BufferSize, 
			p, 
			Length, 
			pTakenBufferSize, 
			pTakenLength
			);
	}

	virtual
	void
	EncodeFromUtf16 (
		void* pBuffer,
		size_t BufferSize,
		const utf16_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		)
	{
		rtl::CUtfToAsciiConvertT <rtl::CUtf16>::Convert (
			(char*) pBuffer, 
			BufferSize, 
			p, 
			Length, 
			pTakenBufferSize, 
			pTakenLength
			);
	}

	virtual
	void
	EncodeFromUtf32 (
		void* pBuffer,
		size_t BufferSize,
		const utf32_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		)
	{
		rtl::CUtfToAsciiConvertT <rtl::CUtf32>::Convert (
			(char*) pBuffer, 
			BufferSize, 
			p, 
			Length, 
			pTakenBufferSize, 
			pTakenLength
			);
	}

	virtual
	void
	DecodeToUtf8 (
		utf8_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength,
		size_t* pTakenSize,
		size_t* pExpectedSize
		)
	{
		rtl::CAsciiToUtfConvertT <rtl::CUtf8>::Convert (
			pBuffer, 
			BufferLength, 
			(const char*) p, 
			Size,
			pTakenBufferLength, 
			pTakenSize, 
			pExpectedSize
			);
	}

	virtual
	void
	DecodeToUtf16 (
		utf16_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		)
	{
		rtl::CAsciiToUtfConvertT <rtl::CUtf16>::Convert (
			pBuffer, 
			BufferLength, 
			(const char*) p, 
			Size,
			pTakenBufferLength, 
			pTakenSize, 
			pExpectedSize
			);
	}

	virtual
	void
	DecodeToUtf32 (
		utf32_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		)
	{
		rtl::CAsciiToUtfConvertT <rtl::CUtf32>::Convert (
			pBuffer, 
			BufferLength, 
			(const char*) p, 
			Size,
			pTakenBufferLength, 
			pTakenSize, 
			pExpectedSize
			);
	}
};

//.............................................................................

template <typename T>
class CUtfCodecT: public CCharCodec
{
public:
	CUtfCodecT ()
	{
		m_UnitSize = sizeof (T::C);
	}

	virtual
	void
	EncodeFromUtf8 (
		void* pBuffer,
		size_t BufferSize,
		const utf8_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		)
	{
		size_t BufferLength = BufferSize / sizeof (T::C);
		size_t TakenDstLength;

		rtl::CUtfConvertT <T, rtl::CUtf8>::Convert (
			(T::C*) pBuffer, 
			BufferLength, 
			p, 
			Length, 
			&TakenDstLength, 
			pTakenLength
			);

		if (pTakenBufferSize)
			*pTakenBufferSize = TakenDstLength * sizeof (T::C);
	}

	virtual
	void
	EncodeFromUtf16 (
		void* pBuffer,
		size_t BufferSize,
		const utf16_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		)
	{
		size_t BufferLength = BufferSize / sizeof (T::C);
		size_t TakenDstLength;

		rtl::CUtfConvertT <T, rtl::CUtf16>::Convert (
			(T::C*) pBuffer, 
			BufferLength, 
			p, 
			Length, 
			&TakenDstLength, 
			pTakenLength
			);

		if (pTakenBufferSize)
			*pTakenBufferSize = TakenDstLength * sizeof (T::C);
	}

	virtual
	void
	EncodeFromUtf32 (
		void* pBuffer,
		size_t BufferSize,
		const utf32_t* p,
		size_t Length,
		size_t* pTakenBufferSize = NULL,
		size_t* pTakenLength = NULL
		)
	{
		size_t BufferLength = BufferSize / sizeof (T::C);
		size_t TakenDstLength;

		rtl::CUtfConvertT <T, rtl::CUtf32>::Convert (
			(T::C*) pBuffer, 
			BufferLength, 
			p, 
			Length, 
			&TakenDstLength, 
			pTakenLength
			);

		if (pTakenBufferSize)
			*pTakenBufferSize = TakenDstLength * sizeof (T::C);
	}

	virtual
	void
	DecodeToUtf8 (
		utf8_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		)
	{
		size_t Length = Size / sizeof (T::C);
		ASSERT (Length || !Size); // don't pass less than one unit

		size_t TakenLength;
		size_t ExpectedLength;

		rtl::CUtfConvertT <rtl::CUtf8, T>::Convert (
			pBuffer, 
			BufferLength, 
			(T::C*) p, 
			Length, 
			pTakenBufferLength, 
			&TakenLength, 
			&ExpectedLength
			);

		if (pTakenSize)
			*pTakenSize = TakenLength * sizeof (T::C);

		if (pExpectedSize)
			*pExpectedSize = ExpectedLength * sizeof (T::C);
	}

	virtual
	void
	DecodeToUtf16 (
		utf16_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		)
	{
		size_t Length = Size / sizeof (T::C);
		ASSERT (Length || !Size); // don't pass less than one unit

		size_t TakenLength;
		size_t ExpectedLength;

		rtl::CUtfConvertT <rtl::CUtf16, T>::Convert (
			pBuffer, 
			BufferLength, 
			(T::C*) p, 
			Length, 
			pTakenBufferLength, 
			&TakenLength, 
			&ExpectedLength
			);

		if (pTakenSize)
			*pTakenSize = TakenLength * sizeof (T::C);

		if (pExpectedSize)
			*pExpectedSize = ExpectedLength * sizeof (T::C);
	}

	virtual
	void
	DecodeToUtf32 (
		utf32_t* pBuffer,
		size_t BufferLength,
		const void* p,
		size_t Size,
		size_t* pTakenBufferLength = NULL,
		size_t* pTakenSize = NULL,
		size_t* pExpectedSize = NULL
		)
	{
		size_t Length = Size / sizeof (T::C);
		ASSERT (Length || !Size); // don't pass less than one unit

		size_t TakenLength;
		size_t ExpectedLength;

		rtl::CUtfConvertT <rtl::CUtf32, T>::Convert (
			pBuffer, 
			BufferLength, 
			(T::C*) p, 
			Length, 
			pTakenBufferLength, 
			&TakenLength, 
			&ExpectedLength
			);

		if (pTakenSize)
			*pTakenSize = TakenLength * sizeof (T::C);

		if (pExpectedSize)
			*pExpectedSize = ExpectedLength * sizeof (T::C);
	}
};

//.............................................................................

class CCodePointDecoder
{
protected:
	CCharCodec* m_pCharCodec;

	char m_Accumulator [8];
	size_t m_AccumulatorCurrentSize;
	size_t m_AccumulatorExpectedSize;

public:
	CCodePointDecoder (
		ECharCodec CodecKind = ECharCodec_Utf8,
		uint32_t State = 0
		)
	{
		Setup (CodecKind, State);
	}

	CCharCodec*
	GetCharCodec ()
	{
		return m_pCharCodec;
	}

	void
	Setup (
		ECharCodec CodecKind,
		uint32_t State = 0
		)
	{
		Setup (rtl::GetCharCodec (CodecKind), State);
	}

	void
	Setup (
		CCharCodec* pCodec,
		uint32_t State = 0
		)
	{
		m_pCharCodec = pCodec;
		LoadState (State);
	}

	void
	ResetAccumulator ()
	{
		m_AccumulatorCurrentSize = 0;
		m_AccumulatorExpectedSize = 0;
	}

	void
	Clear ()
	{
		m_pCharCodec = NULL;
		ResetAccumulator ();
	}

	void
	LoadState (uint32_t State);

	uint32_t
	SaveState ();

	// Decode () returns taken size or -1 if code point is incomplete yet

	size_t
	Decode (
		utf32_t* pCodePoint,
		const void* p,
		size_t Size
		); 
};

//.............................................................................

} // namespace rtl
} // namespace axl
