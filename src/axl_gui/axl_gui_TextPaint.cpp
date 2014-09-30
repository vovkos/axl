#include "pch.h"
#include "axl_gui_TextPaint.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace gui {

//.............................................................................

void
TextPaint::init (Canvas* canvas)
{
	m_canvas = canvas;
	m_p = NULL;
	m_begin = NULL;
	m_end = NULL;
	m_attr = NULL;
	m_attrBegin = NULL;
	m_attrEnd = NULL;
	m_top = 0;
	m_bottom = 0;
	m_hexEncodingFlags = 0; 
	m_unprintableChar = '.';
}

// calc monospace/proportional text rect

Rect
TextPaint::calcTextRect_utf8 (
	const utf8_t* text,
	size_t length
	)
{
	Font* font = m_canvas->m_baseFont->getFontMod (m_canvas->m_defTextAttr.m_fontFlags);
	Size size = font->calcTextSize_utf8 (text, length);
	return Rect (m_point.m_x, m_top, m_point.m_x + size.m_width, m_bottom);
}

Rect
TextPaint::calcTextRect_utf32 (
	const utf32_t* text,
	size_t length
	)
{
	Font* font = m_canvas->m_baseFont->getFontMod (m_canvas->m_defTextAttr.m_fontFlags);
	Size size = font->calcTextSize_utf32 (text, length);
	return Rect (m_point.m_x, m_top, m_point.m_x + size.m_width, m_bottom);
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
	Font* font = m_canvas->m_baseFont->getFontMod (m_canvas->m_defTextAttr.m_fontFlags);
	Size size = font->calcTextSize (" ", 1);
	return paintSpace_p (length * size.m_width, color);
}

// text

int
TextPaint::paintText_utf8 (
	const utf8_t* text,
	size_t length
	)
{
	if (length == -1)
		length = strlen (text);
	
	m_p = text;
	m_begin = text;
	m_end = text + length;

	return paintTextPart_utf8 (length);
}

int
TextPaint::paintText_utf32 (
	const utf32_t* text,
	size_t length
	)
{
	if (length == -1)
		length = rtl::StringDetails_utf32::calcLength (text);
	
	m_p = (char*) text;
	m_begin = (char*) text;
	m_end = (char*) (text + length);

	return paintTextPart_utf32 (length);
}

int
TextPaint::paintTextPart_utf8 (size_t length)
{
	ASSERT (length <= (size_t) (m_end - m_p));

	if (!length)
		return m_point.m_x;

	Rect rect = calcTextRect_utf8 (m_p, length);
	m_canvas->drawText_utf8 (m_point, rect, m_p, length);
	
	m_p += length;
	m_point.m_x = rect.m_right;

	return m_point.m_x;
}

int
TextPaint::paintTextPart_utf32 (size_t length)
{
	ASSERT (length <= (size_t) ((m_end - m_p) / sizeof (utf32_t)));

	if (!length)
		return m_point.m_x;

	Rect rect = calcTextRect_utf32 ((utf32_t*) m_p, length);
	m_canvas->drawText_utf32 (m_point, rect, (utf32_t*) m_p, length);
	
	m_p += length * sizeof (utf32_t);
	m_point.m_x = rect.m_right;

	return m_point.m_x;
}

// hyper text

int
TextPaint::paintHyperText_utf8 (
	const TextAttrAnchorArray* attrArray,
	const utf8_t* text,
	size_t length
	)
{
	const TextAttrAnchor* attr = attrArray ? attrArray->ca () : NULL;
	size_t attrCount = attrArray ? attrArray->getCount () : 0;

	size_t offset = 0;

	if (length == -1)
		length = strlen (text);
	
	m_p = text;
	m_begin = text;
	m_end = text + length;
	m_attr = attr;
	m_attrBegin = attr;
	m_attrEnd = attr + attrCount;

	if (!length)
		return m_point.m_x;

	if (!attrCount)
	{
		m_canvas->m_defTextAttr.clear ();
		return paintTextPart_utf8 (length);
	}

	const TextAttrAnchor* nextAttr;

	if (m_attr->m_offset <= offset)
	{
		m_canvas->m_defTextAttr = m_attr->m_attr;
		nextAttr = m_attr + 1;
	}
	else
	{
		m_canvas->m_defTextAttr.clear ();
		nextAttr = m_attr;
	}

	const char* p = text;
	const char* end = p + length;

	while (p < end && nextAttr < m_attrEnd)
	{	
		size_t maxLength = end - p;

		length = nextAttr->m_offset - offset;
		if (length > maxLength)
			return paintTextPart_utf8 (maxLength);

		paintTextPart_utf8 (length);

		m_canvas->m_defTextAttr = nextAttr->m_attr;
		m_attr = nextAttr;
		nextAttr++;

		p = m_p;
		offset = p - m_begin;
	}

	if (p < end)
		paintTextPart_utf8 (end - p);

	return m_point.m_x;
}

