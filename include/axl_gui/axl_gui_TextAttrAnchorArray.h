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

#define _AXL_GUI_TEXTATTRANCHORARRAY_H

#include "axl_gui_TextAttr.h"

namespace axl {
namespace gui {

//..............................................................................

struct TextAttrAnchor {
	size_t m_offset;
	TextAttr m_attr;

	TextAttrAnchor() {
		m_offset = 0;
	}

	TextAttrAnchor(
		size_t offset,
		const TextAttr& attr
	) {
		m_offset = offset;
		m_attr = attr;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class TextAttrAnchorArray {
protected:
	sl::Array<TextAttrAnchor> m_array;

public:
	operator const TextAttrAnchor* () const {
		return m_array;
	}

	bool
	isEmpty() const {
		return m_array.isEmpty();
	}

	size_t
	getCount() const {
		return m_array.getCount();
	}

	const TextAttrAnchor*
	cp() const {
		return m_array.cp();
	}

	void
	clear() {
		m_array.clear();
	}

	void
	clearBefore(size_t offset);

	void
	copy(
		const TextAttrAnchor* anchor,
		size_t count
	) {
		m_array.copy(anchor, count);
	}

	void
	copy(const TextAttrAnchorArray& attrAnchorArray) {
		m_array = attrAnchorArray.m_array;
	}

	void
	overlay(
		const TextAttrAnchor* anchor,
		size_t count
	);

	void
	overlay(const TextAttrAnchorArray& attrAnchorArray) {
		overlay(attrAnchorArray.m_array, attrAnchorArray.m_array.getCount());
	}

	TextAttr
	getAttr(size_t offset) const {
		size_t anchor = findAnchor(offset);
		return anchor == -1 ? m_array[anchor].m_attr : TextAttr();
	}

	void
	setAttr(
		size_t beginOffset,
		size_t endOffset,
		const TextAttr& attr
	);

	void
	setTailAttr(
		size_t offset,
		const TextAttr& attr
	);

#ifdef _AXL_DEBUG
	void
	print(FILE* file = stdout);
#endif

protected:
	size_t
	findAnchor(size_t offset) const;

	size_t
	getAnchor(size_t offset);

	void
	normalize(
		size_t start,
		size_t end
	);
};

//..............................................................................

} // namespace gui
} // namespace axl
