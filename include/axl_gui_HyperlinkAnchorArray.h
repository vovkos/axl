// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	size_t m_Offset;
	rtl::CString m_Hyperlink;

	THyperlinkAnchor ()
	{
		m_Offset = 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CHyperlinkAnchorArray
{
protected:
	rtl::CStdListT <THyperlinkAnchor> m_List;
	rtl::CArrayT <THyperlinkAnchor*> m_Array;

public:
	bool
	IsEmpty () const
	{
		return m_List.IsEmpty ();
	}

	size_t 
	GetCount () const
	{
		return m_List.GetCount ();
	}

	rtl::CIteratorT <THyperlinkAnchor> 
	GetHead () const
	{
		return m_List.GetHead ();
	}

	rtl::CIteratorT <THyperlinkAnchor> 
	GetTail () const
	{
		return m_List.GetHead ();
	}

	void 
	Clear ()
	{ 
		m_List.Clear ();
		m_Array.Clear ();	
	}

	THyperlinkAnchor* 
	Find (size_t Offset) const;

	THyperlinkAnchor* 
	OpenHyperlink (
		size_t Offset, 
		const char* pHyperlink, 
		size_t Length = -1
		);

	THyperlinkAnchor* 
	CloseHyperlink (size_t Offset)
	{
		return OpenHyperlink (Offset, NULL, 0);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
