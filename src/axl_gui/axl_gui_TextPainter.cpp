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
#include "axl_gui_TextPainter.h"
#include "axl_gui_Font.h"
#include "axl_enc_HexEncoding.h"
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace gui {

//..............................................................................

void
TextPainter::init(Canvas* canvas) {
	m_canvas = canvas;
	m_top = 0;
	m_bottom = 0;
	m_hexEncodingFlags = 0;
	m_unprintableChar = '.';
}

// space

int
TextPainter::drawSpace_p(
	int width,
	uint_t color
) {
	Rect rect;
	rect.m_left = m_point.m_x;
	rect.m_top = m_top;
	rect.m_right = m_point.m_x + width;
	rect.m_bottom = m_bottom;

	m_canvas->drawRect(rect, color);

	m_point.m_x = rect.m_right;
	return m_point.m_x;
}

int
TextPainter::drawSpace(
	size_t length,
	uint_t color
) {
	Size size = m_canvas->calcCharSize(' ');
	return drawSpace_p(length * size.m_width, color);
}

// text

int
TextPainter::drawText_utf8(
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf8& text
) {
	if (text.isEmpty())
		return m_point.m_x;

	Engine* engine = m_canvas->getEngine();
	Font* font = m_canvas->m_font->getFontMod(fontFlags);
	Size size = engine->calcTextSize_utf8(font, m_canvas, text);
	int right = m_point.m_x + size.m_width;

	m_canvas->drawText_utf8(
		m_point.m_x,
		m_point.m_y,
		m_point.m_x,
		m_point.m_y,
		right,
		m_bottom,
		textColor,
		backColor,
		fontFlags,
		text
	);

	m_point.m_x = right;
	return right;
}

int
TextPainter::drawText_utf32(
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf32& text
) {
	if (text.isEmpty())
		return m_point.m_x;

	Engine* engine = m_canvas->getEngine();
	Font* font = m_canvas->m_font->getFontMod(fontFlags);
	Size size = engine->calcTextSize_utf32(font, m_canvas, text);
	int right = m_point.m_x + size.m_width;

	m_canvas->drawText_utf32(
		m_point.m_x,
		m_point.m_y,
		m_point.m_x,
		m_point.m_y,
		right,
		m_bottom,
		textColor,
		backColor,
		fontFlags,
		text
	);

	m_point.m_x = right;
	return right;
}

// hyper text

int
TextPainter::drawHyperText_utf8(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const sl::StringRef_utf8& text
) {
	if (!attrArray || attrArray->isEmpty())
		return drawText_utf8(textColor0, backColor0, fontFlags0, text);

	if (text.isEmpty())
		return m_point.m_x;

	TextAttr attr0(textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->cp();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount();

	if (!nextAttr->m_offset) {
		attr.overlay(nextAttr->m_attr);
		nextAttr++;
	}

	const utf8_t* p = text.cp();
	size_t length = text.getLength();
	size_t offset = 0;

	while (offset < length && nextAttr < attrEnd) {
		size_t leftover = length - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return drawText_utf8(attr, sl::StringRef_utf8(p, leftover));

		drawText_utf8(attr, sl::StringRef_utf8(p, chunk));

		attr.overlay(attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
	}

	if (offset < length) {
		size_t leftover = length - offset;
		drawText_utf8(attr, sl::StringRef_utf8(p, leftover));
	}

	return m_point.m_x;
}

int
TextPainter::drawHyperText_utf32(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const sl::StringRef_utf32& text
) {
	if (!attrArray || attrArray->isEmpty())
		return drawText_utf32(textColor0, backColor0, fontFlags0, text);

	if (text.isEmpty())
		return m_point.m_x;

	TextAttr attr0(textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->cp();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount();

	if (!nextAttr->m_offset) {
		attr.overlay(nextAttr->m_attr);
		nextAttr++;
	}

	const utf32_t* p = text.cp();
	size_t length = text.getLength();
	size_t offset = 0;

	while (offset < length && nextAttr < attrEnd) {
		size_t leftover = length - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return drawText_utf32(attr, sl::StringRef_utf32(p, leftover));

		drawText_utf32(attr, sl::StringRef_utf32(p, chunk));

		attr.overlay(attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
	}

	if (offset < length) {
		size_t leftover = length - offset;
		drawText_utf32(attr, sl::StringRef_utf32(p, leftover));
	}

	return m_point.m_x;
}

int
TextPainter::drawSelHyperText_utf8(
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	const sl::StringRef_utf8& text
) {
	if (selStart >= selEnd)
		return drawHyperText_utf8(textColor, backColor, fontFlags, attrArray, text);

	if (attrArray)
		m_selOverlayBuffer.copy(attrArray->cp(), attrArray->getCount());
	else
		m_selOverlayBuffer.clear();

	m_selOverlayBuffer.setAttr(selStart, selEnd, selAttr);
	return drawHyperText_utf8(textColor, backColor, fontFlags, &m_selOverlayBuffer, text);
}

int
TextPainter::drawSelHyperText_utf32(
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	const sl::StringRef_utf32& text
) {
	if (selStart >= selEnd)
		return drawHyperText_utf32(textColor, backColor, fontFlags, attrArray, text);

	if (attrArray)
		m_selOverlayBuffer.copy(attrArray->cp(), attrArray->getCount());
	else
		m_selOverlayBuffer.clear();

	m_selOverlayBuffer.setAttr(selStart, selEnd, selAttr);
	return drawHyperText_utf32(textColor, backColor, fontFlags, &m_selOverlayBuffer, text);
}

// bin hex

int
TextPainter::drawBinHex(
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	size_t halfBitOffset,
	const void* p,
	size_t size,
	uint_t flags
) {
	if (!size)
		return m_point.m_x;

	enc::HexEncoding::encode(&m_stringBuffer, p, size, flags);
	m_stringBuffer.append(' ');

	if (halfBitOffset < size) {
		size_t i = halfBitOffset * 3;
		size_t j = i + 1;

		ASSERT(j < m_stringBuffer.getLength());

		char* buffer = m_stringBuffer.getBuffer();
		buffer[i] = buffer[j];
		buffer[j] = ' ';
	}

	return drawText_utf8(
		textColor,
		backColor,
		fontFlags,
		m_stringBuffer
	);
}

int
TextPainter::drawHyperBinHex(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	size_t halfBitOffset,
	const void* p0,
	size_t size,
	uint_t flags
) {
	if (!attrArray || attrArray->isEmpty())
		return drawBinHex(textColor0, backColor0, fontFlags0, halfBitOffset, p0, size, flags);

	if (!size)
		return m_point.m_x;

	TextAttr attr0(textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->cp();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount();

	if (!nextAttr->m_offset) {
		attr.overlay(nextAttr->m_attr);
		nextAttr++;
	}

	size_t offset = 0;
	const char* p = (const char*) p0;

	while (offset < size && nextAttr < attrEnd) {
		size_t leftover = size - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return drawBinHex(attr, halfBitOffset, p, leftover, flags);

		drawBinHex(attr, halfBitOffset, p, chunk, flags);

		attr.overlay(attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
		halfBitOffset -= chunk;
	}

	if (offset < size) {
		size_t leftover = size - offset;
		drawBinHex(attr, halfBitOffset, p, leftover, flags);
	}

	return m_point.m_x;
}

int
TextPainter::drawSelHyperBinHex(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	size_t halfBitOffset,
	const void* p,
	size_t size,
	uint_t flags
) {
	if (selStart >= selEnd)
		return drawHyperBinHex(textColor0, backColor0, fontFlags0, attrArray, halfBitOffset, p, size, flags);

	if (attrArray)
		m_selOverlayBuffer.copy(attrArray->cp(), attrArray->getCount());
	else
		m_selOverlayBuffer.clear();

	m_selOverlayBuffer.setAttr(selStart, selEnd, selAttr);
	return drawHyperBinHex(textColor0, backColor0, fontFlags0, &m_selOverlayBuffer, halfBitOffset, p, size, flags);
}

// bin text

size_t
TextPainter::buildBinTextString(
	sl::Array<utf32_t>* binTextBuffer,
	sl::String* stringBuffer,
	enc::CharCodecKind codecKind,
	const void* p,
	size_t dataSize,
	size_t bufferSize,
	utf32_t unprintableChar
) {
	size_t length = buildBinTextBuffer(binTextBuffer, codecKind, p, dataSize, bufferSize, unprintableChar);
	stringBuffer->copy(sl::StringRef_utf32(*binTextBuffer, length));
	return length;
}

sl::String
TextPainter::buildBinTextString(
	enc::CharCodecKind codecKind,
	const void* p,
	size_t dataSize,
	size_t bufferSize,
	utf32_t unprintableChar
) {
	sl::Array<utf32_t> binTextBuffer;
	sl::String stringBuffer;
	buildBinTextString(&binTextBuffer, &stringBuffer, codecKind, p, dataSize, bufferSize, unprintableChar);
	return stringBuffer;
}

int
TextPainter::drawBinText(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	enc::CharCodecKind codecKind,
	const void* p,
	size_t dataSize,
	size_t bufferSize
) {
	if (!dataSize)
		return m_point.m_x;

	size_t length = buildBinTextBuffer(codecKind, p, dataSize, bufferSize);
	return drawText_utf32(sl::StringRef_utf32(m_binTextBuffer, length));
}

int
TextPainter::drawHyperBinText(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	enc::CharCodecKind codecKind,
	const void* p,
	size_t dataSize,
	size_t bufferSize
) {
	if (!dataSize)
		return m_point.m_x;

	size_t length = buildBinTextBuffer(codecKind, p, dataSize, bufferSize);
	return drawHyperText_utf32(
		textColor0,
		backColor0,
		fontFlags0,
		attrArray,
		sl::StringRef_utf32(m_binTextBuffer, length)
	);
}

int
TextPainter::drawSelHyperBinText(
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	enc::CharCodecKind codecKind,
	const void* p,
	size_t dataSize,
	size_t bufferSize
) {
	if (!dataSize)
		return m_point.m_x;

	size_t length = buildBinTextBuffer(codecKind, p, dataSize, bufferSize);
	return drawSelHyperText_utf32(
		textColor0,
		backColor0,
		fontFlags0,
		attrArray,
		selAttr,
		selStart,
		selEnd,
		sl::StringRef_utf32(m_binTextBuffer, length)
	);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class BinTextBuilderBase {
public:
	virtual
	size_t
	build(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t unprintableChar
	) = 0;
};

template <typename Decoder>
class BinTextBuilder: public BinTextBuilderBase {
protected:
	utf32_t* m_dst;
	utf32_t* m_dstEnd;
	const char* m_src;
	utf32_t m_unprintableChar;

public:
#if (_AXL_DEBUG)
	bool
	isBufferOverflow() const {
		return m_dst > m_dstEnd + 4;
	}
#endif

	virtual
	size_t
	build(
		utf32_t* buffer,
		size_t bufferLength,
		const void* p,
		size_t size,
		utf32_t unprintableChar
	) {
		m_dst = buffer;
		m_dstEnd = buffer + bufferLength;
		m_src = (char*)p;
		m_unprintableChar = unprintableChar;

		Decoder::decode(*this, (char*)p, (char*)p + size);

		ASSERT(m_dst <= m_dstEnd + 4);
		return m_dst - buffer;
	}

	// DecodeEmitter

	bool
	canEmit() const {
		return m_dst < m_dstEnd;
	}

	void
	emitCp(
		const char* p,
		utf32_t c
	) {
		*m_dst++ = enc::isPrintableNonMark(c) ? c : m_unprintableChar;

		for (const char* cb = m_src + 1; cb < p; cb++)
			*m_dst++ = m_unprintableChar;

		m_src = p;
	}

	void
	emitCu(
		const char* p,
		utf32_t c
	) {
		*m_dst++ = m_unprintableChar;
		m_src = p;
	}

	void
	emitCpAfterCu(
		const char* p,
		utf32_t c
	) {
		emitCp(p, c);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BinTextBuilderBase*
getBinTextBuilder(enc::CharCodecKind codecKind) {
	static BinTextBuilder<enc::AsciiDecoder>     asciiBuilder;
	static BinTextBuilder<enc::Utf8Decoder>      utf8Builder;
	static BinTextBuilder<enc::Utf16sDecoder>    utf16Builder;
	static BinTextBuilder<enc::Utf16sDecoder_be> utf16Builder_be;
	static BinTextBuilder<enc::Utf32sDecoder>    utf32Builder;
	static BinTextBuilder<enc::Utf32sDecoder_be> utf32Builder_be;

	static BinTextBuilderBase* builderTable[enc::CharCodecKind__Count] = {
		&asciiBuilder,
		&utf8Builder,
		&utf16Builder,
		&utf16Builder_be,
		&utf32Builder,
		&utf32Builder_be,
	};

	if (codecKind >= countof(builderTable)) {
		ASSERT(false);
		codecKind = enc::CharCodecKind_Ascii;
	}

	return builderTable[codecKind];
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
TextPainter::buildBinTextBuffer(
	sl::Array<utf32_t>* binTextBuffer,
	enc::CharCodecKind codecKind,
	const void* p0,
	size_t dataSize,
	size_t bufferSize,
	utf32_t unprintableChar
) {
	binTextBuffer->setCount(dataSize + 4); // UTF-8 can emit up to 4 codepoints at once
	utf32_t* buffer = binTextBuffer->p();

	BinTextBuilderBase* builder = getBinTextBuilder(codecKind);

	size_t length = builder->build(
		buffer,
		dataSize,
		(char*)p0,
		bufferSize,
		unprintableChar
	);

	for (size_t i = length; i < dataSize; i++)
		buffer[i] = unprintableChar;

	return dataSize;
}

//..............................................................................

} // namespace gui
} // namespace axl
