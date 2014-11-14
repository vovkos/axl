#include "pch.h"
#include "axl_gui_HyperlinkAnchorArray.h"

namespace axl {
namespace gui {
	
//.............................................................................

HyperlinkAnchor* 
HyperlinkAnchorArray::find (size_t offset) const
{
	HyperlinkAnchor* result = NULL;

	size_t begin = 0;
	size_t end = m_array.getCount ();

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;

		HyperlinkAnchor* anchor = m_array [mid];
		if (anchor->m_offset == offset)
			return anchor;

		if (anchor->m_offset < offset)
		{
			result = anchor;
			begin = mid + 1;
		}
		else
		{
			end = mid;
		}
	}

	return result;
}

HyperlinkAnchor* 
HyperlinkAnchorArray::openHyperlink (
	size_t offset,
	const char* hyperlink,
	size_t length
	)
{
	HyperlinkAnchor* anchor;

	size_t count = m_array.getCount ();
	if (count)
	{
		anchor = m_array [count - 1];
		
		if (anchor->m_hyperlink.cmp (hyperlink, length) == 0)
			return anchor; // same attr
		
		if (anchor->m_offset == offset)
		{
			if (count >= 2)
			{
				HyperlinkAnchor* prevAnchor = m_array [count - 2];
				if (prevAnchor->m_hyperlink.cmp (hyperlink, length) == 0)
				{
					// remove last anchor to normalize array

					m_list.removeTail ();
					m_array.pop ();

					AXL_MEM_DELETE (anchor);

					return prevAnchor; 
				}
			}

			// modify last anchor

			anchor->m_hyperlink.copy (hyperlink, length);
			return anchor;
		}
	}

	// create new anchor

	anchor = AXL_MEM_NEW (HyperlinkAnchor);
	anchor->m_offset = offset;
	anchor->m_hyperlink.copy (hyperlink, length);
	m_list.insertTail (anchor);
	m_array.append (anchor);

	return anchor;
}

HyperlinkAnchor* 
HyperlinkAnchorArray::closeHyperlink (
	size_t offset,
	bool closeEmpty
	)
{
	if (m_list.isEmpty ())		
		return NULL;

	HyperlinkAnchor* tail = *m_list.getTail ();
	if (tail->m_hyperlink.isEmpty ())
		return NULL;

	if (!closeEmpty && tail->m_offset == offset)
		return NULL;

	return openHyperlink (offset, NULL, 0);
}


//.............................................................................

} // namespace gui
} // namespace axl
