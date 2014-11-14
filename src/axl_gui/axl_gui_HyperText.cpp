#include "pch.h"
#include "axl_gui_HyperText.h"
#include "axl_gui_AnsiAttrParser.h"

namespace axl {
namespace gui {

//.............................................................................

void
HyperText::clear ()
{
	m_source.clear ();
	m_text.clear ();
	m_attrArray.clear ();
	m_hyperlinkArray.clear ();
	m_hyperlinkXMap.clear ();
}

size_t
HyperText::backspace (size_t backLength)
{
	// don't touch m_Source!

	// TODO: backspace attributes and hyperlinks

	size_t length = m_text.getLength ();
	if (backLength >= length)
	{
		m_text.clear ();
		return 0;
	}

	m_text.reduceLength (backLength);
	return length - backLength;
}

size_t 
HyperText::appendPlainText (
	const char* text, 
	size_t length
	)
{
	if (length == -1)
		length = strlen (text);

	m_source.append (text, length);
	return m_text.append (text, length);
}

size_t 
HyperText::appendPlainText (
	char c, 
	size_t count
	)
{
	m_source.append (c, count);
	return m_text.append (c, count);
}

void
parseAnsiAttr (
	TextAttr* attr,
	const char* p, 
	size_t length
	)
{
}

size_t
HyperText::appendHyperText (
	const TextAttr& baseAttr,
	const char* text,
	size_t length
	)
{
	TextAttr attr = baseAttr;
	AnsiAttrParser attrParser;

	const char* p = text;
	const char* end;
	
	size_t lastLength = m_text.getLength ();
	
	if (length == -1)
		length = strlen (text);

	end = p + length;

	m_source.append (p, length);

	for (;;)
	{
		const char* token = p;
		while (token < end && *token != 0x1b)	
			token++;

		size_t textLength = m_text.append (p, token - p);
		p = token + 1;

		if (p + 1 >= end)
			break;

		if (*p == '^') // hyperlink
		{
			p++;

			const char* arg = p;
			while (p < end && *p != 0x1b)
				p++;

			if (p + 1 >= end)
				break;

			const char* argEnd = p;

			if (p [1] == '\\') // ST: string terminator
				p += 2;

			m_hyperlinkArray.openHyperlink (textLength, arg, argEnd - arg);
		}
		else if (*p == '[') // CSI
		{
			p++;

			const char* arg = p;
			while (p < end && !isalpha (*p))
				p++;

			const char* argEnd = p;
			if (p >= end)
				break;

			switch (*p)
			{
			case 'm':
				m_hyperlinkArray.closeHyperlink (textLength);
				m_attrArray.setAttr (lastLength, textLength, attr);
				lastLength = textLength;
				attrParser.parse (
					&attr,
					baseAttr, 
					arg, 
					argEnd - arg
					);
				break;

			case 'D':				
				backspace (argEnd == arg ? 1 : atoi (arg));
				break;

			case 'K':
			case 'J':
				clear ();
				break;
			}

			p++;
		}
	}

	m_attrArray.setAttr (lastLength, m_text.getLength (), attr);
	return length;
}

HyperlinkAnchor*
HyperText::findHyperlinkByX (int x) const
{
	HyperlinkAnchor* result = NULL;

	size_t begin = 0;
	size_t end = m_hyperlinkXMap.getCount ();

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;

		const HyperlinkXMapEntry* mapEntry = &m_hyperlinkXMap [mid];
		if (mapEntry->m_x == x)
		{
			result = mapEntry->m_anchor;
			break;
		}

		if (mapEntry->m_x < x)
		{
			result = mapEntry->m_anchor;
			begin = mid + 1;
		}
		else
		{
			end = mid;
		}
	}

	return result && !result->m_hyperlink.isEmpty () ? result : NULL;
}

void
HyperText::calcHyperlinkXMap (Font* baseFont)
{
	int x = 0;
	size_t offset = 0;
	size_t length = m_text.getLength ();
	size_t attrCount = m_attrArray.getCount ();
	size_t hyperlinkCount = m_hyperlinkArray.getCount ();

	const TextAttrAnchor* attrAnchor = m_attrArray;
	const TextAttrAnchor* attrEnd = attrAnchor + attrCount;
	HyperlinkXMapEntry* hyperlinkXMapEntry;
	
	uint_t fontFlags = 0;
	Font* font = baseFont->getFontMod (fontFlags);

	m_hyperlinkXMap.setCount (hyperlinkCount);
	hyperlinkXMapEntry = m_hyperlinkXMap;

	rtl::Iterator <HyperlinkAnchor> it = m_hyperlinkArray.getHead ();
	for (; it; it++)
	{
		HyperlinkAnchor* hyperlinkAnchor = *it;
		Size size;

		for (; attrAnchor < attrEnd && attrAnchor->m_offset < hyperlinkAnchor->m_offset; attrAnchor++)
		{
			if (attrAnchor->m_attr.m_fontFlags == fontFlags)
				continue;

			size = font->calcTextSize (m_text.cc () + offset, attrAnchor->m_offset - offset);			
			
			x += size.m_width;
			offset = attrAnchor->m_offset;

			fontFlags = attrAnchor->m_attr.m_fontFlags;
			font = baseFont->getFontMod (fontFlags);
		}

		size = font->calcTextSize (m_text.cc () + offset, hyperlinkAnchor->m_offset - offset);
		
		x += size.m_width;
		offset = hyperlinkAnchor->m_offset;

		hyperlinkXMapEntry->m_x = x;
		hyperlinkXMapEntry->m_anchor = hyperlinkAnchor;

		hyperlinkAnchor++; 
		hyperlinkXMapEntry++;
	}
}

Size
HyperText::calcTextSize (Font* baseFont) const
{
	Size size;

	int x = 0;
	size_t offset = 0;
	size_t length = m_text.getLength ();
	size_t attrCount = m_attrArray.getCount ();

	uint_t fontFlags = 0;
	Font* font = baseFont->getFontMod (fontFlags);

	for (size_t i = 0; i < attrCount; i++)
	{
		const TextAttrAnchor* attrAnchor = &m_attrArray [i];

		if (attrAnchor->m_attr.m_fontFlags == fontFlags)
			continue;

		size = font->calcTextSize (m_text.cc () + offset, attrAnchor->m_offset - offset);
		
		x += size.m_width;
		offset = attrAnchor->m_offset;

		fontFlags = attrAnchor->m_attr.m_fontFlags;
		font = baseFont->getFontMod (fontFlags);
	}

	size = font->calcTextSize (m_text.cc () + offset, length - offset);
	size.m_width += x;

	return size;
}

//.............................................................................

} // namespace gui
} // namespace axl
