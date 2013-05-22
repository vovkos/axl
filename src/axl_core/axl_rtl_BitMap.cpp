#include "pch.h"
#include "axl_rtl_BitMap.h"
#include "axl_rtl_BitIdx.h"

namespace axl {
namespace rtl {

//.............................................................................

bool
GetBit (
	const size_t* pMap,
	size_t PageCount,
	size_t Bit)
{
	size_t Page = Bit / _AXL_PTR_BITNESS;
	if (Page >= PageCount)
		return false;

	size_t Mask = (size_t) 1 << (Bit & (_AXL_PTR_BITNESS - 1));
	
	return (pMap [Page] & Mask) != 0;
}

bool
SetBit (
	size_t* pMap,
	size_t PageCount,
	size_t Bit,
	bool Value
	)
{
	size_t Page = Bit / _AXL_PTR_BITNESS;
	if (Page >= PageCount)
		return false;

	size_t* p = pMap + Page;
	size_t Mask = (size_t) 1 << (Bit & (_AXL_PTR_BITNESS - 1));
	size_t Old = *p;

	if (Value)
		*p |= Mask;
	else
		*p &= ~Mask;

	return *p != Old;
}

bool
SetBitRange ( 
	size_t* pMap,
	size_t PageCount,
	size_t From,
	size_t To,
	bool Value
	)
{
	bool HasChanged = false;

	size_t BitCount = PageCount * _AXL_PTR_BITNESS;

	if (From >= BitCount)
		return false;

	if (To > BitCount)
		To = BitCount;

	size_t PageIndex = From / _AXL_PTR_BITNESS;
	size_t* p = pMap + PageIndex;

	From -= PageIndex * _AXL_PTR_BITNESS;
	To -= PageIndex * _AXL_PTR_BITNESS;

	size_t Mask;
	size_t Old;

	if (Value)
	{
		Old = *p;

		if (To < _AXL_PTR_BITNESS)
		{
			Mask = GetBitmask (From, To);
			*p |= Mask;
			return *p != Old;
		}

		Mask = GetHiBitmask (From);
		*p |= Mask;

		if (*p != Old)
			HasChanged = true;

		To -= _AXL_PTR_BITNESS;
		p++;
	
		while (To >= _AXL_PTR_BITNESS)
		{
			if (*p != -1)
				HasChanged = true;

			*p = -1;
			To -= _AXL_PTR_BITNESS;
			p++;
		}

		if (To)
		{
			Mask = GetLoBitmask (To);
			Old = *p;
			*p |= Mask;

			if (*p != Old)
				HasChanged = true;
		}
	}
	else
	{
		Old = *p;

		if (To < _AXL_PTR_BITNESS)
		{
			Mask = GetBitmask (From, To);
			*p &= ~Mask;
			return *p != Old;
		}

		Mask = GetHiBitmask (From);
		*p &= ~Mask;

		if (*p != Old)
			HasChanged = true;

		To -= _AXL_PTR_BITNESS;
		p++;
	
		while (To >= _AXL_PTR_BITNESS)
		{
			if (*p != 0)
				HasChanged = true;

			*p = 0;
			To -= _AXL_PTR_BITNESS;
			p++;
		}

		if (To)
		{
			Mask = GetLoBitmask (To);
			Old = *p;
			*p &= ~Mask;

			if (*p != Old)
				HasChanged = true;
		}
	}

	return HasChanged;
}

bool
MergeBitMaps ( 
	size_t* pMap,
	const size_t* pMap2,
	size_t PageCount,
	EBitOp Op
	)
{
	bool HasChanged = false;

	size_t Old;

	size_t* p = pMap;
	size_t* pEnd = p + PageCount;
	const size_t* p2 = pMap2;

	switch (Op)
	{
	case EBitOp_Or:
		for (; p < pEnd; p++, p2++)
		{
			Old = *p;
			*p |= *p2;
			if (*p != Old)
				HasChanged = true;
		}

		break;

	case EBitOp_And:
		{
			Old = *p;
			*p &= *p2;
			if (*p != Old)
				HasChanged = true;
		}

		break;

	case EBitOp_Xor:
		{
			Old = *p;
			*p ^= *p2;
			if (*p != Old)
				HasChanged = true;
		}

		break;

	default:
		ASSERT (false);	
	}

	return HasChanged;
}

size_t 
FindBit (
	const size_t* pMap,
	size_t PageCount,
	size_t From,
	bool Value
	)
{
	size_t i = From / _AXL_PTR_BITNESS;
	if (i >= PageCount)
		return -1;

	const size_t* p = pMap + i;

	From -= i * _AXL_PTR_BITNESS;

	if (Value)
	{
		size_t x = *p & GetHiBitmask (From);

		if (x)
			return i * _AXL_PTR_BITNESS + GetLoBitIdx (x);

		for (i++, p++; i < PageCount; i++, p++)
		{
			if (*p != 0)
				return i * _AXL_PTR_BITNESS + GetLoBitIdx (*p);
		}
	}
	else
	{
		size_t x = ~*p & GetHiBitmask (From);

		if (!x)
			return i * _AXL_PTR_BITNESS + GetLoBitIdx (x);

		for (i++, p++; i < PageCount; i++, p++)
		{
			if (*p != -1)
				return i * _AXL_PTR_BITNESS + GetLoBitIdx (~*p);
		}
	}

	return -1;
}

//.............................................................................

bool
CBitMap::Create (size_t BitCount)
{
	bool Result = SetBitCount (BitCount);
	if (!Result)
		return false;

	Clear ();
	return true;
}

bool 
CBitMap::SetBitCount (size_t BitCount)
{
	size_t PageCount = BitCount / _AXL_PTR_BITNESS;
	if (BitCount & (_AXL_PTR_BITNESS - 1))
		PageCount++;

	return m_Map.SetCount (PageCount);
}

void 
CBitMap::Copy (const CBitMap& Src)
{
	size_t Count = Src.m_Map.GetCount ();
	m_Map.SetCount (Count);
	memcpy (m_Map, Src.m_Map, Count * sizeof (size_t));
}

int
CBitMap::Cmp (const CBitMap& Src)
{
	size_t Count = m_Map.GetCount ();
	size_t Count2 = Src.m_Map.GetCount ();

	return 
		Count < Count2 ? -1 :
		Count > Count2 ? 1 :
		memcmp (m_Map, Src.m_Map, Count * sizeof (size_t));
}

bool 
CBitMap::Merge (
	const CBitMap& BitMap2, 
	EBitOp Op
	)
{
	size_t PageCount = m_Map.GetCount ();
	size_t PageCount2 = BitMap2.m_Map.GetCount ();
	
	return rtl::MergeBitMaps (m_Map, BitMap2.m_Map, AXL_MIN (PageCount, PageCount2), Op);
}

bool
CBitMap::MergeResize (
	const CBitMap& BitMap2, 
	EBitOp Op
	)
{
	size_t PageCount2 = BitMap2.m_Map.GetCount ();
	size_t PageCount = m_Map.EnsureCount (PageCount2);

	return rtl::MergeBitMaps (m_Map, BitMap2.m_Map, PageCount, Op);
}

//.............................................................................

} // namespace rtl
} // namespace axl
