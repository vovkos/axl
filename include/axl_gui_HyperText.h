// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_HYPERTEXT_H

#include "axl_gui_HyperlinkAnchorArray.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_gui_Font.h"

namespace axl {
namespace gui {

//.............................................................................

class CHyperText
{
protected:
	struct THyperlinkXMapEntry
	{
		int m_x;
		THyperlinkAnchor* m_anchor;
	};

protected:
	rtl::CString m_source;
	rtl::CString m_text;
	CTextAttrAnchorArray m_attrArray;
	CHyperlinkAnchorArray m_hyperlinkArray;
	rtl::CArrayT <THyperlinkXMapEntry> m_hyperlinkXMap;

public:
	rtl::CString
	getSource () const 
	{
		return m_source;
	}

	rtl::CString
	getText () const 
	{
		return m_text;
	}

	const CTextAttrAnchorArray*
	getAttrArray () const 
	{
		return &m_attrArray;
	}

	void 
	clear ();

	size_t 
	setHyperText (
		const TTextAttr& baseAttr,
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
		return setHyperText (TTextAttr (), text, length);
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
		const TTextAttr& baseAttr,
		const char* text, 
		size_t length = -1
		);

	size_t 
	appendHyperText (
		const char* text, 
		size_t length = -1
		)
	{
		return appendHyperText (TTextAttr (), text, length);
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

	TSize
	calcTextSize (CFont* font) const;

	void 
	calcHyperlinkXMap (CFont* font);

	THyperlinkAnchor* 
	findHyperlinkByX (int x) const ;

	THyperlinkAnchor* 
	findHyperlinkByOffset (size_t offset) const 
	{
		return m_hyperlinkArray.find (offset);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
