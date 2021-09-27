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

#include "pch.h"
#include "axl_enc_CharCodec.h"

namespace axl {
namespace enc {

//..............................................................................

CharCodec*
getCharCodec(CharCodecKind codecKind) {
	static AsciiCodec         asciiCodec;
	static UtfCodec<Utf8>     utf8Codec;
	static UtfCodec<Utf16>    utf16Codec;
	static UtfCodec<Utf16_be> utf16Codec_be;
	static UtfCodec<Utf32>    utf32Codec;
	static UtfCodec<Utf32_be> utf32Codec_be;

	static CharCodec* codecTable[] = {
		&asciiCodec,
		&utf8Codec,
		&utf16Codec,
		&utf16Codec_be,
		&utf32Codec,
		&utf32Codec_be,
	};

	return (size_t)codecKind < countof(codecTable) ? codecTable[codecKind] : NULL;
}

//..............................................................................

size_t
CharCodec::encode_utf8(
	sl::Array<char>* buffer,
	const utf8_t* p,
	size_t length
) {
	buffer->clear();
	buffer->reserve(length * m_unitSize);

	const utf8_t* end = p + length;
	while (p < end) {
		char tmpBuffer[256];

		size_t takenLength;
		size_t takenBufferSize = encode_utf8(tmpBuffer, sizeof(tmpBuffer), p, end - p, &takenLength);
		if (!takenLength)
			break;

		bool result = buffer->append(tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount();
}

size_t
CharCodec::encode_utf16(
	sl::Array<char>* buffer,
	const utf16_t* p,
	size_t length
) {
	buffer->clear();
	buffer->reserve(length * m_unitSize);

	const utf16_t* end = p + length;
	while (p < end) {
		char tmpBuffer[256];

		size_t takenLength;
		size_t takenBufferSize = encode_utf16(tmpBuffer, sizeof(tmpBuffer), p, end - p, &takenLength);
		if (!takenLength)
			break;

		bool result = buffer->append(tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount();
}

size_t
CharCodec::encode_utf32(
	sl::Array<char>* buffer,
	const utf32_t* p,
	size_t length
) {
	buffer->clear();
	buffer->reserve(length * m_unitSize);

	const utf32_t* end = p + length;
	while (p < end) {
		char tmpBuffer[256];

		size_t takenLength;
		size_t takenBufferSize = encode_utf32(tmpBuffer, sizeof(tmpBuffer), p, end - p, &takenLength);
		if (!takenLength)
			break;

		bool result = buffer->append(tmpBuffer, takenBufferSize) != -1;
		if (!result)
			return -1;

		p += takenLength;
	}

	return buffer->getCount();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
CharCodec::decode_utf8(
	sl::String_utf8* string,
	const void* _p,
	size_t size
) {
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	string->clear();
	string->reserve(length);

	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end) {
		utf8_t tmpBuffer[256];

		size_t takenSize;
		size_t takenBufferLength = decode_utf8(tmpBuffer, countof(tmpBuffer), p, end - p, &takenSize);
		if (!takenBufferLength)
			break;

		bool result = string->append(tmpBuffer, takenBufferLength) != -1;
		if (!result)
			return -1;

		p += takenSize;
	}

	return string->getLength();
}

size_t
CharCodec::decode_utf16(
	sl::String_utf16* string,
	const void* _p,
	size_t size
) {
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	string->clear();
	string->reserve(length);

	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end) {
		utf16_t tmpBuffer[256];

		size_t takenSize;
		size_t takenBufferLength = decode_utf16(tmpBuffer, countof(tmpBuffer), p, end - p, &takenSize);
		if (!takenBufferLength)
			break;

		bool result = string->append(tmpBuffer, takenBufferLength) != -1;
		if (!result)
			return -1;

		p += takenSize;
	}

	return string->getLength();
}

size_t
CharCodec::decode_utf32(
	sl::String_utf32* string,
	const void* _p,
	size_t size
) {
	size_t length = size / m_unitSize;
	size_t leftover = size % m_unitSize;

	string->clear();
	string->reserve(length);

	const char* p = (const char*) _p;
	const char* end = p + size - leftover;
	while (p < end) {
		utf32_t tmpBuffer[256];

		size_t takenSize;
		size_t takenBufferLength = decode_utf32(tmpBuffer, countof(tmpBuffer), p, end - p, &takenSize);
		if (!takenBufferLength)
			break;

		bool result = string->append(tmpBuffer, takenBufferLength) != -1;
		if (!result)
			return -1;

		p += takenSize;
	}

	return string->getLength();
}

//..............................................................................

void
CodePointDecoder::loadState(uint32_t state) {
	*((uint32_t*)m_accumulator) = state;
	m_accumulatorCount = (state & 0x03000000) >> 24;
}

uint32_t
CodePointDecoder::saveState() {
	uint32_t state = *((const uint32_t*) m_accumulator) & 0x00ffffff;
	state |= (m_accumulatorCount & 0x03) << 24;
	return state;
}

size_t
CodePointDecoder::decode(
	utf32_t* buffer,
	size_t bufferLength,
	const void* p,
	size_t size,
	size_t* takenSize_o
) {
	size_t takenBufferLength;
	size_t takenSize;

	if (!m_accumulatorCount) {
		takenBufferLength = decodeImpl(buffer, bufferLength, p, size, &takenSize);

		if (takenSize_o)
			*takenSize_o = takenSize;

		return takenBufferLength;
	}

	ASSERT(m_accumulatorCount < sizeof(m_accumulator));

	size_t accumulatorLeftoverSize = sizeof(m_accumulator) - m_accumulatorCount;
	size_t copySize = AXL_MIN(size, accumulatorLeftoverSize);

	memcpy(m_accumulator + m_accumulatorCount, p, copySize);

	takenBufferLength = m_charCodec->decode_utf32(
		buffer,
		1,
		m_accumulator,
		m_accumulatorCount + copySize,
		&takenSize
	);

	if (!takenBufferLength) {
		ASSERT(takenSize == 0);
		m_accumulatorCount += copySize;

		if (takenSize_o)
			*takenSize_o = size;

		return 0;
	}

	ASSERT(takenBufferLength == 1 && takenSize > m_accumulatorCount);

	takenSize -= m_accumulatorCount;
	m_accumulatorCount = 0;

	if (takenSize < size) {
		size_t takenSize2;
		takenBufferLength += decodeImpl(
			buffer + 1,
			bufferLength - 1,
			(char*)p + takenSize,
			size - takenSize,
			&takenSize2
		);

		takenSize += takenSize2;
	}

	if (takenSize_o)
		*takenSize_o = takenSize;

	return takenBufferLength;
}

size_t
CodePointDecoder::decode(
	uchar_t* cplBuffer,
	utf32_t* textBuffer,
	size_t bufferLength,
	const void* p,
	size_t size,
	size_t* takenSize_o
) {
	size_t takenBufferLength;
	size_t takenSize;

	if (!m_accumulatorCount) {
		takenBufferLength = decodeImpl(cplBuffer, textBuffer, bufferLength, p, size, &takenSize);

		if (takenSize_o)
			*takenSize_o = takenSize;

		return takenBufferLength;
	}

	ASSERT(m_accumulatorCount < sizeof(m_accumulator));

	size_t accumulatorLeftoverSize = sizeof(m_accumulator) - m_accumulatorCount;
	size_t copySize = AXL_MIN(size, accumulatorLeftoverSize);

	memcpy(m_accumulator + m_accumulatorCount, p, copySize);

	takenBufferLength = m_charCodec->decode_utf32(
		cplBuffer,
		textBuffer,
		1,
		m_accumulator,
		m_accumulatorCount + copySize,
		&takenSize
	);

	if (!takenBufferLength) {
		ASSERT(takenSize == 0);
		m_accumulatorCount += copySize;

		if (takenSize_o)
			*takenSize_o = size;

		return 0;
	}

	ASSERT(takenBufferLength == 1 && takenSize > m_accumulatorCount);

	takenSize -= m_accumulatorCount;
	m_accumulatorCount = 0;

	if (takenSize < size) {
		size_t takenSize2;
		takenBufferLength += decodeImpl(
			cplBuffer + 1,
			textBuffer + 1,
			bufferLength - 1,
			(char*)p + takenSize,
			size - takenSize,
			&takenSize2
		);

		takenSize += takenSize2;
	}

	if (takenSize_o)
		*takenSize_o = takenSize;

	return takenBufferLength;
}

size_t
CodePointDecoder::decodeImpl(
	utf32_t* buffer,
	size_t bufferLength,
	const void* p,
	size_t size,
	size_t* takenSize_o
) {
	ASSERT(takenSize_o);

	size_t takenSize;
	size_t takenBufferLength = m_charCodec->decode_utf32(
		buffer,
		bufferLength,
		p,
		size,
		&takenSize
	);

	if (takenBufferLength < bufferLength && takenSize < size) {
		ASSERT(size - takenSize < sizeof(m_accumulator));
		m_accumulatorCount = size - takenSize;
		memcpy(m_accumulator, (char*)p + takenSize, m_accumulatorCount);
		takenSize += m_accumulatorCount;
	}

	*takenSize_o = takenSize;
	return takenBufferLength;
}

size_t
CodePointDecoder::decodeImpl(
	uchar_t* cplBuffer,
	utf32_t* textBuffer,
	size_t bufferLength,
	const void* p,
	size_t size,
	size_t* takenSize_o
) {
	ASSERT(takenSize_o);

	size_t takenSize;

	size_t takenBufferLength = m_charCodec->decode_utf32(
		cplBuffer,
		textBuffer,
		bufferLength,
		p,
		size,
		&takenSize
	);

	if (takenBufferLength < bufferLength && takenSize < size) {
		ASSERT(size - takenSize < sizeof(m_accumulator));
		m_accumulatorCount = size - takenSize;
		memcpy(m_accumulator, (char*)p + takenSize, m_accumulatorCount);
		takenSize += m_accumulatorCount;
	}

	*takenSize_o = takenSize;
	return takenBufferLength;
}

//..............................................................................

} // namespace enc
} // namespace axl
