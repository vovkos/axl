#include "pch.h"
#include "axl_gui_TextAttrAnchorArray.h"

namespace axl {
namespace gui {

//.............................................................................


#ifdef _AXL_DEBUG

void
TextAttrAnchorArray::trace ()
{
	size_t count = getCount ();
	dbg::trace ("--- CTextAttrAnchorArray {%d}---\n", count);

	for (size_t i = 0; i < count; i++)
	{
		const TextAttrAnchor* anchor = &(m_array [i]);
		dbg::trace ("[%d] ofs:%02x fc:%x\n", i, anchor->m_offset, anchor->m_attr.m_foreColor);
	}
}

#endif

size_t
TextAttrAnchorArray::findAnchor (size_t offset) const
{
	size_t index = -1;
	size_t count = m_array.getCount ();
	size_t begin = 0;
	size_t end = count;

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;
		const TextAttrAnchor* anchor = &m_array [mid];

		ASSERT (mid < count);
			
		if (anchor->m_offset == offset)
			return mid;

		if (anchor->m_offset < offset)
		{
			index = mid;
			begin = mid + 1;
		}
		else
		{
			end = mid;
		}
	}

	return index;
}

size_t
TextAttrAnchorArray::getStartAnchor (size_t offset)
{
	size_t index = findAnchor (offset);
	if (index == -1)
	{
		m_array.insert (0, TextAttrAnchor (offset, TextAttr ()));
		return 0;
	}

	TextAttrAnchor* anchor = &m_array [index];
	if (anchor->m_offset == offset)
		return index;

	TextAttr lastAttr = anchor->m_attr;
	index++;

	m_array.insert (index, TextAttrAnchor (offset, lastAttr));
	return index;
}

size_t
TextAttrAnchorArray::getEndAnchor (size_t offset)
{
	size_t index = findAnchor (offset);
	if (index == -1)
	{
		m_array.insert (0, TextAttrAnchor (offset, TextAttr ()));
		return 0;
	}

	TextAttrAnchor* anchor = &m_array [index];
	if (anchor->m_offset == offset)
		return index;

	TextAttr lastAttr = anchor->m_attr;
	index++;

	m_array.insert (index, TextAttrAnchor (offset, lastAttr));
	return index;
}

void
TextAttrAnchorArray::normalize (
	size_t start,
	size_t end
	)
{
	size_t removeIndex = -1;
	size_t removeCount = 0;
		
	TextAttrAnchor lastAnchor;

	if (start)
		lastAnchor = m_array [start - 1];

	for (size_t i = start; i <= end; i++)
	{
		TextAttrAnchor* anchor = &m_array [i];
		
		if (anchor->m_attr.cmp (lastAnchor.m_attr) == 0)
		{
			if (!removeCount)
				removeIndex = i;

			removeCount++;
		}
		else 
		{
			lastAnchor = *anchor;

			if (removeCount)
			{
				m_array.remove (removeIndex, removeCount);
				i -= removeCount;
				end -= removeCount;
				removeCount = 0;
			}
		}
	}

	if (removeCount)
		m_array.remove (removeIndex, removeCount);
}

void 
TextAttrAnchorArray::clearBefore (size_t offset)
{
	size_t anchor = findAnchor (offset);
	if (anchor != -1)
		m_array.remove (0, anchor + 1);
}

void
TextAttrAnchorArray::setAttr (
	size_t beginOffset, 
	size_t endOffset, 
	const TextAttr& attr
	)
{
	if (beginOffset >= endOffset)
		return;

	m_array.ensureExclusive ();

	// important: END anchor first! otherwise the first anchor could "bleed" to the right

	size_t endIdx = getEndAnchor (endOffset);

	size_t oldCount = m_array.getCount ();
	size_t startIdx = getStartAnchor (beginOffset);

	// detect insertion

	if (startIdx <= endIdx && m_array.getCount () > oldCount)
		endIdx++;

	for (size_t i = startIdx; i < endIdx; i++)
		m_array [i].m_attr.overlay (attr);

	normalize (startIdx, endIdx);
}

//.............................................................................

} // namespace gui
} // namespace axl
