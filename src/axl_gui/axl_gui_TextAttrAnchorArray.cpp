#include "pch.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace gui {
	
//.............................................................................

size_t
CTextAttrAnchorArray::findAnchor (size_t offset) const
{
	size_t index = -1;
	size_t count = m_array.getCount ();
	size_t begin = 0;
	size_t end = count;

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;
		const TTextAttrAnchor* anchor = &m_array [mid];

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
CTextAttrAnchorArray::getStartAnchor (
	size_t offset,
	size_t metric
	)
{
	size_t index = findAnchor (offset);
	if (index == -1)
	{
		m_array.insert (0, TTextAttrAnchor (offset, TTextAttr (), metric));
		return 0;
	}

	TTextAttrAnchor* anchor = &m_array [index];
	if (anchor->m_metric > metric || anchor->m_offset == offset)
		return index;

	TTextAttr lastAttr = anchor->m_attr;
	index++;

	m_array.insert (index, TTextAttrAnchor (offset, lastAttr, metric));
	return index;
}

size_t
CTextAttrAnchorArray::getEndAnchor (
	size_t offset,
	size_t metric
	)
{
	size_t index = findAnchor (offset);
	if (index == -1)
	{
		m_array.insert (0, TTextAttrAnchor (offset, TTextAttr (), 0));
		return 0;
	}

	TTextAttrAnchor* anchor = &m_array [index];
	if (anchor->m_metric > metric || anchor->m_offset == offset)
		return index;

	TTextAttr lastAttr = anchor->m_attr;
	size_t lastMetric = anchor->m_metric;
	index++;

	m_array.insert (index, TTextAttrAnchor (offset, lastAttr, lastMetric));
	return index;
}

void
CTextAttrAnchorArray::normalize (
	size_t start,
	size_t end
	)
{
	size_t removeIndex = -1;
	size_t removeCount = 0;
		
	TTextAttrAnchor lastAnchor;

	if (start)
		lastAnchor = m_array [start - 1];

	for (size_t i = start; i <= end; i++)
	{
		TTextAttrAnchor* anchor = &m_array [i];
		
		if (anchor->m_metric >= lastAnchor.m_metric && anchor->m_attr.cmp (lastAnchor.m_attr) == 0)
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
CTextAttrAnchorArray::clearBefore (size_t offset)
{
	size_t anchor = findAnchor (offset);
	if (anchor != -1)
		m_array.remove (0, anchor + 1);
}

void
traceAttrArray (const CTextAttrAnchorArray& array)
{
	size_t count = array.getCount ();
	dbg::trace ("--- CTextAttrAnchorArray {%d}---\n", count);

	for (size_t i = 0; i < count; i++)
	{
		const gui::TTextAttrAnchor* anchor = &(array [i]);
		dbg::trace ("[%d] ofs:%02x m:%d fc:%x\n", i, anchor->m_offset, anchor->m_metric, anchor->m_attr.m_foreColor);
	}
}

void
CTextAttrAnchorArray::setAttr (
	size_t beginOffset, 
	size_t endOffset, 
	const TTextAttr& attr,
	size_t metric
	)
{
	if (beginOffset >= endOffset)
		return;

	m_array.ensureExclusive ();

	// important: END anchor first! otherwise the first anchor could "bleed" metric to the right

	size_t endIdx = getEndAnchor (endOffset, metric);

	size_t oldCount = m_array.getCount ();
	size_t startIdx = getStartAnchor (beginOffset, metric);

	// detect insertion

	if (startIdx <= endIdx && m_array.getCount () > oldCount)
		endIdx++;

	for (size_t i = startIdx; i < endIdx; i++)
	{
		TTextAttrAnchor* anchor = &m_array [i];
		if (anchor->m_metric <= metric)
		{
			anchor->m_metric = metric;
			anchor->m_attr.overlay (attr);
		}
	}

	normalize (startIdx, endIdx);
	// TraceAttrArray (*this);
}

//.............................................................................

} // namespace gui
} // namespace axl
