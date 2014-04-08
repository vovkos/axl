#include "pch.h"
#include "axl_rtl_CharCodec.h"

namespace axl {
namespace rtl {

//.............................................................................

CCharCodec* 
GetCharCodec (ECharCodec CodecKind)
{
	static CAsciiCodec AsciiCodec; 
	static CUtfCodecT <rtl::CUtf8>     Utf8Codec; 
	static CUtfCodecT <rtl::CUtf16>    Utf16Codec; 
	static CUtfCodecT <rtl::CUtf16_be> Utf16Codec_be; 
	static CUtfCodecT <rtl::CUtf32>    Utf32Codec; 
	static CUtfCodecT <rtl::CUtf32_be> Utf32Codec_be; 
	
	static CCharCodec* CodecTable [] = 
	{
		&AsciiCodec,
		&Utf8Codec,
		&Utf16Codec,
		&Utf16Codec_be,
		&Utf32Codec,
		&Utf32Codec_be,
	};

	return (size_t) CodecKind < countof (CodecTable) ? CodecTable [CodecKind] : NULL;
}

//.............................................................................

void
CCharCodec::EncodeFromUtf8 (
	rtl::CArrayT <char>* pBuffer,
	const utf8_t* p,
	size_t Length
	)
{
	pBuffer->Clear ();

	const utf8_t* pEnd = p + Length;
	while (p < pEnd)
	{
		char Buffer [256];
		
		size_t TakenBufferSize;
		size_t TakenLength;

		EncodeFromUtf8 (Buffer, sizeof (Buffer), p, pEnd - p, &TakenBufferSize, &TakenLength);
		if (!TakenLength)
			break;

		pBuffer->Append (Buffer, TakenBufferSize);
		p += TakenLength;
	}
}

void
CCharCodec::EncodeFromUtf16 (
	rtl::CArrayT <char>* pBuffer,
	const utf16_t* p,
	size_t Length
	)
{
	pBuffer->Clear ();

	const utf16_t* pEnd = p + Length;
	while (p < pEnd)
	{
		char Buffer [256];
		
		size_t TakenBufferSize;
		size_t TakenLength;

		EncodeFromUtf16 (Buffer, sizeof (Buffer), p, pEnd - p, &TakenBufferSize, &TakenLength);
		if (!TakenLength)
			break;

		pBuffer->Append (Buffer, TakenBufferSize);
		p += TakenLength;
	}
}

void
CCharCodec::EncodeFromUtf32 (
	rtl::CArrayT <char>* pBuffer,
	const utf32_t* p,
	size_t Length
	)
{
	pBuffer->Clear ();

	const utf32_t* pEnd = p + Length;
	while (p < pEnd)
	{
		char Buffer [256];
		
		size_t TakenBufferSize;
		size_t TakenLength;

		EncodeFromUtf32 (Buffer, sizeof (Buffer), p, pEnd - p, &TakenBufferSize, &TakenLength);
		if (!TakenLength)
			break;

		pBuffer->Append (Buffer, TakenBufferSize);
		p += TakenLength;
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

rtl::CArrayT <char>
CCharCodec::EncodeFromUtf8 (
	const utf8_t* p,
	size_t Length
	)
{
	rtl::CArrayT <char> Buffer;
	EncodeFromUtf8 (&Buffer, p, Length);
	return Buffer;
}

rtl::CArrayT <char>
CCharCodec::EncodeFromUtf16 (
	const utf16_t* p,
	size_t Length
	)
{
	rtl::CArrayT <char> Buffer;
	EncodeFromUtf16 (&Buffer, p, Length);
	return Buffer;
}

rtl::CArrayT <char>
CCharCodec::EncodeFromUtf32 (
	const utf32_t* p,
	size_t Length
	)
{
	rtl::CArrayT <char> Buffer;
	EncodeFromUtf32 (&Buffer, p, Length);
	return Buffer;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CCharCodec::DecodeToUtf8 (
	rtl::CArrayT <utf8_t>* pBuffer,
	const void* _p,
	size_t Size
	)
{
	pBuffer->Clear ();

	const char* p = (const char*) _p;
	const char* pEnd = p + Size;
	while (p < pEnd)
	{
		utf8_t Buffer [256];
		
		size_t TakenBufferSize;
		size_t TakenLength;

		DecodeToUtf8 (Buffer, sizeof (Buffer), p, pEnd - p, &TakenBufferSize, &TakenLength);
		if (!TakenLength)
			break;

		pBuffer->Append (Buffer, TakenBufferSize);
		p += TakenLength;
	}
}

void
CCharCodec::DecodeToUtf16 (
	rtl::CArrayT <utf16_t>* pBuffer,
	const void* _p,
	size_t Size
	)
{
	pBuffer->Clear ();

	const char* p = (const char*) _p;
	const char* pEnd = p + Size;
	while (p < pEnd)
	{
		utf16_t Buffer [256];
		
		size_t TakenBufferSize;
		size_t TakenLength;

		DecodeToUtf16 (Buffer, sizeof (Buffer), p, pEnd - p, &TakenBufferSize, &TakenLength);
		if (!TakenLength)
			break;

		pBuffer->Append (Buffer, TakenBufferSize);
		p += TakenLength;
	}
}

void
CCharCodec::DecodeToUtf32 (
	rtl::CArrayT <utf32_t>* pBuffer,
	const void* _p,
	size_t Size
	)
{
	pBuffer->Clear ();

	const char* p = (const char*) _p;
	const char* pEnd = p + Size;
	while (p < pEnd)
	{
		utf32_t Buffer [256];
		
		size_t TakenBufferSize;
		size_t TakenLength;

		DecodeToUtf32 (Buffer, sizeof (Buffer), p, pEnd - p, &TakenBufferSize, &TakenLength);
		if (!TakenLength)
			break;

		pBuffer->Append (Buffer, TakenBufferSize);
		p += TakenLength;
	}
}

rtl::CArrayT <utf8_t>
CCharCodec::DecodeToUtf8 (
	const void* p,
	size_t Size
	)
{
	rtl::CArrayT <utf8_t> Buffer;
	DecodeToUtf8 (&Buffer, p, Size);
	return Buffer;
}

rtl::CArrayT <utf16_t>
CCharCodec::DecodeToUtf16 (
	const void* p,
	size_t Size
	)
{
	rtl::CArrayT <utf16_t> Buffer;
	DecodeToUtf16 (&Buffer, p, Size);
	return Buffer;
}

rtl::CArrayT <utf32_t>
CCharCodec::DecodeToUtf32 (
	const void* p,
	size_t Size
	)
{
	rtl::CArrayT <utf32_t> Buffer;
	DecodeToUtf32 (&Buffer, p, Size);
	return Buffer;
}

//.............................................................................

size_t
CCodePointDecoder::Decode (
	utf32_t* pCodePoint,
	const void* p,
	size_t Size
	)
{
	size_t UnitSize = m_pCharCodec->GetUnitSize ();
	ASSERT (UnitSize <= sizeof (m_Accumulator));

	size_t TakenBufferLength;
	size_t TakenSize;
	size_t ExpectedSize;

	if (m_AccumulatorExpectedSize)
	{
		size_t AccumulatorMissingSize = m_AccumulatorExpectedSize - m_AccumulatorCurrentSize;		

		if (Size < AccumulatorMissingSize) // not yet
		{
			memcpy (m_Accumulator + m_AccumulatorCurrentSize, p, Size);
			m_AccumulatorCurrentSize += Size;
			return -1;
		}

		memcpy (m_Accumulator + m_AccumulatorCurrentSize, p, AccumulatorMissingSize);

		m_pCharCodec->DecodeToUtf32 (
			pCodePoint, 
			1, 
			m_Accumulator, 
			m_AccumulatorExpectedSize, 
			&TakenBufferLength,
			&TakenSize,
			&ExpectedSize
			);

		if (!TakenBufferLength) // still not yet (rare, but might actually happen with UTF-16)
		{
			ASSERT (
				m_AccumulatorCurrentSize == 1 && 
				m_AccumulatorExpectedSize == 2 && 
				ExpectedSize == 4
				);

			m_AccumulatorCurrentSize += AccumulatorMissingSize;
			m_AccumulatorExpectedSize = ExpectedSize;
			return -1;
		}

		m_AccumulatorExpectedSize = 0;
		m_AccumulatorCurrentSize = 0;
		return AccumulatorMissingSize;
	}

	if (Size < UnitSize) // wait for the whole unit
	{
		memcpy (m_Accumulator, p, Size);
		m_AccumulatorCurrentSize = Size;
		m_AccumulatorExpectedSize = UnitSize;
		return -1;
	}

	m_pCharCodec->DecodeToUtf32 (
		pCodePoint, 
		1, 
		p, 
		Size,
		&TakenBufferLength,
		&TakenSize,
		&ExpectedSize
		);

	if (!TakenBufferLength)
	{
		ASSERT (!TakenSize && Size < ExpectedSize);

		memcpy (m_Accumulator, p, Size);
		m_AccumulatorCurrentSize = Size;
		m_AccumulatorExpectedSize = ExpectedSize;
		return -1;
	}

	return TakenSize;
}

//.............................................................................

} // namespace rtl
} // namespace axl
