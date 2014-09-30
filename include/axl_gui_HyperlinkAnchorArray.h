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

struct THyperlinkAnchor: rtl::TListLink
{
	size_t m_offset;
	rtl::CString m_hyperlink;

	THyperlinkAnchor ()
	{
		m_offset = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CHyperlinkAnchorArray
{
protected:
	rtl::CStdListT <THyperlinkAnchor> m_list;
	rtl::CArrayT <THyperlinkAnchor*> m_array;

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

	rtl::CIteratorT <THyperlinkAnchor> 
	getHead () const
	{
		return m_list.getHead ();
	}

	rtl::CIteratorT <THyperlinkAnchor> 
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

	THyperlinkAnchor* 
	find (size_t offset) const;

	THyperlinkAnchor* 
	openHyperlink (
		size_t offset, 
		const char* hyperlink, 
		size_t length = -1
		);

	THyperlinkAnchor* 
	closeHyperlink (size_t offset)
	{
		return openHyperlink (offset, NULL, 0);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
