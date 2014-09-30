#include "pch.h"
#include "axl_rtl_CharCodec.h"

namespace axl {
namespace rtl {

//.............................................................................

CCharCodec* 
getCharCodec (ECharCodec codecKind)
{
	static CAsciiCodec asciiCodec; 
	static CUtfCodecT <rtl::CUtf8>     utf8Codec; 
	static CUtfCodecT <rtl::CUtf16>    utf16Codec; 
	static CUtfCodecT <rtl::CUtf16_be> utf16Codec_be; 
	static CUtfCodecT <rtl::CUtf32>    utf32Codec; 
	static CUtfCodecT <rtl::CUtf32_be> utf32Codec_be; 
	
	static CCharCodec* codecTable [] = 
	{
		&asciiCodec,
		&utf8Codec,
		&utf16Codec,
		&utf16Codec_be,
		&utf32Codec,
		&utf32Codec_be,
	};

	return (size_t) codecKind < countof (codecTable) ? codecTable [codecKind] : NULL;
}

//.............................................................................

void
CCharCodec::encodeFromUtf8 (
	rtl::CArrayT <char>* buffer,
	const utf8_t* p,
	size_t length
	)
{
	buffer->clear ();

	const utf8_t* end = p + length;
	while (p < end)
	{
		char tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		encodeFromUtf8 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		buffer->append (tmpBuffer, takenBufferSize);
		p += takenLength;
	}
}

void
CCharCodec::encodeFromUtf16 (
	rtl::CArrayT <char>* buffer,
	const utf16_t* p,
	size_t length
	)
{
	buffer->clear ();

	const utf16_t* end = p + length;
	while (p < end)
	{
		char tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		encodeFromUtf16 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		buffer->append (tmpBuffer, takenBufferSize);
		p += takenLength;
	}
}

void
CCharCodec::encodeFromUtf32 (
	rtl::CArrayT <char>* buffer,
	const utf32_t* p,
	size_t length
	)
{
	buffer->clear ();

	const utf32_t* end = p + length;
	while (p < end)
	{
		char tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		encodeFromUtf32 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		buffer->append (tmpBuffer, takenBufferSize);
		p += takenLength;
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

rtl::CArrayT <char>
CCharCodec::encodeFromUtf8 (
	const utf8_t* p,
	size_t length
	)
{
	rtl::CArrayT <char> buffer;
	encodeFromUtf8 (&buffer, p, length);
	return buffer;
}

rtl::CArrayT <char>
CCharCodec::encodeFromUtf16 (
	const utf16_t* p,
	size_t length
	)
{
	rtl::CArrayT <char> buffer;
	encodeFromUtf16 (&buffer, p, length);
	return buffer;
}

rtl::CArrayT <char>
CCharCodec::encodeFromUtf32 (
	const utf32_t* p,
	size_t length
	)
{
	rtl::CArrayT <char> buffer;
	encodeFromUtf32 (&buffer, p, length);
	return buffer;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CCharCodec::decodeToUtf8 (
	rtl::CArrayT <utf8_t>* buffer,
	const void* _p,
	size_t size
	)
{
	buffer->clear ();

	const char* p = (const char*) _p;
	const char* end = p + size;
	while (p < end)
	{
		utf8_t tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		decodeToUtf8 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		buffer->append (tmpBuffer, takenBufferSize);
		p += takenLength;
	}
}

void
CCharCodec::decodeToUtf16 (
	rtl::CArrayT <utf16_t>* buffer,
	const void* _p,
	size_t size
	)
{
	buffer->clear ();

	const char* p = (const char*) _p;
	const char* end = p + size;
	while (p < end)
	{
		utf16_t tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		decodeToUtf16 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		buffer->append (tmpBuffer, takenBufferSize);
		p += takenLength;
	}
}

void
CCharCodec::decodeToUtf32 (
	rtl::CArrayT <utf32_t>* buffer,
	const void* _p,
	size_t size
	)
{
	buffer->clear ();

	const char* p = (const char*) _p;
	const char* end = p + size;
	while (p < end)
	{
		utf32_t tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		decodeToUtf32 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		buffer->append (tmpBuffer, takenBufferSize);
		p += takenLength;
	}
}

rtl::CArrayT <utf8_t>
CCharCodec::decodeToUtf8 (
	const void* p,
	size_t size
	)
{
	rtl::CArrayT <utf8_t> buffer;
	decodeToUtf8 (&buffer, p, size);
	return buffer;
}

rtl::CArrayT <utf16_t>
CCharCodec::decodeToUtf16 (
	const void* p,
	size_t size
	)
{
	rtl::CArrayT <utf16_t> buffer;
	decodeToUtf16 (&buffer, p, size);
	return buffer;
}

rtl::CArrayT <utf32_t>
CCharCodec::decodeToUtf32 (
	const void* p,
	size_t size
	)
{
	rtl::CArrayT <utf32_t> buffer;
	decodeToUtf32 (&buffer, p, size);
	return buffer;
}

//.............................................................................

void
CCodePointDecoder::loadState (uint32_t state)
{
	*((uint32_t*) m_accumulator) = state;
	m_accumulatorCurrentSize = (state & 0x0f000000) >> 24;
	m_accumulatorExpectedSize = (state & 0xf0000000) >> 28;

	ASSERT (m_accumulatorCurrentSize <= 3 && m_accumulatorExpectedSize <= 4);
}

uint32_t
CCodePointDecoder::saveState ()
{
	uint32_t state = *((const uint32_t*) m_accumulator) & 0x00ffffff;
	state |= (m_accumulatorCurrentSize & 0x0f) << 24;
	state |= (m_accumulatorExpectedSize & 0x0f) << 28;

	return state;
}

size_t
CCodePointDecoder::decode (
	utf32_t* codePoint,
	const void* p,
	size_t size
	)
{
	size_t unitSize = m_charCodec->getUnitSize ();
	ASSERT (unitSize <= sizeof (m_accumulator));

	size_t takenBufferLength;
	size_t takenSize;
	size_t expectedSize;

	if (m_accumulatorExpectedSize)
	{
		size_t accumulatorMissingSize = m_accumulatorExpectedSize - m_accumulatorCurrentSize;		

		if (size < accumulatorMissingSize) // not yet
		{
			memcpy (m_accumulator + m_accumulatorCurrentSize, p, size);
			m_accumulatorCurrentSize += size;
			return -1;
		}

		memcpy (m_accumulator + m_accumulatorCurrentSize, p, accumulatorMissingSize);

		m_charCodec->decodeToUtf32 (
			codePoint, 
			1, 
			m_accumulator, 
			m_accumulatorExpectedSize, 
			&takenBufferLength,
			&takenSize,
			&expectedSize
			);

		if (!takenBufferLength) // still not yet (rare, but might actually happen with UTF-16)
		{
			ASSERT (
				m_accumulatorCurrentSize == 1 && 
				m_accumulatorExpectedSize == 2 && 
				expectedSize == 4
				);

			m_accumulatorCurrentSize += accumulatorMissingSize;
			m_accumulatorExpectedSize = expectedSize;
			return -1;
		}

		m_accumulatorExpectedSize = 0;
		m_accumulatorCurrentSize = 0;
		return accumulatorMissingSize;
	}

	if (size < unitSize) // wait for the whole unit
	{
		memcpy (m_accumulator, p, size);
		m_accumulatorCurrentSize = size;
		m_accumulatorExpectedSize = unitSize;
		return -1;
	}

	m_charCodec->decodeToUtf32 (
		codePoint, 
		1, 
		p, 
		size,
		&takenBufferLength,
		&takenSize,
		&expectedSize
		);

	if (!takenBufferLength)
	{
		ASSERT (!takenSize && size < expectedSize);

		memcpy (m_accumulator, p, size);
		m_accumulatorCurrentSize = size;
		m_accumulatorExpectedSize = expectedSize;
		return -1;
	}

	return takenSize;
}

//.............................................................................

} // namespace rtl
} // namespace axl
