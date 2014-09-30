#include "pch.h"
#include "axl_gui_HyperText.h"

namespace axl {
namespace gui {

//.............................................................................

void
Hyperext::clear ()
{
	m_source.clear ();
	m_text.clear ();
	m_attrArray.clear ();
	m_hyperlinkArray.clear ();
	m_hyperlinkXMap.clear ();
}

size_t
Hyperext::backspace (size_t backLength)
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
Hyperext::appendPlainText (
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
Hyperext::appendPlainText (
	char c, 
	size_t count
	)
{
	m_source.append (c, count);
	return m_text.append (c, count);
}

size_t
Hyperext::appendHyperText (
	const TextAttr& baseAttr,
	const char* text,
	size_t length
	)
{
	TextAttr attr = baseAttr;

	const char* p = text;
	const char* end;
	
	size_t lastLength = m_text.getLength ();
	
	if (length == -1)
		length = strlen (text);

	end = p + length;

	m_source.append (p, length);

	for (;;)
	{
		const char* tag;
		const char* tagEnd;
		const char* param;

		tag = strchr_e (p, end, '<');
		if (!tag)
		{
			m_text.append (p, end - p);
			break;
		}

		m_text.append (p, tag - p);

		tagEnd = strchr_e (tag + 1, end, '>');
		if (!tagEnd)
			break;

		length = m_text.getLength ();
		m_attrArray.setAttr (lastLength, length, attr);
		lastLength = length;

		param = strchr_e (tag + 1, tagEnd, '=');
		if (param)
		{
			attr.parse (tag + 1);
			m_hyperlinkArray.openHyperlink (length, param + 1, tagEnd - param - 1);
		}
		else if (tag[1] == '\b') // backspace
		{
			size_t backLength = tag[2] != '*' ? (size_t) strtoul (tag + 2, NULL, 10) : -1;
			backspace (backLength ? backLength : 1);
		}
		else 
		{
			attr.parse (tag + 1);
			m_hyperlinkArray.closeHyperlink (length);
		}

		p = tagEnd + 1;
	}

	length = m_text.getLength ();
	m_attrArray.setAttr (lastLength, length, attr);
	return length;
}

HyperlinkAnchor*
Hyperext::findHyperlinkByX (int x) const
{
	HyperlinkAnchor* result = NULL;

	size_t begin = 0;
	size_t end = m_hyperlinkXMap.getCount ();

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;

		const HyperlinkXMapEntry* mapEntry = &m_hyperlinkXMap [mid];
		if (mapEntry->m_x == x)
			return mapEntry->m_anchor;

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

	return result;
}

void
Hyperext::calcHyperlinkXMap (Font* baseFont)
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
Hyperext::calcTextSize (Font* baseFont) const
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
