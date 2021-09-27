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
#include "axl_gui_HyperText.h"
#include "axl_gui_AnsiAttrParser.h"
#include "axl_gui_Widget.h"

namespace axl {
namespace gui {

//..............................................................................

void
HyperText::clear() {
	m_source.clear();
	m_text.clear();
	m_attrArray.clear();
	m_hyperlinkArray.clear();
	m_hyperlinkXMap.clear();
}

size_t
HyperText::backspace(size_t backLength) {
	// don't touch m_Source!

	// TODO: backspace attributes and hyperlinks

	size_t length = m_text.getLength();
	if (backLength >= length) {
		m_text.clear();
		return 0;
	}

	m_text.chop(backLength);
	return length - backLength;
}

size_t
HyperText::appendPlainText(const sl::StringRef& text) {
	size_t i = text.reverseFind('\x15'); // Ctrl+U -- clear
	if (i != -1) {
		clear();
		return appendPlainText(text.getSubString(i + 1));
	}

	m_source.append(text);
	return m_text.append(text);
}

size_t
HyperText::appendChar(
	utf32_t c,
	size_t count
) {
	if (c == 0x15) { // Ctrl+U -- clear
		clear();
		return 0;
	}

	m_source.append(c, count);
	return m_text.append(c, count);
}

size_t
HyperText::appendHyperText(
	const TextAttr& baseAttr,
	const sl::StringRef& text
) {
	size_t i = text.reverseFind('\x15'); // Ctrl+U -- clear
	if (i != -1) {
		clear();
		return appendHyperText(baseAttr, text.getSubString(i + 1));
	}

	TextAttr attr = baseAttr;
	AnsiAttrParser attrParser;

	size_t lastLength = m_text.getLength();

	const char* p = text.cp();
	const char* end = text.getEnd();

	m_source.append(text);

	for (;;) {
		const char* token = p;
		while (token < end && *token != 0x1b)
			token++;

		size_t textLength = m_text.append(p, token - p);
		p = token + 1;

		if (p + 1 >= end)
			break;

		if (*p == '^') { // hyperlink
			p++;

			const char* arg = p;
			while (p < end && *p != 0x1b)
				p++;

			if (p + 1 >= end)
				break;

			const char* argEnd = p;

			if (p[1] == '\\') // ST: string terminator
				p += 2;

			m_hyperlinkArray.openHyperlink(textLength, sl::StringRef(arg, argEnd - arg));
		} else if (*p == '[') { // CSI
			p++;

			const char* arg = p;
			while (p < end && !isalpha(*p))
				p++;

			const char* argEnd = p;
			if (p >= end)
				break;

			switch (*p) {
			case 'm':
				m_hyperlinkArray.closeHyperlink(textLength);
				m_attrArray.setAttr(lastLength, textLength, attr);
				lastLength = textLength;
				attrParser.parse(
					&attr,
					baseAttr,
					sl::StringRef(arg, argEnd - arg)
				);
				break;

			case 'D':
				backspace(argEnd == arg ? 1 : atoi(arg));
				break;

			case 'K':
			case 'J':
				clear();
				break;
			}

			p++;
		}
	}

	m_attrArray.setAttr(lastLength, m_text.getLength(), attr);
	return m_text.getLength();
}

const HyperlinkAnchor*
HyperText::findHyperlinkByX(int x) const {
	const HyperlinkAnchor* result = NULL;

	size_t begin = 0;
	size_t end = m_hyperlinkXMap.getCount();

	while (begin < end) {
		size_t mid = (begin + end) / 2;

		const HyperlinkXMapEntry* mapEntry = &m_hyperlinkXMap[mid];
		if (mapEntry->m_x == x) {
			result = mapEntry->m_anchor;
			break;
		}

		if (mapEntry->m_x < x) {
			result = mapEntry->m_anchor;
			begin = mid + 1;
		} else {
			end = mid;
		}
	}

	return result && !result->m_hyperlink.isEmpty() ? result : NULL;
}

const HyperlinkAnchor*
HyperText::findHyperlinkByOffset(size_t offset) const {
	if (offset >= m_text.getLength())
		return NULL;

	const HyperlinkAnchor* anchor = m_hyperlinkArray.find(offset);
	return anchor && !anchor->m_hyperlink.isEmpty() ? anchor : NULL;
}

void
HyperText::calcHyperlinkXMap(Widget* widget) {
	Engine* engine = widget->m_widgetDriver.getEngine();
	Font* baseFont = widget->m_widgetDriver.getFont();

	int x = 0;
	size_t offset = 0;
	size_t attrCount = m_attrArray.getCount();
	size_t hyperlinkCount = m_hyperlinkArray.getCount();

	const TextAttrAnchor* attrAnchor = m_attrArray;
	const TextAttrAnchor* attrEnd = attrAnchor + attrCount;
	HyperlinkXMapEntry* hyperlinkXMapEntry;

	uint_t fontFlags = 0;
	Font* font = baseFont->getFontMod(fontFlags);

	m_hyperlinkXMap.setCount(hyperlinkCount);
	hyperlinkXMapEntry = m_hyperlinkXMap;

	sl::ConstIterator<HyperlinkAnchor> it = m_hyperlinkArray.getHead();
	for (; it; it++) {
		const HyperlinkAnchor* hyperlinkAnchor = *it;
		Size size;

		for (; attrAnchor < attrEnd && attrAnchor->m_offset < hyperlinkAnchor->m_offset; attrAnchor++) {
			if (attrAnchor->m_attr.m_fontFlags == fontFlags)
				continue;

			size = engine->calcTextSize_utf32(
				font,
				widget,
				m_text.getSubString(offset, attrAnchor->m_offset - offset)
			);

			x += size.m_width;
			offset = attrAnchor->m_offset;

			fontFlags = attrAnchor->m_attr.m_fontFlags;
			font = baseFont->getFontMod(fontFlags);
		}

		size = engine->calcTextSize_utf32(
			font,
			widget,
			m_text.getSubString(offset, hyperlinkAnchor->m_offset - offset)
		);

		x += size.m_width;
		offset = hyperlinkAnchor->m_offset;

		hyperlinkXMapEntry->m_x = x;
		hyperlinkXMapEntry->m_anchor = hyperlinkAnchor;

		hyperlinkAnchor++;
		hyperlinkXMapEntry++;
	}
}

Size
HyperText::calcTextSize(Widget* widget) const {
	Engine* engine = widget->m_widgetDriver.getEngine();
	Font* baseFont = widget->m_widgetDriver.getFont();

	Size size;

	int x = 0;
	size_t offset = 0;
	size_t length = m_text.getLength();
	size_t attrCount = m_attrArray.getCount();

	uint_t fontFlags = 0;
	Font* font = baseFont->getFontMod(fontFlags);

	for (size_t i = 0; i < attrCount; i++) {
		const TextAttrAnchor* attrAnchor = &m_attrArray[i];

		if (attrAnchor->m_attr.m_fontFlags == fontFlags)
			continue;

		size = engine->calcTextSize_utf32(
			font,
			widget,
			m_text.getSubString(offset, attrAnchor->m_offset - offset)
		);

		x += size.m_width;
		offset = attrAnchor->m_offset;

		fontFlags = attrAnchor->m_attr.m_fontFlags;
		font = baseFont->getFontMod(fontFlags);
	}

	size = engine->calcTextSize_utf32(
		font,
		widget,
		m_text.getSubString(offset, length - offset)
	);

	size.m_width += x;

	return size;
}

//..............................................................................

} // namespace gui
} // namespace axl
