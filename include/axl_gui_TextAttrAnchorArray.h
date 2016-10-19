// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_TEXTATTRANCHORARRAY_H

#include "axl_gui_TextAttr.h"

namespace axl {
namespace gui {

//..............................................................................

struct TextAttrAnchor
{
	size_t m_offset;
	TextAttr m_attr;

	TextAttrAnchor ()
	{
		m_offset = 0;
	}

	TextAttrAnchor (
		size_t offset,
		const TextAttr& attr
		)
	{
		m_offset = offset;
		m_attr = attr;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class TextAttrAnchorArray
{
protected:
	sl::Array <TextAttrAnchor> m_array;

public:
	operator const TextAttrAnchor* () const
	{
		return m_array;
	}

	bool
	isEmpty () const
	{
		return m_array.isEmpty ();
	}

	size_t
	getCount () const
	{
		return m_array.getCount ();
	}

	const TextAttrAnchor*
	cp () const
	{
		return m_array.cp ();
	}

	void
	clear ()
	{
		m_array.clear ();
	}

	void
	clearBefore (size_t offset);

	void
	copy (
		const TextAttrAnchor* attrAnchorArray,
		size_t attrAnchorCount
		)
	{
		m_array.copy (attrAnchorArray, attrAnchorCount);
	}

	TextAttr
	getAttr (size_t offset) const
	{
		size_t anchor = findAnchor (offset);
		return anchor == -1 ? m_array [anchor].m_attr : TextAttr ();
	}

	void
	setAttr (
		size_t beginOffset,
		size_t endOffset,
		const TextAttr& attr
		);

#ifdef _AXL_DEBUG
	void
	trace ();
#endif

protected:
	size_t
	findAnchor (size_t offset) const;

	size_t
	getStartAnchor (size_t offset);

	size_t
	getEndAnchor (size_t offset);

	void
	normalize (
		size_t start,
		size_t end
		);
};

//..............................................................................

} // namespace gui
} // namespace axl
