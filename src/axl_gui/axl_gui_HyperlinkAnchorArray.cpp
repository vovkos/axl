#include "pch.h"
#include "axl_gui_HyperlinkAnchorArray.h"

namespace axl {
namespace gui {
	
//.............................................................................

THyperlinkAnchor* 
CHyperlinkAnchorArray::find (size_t offset) const
{
	THyperlinkAnchor* result = NULL;

	size_t begin = 0;
	size_t end = m_array.getCount ();

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;

		THyperlinkAnchor* anchor = m_array [mid];
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

THyperlinkAnchor* 
CHyperlinkAnchorArray::openHyperlink (
	size_t offset,
	const char* hyperlink,
	size_t length
	)
{
	THyperlinkAnchor* anchor;

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
				THyperlinkAnchor* prevAnchor = m_array [count - 2];
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

	anchor = AXL_MEM_NEW (THyperlinkAnchor);
	anchor->m_offset = offset;
	anchor->m_hyperlink.copy (hyperlink, length);
	m_list.insertTail (anchor);
	m_array.append (anchor);

	return anchor;
}

//.............................................................................

} // namespace gui
} // namespace axl
