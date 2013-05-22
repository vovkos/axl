#include "pch.h"
#include "axl_gui_HyperlinkAnchorArray.h"

namespace axl {
namespace gui {
	
//.............................................................................

THyperlinkAnchor* 
CHyperlinkAnchorArray::Find (size_t Offset) const
{
	THyperlinkAnchor* pResult = NULL;

	size_t Begin = 0;
	size_t End = m_Array.GetCount ();

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;

		THyperlinkAnchor* pAnchor = m_Array [Mid];
		if (pAnchor->m_Offset == Offset)
			return pAnchor;

		if (pAnchor->m_Offset < Offset)
		{
			pResult = pAnchor;
			Begin = Mid + 1;
		}
		else
		{
			End = Mid;
		}
	}

	return pResult;
}

THyperlinkAnchor* 
CHyperlinkAnchorArray::OpenHyperlink (
	size_t Offset,
	const char* pHyperlink,
	size_t Length
	)
{
	THyperlinkAnchor* pAnchor;

	size_t Count = m_Array.GetCount ();
	if (Count)
	{
		pAnchor = m_Array [Count - 1];
		
		if (pAnchor->m_Hyperlink.Cmp (pHyperlink, Length) == 0)
			return pAnchor; // same attr
		
		if (pAnchor->m_Offset == Offset)
		{
			if (Count >= 2)
			{
				THyperlinkAnchor* pPrevAnchor = m_Array [Count - 2];
				if (pPrevAnchor->m_Hyperlink.Cmp (pHyperlink, Length) == 0)
				{
					// remove last anchor to normalize array

					m_List.RemoveTail ();
					m_Array.Pop ();

					AXL_MEM_DELETE (pAnchor);

					return pPrevAnchor; 
				}
			}

			// modify last anchor

			pAnchor->m_Hyperlink.Copy (pHyperlink, Length);
			return pAnchor;
		}
	}

	// create new anchor

	pAnchor = AXL_MEM_NEW (THyperlinkAnchor);
	pAnchor->m_Offset = Offset;
	pAnchor->m_Hyperlink.Copy (pHyperlink, Length);
	m_List.InsertTail (pAnchor);
	m_Array.Append (pAnchor);

	return pAnchor;
}

//.............................................................................

} // namespace gui
} // namespace axl
