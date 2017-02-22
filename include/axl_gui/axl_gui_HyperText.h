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

#pragma once

#define _AXL_GUI_HYPERTEXT_H

#include "axl_gui_HyperlinkAnchorArray.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_gui_Font.h"

namespace axl {
namespace gui {

//..............................................................................

class HyperText
{
protected:
	struct HyperlinkXMapEntry
	{
		int m_x;
		HyperlinkAnchor* m_anchor;
	};

protected:
	sl::String m_source;
	sl::String m_text;
	TextAttrAnchorArray m_attrArray;
	HyperlinkAnchorArray m_hyperlinkArray;
	sl::Array <HyperlinkXMapEntry> m_hyperlinkXMap;

public:
	const sl::String&
	getSource () const
	{
		return m_source;
	}

	const sl::String&
	getText () const
	{
		return m_text;
	}

	const TextAttrAnchorArray*
	getAttrArray () const
	{
		return &m_attrArray;
	}

	void
	clear ();

	size_t
	setHyperText (
		const TextAttr& baseAttr,
		const sl::StringRef& text
		)
	{
		clear ();
		return appendHyperText (baseAttr, text);
	}

	size_t
	setHyperText (const sl::StringRef& text)
	{
		return setHyperText (TextAttr (), text);
	}

	size_t
	setPlainText (const sl::StringRef& text)
	{
		clear ();
		return appendPlainText (text);
	}

	size_t
	setPlainText (
		char c,
		size_t count = 1
		)
	{
		clear ();
		return appendPlainText (c, count);
	}

	size_t
	appendHyperText (
		const TextAttr& baseAttr,
		const sl::StringRef& text
		);

	size_t
	appendHyperText (const sl::StringRef& text)
	{
		return appendHyperText (TextAttr (), text);
	}

	size_t
	appendPlainText (const sl::StringRef& text);

	size_t
	appendPlainText (
		char c,
		size_t count = 1
		);

	size_t
	backspace (size_t backLength);

	Size
	calcTextSize (Font* font) const;

	void
	calcHyperlinkXMap (Font* font);

	HyperlinkAnchor*
	findHyperlinkByX (int x) const;

	HyperlinkAnchor*
	findHyperlinkByOffset (size_t offset) const
	{
		HyperlinkAnchor* anchor = m_hyperlinkArray.find (offset);
		return anchor && !anchor->m_hyperlink.isEmpty () ? anchor : NULL;
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
