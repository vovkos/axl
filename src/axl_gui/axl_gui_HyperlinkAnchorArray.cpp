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

#include "pch.h"
#include "axl_gui_HyperlinkAnchorArray.h"

namespace axl {
namespace gui {

//..............................................................................

const HyperlinkAnchor*
HyperlinkAnchorArray::find(size_t offset) const {
	const HyperlinkAnchor* result = NULL;

	size_t begin = 0;
	size_t end = m_array.getCount();

	while (begin < end) {
		size_t mid = (begin + end) / 2;

		HyperlinkAnchor* anchor = m_array[mid];
		if (anchor->m_offset == offset)
			return anchor;

		if (anchor->m_offset < offset) {
			result = anchor;
			begin = mid + 1;
		} else {
			end = mid;
		}
	}

	return result;
}

HyperlinkAnchor*
HyperlinkAnchorArray::openHyperlink(
	size_t offset,
	const sl::StringRef& hyperlink
) {
	HyperlinkAnchor* anchor;

	size_t count = m_array.getCount();
	if (count) {
		anchor = m_array[count - 1];

		if (anchor->m_hyperlink == hyperlink)
			return anchor; // same attr

		if (anchor->m_offset == offset) {
			if (count >= 2) {
				HyperlinkAnchor* prevAnchor = m_array[count - 2];
				if (prevAnchor->m_hyperlink == hyperlink) {
					// remove last anchor to normalize array

					m_list.removeTail();
					m_array.pop();

					AXL_MEM_DELETE(anchor);

					return prevAnchor;
				}
			}

			// modify last anchor

			anchor->m_hyperlink = hyperlink;
			return anchor;
		}
	}

	// create new anchor

	anchor = AXL_MEM_NEW(HyperlinkAnchor);
	anchor->m_offset = offset;
	anchor->m_hyperlink = hyperlink;
	m_list.insertTail(anchor);
	m_array.append(anchor);

	return anchor;
}

HyperlinkAnchor*
HyperlinkAnchorArray::closeHyperlink(
	size_t offset,
	bool closeEmpty
) {
	if (!isHyperlinkOpened())
		return NULL;

	if (!closeEmpty && m_list.getTail()->m_offset == offset)
		return NULL;

	return openHyperlink(offset, NULL);
}


//..............................................................................

} // namespace gui
} // namespace axl