int
TextPaint::paintHyperText_utf32 (
	const TextAttrAnchorArray* attrArray,
	const utf32_t* text,
	size_t length
	)
{
	const TextAttrAnchor* attr = attrArray ? attrArray->ca () : NULL;
	size_t attrCount = attrArray ? attrArray->getCount () : 0;

	size_t offset = 0;

	if (length == -1)
		length = rtl::StringDetails_utf32::calcLength (text);
	
	m_p = (char*) text;
	m_begin = (char*) text;
	m_end = (char*) (text + length);
	m_attr = attr;
	m_attrBegin = attr;
	m_attrEnd = attr + attrCount;

	if (!length)
		return m_point.m_x;

	if (!attrCount)
	{
		m_canvas->m_defTextAttr.clear ();
		return paintTextPart_utf32 (length);
	}

	const TextAttrAnchor* nextAttr;

	if (m_attr->m_offset <= offset)
	{
		m_canvas->m_defTextAttr = m_attr->m_attr;
		nextAttr = m_attr + 1;
	}
	else
	{
		m_canvas->m_defTextAttr.clear ();
		nextAttr = m_attr;
	}

	const utf32_t* p = text;
	const utf32_t* end = p + length;

	while (p < end && nextAttr < m_attrEnd)
	{	
		size_t maxLength = end - p;

		length = nextAttr->m_offset - offset;
		if (length > maxLength)
			return paintTextPart_utf32 (maxLength);

		paintTextPart_utf32 (length);

		m_canvas->m_defTextAttr = nextAttr->m_attr;
		m_attr = nextAttr;
		nextAttr++;

		p = (utf32_t*) m_p;
		offset = p - (utf32_t*) m_begin;
	}

	if (p < end)
		paintTextPart_utf32 (end - p);

	return m_point.m_x;
}

int
TextPaint::paintSelHyperText_utf8 (
	const TextAttrAnchorArray* attrArray,
	size_t selStart,
	size_t selEnd,
	const utf8_t* text,
	size_t length
	)
{
	if (selStart >= selEnd) 
		return paintHyperText_utf8 (attrArray, text, length);

	if (length == -1)
		length = strlen (text);

	if (attrArray)
		m_selOverlay.copy (*attrArray, attrArray->getCount ());
	else
		m_selOverlay.clear ();

	m_selOverlay.setAttr (selStart, selEnd, m_selAttr, -1);
	return paintHyperText_utf8 (&m_selOverlay, text, length);
}

int
TextPaint::paintSelHyperText_utf32 (
	const TextAttrAnchorArray* attrArray,
	size_t selStart,
	size_t selEnd,
	const utf32_t* text,
	size_t length
	)
{
	if (selStart >= selEnd) 
		return paintHyperText_utf32 (attrArray, text, length);

	if (length == -1)
		length = rtl::StringDetails_utf32::calcLength (text);

	if (attrArray)
		m_selOverlay.copy (*attrArray, attrArray->getCount ());
	else
		m_selOverlay.clear ();

	m_selOverlay.setAttr (selStart, selEnd, m_selAttr, -1);
	return paintHyperText_utf32 (&m_selOverlay, text, length);
}

// bin hex

int
TextPaint::paintBinHexPart (size_t size)
{
	size_t maxSize = m_end - m_p;
	if (size > maxSize)
		size = maxSize;

	if (!size)
		return m_point.m_x;

	rtl::HexEncoding::encode (&m_stringBuffer, m_p, size, 0);
	m_stringBuffer.append (' ');
	
	size_t length = m_stringBuffer.getLength ();
	Rect rect = calcTextRect_utf8 (m_stringBuffer, length);
	m_canvas->drawText (m_point, rect, m_stringBuffer, length);

	m_p += size;
	m_point.m_x = rect.m_right;

	return m_point.m_x;
}

