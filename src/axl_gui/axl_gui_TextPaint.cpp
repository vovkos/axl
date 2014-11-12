#include "pch.h"
#include "axl_gui_TextPaint.h"
#include "axl_gui_Font.h"
#include "axl_enc_HexEncoding.h"

namespace axl {
namespace gui {

//.............................................................................

void
TextPaint::init (Canvas* canvas)
{
	m_canvas = canvas;
	m_top = 0;
	m_bottom = 0;
	m_hexEncodingFlags = 0; 
	m_unprintableChar = '.';
}

// space

int 
TextPaint::paintSpace_p (
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
TextPaint::paintSpace (
	size_t length,
	uint_t color
	)
{
	Size size = m_canvas->m_font->calcTextSize (" ", 1);
	return paintSpace_p (length * size.m_width, color);
}

// text

int
TextPaint::paintText_utf8 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const utf8_t* text,
	size_t length
	)
{
	if (length == -1)
		length = strlen (text);
	
	if (!length)
		return m_point.m_x;

	Font* font = m_canvas->m_font->getFontMod (fontFlags);
	Size size = font->calcTextSize_utf8 (text, length);
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
		text, 
		length
		);
	
	m_point.m_x = right;
	return right;
}

int
TextPaint::paintText_utf32 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const utf32_t* text,
	size_t length
	)
{
	if (length == -1)
		length = rtl::StringDetails_utf32::calcLength (text);
	
	if (!length)
		return m_point.m_x;

	Font* font = m_canvas->m_font->getFontMod (fontFlags);
	Size size = font->calcTextSize_utf32 (text, length);
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
		text, 
		length
		);
	
	m_point.m_x = right;
	return right;
}

// hyper text

