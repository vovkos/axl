// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_HYPERTEXT_H

#include "axl_gui_HyperlinkAnchorArray.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_gui_Font.h"

namespace axl {
namespace gui {

//.............................................................................

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
	sl::String
	getSource () const 
	{
		return m_source;
	}

	sl::String
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
		const char* text, 
		size_t length = -1
		)
	{
		clear ();
		return appendHyperText (baseAttr, text, length);
	}

	size_t 
	setHyperText (
		const char* text, 
		size_t length = -1
		)
	{
		return setHyperText (TextAttr (), text, length);
	}

	size_t 
	setPlainText (
		const char* text, 
		size_t length = -1
		)
	{
		clear ();
		return appendPlainText (text, length);
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
		const char* text, 
		size_t length = -1
		);

	size_t 
	appendHyperText (
		const char* text, 
		size_t length = -1
		)
	{
		return appendHyperText (TextAttr (), text, length);
	}

	size_t 
	appendPlainText (
		const char* text, 
		size_t length = -1
		);

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

//.............................................................................

} // namespace gui
} // namespace axl