int
TextPaint::paintBinHex (
	const void* p,
	size_t size
	)
{
	m_p = (const char*) p;
	m_begin = m_p;
	m_end = m_p + size;

	return paintBinHexPart (-1);
}

int
TextPaint::paintHyperBinHex (
	const TextAttrAnchorArray* attrArray,
	const void* _p,
	size_t size
	)
{
	const TextAttrAnchor* attr = attrArray ? attrArray->ca () : NULL;
	size_t attrCount = attrArray ? attrArray->getCount () : 0;

	const char* p = (char*) _p;
	const char* end; 

	size_t offset = 0;

	m_p = p;
	m_begin = p;
	m_end = p + size;
	m_attr = attr;
	m_attrBegin = attr;
	m_attrEnd = attr + attrCount;

	if (!size)
		return m_point.m_x;

	if (!attrCount)
	{
		m_canvas->m_defTextAttr.clear ();
		return paintBinHexPart (size);
	}

	const TextAttrAnchor* nextAttr;

	if (m_attr->m_offset <= offset)
	{
		m_canvas->m_defTextAttr = m_attr->m_attr;
		nextAttr = m_attr + 1;
	}
	else
	{
		m_canvas->m_defTextAttr.clear ();
		nextAttr = m_attr;
	}

	end = p + size;
	while (p < end && nextAttr < m_attrEnd)
	{	
		size_t maxSize = end - p;

		size = nextAttr->m_offset - offset;
		if (size > maxSize)
			return paintBinHexPart (maxSize);

		paintBinHexPart (size);

		p = m_p;
		offset = p - m_begin;

		m_canvas->m_defTextAttr = nextAttr->m_attr;
		m_attr = nextAttr;
		nextAttr++;
	}

	if (p < end)
		paintBinHexPart (end - p);

	return m_point.m_x;
}

int
TextPaint::paintSelHyperBinHex (
	const TextAttrAnchorArray* attrArray,
	size_t selStart,
	size_t selEnd,
	const void* p,
	size_t size
	)
{
	if (selStart >= selEnd) 
		return paintHyperBinHex (attrArray, p, size);

	if (attrArray)
		m_selOverlay.copy (*attrArray, attrArray->getCount ());
	else
		m_selOverlay.clear ();

	m_selOverlay.setAttr (selStart, selEnd, m_selAttr, -1);
	return paintHyperBinHex (&m_selOverlay, p, size);
}

int
TextPaint::paintHyperBinHex4BitCursor (
	const TextAttrAnchorArray* attrArray, 
	size_t cursorPos, 
	const void* p, 
	size_t size
	)
{
	if (cursorPos >= size) 
		return paintHyperBinHex (attrArray, p, size);

	paintHyperBinHex (attrArray, p, cursorPos);

	char* cursor = (char*) p + cursorPos;
	
	gui::TextAttr attr = m_canvas->m_defTextAttr;

	char c = rtl::HexEncoding::getHexChar_l (*cursor & 0xf);
	char charBuffer [8] = { c, ' ', ' ', 0 };
	Rect rect = calcTextRect_utf8 (charBuffer, 3);
	m_canvas->m_defTextAttr.overlay (m_selAttr);
	m_canvas->drawText (m_point, rect, charBuffer, 3);

	m_point.m_x = rect.m_right;

	if (cursorPos + 1 < size)
	{
		size_t leftover = size - cursorPos - 1;

		m_p = m_p + cursorPos + 1;
		m_canvas->m_defTextAttr = attr;

		if (!attrArray)
		{
			paintHyperBinHex (NULL, cursor + 1, leftover);
		}
		else
		{
			m_selOverlay = *attrArray;
			m_selOverlay.clearBefore (cursorPos);
			paintHyperBinHex (&m_selOverlay, cursor + 1, leftover);
		}		
	}

	return m_point.m_x;
}

// bin text