int
TextPaint::paintHyperText_utf8 (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const utf8_t* text,
	size_t length
	)
{
	if (!attrArray || attrArray->isEmpty ())
		return paintText_utf8 (textColor0, backColor0, fontFlags0, text, length);

	if (length == -1)
		length = strlen (text);
	
	if (!length)
		return m_point.m_x;
	
	TextAttr attr0 (textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->ca ();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount ();

	if (!nextAttr->m_offset)
	{
		attr.overlay (nextAttr->m_attr);
		nextAttr++;
	}

	size_t offset = 0;
	const utf8_t* p = text;

	while (offset < length && nextAttr < attrEnd)
	{	
		size_t leftover = length - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return paintText_utf8 (attr, p, leftover);

		paintText_utf8 (attr, p, chunk);

		attr.overlay (attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
	}

	if (offset < length)
	{
		size_t leftover = length - offset;
		paintText_utf8 (attr, p, leftover);
	}

	return m_point.m_x;
}

int
TextPaint::paintHyperText_utf32 (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const utf32_t* text,
	size_t length
	)
{
	if (!attrArray || attrArray->isEmpty ())
		return paintText_utf32 (textColor0, backColor0, fontFlags0, text, length);

	if (length == -1)
		length = rtl::StringDetails_utf32::calcLength (text);
	
	if (!length)
		return m_point.m_x;
	
	TextAttr attr0 (textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->ca ();
	const TextAttrAnchor* attrEnd = nextAttr + attrArray->getCount ();

	if (!nextAttr->m_offset)
	{
		attr.overlay (attr, nextAttr->m_attr);
		nextAttr++;
	}

	size_t offset = 0;
	const utf32_t* p = text;

	while (offset < length && nextAttr < attrEnd)
	{	
		size_t leftover = length - offset;
		size_t chunk = nextAttr->m_offset - offset;
		if (chunk >= leftover)
			return paintText_utf32 (attr, p, leftover);

		paintText_utf32 (attr, p, chunk);

		attr.overlay (attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
	}

	if (offset < length)
	{
		size_t leftover = length - offset;
		paintText_utf32 (attr, p, leftover);
	}

	return m_point.m_x;
}

int
TextPaint::paintSelHyperText_utf8 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	const utf8_t* text,
	size_t length
	)
{
	if (selStart >= selEnd) 
		return paintHyperText_utf8 (textColor, backColor, fontFlags, attrArray, text, length);

	if (attrArray)
		m_selOverlayBuffer.copy (attrArray->ca (), attrArray->getCount ());
	else
		m_selOverlayBuffer.clear ();

	m_selOverlayBuffer.setAttr (selStart, selEnd, selAttr);
	return paintHyperText_utf8 (textColor, backColor, fontFlags, &m_selOverlayBuffer, text, length);
}

int
TextPaint::paintSelHyperText_utf32 (
	uint_t textColor,
	uint_t backColor,
	uint_t fontFlags,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	const utf32_t* text,
	size_t length
	)
{
	if (selStart >= selEnd) 
		return paintHyperText_utf32 (textColor, backColor, fontFlags, attrArray, text, length);

	if (attrArray)
		m_selOverlayBuffer.copy (attrArray->ca (), attrArray->getCount ());
	else
		m_selOverlayBuffer.clear ();

	m_selOverlayBuffer.setAttr (selStart, selEnd, selAttr);
	return paintHyperText_utf32 (textColor, backColor, fontFlags, &m_selOverlayBuffer, text, length);
}

// bin hex

int
TextPaint::paintBinHex (
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

		m_stringBuffer.getBuffer () [i] = m_stringBuffer [j];
		m_stringBuffer.getBuffer () [j] = ' ';
	}
	
	return paintText_utf8 (
		textColor,
		backColor,
		fontFlags,
		m_stringBuffer, 
		m_stringBuffer.getLength ()
		);
}

int
TextPaint::paintHyperBinHex (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	size_t halfBitOffset,
	const void* p0,
	size_t size
	)
{
	if (!attrArray)
		return paintBinHex (textColor0, backColor0, fontFlags0, halfBitOffset, p0, size);

	if (!size)
		return m_point.m_x;
	
	TextAttr attr0 (textColor0, backColor0, fontFlags0);
	TextAttr attr = attr0;

	const TextAttrAnchor* nextAttr = attrArray->ca ();
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
			return paintBinHex (attr, halfBitOffset, p, leftover);

		paintBinHex (attr, halfBitOffset, p, chunk);

		attr.overlay (attr0, nextAttr->m_attr);
		nextAttr++;
		p += chunk;
		offset += chunk;
		halfBitOffset -= chunk;
	}

	if (offset < size)
	{
		size_t leftover = size - offset;
		paintBinHex (attr, halfBitOffset, p, leftover);
	}

	return m_point.m_x;
}

int
TextPaint::paintSelHyperBinHex (
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
		return paintHyperBinHex (textColor0, backColor0, fontFlags0, attrArray, halfBitOffset, p, size);

	if (attrArray)
		m_selOverlayBuffer.copy (attrArray->ca (), attrArray->getCount ());
	else
		m_selOverlayBuffer.clear ();

	m_selOverlayBuffer.setAttr (selStart, selEnd, selAttr);
	return paintHyperBinHex (textColor0, backColor0, fontFlags0, &m_selOverlayBuffer, halfBitOffset, p, size);
}

// bin text

void
TextPaint::buildBinTextBuffer (
	enc::CharCodec* codec,
	const void* p0,
	size_t size
	)
{
	m_binTextBuffer.setCount (size);

	size_t unitSize = codec->getUnitSize ();
	size_t i = 0;

	const char* p = (const char*) p0;
	const char* end = p + size;

	while (p < end)
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
			leftover = end - p;
			if (expectedSize <= leftover)
			{
				codec->decodeToUtf32 (&codePoint, 1, p, leftover, &takenBufferLength);
				
				if (takenBufferLength == 1) // might still be not enough (e.g. UTF-16)
				{
					m_binTextBuffer [i] = enc::utfIsPrintable (codePoint) ? codePoint : m_unprintableChar;
					i++;
				}
			}

			size_t end = i + leftover;
			for (; i < end; i++)
				m_binTextBuffer [i] = m_unprintableChar;

			break;
		}

		m_binTextBuffer [i] = enc::utfIsPrintable (codePoint) ? codePoint : m_unprintableChar;

		size_t end = i + takenSize;
		for (i++; i < end; i++)
			m_binTextBuffer [i] = m_unprintableChar;

		p += takenSize;
	}
}

int
TextPaint::paintBinText (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	enc::CharCodec* codec,
	const void* p,
	size_t size
	)
{
	if (!size)
		return m_point.m_x;

	buildBinTextBuffer (codec, p, size);
	return paintText_utf32 (m_binTextBuffer, size);
}

int
TextPaint::paintHyperBinText (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	enc::CharCodec* codec,
	const void* p,
	size_t size
	)
{
	if (!size)
		return m_point.m_x;

	buildBinTextBuffer (codec, p, size);
	return paintHyperText_utf32 (
		textColor0,
		backColor0,
		fontFlags0,
		attrArray,
		m_binTextBuffer, 
		size
		);
}

int
TextPaint::paintSelHyperBinText (
	uint_t textColor0,
	uint_t backColor0,
	uint_t fontFlags0,
	const TextAttrAnchorArray* attrArray,
	const TextAttr& selAttr,
	size_t selStart,
	size_t selEnd,
	enc::CharCodec* codec,
	const void* p,
	size_t size
	)
{
	if (!size)
		return m_point.m_x;

	buildBinTextBuffer (codec, p, size);
	return paintSelHyperText_utf32 (
		textColor0,
		backColor0,
		fontFlags0,
		attrArray,
		selAttr,
		selStart,
		selEnd,
		m_binTextBuffer, 
		size
		);
}

//.............................................................................

} // namespace gui
} // namespace axl
