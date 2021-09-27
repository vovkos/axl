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

#define _AXL_GUI_HYPERLINKANCHORARRAY_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//..............................................................................

struct HyperlinkAnchor: sl::ListLink {
	size_t m_offset;
	sl::String m_hyperlink;

	HyperlinkAnchor() {
		m_offset = 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HyperlinkAnchorArray {
protected:
	sl::List<HyperlinkAnchor> m_list;
	sl::Array<HyperlinkAnchor*> m_array;

public:
	bool
	isEmpty() const {
		return m_list.isEmpty();
	}

	bool
	isHyperlinkOpened() const {
		return !m_list.isEmpty() && !m_list.getTail()->m_hyperlink.isEmpty();
	}

	size_t
	getCount() const {
		return m_list.getCount();
	}

	sl::ConstIterator<HyperlinkAnchor>
	getHead() const {
		return m_list.getHead();
	}

	sl::ConstIterator<HyperlinkAnchor>
	getTail() const {
		return m_list.getHead();
	}

	void
	clear() {
		m_list.clear();
		m_array.clear();
	}

	const HyperlinkAnchor*
	find(size_t offset) const;

	HyperlinkAnchor*
	openHyperlink(
		size_t offset,
		const sl::StringRef& hyperlink
	);

	HyperlinkAnchor*
	closeHyperlink(
		size_t offset,
		bool closeEmpty = false
	);
};

//..............................................................................

} // namespace gui
} // namespace axl
