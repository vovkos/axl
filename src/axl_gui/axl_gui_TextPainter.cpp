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

namespace axl {
namespace gui {

//..............................................................................

void
TextPainter::init (Canvas* canvas)
{
	m_canvas = canvas;
	m_top = 0;
	m_bottom = 0;
	m_hexEncodingFlags = 0;
	m_unprintableChar = '.';
}

// space

int
TextPainter::drawSpace_p (
	int width,
	uint_t color
	)
{
	Rect rect;
	rect.m_left = m_point.m_x;
	rect.m_top = m_top;
	rect.m_right = m_point.m_x + width;
	rect.m_bottom = m_bottom;

	m_canvas->drawRect (rect, color);

	m_point.m_x = rect.m_right;
	return m_point.m_x;
}

int
TextPainter::drawSpace (
	size_t length,
	uint_t color
	)
{
	Size size = m_canvas->m_font->calcTextSize (sl::StringRef (" ", 1));
	return drawSpace_p (length * size.m_width, color);
}

// text

int
TextPainter::drawText_utf8 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf8& text
	)
{
	if (text.isEmpty ())
		return m_point.m_x;

	Font* font = m_canvas->m_font->getFontMod (fontFlags);
	Size size = font->calcTextSize_utf8 (text);
	int right = m_point.m_x + size.m_width;

	m_canvas->drawText_utf8 (
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
TextPainter::drawText_utf32 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const sl::StringRef_utf32& text
	)
{
	if (text.isEmpty ())
		return m_point.m_x;

	Font* font = m_canvas->m_font->getFontMod (fontFlags);
	Size size = font->calcTextSize_utf32 (text);
	int right = m_point.m_x + size.m_width;

	m_canvas->drawText_utf32 (
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
TextPainter::drawHyperText_utf8 (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const sl::StringRef_utf8& text
	)
{
	if (!attrArray || attrArray->isEmpty ())
		return drawText_utf8 (textColor0, backColor0, fontFlags0, text);

	if (text.isEmpty ())
		return m_point.m_x;

	TextAttr attr0 (textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->cp ();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount ();

	if (!nextAttr->m_offset)
	{
		attr.overlay (nextAttr->m_attr);
		nextAttr++;
	}

	const utf8_t* p = text.cp ();
	size_t length = text.getLength ();
	size_t offset = 0;

	while (offset < length && nextAttr < attrEnd)
	{
		size_t leftover = length - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return drawText_utf8 (attr, sl::StringRef_utf8 (p, leftover));

		drawText_utf8 (attr, sl::StringRef_utf8 (p, chunk));

		attr.overlay (attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
	}

	if (offset < length)
	{
		size_t leftover = length - offset;
		drawText_utf8 (attr, sl::StringRef_utf8 (p, leftover));
	}

	return m_point.m_x;
}

int
TextPainter::drawHyperText_utf32 (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const sl::StringRef_utf32& text
	)
{
	if (!attrArray || attrArray->isEmpty ())
		return drawText_utf32 (textColor0, backColor0, fontFlags0, text);

	if (text.isEmpty ())
		return m_point.m_x;

	TextAttr attr0 (textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->cp ();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount ();

	if (!nextAttr->m_offset)
	{
		attr.overlay (attr, nextAttr->m_attr);
		nextAttr++;
	}

	const utf32_t* p = text.cp ();
	size_t length = text.getLength ();
	size_t offset = 0;

	while (offset < length && nextAttr < attrEnd)
	{
		size_t leftover = length - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return drawText_utf32 (attr, sl::StringRef_utf32 (p, leftover));

		drawText_utf32 (attr, sl::StringRef_utf32 (p, chunk));

		attr.overlay (attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
	}

	if (offset < length)
	{
		size_t leftover = length - offset;
		drawText_utf32 (attr, sl::StringRef_utf32 (p, leftover));
	}

	return m_point.m_x;
}

int
TextPainter::drawSelHyperText_utf8 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	const sl::StringRef_utf8& text
	)
{
	if (selStart >= selEnd)
		return drawHyperText_utf8 (textColor, backColor, fontFlags, attrArray, text);

	if (attrArray)
		m_selOverlayBuffer.copy (attrArray->cp (), attrArray->getCount ());
	else
		m_selOverlayBuffer.clear ();

	m_selOverlayBuffer.setAttr (selStart, selEnd, selAttr);
	return drawHyperText_utf8 (textColor, backColor, fontFlags, &m_selOverlayBuffer, text);
}

int
TextPainter::drawSelHyperText_utf32 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	const sl::StringRef_utf32& text
	)
{
	if (selStart >= selEnd)
		return drawHyperText_utf32 (textColor, backColor, fontFlags, attrArray, text);

	if (attrArray)
		m_selOverlayBuffer.copy (attrArray->cp (), attrArray->getCount ());
	else
		m_selOverlayBuffer.clear ();

	m_selOverlayBuffer.setAttr (selStart, selEnd, selAttr);
	return drawHyperText_utf32 (textColor, backColor, fontFlags, &m_selOverlayBuffer, text);
}

// bin hex

int
TextPainter::drawBinHex (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	size_t halfBitOffset,
	const void* p,
	size_t size
	)
{
	if (!size)
		return m_point.m_x;

	enc::HexEncoding::encode (&m_stringBuffer, p, size, 0);
	m_stringBuffer.append (' ');

	if (halfBitOffset < size)
	{
		size_t i = halfBitOffset * 3;
		size_t j = i + 1;

		ASSERT (j < m_stringBuffer.getLength ());

		char* buffer = m_stringBuffer.getBuffer ();
		buffer [i] = buffer [j];
		buffer [j] = ' ';
	}

	return drawText_utf8 (
		textColor,
		backColor,
		fontFlags,
		m_stringBuffer
		);
}

int
TextPainter::drawHyperBinHex (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	size_t halfBitOffset,
	const void* p0,
	size_t size
	)
{
	if (!attrArray || attrArray->isEmpty ())
		return drawBinHex (textColor0, backColor0, fontFlags0, halfBitOffset, p0, size);

	if (!size)
		return m_point.m_x;

	TextAttr attr0 (textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->cp ();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount ();

	if (!nextAttr->m_offset)
	{
		attr.overlay (attr, nextAttr->m_attr);
		nextAttr++;
	}

	size_t offset = 0;
	const char* p = (const char*) p0;

	while (offset < size && nextAttr < attrEnd)
	{
		size_t leftover = size - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return drawBinHex (attr, halfBitOffset, p, leftover);

		drawBinHex (attr, halfBitOffset, p, chunk);

		attr.overlay (attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
		halfBitOffset -= chunk;
	}

	if (offset < size)
	{
		size_t leftover = size - offset;
		drawBinHex (attr, halfBitOffset, p, leftover);
	}

	return m_point.m_x;
}

int
TextPainter::drawSelHyperBinHex (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	size_t halfBitOffset,
	const void* p,
	size_t size
	)
{
	if (selStart >= selEnd)
		return drawHyperBinHex (textColor0, backColor0, fontFlags0, attrArray, halfBitOffset, p, size);

	if (attrArray)
		m_selOverlayBuffer.copy (attrArray->cp (), attrArray->getCount ());
	else
		m_selOverlayBuffer.clear ();

	m_selOverlayBuffer.setAttr (selStart, selEnd, selAttr);
	return drawHyperBinHex (textColor0, backColor0, fontFlags0, &m_selOverlayBuffer, halfBitOffset, p, size);
}

// bin text

size_t
TextPainter::buildBinTextBuffer (
	enc::CharCodec* codec,
	const void* p0,
	size_t dataSize,
	size_t bufferSize
	)
{
	m_binTextBuffer.setCount (dataSize);

	size_t unitSize = codec->getUnitSize ();
	size_t i = 0;

	const char* p = (const char*) p0;
	const char* end = p + bufferSize;

	while (i < dataSize && p < end)
	{
		utf32_t codePoint;
		size_t takenBufferLength = 0;
		size_t takenSize = 0;
		size_t expectedSize;

		size_t leftover = end - p;
		if (leftover < unitSize)
			expectedSize = unitSize;
		else
			codec->decodeToUtf32 (&codePoint, 1, p, leftover, &takenBufferLength, &takenSize, &expectedSize);

		if (!takenSize)
		{
			size_t end = i + leftover;
			if (end > dataSize)
				end = dataSize;

			for (; i < end; i++)
				m_binTextBuffer [i] = m_unprintableChar;

			break;
		}

		m_binTextBuffer [i] = enc::utfIsPrintableNonMark (codePoint) ? codePoint : m_unprintableChar;

		size_t end = i + takenSize;
		if (end > dataSize)
			end = dataSize;

		for (i++; i < end; i++)
			m_binTextBuffer [i] = m_unprintableChar;

		p += takenSize;
	}

	return i;
}

int
TextPainter::drawBinText (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	enc::CharCodec* codec,
	const void* p,
	size_t dataSize,
	size_t bufferSize
	)
{
	if (!dataSize)
		return m_point.m_x;

	size_t length = buildBinTextBuffer (codec, p, dataSize, bufferSize);
	return drawText_utf32 (sl::StringRef_utf32 (m_binTextBuffer, length));
}

int
TextPainter::drawHyperBinText (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	enc::CharCodec* codec,
	const void* p,
	size_t dataSize,
	size_t bufferSize
	)
{
	if (!dataSize)
		return m_point.m_x;

	size_t length = buildBinTextBuffer (codec, p, dataSize, bufferSize);
	return drawHyperText_utf32 (
		textColor0,
		backColor0,
		fontFlags0,
		attrArray,
		sl::StringRef_utf32 (m_binTextBuffer, length)
		);
}

int
TextPainter::drawSelHyperBinText (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	enc::CharCodec* codec,
	const void* p,
	size_t dataSize,
	size_t bufferSize
	)
{
	if (!dataSize)
		return m_point.m_x;

	size_t length = buildBinTextBuffer (codec, p, dataSize, bufferSize);
	return drawSelHyperText_utf32 (
		textColor0,
		backColor0,
		fontFlags0,
		attrArray,
		selAttr,
		selStart,
		selEnd,
		sl::StringRef_utf32 (m_binTextBuffer, length)
		);
}

//..............................................................................

} // namespace gui
} // namespace axl
