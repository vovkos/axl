#include "stdafx.h"
#include "axl_rtl_BuddyAllocMap.h"
#include "axl_rtl_BitIdx.h"
#include "axl_err_Error.h"

namespace axl {
namespace rtl {

//.............................................................................

void
CBuddyAllocMap::CLevel::Format (
	TPage* pPage,
	size_t Count
	)
{
	size_t i;

	m_AvailablePageList.Clear ();
	m_pFirstPage = pPage;

	ASSERT(Count);
	for (i = 0; i < Count; i++, pPage++)
	{
		m_AvailablePageList.InsertTail (pPage);
		pPage->m_Map = 0;
	}
}

void
CBuddyAllocMap::CLevel::SetPageMap (
	TPage* pPage,
	size_t Map
	)
{
	if (Map == -1 && pPage->m_Map != -1)
		m_AvailablePageList.Remove (pPage);
	else if (Map != -1 && pPage->m_Map == -1)
		m_AvailablePageList.InsertTail (pPage);

	pPage->m_Map = Map;
}

void
CBuddyAllocMap::CLevel::SetBit (
	TPage* pPage,
	size_t Bit,
	bool Value
	)
{
	size_t Mask;

	pPage += Bit / AXL_PTR_BITNESS;
	Bit &= AXL_PTR_BITNESS - 1;
	Mask = (size_t) 1 << Bit;

	if (Value)
	{
		ASSERT ((pPage->m_Map & Mask) == 0);
		SetPageMap (pPage, pPage->m_Map | Mask);
	}
	else
	{
		ASSERT ((pPage->m_Map & Mask) == Mask);
		SetPageMap (pPage, pPage->m_Map & ~Mask);
	}
}

void
CBuddyAllocMap::CLevel::SetBitRange (
	TPage* pPage,
	size_t From,
	size_t To,
	bool Value
	)
{
	size_t PageIndex = From / AXL_PTR_BITNESS;
	size_t Mask;

	pPage += PageIndex;
	From -= PageIndex * AXL_PTR_BITNESS;
	To -= PageIndex * AXL_PTR_BITNESS;

	if (Value)
	{
		if (To < AXL_PTR_BITNESS)
		{
			Mask = GetBitmask (From, To);
			ASSERT((pPage->m_Map & Mask) == 0);
			SetPageMap (pPage, pPage->m_Map | Mask);
			return;
		}

		Mask = GetHiBitmask(From);
		ASSERT((pPage->m_Map & Mask) == 0);
		SetPageMap (pPage, pPage->m_Map | Mask);

		To -= AXL_PTR_BITNESS;
		pPage++;
	
		while (To >= AXL_PTR_BITNESS)
		{
			ASSERT(pPage->m_Map == 0);
			pPage->m_Map = -1;
			To -= AXL_PTR_BITNESS;
			pPage++;
		}

		if (To)
		{
			Mask = GetLoBitmask(To);
			ASSERT((pPage->m_Map & Mask) == 0);
			SetPageMap (pPage, pPage->m_Map | Mask);
		}
	}
	else
	{
		if (To < AXL_PTR_BITNESS)
		{
			Mask = GetBitmask(From, To);
			ASSERT((pPage->m_Map & Mask) == Mask);
			SetPageMap (pPage, pPage->m_Map & ~Mask);
			return;
		}

		Mask = GetHiBitmask(From);
		ASSERT((pPage->m_Map & Mask) == Mask);
		SetPageMap (pPage, pPage->m_Map & ~Mask);

		To -= AXL_PTR_BITNESS;
		pPage++;
	
		while (To >= AXL_PTR_BITNESS)
		{
			ASSERT(pPage->m_Map == -1);
			pPage->m_Map = 0;
			To -= AXL_PTR_BITNESS;
			pPage++;
		}

		if (To)
		{
			Mask = GetLoBitmask(To);
			ASSERT((pPage->m_Map & Mask) == Mask);
			SetPageMap (pPage, pPage->m_Map & ~Mask);
		}
	}
}

//.............................................................................

CBuddyAllocMap::CBuddyAllocMap ()
{
	m_Width = 0;
	m_Height = 0;
	m_TotalSize = 0;
	m_FreeSizeTop = 0;
	m_FreeSizeBottom = 0;
	m_MaxAllocSize = 0;
}

bool
CBuddyAllocMap::Create(
	size_t Width,
	size_t Height
	)
{
	bool_t Result;

	size_t TotalPageCount;
	size_t PageCount;

	CLevel* pLevel;
	CLevel* pEnd;
	
	TPage* pPage;

	Close();

	if (!Width)
		Width = 1;

	if (!Height)
		Height = 1;

	TotalPageCount = ((1 << Height) - 1) * Width;
	
	Result = m_PageArray.SetCount (TotalPageCount) && m_LevelArray.SetCount (Height);
	if (!Result)
	{
		Close ();
		return false;
	}

	pPage = m_PageArray;
	pLevel = m_LevelArray;
	pEnd = pLevel + Height;

	PageCount = Width << (Height - 1); // on the bottom level

	m_Width = Width;
	m_Height = Height;
	m_TotalSize = PageCount * AXL_PTR_BITNESS;
	m_FreeSizeTop = m_TotalSize;
	m_FreeSizeBottom = m_TotalSize;
	m_MaxAllocSize = (size_t) 1 << (Height - 1);

	for (; pLevel < pEnd; pLevel++)
	{
		pLevel->Format (pPage, PageCount);

		pPage += PageCount;
		PageCount /= 2;
	}

	return true;
}

void
CBuddyAllocMap::Close ()
{
	m_PageArray.Clear();
	m_LevelArray.Clear();

	m_MaxAllocSize = 0;
	m_TotalSize = 0;
	m_FreeSizeTop = 0;
	m_FreeSizeBottom = 0;
}

void
CBuddyAllocMap::Clear ()
{
	TPage* pPage = m_PageArray;
	CLevel* pLevel = m_LevelArray;
	CLevel* pEnd = pLevel + m_Height;

	size_t PageCount = m_Width << (m_Height - 1); // on the bottom level

	for (; pLevel < pEnd; pLevel++)
	{
		pLevel->Format(pPage, PageCount);

		pPage += PageCount;
		PageCount /= 2;
	}

	m_FreeSizeTop = m_TotalSize;
	m_FreeSizeBottom = m_TotalSize;
}

size_t
CBuddyAllocMap::Allocate (size_t Size)
{
	CLevel* pLevel;
	TPage* pPage;

	size_t PageCount;
	size_t BitSize;
	size_t BitIndex;
	size_t Address;
	size_t AddressEnd;
	size_t i;

	size_t Level;

	if (Size > m_MaxAllocSize)
	{
		err::SetError (err::EStatus_InvalidParameter);
		return -1;
	}

	Level = GetHiBitIdx(Size);

	if (Level && (Size & ((1 << Level) - 1)))
		Level++;

	ASSERT(Level < m_Height);
	pLevel = m_LevelArray + Level;

	pPage = pLevel->GetFirstAvailablePage ();
	if (!pPage)
	{
		err::SetError(err::EStatus_InsufficientResources);
		return -1;
	}

	ASSERT(pPage->m_Map != -1);

	BitIndex = GetHiBitIdx(((pPage->m_Map + 1) | pPage->m_Map) ^ pPage->m_Map);
	BitSize = (size_t) 1 << Level;

	size_t PageIndex = pPage - pLevel->GetFirstPage ();

	Address = (PageIndex * AXL_PTR_BITNESS + BitIndex) * BitSize;
	AddressEnd = Address + Size;

	m_FreeSizeBottom -= Size;

	// mark starting with the bottom level (level 0)

	pLevel = m_LevelArray;
	pPage = m_PageArray;
	PageCount = m_Width << (m_Height - 1); // on the bottom level
	BitSize = 1;

	// below the allocation level we mark ranges of clusters

	for (i = 0; i < Level; i++)
	{
		size_t From = Address / BitSize;
		size_t To = AddressEnd / BitSize;

		ASSERT(!(Address % BitSize)); // beacuse we below the allocation level

		if (AddressEnd % BitSize)
			To++;

		pLevel->SetBitRange(pPage, From, To, true);

		pLevel++;
		pPage += PageCount;
		PageCount /= 2;
		BitSize *= 2;
	}

	// from the allocation level and above we only mark one cluster, also we can stop as soon as we find a marked cluster

	for (; i < m_Height; i++)
	{
		size_t Bit = Address / BitSize;

		if (GetBit(pPage, Bit))
			break;

		pLevel->SetBit(pPage, Bit, true);

		pLevel++;
		pPage += PageCount;
		PageCount /= 2;
		BitSize *= 2;
	}


	if (i == m_Height) // we marked all the way to the top, adjust top level free size
		m_FreeSizeTop -= (size_t) 1 << (m_Height - 1);

	return Address;
}

void
CBuddyAllocMap::Mark (
	size_t Address,
	size_t Size
	)
{
	CLevel* pLevel = m_LevelArray;
	TPage* pPage = m_PageArray;

	size_t PageCount = m_Width << (m_Height - 1); // on the bottom level
	size_t BitSize = 1;
	size_t AddressEnd = Address + Size;
	size_t i;

	size_t Level = GetHiBitIdx(Size);

	if (Level && (Size & ((1 << Level) - 1)))
		Level++;

	m_FreeSizeBottom -= Size;

	// below the allocation level we mark ranges of clusters

	for (i = 0; i < Level; i++)
	{
		size_t From = Address / BitSize;
		size_t To = AddressEnd / BitSize;

		ASSERT(!(Address % BitSize)); // beacuse we below the allocation level

		if (AddressEnd % BitSize)
			To++;

		pLevel->SetBitRange(pPage, From, To, true);

		pLevel++;
		pPage += PageCount;
		PageCount /= 2;
		BitSize *= 2;
	}

	// from the allocation level and above we only mark one cluster, also we can stop as soon as we find a marked cluster

	for (; i < m_Height; i++)
	{
		size_t Bit = Address / BitSize;

		if (GetBit(pPage, Bit))
			break;

		pLevel->SetBit(pPage, Bit, true);

		pLevel++;
		pPage += PageCount;
		PageCount /= 2;
		BitSize *= 2;
	}

	if (i == m_Height) // we marked all the way to the top, adjust top level free size
		m_FreeSizeTop -= (size_t) 1 << (m_Height - 1);
}

void
CBuddyAllocMap::Free (
	size_t Address,
	size_t Size
	)
{
	CLevel* pLevel = m_LevelArray;
	TPage* pPage = m_PageArray;
	TPage* pPrevPage = NULL;

	size_t PageCount = m_Width << (m_Height - 1); // on the bottom level
	size_t BitSize = 1;
	size_t Bit;
	size_t PrevBit;
	size_t AddressEnd = Address + Size;
	size_t i;

	bool_t IsBlocked = false;

	size_t Level = GetHiBitIdx(Size);

	if (Level && (Size & ((1 << Level) - 1)))
		Level++;

	m_FreeSizeBottom += Size;

	// below the allocation level we unmark ranges of clusters

	for (i = 0; i < Level; i++)
	{
		size_t From = Address / BitSize;
		size_t To = AddressEnd / BitSize;
		size_t Mod = AddressEnd % BitSize;

		ASSERT(!(Address % BitSize)); // beacuse we below the allocation level

		if (Mod && !IsBlocked)
		{
			ASSERT(pPrevPage != NULL);
			
			if (GetBit(pPrevPage, PrevBit))		
			{
				IsBlocked = true;
			}
			else
			{
				AddressEnd += BitSize - Mod;
				To++;
			}
		}
		
		pLevel->SetBitRange(pPage, From, To, false);

		pLevel++;
		pPrevPage = pPage;
		PrevBit = To;
		pPage += PageCount;
		PageCount /= 2;
		BitSize *= 2;
	}

	if (IsBlocked)
		return;

	// from the allocation level and above we only unmark one cluster, and only if both children are free

	PrevBit--; // in the next cycle PrevBit has meaning of the last cleared cluster

	for (; i < m_Height; i++)
	{
		// check adjacent cluster

		if (pPrevPage)
		{
			if (PrevBit & 1)
			{
				if (GetBit(pPrevPage, PrevBit - 1))
					break;
			}
			else
			{
				if (GetBit(pPrevPage, PrevBit + 1))
					break;
			}
		}

		Bit = Address / BitSize;
		pLevel->SetBit(pPage, Bit, false);

		pLevel++;
		pPrevPage = pPage;
		PrevBit = Bit;
		pPage += PageCount;
		PageCount /= 2;
		BitSize *= 2;
	}

	if (i == m_Height) // we marked all the way to the top, adjust top level free size
		m_FreeSizeTop += (size_t) 1 << (m_Height - 1);
}

//.............................................................................

} // namespace rtl
} // namespace axl
