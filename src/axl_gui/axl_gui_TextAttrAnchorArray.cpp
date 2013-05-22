#include "pch.h"
#include "axl_gui_TextAttrAnchorArray.h"

namespace axl {
namespace gui {
	
//.............................................................................

size_t
CTextAttrAnchorArray::FindAnchor (size_t Offset)
{
	size_t Index = -1;
	size_t Count = m_Array.GetCount ();
	size_t Begin = 0;
	size_t End = Count;

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;
		TTextAttrAnchor* pAnchor = &m_Array [Mid];

		ASSERT (Mid < Count);
			
		if (pAnchor->m_Offset == Offset)
			return Mid;

		if (pAnchor->m_Offset < Offset)
		{
			Index = Mid;
			Begin = Mid + 1;
		}
		else
		{
			End = Mid;
		}
	}

	return Index;
}

size_t
CTextAttrAnchorArray::GetStartAnchor (
	size_t Offset,
	size_t Metric
	)
{
	size_t Index = FindAnchor (Offset);
	if (Index == -1)
	{
		m_Array.Insert (0, TTextAttrAnchor (Offset, TTextAttr (), Metric));
		return 0;
	}

	TTextAttrAnchor* pAnchor = &m_Array [Index];
	if (pAnchor->m_Metric > Metric || pAnchor->m_Offset == Offset)
		return Index;

	TTextAttr LastAttr = pAnchor->m_Attr;
	Index++;

	m_Array.Insert (Index, TTextAttrAnchor (Offset, LastAttr, Metric));
	return Index;
}

size_t
CTextAttrAnchorArray::GetEndAnchor (
	size_t Offset,
	size_t Metric
	)
{
	size_t Index = FindAnchor (Offset);
	if (Index == -1)
	{
		m_Array.Insert (0, TTextAttrAnchor (Offset, TTextAttr (), 0));
		return 0;
	}

	TTextAttrAnchor* pAnchor = &m_Array [Index];
	if (pAnchor->m_Metric > Metric || pAnchor->m_Offset == Offset)
		return Index;

	TTextAttr LastAttr = pAnchor->m_Attr;
	size_t LastMetric = pAnchor->m_Metric;
	Index++;

	m_Array.Insert (Index, TTextAttrAnchor (Offset, LastAttr, LastMetric));
	return Index;
}

void
CTextAttrAnchorArray::Normalize (
	size_t Start,
	size_t End
	)
{
	size_t RemoveIndex = -1;
	size_t RemoveCount = 0;
		
	TTextAttrAnchor LastAnchor;

	if (Start)
		LastAnchor = m_Array [Start - 1];

	for (size_t i = Start; i <= End; i++)
	{
		TTextAttrAnchor* pAnchor = &m_Array [i];
		
		if (pAnchor->m_Metric >= LastAnchor.m_Metric && pAnchor->m_Attr.Cmp (LastAnchor.m_Attr) == 0)
		{
			if (!RemoveCount)
				RemoveIndex = i;

			RemoveCount++;
		}
		else 
		{
			LastAnchor = *pAnchor;

			if (RemoveCount)
			{
				m_Array.Remove (RemoveIndex, RemoveCount);
				i -= RemoveCount;
				End -= RemoveCount;
				RemoveCount = 0;
			}
		}
	}

	if (RemoveCount)
		m_Array.Remove (RemoveIndex, RemoveCount);
}

void
CTextAttrAnchorArray::SetAttr (
	const TTextAttr& Attr,
	size_t Begin,
	size_t End,
	size_t Metric
	)
{
	if (Begin >= End)
		return;

	m_Array.EnsureExclusive ();

	// important: END anchor first! otherwise the first anchor could "bleed" metric to the right

	size_t EndIdx = GetEndAnchor (End, Metric);

	size_t OldCount = m_Array.GetCount ();
	size_t StartIdx = GetStartAnchor (Begin, Metric);

	// detect insertion

	if (StartIdx <= EndIdx && m_Array.GetCount () > OldCount)
		EndIdx++;

	for (size_t i = StartIdx; i < EndIdx; i++)
	{
		TTextAttrAnchor* pAnchor = &m_Array [i];
		if (pAnchor->m_Metric <= Metric)
		{
			pAnchor->m_Metric = Metric;
			pAnchor->m_Attr.Overlay (Attr);
		}
	}

	Normalize (StartIdx, EndIdx);
}

//.............................................................................

} // namespace gui
} // namespace axl