int
TextPaint::paintBinTextPart (
	rtl::CharCodec* codec,
	size_t size
	)
{
	size_t maxSize = m_end - m_p;
	if (size > maxSize)
		size = maxSize;

	if (!size)
		return m_point.m_x;

	m_binTextBuffer.setCount (size);

	size_t unitSize = codec->getUnitSize ();
	size_t i = 0;

	const char* p = m_p;
	const char* end = p + size;

	size_t unitSizeMod = (p - m_begin) % unitSize;
	if (unitSizeMod)
	{
		size_t incompleteSize = unitSize - unitSizeMod;

		if (incompleteSize > size)
			incompleteSize = size;

		for (; i < incompleteSize; i++)
			m_binTextBuffer [i] = m_unprintableChar;

		p += incompleteSize;
	}

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
			size_t end = i + leftover;

			leftover = m_end - p;
			if (expectedSize <= leftover)
			{
				codec->decodeToUtf32 (&codePoint, 1, p, leftover, &takenBufferLength);
				
				if (takenBufferLength == 1) // might still be not enough (e.g. UTF-16)
				{
					m_binTextBuffer [i] = isPrintable (codePoint) ? codePoint : m_unprintableChar;
					i++;
				}
			}

			for (; i < end; i++)
				m_binTextBuffer [i] = m_unprintableChar;

			break;
		}

		m_binTextBuffer [i] = isPrintable (codePoint) ? codePoint : m_unprintableChar;

		size_t end = i + takenSize;
		for (i++; i < end; i++)
			m_binTextBuffer [i] = m_unprintableChar;

		p += takenSize;
	}

	ASSERT (i == size);
	
	Rect rect = calcTextRect_utf32 (m_binTextBuffer, size);
	m_canvas->drawText_utf32 (m_point, rect, m_binTextBuffer, size);

	m_p = m_p + size;
	m_point.m_x = rect.m_right;

	return m_point.m_x;
}

int
TextPaint::paintBinText (
	rtl::CharCodec* codec,
	const void* p,
	size_t size
	)
{
	m_p = (const char*) p;
	m_begin = m_p;
	m_end = m_p + size;

	return paintBinTextPart (codec, -1);
}

int
TextPaint::paintHyperBinText (
	rtl::CharCodec* codec,
	const TextAttrAnchorArray* attrArray,
	const void* _p,
	size_t size
	)
{
	const TextAttrAnchor* attr = attrArray ? attrArray->ca () : NULL;
	size_t attrCount = attrArray ? attrArray->getCount () : 0;

	const char* p = (const char*) _p;
	const char* end; 

	size_t offset = 0;

	m_p = p;
	m_begin = p;
	m_end = p + size;
	m_attr = attr;
	m_attrBegin = attr;
	m_attrEnd = attr + attrCount;

	if (!size)
		return m_point.m_x;

	if (!attrCount)
	{
		m_canvas->m_defTextAttr.clear ();
		return paintBinTextPart (codec, size);
	}

	const TextAttrAnchor* nextAttr;

	if (m_attr->m_offset <= offset)
	{
		m_canvas->m_defTextAttr = m_attr->m_attr;
		nextAttr = m_attr + 1;
	}
	else
	{
		m_canvas->m_defTextAttr.clear ();
		nextAttr = m_attr;
	}

	end = p + size;
	while (p < end && nextAttr < m_attrEnd)
	{	
		size_t maxSize = end - p;

		size = nextAttr->m_offset - offset;
		if (size > maxSize)
			return paintBinTextPart (codec, maxSize);

		paintBinTextPart (codec, size);

		p = m_p;
		offset = p - m_begin;

		m_canvas->m_defTextAttr = nextAttr->m_attr;
		m_attr = nextAttr;
		nextAttr++;
	}

	if (p < end)
		paintBinTextPart (codec, end - p);

	return m_point.m_x;
}

int
TextPaint::paintSelHyperBinText (
	rtl::CharCodec* codec,
	const TextAttrAnchorArray* attrArray,
	size_t selStart,
	size_t selEnd,
	const void* p,
	size_t size
	)
{
	if (selStart >= selEnd) 
		return paintHyperBinText (codec, attrArray, p, size);

	if (attrArray)
		m_selOverlay.copy (*attrArray, attrArray->getCount ());
	else
		m_selOverlay.clear ();

	m_selOverlay.setAttr (selStart, selEnd, m_selAttr, -1);
	return paintHyperBinText (codec, &m_selOverlay, p, size);
}

//.............................................................................

} // namespace gui
} // namespace axl
