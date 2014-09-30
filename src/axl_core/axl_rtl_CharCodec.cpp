#include "pch.h"
#include "axl_rtl_CharCodec.h"

namespace axl {
namespace rtl {

//.............................................................................

CharCodec* 
getCharCodec (CharCodecKind codecKind)
{
	static AsciiCodec asciiCodec; 
	static UtfCodec <rtl::Utf8>     utf8Codec; 
	static UtfCodec <rtl::Utf16>    utf16Codec; 
	static UtfCodec <rtl::Utf16_be> utf16Codec_be; 
	static UtfCodec <rtl::Utf32>    utf32Codec; 
	static UtfCodec <rtl::Utf32_be> utf32Codec_be; 
	
	static CharCodec* codecTable [] = 
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
CharCodec::encodeFromUtf8 (
	rtl::Array <char>* buffer,
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
CharCodec::encodeFromUtf16 (
	rtl::Array <char>* buffer,
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
CharCodec::encodeFromUtf32 (
	rtl::Array <char>* buffer,
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

rtl::Array <char>
CharCodec::encodeFromUtf8 (
	const utf8_t* p,
	size_t length
	)
{
	rtl::Array <char> buffer;
	encodeFromUtf8 (&buffer, p, length);
	return buffer;
}

rtl::Array <char>
CharCodec::encodeFromUtf16 (
	const utf16_t* p,
	size_t length
	)
{
	rtl::Array <char> buffer;
	encodeFromUtf16 (&buffer, p, length);
	return buffer;
}

rtl::Array <char>
CharCodec::encodeFromUtf32 (
	const utf32_t* p,
	size_t length
	)
{
	rtl::Array <char> buffer;
	encodeFromUtf32 (&buffer, p, length);
	return buffer;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CharCodec::decodeToUtf8 (
	rtl::Array <utf8_t>* buffer,
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
CharCodec::decodeToUtf16 (
	rtl::Array <utf16_t>* buffer,
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
CharCodec::decodeToUtf32 (
	rtl::Array <utf32_t>* buffer,
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

rtl::Array <utf8_t>
CharCodec::decodeToUtf8 (
	const void* p,
	size_t size
	)
{
	rtl::Array <utf8_t> buffer;
	decodeToUtf8 (&buffer, p, size);
	return buffer;
}

rtl::Array <utf16_t>
CharCodec::decodeToUtf16 (
	const void* p,
	size_t size
	)
{
	rtl::Array <utf16_t> buffer;
	decodeToUtf16 (&buffer, p, size);
	return buffer;
}

rtl::Array <utf32_t>
CharCodec::decodeToUtf32 (
	const void* p,
	size_t size
	)
{
	rtl::Array <utf32_t> buffer;
	decodeToUtf32 (&buffer, p, size);
	return buffer;
}

//.............................................................................

void
CodePointDecoder::loadState (uint32_t state)
{
	*((uint32_t*) m_accumulator) = state;
	m_accumulatorCurrentSize = (state & 0x0f000000) >> 24;
	m_accumulatorExpectedSize = (state & 0xf0000000) >> 28;

	ASSERT (m_accumulatorCurrentSize <= 3 && m_accumulatorExpectedSize <= 4);
}

uint32_t
CodePointDecoder::saveState ()
{
	uint32_t state = *((const uint32_t*) m_accumulator) & 0x00ffffff;
	state |= (m_accumulatorCurrentSize & 0x0f) << 24;
	state |= (m_accumulatorExpectedSize & 0x0f) << 28;

	return state;
}

size_t
CodePointDecoder::decode (
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
