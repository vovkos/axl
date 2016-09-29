// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_HYPERLINKANCHORARRAY_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {
	
//.............................................................................

struct HyperlinkAnchor: sl::ListLink
{
	size_t m_offset;
	sl::String m_hyperlink;

	HyperlinkAnchor ()
	{
		m_offset = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HyperlinkAnchorArray
{
protected:
	sl::StdList <HyperlinkAnchor> m_list;
	sl::Array <HyperlinkAnchor*> m_array;

public:
	bool
	isEmpty () const
	{
		return m_list.isEmpty ();
	}

	bool
	isHyperlinkOpened () const
	{
		return !m_list.isEmpty () && !m_list.getTail ()->m_hyperlink.isEmpty ();
	}

	size_t 
	getCount () const
	{
		return m_list.getCount ();
	}

	sl::Iterator <HyperlinkAnchor> 
	getHead () const
	{
		return m_list.getHead ();
	}

	sl::Iterator <HyperlinkAnchor> 
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
