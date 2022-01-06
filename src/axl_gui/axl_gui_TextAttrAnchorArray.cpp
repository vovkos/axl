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
#include "axl_gui_TextAttrAnchorArray.h"

namespace axl {
namespace gui {

//..............................................................................

#ifdef _AXL_DEBUG
void
TextAttrAnchorArray::print(FILE* file) {
	size_t count = getCount();
	fprintf(file, "--- TextAttrAnchorArray {%d}---\n", count);

	for (size_t i = 0; i < count; i++) {
		const TextAttrAnchor* anchor = &(m_array[i]);
		fprintf(file, "[%d] ofs:%02x fg:%x bg:%x\n", i, anchor->m_offset, anchor->m_attr.m_foreColor, anchor->m_attr.m_backColor);
	}
}
#endif

size_t
TextAttrAnchorArray::findAnchor(size_t offset) const {
	size_t index = -1;
	size_t count = m_array.getCount();
	size_t begin = 0;
	size_t end = count;

	while (begin < end) {
		size_t mid = (begin + end) / 2;
		const TextAttrAnchor* anchor = &m_array[mid];

		ASSERT(mid < count);

		if (anchor->m_offset == offset)
			return mid;

		if (anchor->m_offset < offset) {
			index = mid;
			begin = mid + 1;
		} else {
			end = mid;
		}
	}

	return index;
}

size_t
TextAttrAnchorArray::getAnchor(size_t offset) {
	size_t index = findAnchor(offset);
	if (index == -1) {
		m_array.insert(0, TextAttrAnchor(offset, TextAttr()));
		return 0;
	}

	TextAttrAnchor* anchor = &m_array[index];
	if (anchor->m_offset == offset)
		return index;

	TextAttr lastAttr = anchor->m_attr;
	index++;

	m_array.insert(index, TextAttrAnchor(offset, lastAttr));
	return index;
}

void
TextAttrAnchorArray::normalize(
	size_t start,
	size_t end
) {
	size_t removeIndex = -1;
	size_t removeCount = 0;

	TextAttrAnchor lastAnchor;

	if (start)
		lastAnchor = m_array[start - 1];

	for (size_t i = start; i <= end; i++) {
		TextAttrAnchor* anchor = &m_array[i];

		if (anchor->m_attr == lastAnchor.m_attr) {
			if (!removeCount)
				removeIndex = i;

			removeCount++;
		} else {
			lastAnchor = *anchor;

			if (removeCount) {
				m_array.remove(removeIndex, removeCount);
				i -= removeCount;
				end -= removeCount;
				removeCount = 0;
			}
		}
	}

	if (removeCount)
		m_array.remove(removeIndex, removeCount);
}

void
TextAttrAnchorArray::clearBefore(size_t offset) {
	size_t anchor = findAnchor(offset);
	if (anchor != -1)
		m_array.remove(0, anchor + 1);
}

void
TextAttrAnchorArray::overlayImpl(
	const TextAttrAnchor* anchor,
	size_t count
) {
	size_t oldCount = m_array.getCount();
	ASSERT(count && oldCount);

	sl::Array<TextAttrAnchor> array;
	sl::takeOver(&array, &m_array);

	const TextAttrAnchor* oldAnchor = array;
	const TextAttrAnchor* oldEnd = array + oldCount;
	const TextAttrAnchor* newAnchor = anchor;
	const TextAttrAnchor* newEnd = anchor + count;

	TextAttr oldAttr;
	TextAttr newAttr;

	while (oldAnchor < oldEnd && newAnchor < newEnd) {
		if (oldAnchor->m_offset < newAnchor->m_offset) {
			TextAttr attr = TextAttr::getOverlayAttr(oldAnchor->m_attr, newAttr);
			m_array.append(TextAttrAnchor(oldAnchor->m_offset, attr));
			oldAttr = oldAnchor->m_attr;
			oldAnchor++;
		} else {
			TextAttr attr = TextAttr::getOverlayAttr(oldAttr, newAnchor->m_attr);
			m_array.append(TextAttrAnchor(newAnchor->m_offset, attr));
			newAttr = newAnchor->m_attr;
			newAnchor++;
		}
	}

	while (oldAnchor < oldEnd) {
		TextAttr attr = TextAttr::getOverlayAttr(oldAnchor->m_attr, newAttr);
		m_array.append(TextAttrAnchor(oldAnchor->m_offset, attr));
		oldAttr = oldAnchor->m_attr;
		oldAnchor++;
	}

	while (newAnchor < newEnd) {
		TextAttr attr = TextAttr::getOverlayAttr(oldAttr, newAnchor->m_attr);
		m_array.append(TextAttrAnchor(newAnchor->m_offset, attr));
		newAttr = newAnchor->m_attr;
		newAnchor++;
	}
}

void
TextAttrAnchorArray::setAttr(
	size_t beginOffset,
	size_t endOffset,
	const TextAttr& attr
) {
	if (beginOffset >= endOffset)
		return;

	m_array.ensureExclusive();

	// important: END anchor first! otherwise the first anchor could "bleed" to the right

	size_t endIdx = getAnchor(endOffset);
	size_t count = m_array.getCount();
	size_t startIdx = getAnchor(beginOffset);

	// detect insertion

	if (startIdx <= endIdx && m_array.getCount() > count)
		endIdx++;

	for (size_t i = startIdx; i < endIdx; i++)
		m_array[i].m_attr.overlay(attr);

	normalize(startIdx, endIdx);
}

void
TextAttrAnchorArray::setTailAttr(
	size_t offset,
	const TextAttr& attr
) {
	if (m_array.isEmpty()) {
		m_array.append(TextAttrAnchor(offset, attr));
		return;
	}

	TextAttrAnchor* tail = &m_array.getBack();
	ASSERT(offset >= tail->m_offset);
	if (attr != tail->m_attr)
		if (offset == tail->m_offset)
			tail->m_attr = attr;
		else
			m_array.append(TextAttrAnchor(offset, attr));
}

//..............................................................................

} // namespace gui
} // namespace axl
