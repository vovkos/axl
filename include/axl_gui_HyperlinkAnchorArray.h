// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_HYPERLINKANCHORARRAY_H

#include "axl_rtl_String.h"
#include "axl_rtl_Array.h"
#include "axl_rtl_List.h"

namespace axl {
namespace gui {
	
//.............................................................................

struct HyperlinkAnchor: rtl::ListLink
{
	size_t m_offset;
	rtl::String m_hyperlink;

	HyperlinkAnchor ()
	{
		m_offset = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HyperlinkAnchorArray
{
protected:
	rtl::StdList <HyperlinkAnchor> m_list;
	rtl::Array <HyperlinkAnchor*> m_array;

public:
	bool
	isEmpty () const
	{
		return m_list.isEmpty ();
	}

	size_t 
	getCount () const
	{
		return m_list.getCount ();
	}

	rtl::Iterator <HyperlinkAnchor> 
	getHead () const
	{
		return m_list.getHead ();
	}

	rtl::Iterator <HyperlinkAnchor> 
	getTail () const
	{
		return m_list.getHead ();
	}

	void 
	clear ()
	{ 
		m_list.clear ();
		m_array.clear ();	
	}

	HyperlinkAnchor* 
	find (size_t offset) const;

	HyperlinkAnchor* 
	openHyperlink (
		size_t offset, 
		const char* hyperlink, 
		size_t length = -1
		);

	HyperlinkAnchor* 
	closeHyperlink (
		size_t offset,
		bool closeEmpty = false
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
