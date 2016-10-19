#include "pch.h"
#include "axl_enc_CharCodec.h"

namespace axl {
namespace enc {

//.............................................................................

CharCodec* 
getCharCodec (CharCodecKind codecKind)
{
	static AsciiCodec          asciiCodec; 
	static UtfCodec <Utf8>     utf8Codec; 
	static UtfCodec <Utf16>    utf16Codec; 
	static UtfCodec <Utf16_be> utf16Codec_be; 
	static UtfCodec <Utf32>    utf32Codec; 
	static UtfCodec <Utf32_be> utf32Codec_be; 
	
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

size_t
CharCodec::encodeFromUtf8 (
	sl::Array <char>* buffer,
	const utf8_t* p,
	size_t length
	)
{
	buffer->clear ();
	bool result = buffer->reserve (length * m_unitSize);
	if (!result)
		return -1;

	const utf8_t* end = p + length;
	while (p < end)
	{
		char tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		encodeFromUtf8 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		result = buffer->append (tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount ();
}

size_t
CharCodec::encodeFromUtf16 (
	sl::Array <char>* buffer,
	const utf16_t* p,
	size_t length
	)
{
	buffer->clear ();
	bool result = buffer->reserve (length * m_unitSize);
	if (!result)
		return -1;

	const utf16_t* end = p + length;
	while (p < end)
	{
		char tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		encodeFromUtf16 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		result = buffer->append (tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount ();
}

size_t
CharCodec::encodeFromUtf32 (
	sl::Array <char>* buffer,
	const utf32_t* p,
	size_t length
	)
{
	buffer->clear ();
	bool result = buffer->reserve (length * m_unitSize);
	if (!result)
		return -1;

	const utf32_t* end = p + length;
	while (p < end)
	{
		char tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		encodeFromUtf32 (tmpBuffer, sizeof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		result = buffer->append (tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::Array <char>
CharCodec::encodeFromUtf8 (
	const utf8_t* p,
	size_t length
	)
{
	sl::Array <char> buffer;
	encodeFromUtf8 (&buffer, p, length);
	return buffer;
}

sl::Array <char>
CharCodec::encodeFromUtf16 (
	const utf16_t* p,
	size_t length
	)
{
	sl::Array <char> buffer;
	encodeFromUtf16 (&buffer, p, length);
	return buffer;
}

sl::Array <char>
CharCodec::encodeFromUtf32 (
	const utf32_t* p,
	size_t length
	)
{
	sl::Array <char> buffer;
	encodeFromUtf32 (&buffer, p, length);
	return buffer;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
CharCodec::decodeToUtf8 (
	sl::Array <utf8_t>* buffer,
	const void* _p,
	size_t size
	)
{
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	buffer->clear ();
	bool result = buffer->reserve (length);
	if (!result)
		return -1;

	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end)
	{
		utf8_t tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		decodeToUtf8 (tmpBuffer, countof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		result = buffer->append (tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount ();
}

size_t
CharCodec::decodeToUtf16 (
	sl::Array <utf16_t>* buffer,
	const void* _p,
	size_t size
	)
{
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	buffer->clear ();
	bool result = buffer->reserve (length);
	if (!result)
		return -1;

	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end)
	{
		utf16_t tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		decodeToUtf16 (tmpBuffer, countof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		result = buffer->append (tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount ();
}

size_t
CharCodec::decodeToUtf32 (
	sl::Array <utf32_t>* buffer,
	const void* _p,
	size_t size
	)
{
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	buffer->clear ();
	bool result = buffer->reserve (length);
	if (!result)
		return -1;

	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end)
	{
		utf32_t tmpBuffer [256];
		
		size_t takenBufferSize;
		size_t takenLength;

		decodeToUtf32 (tmpBuffer, countof (tmpBuffer), p, end - p, &takenBufferSize, &takenLength);
		if (!takenLength)
			break;

		result = buffer->append (tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount ();
}

sl::Array <utf8_t>
CharCodec::decodeToUtf8 (
	const void* p,
	size_t size
	)
{
	sl::Array <utf8_t> buffer;
	decodeToUtf8 (&buffer, p, size);
	return buffer;
}

sl::Array <utf16_t>
CharCodec::decodeToUtf16 (
	const void* p,
	size_t size
	)
{
	sl::Array <utf16_t> buffer;
	decodeToUtf16 (&buffer, p, size);
	return buffer;
}

sl::Array <utf32_t>
CharCodec::decodeToUtf32 (
	const void* p,
	size_t size
	)
{
	sl::Array <utf32_t> buffer;
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

} // namespace enc
} // namespace axl
