// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
		THyperlinkAnchor* m_pAnchor;
	};

protected:
	rtl::CString m_Text;
	CTextAttrAnchorArray m_AttrArray;
	CHyperlinkAnchorArray m_HyperlinkArray;
	rtl::CArrayT <THyperlinkXMapEntry> m_HyperlinkXMap;

public:
	rtl::CString
	GetText () const 
	{
		return m_Text;
	}

	const CTextAttrAnchorArray*
	GetAttrArray () const 
	{
		return &m_AttrArray;
	}

	void 
	Clear ();

	size_t 
	SetHyperText (
		const TTextAttr& BaseAttr,
		const char* pText, 
		size_t Length = -1
		)
	{
		Clear ();
		return AppendHyperText (BaseAttr, pText, Length);
	}

	size_t 
	SetHyperText (
		const char* pText, 
		size_t Length = -1
		)
	{
		return SetHyperText (TTextAttr (), pText, Length);
	}

	size_t 
	SetPlainText (
		const char* pText, 
		size_t Length = -1
		)
	{
		return m_Text.Copy (pText, Length);
	}

	size_t 
	SetPlainText (
		char Char, 
		size_t Count = 1
		)
	{
		return m_Text.Copy (Char, Count);
	}

	size_t 
	AppendHyperText (
		const TTextAttr& BaseAttr,
		const char* pText, 
		size_t Length = -1
		);

	size_t 
	AppendHyperText (
		const char* pText, 
		size_t Length = -1
		)
	{
		return AppendHyperText (TTextAttr (), pText, Length);
	}

	size_t 
	AppendPlainText (
		const char* pText, 
		size_t Length = -1
		)
	{
		return m_Text.Append (pText, Length);
	}

	size_t 
	AppendPlainText (
		char Char, 
		size_t Count = 1
		)
	{
		return m_Text.Append (Char, Count);
	}

	size_t
	Backspace (size_t BackLength);

	TSize
	CalcTextSize (IFont* pFont) const;

	void 
	CalcHyperlinkXMap (IFont* pFont);

	THyperlinkAnchor* 
	FindHyperlinkByX (int x) const ;

	THyperlinkAnchor* 
	FindHyperlinkByOffset (size_t Offset) const 
	{
		return m_HyperlinkArray.Find (Offset);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
