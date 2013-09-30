// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_TEXTATTRANCHORARRAY_H

#include "axl_gui_Font.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {
	
//.............................................................................

struct TTextAttrAnchor
{
	size_t m_Offset;
	size_t m_Metric; // attributes with higher metric overlay lower metric
	TTextAttr m_Attr;

	TTextAttrAnchor ()
	{
		m_Offset = 0;
		m_Metric = 0;
	}

	TTextAttrAnchor (
		size_t Offset,
		const TTextAttr& Attr,
		size_t Metric = 0
		)
	{
		m_Offset = Offset;
		m_Attr = Attr;
		m_Metric = Metric;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CTextAttrAnchorArray
{
protected:
	rtl::CArrayT <TTextAttrAnchor> m_Array;

public:
	operator const TTextAttrAnchor* () const
	{
		return m_Array;
	}

	bool
	IsEmpty () const
	{
		return m_Array.IsEmpty ();
	}

	size_t 
	GetCount () const
	{
		return m_Array.GetCount ();
	}

	const TTextAttrAnchor*
	ca () const
	{
		return m_Array.ca ();
	}

	void 
	Clear ()
	{
		m_Array.Clear ();
	}

	void
	Copy (
		const TTextAttrAnchor* pAttrAnchorArray,
		size_t AttrAnchorCount
		)
	{
		m_Array.Copy (pAttrAnchorArray, AttrAnchorCount);
	}

	void 
	SetAttr (
		size_t BeginOffset, 
		size_t EndOffset, 
		const TTextAttr& Attr, 
		size_t Metric = 0
		);

protected:
	size_t
	FindAnchor (size_t Offset);

	size_t
	GetStartAnchor (
		size_t Offset,
		size_t Metric
		);

	size_t
	GetEndAnchor (
		size_t Offset,
		size_t Metric
		);

	void
	Normalize (
		size_t Start,
		size_t End
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
