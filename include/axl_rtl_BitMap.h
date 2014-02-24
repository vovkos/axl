// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BITMAP_H

#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

//.............................................................................

enum EBitOp
{
	EBitOp_Or = 0,
	EBitOp_And,
	EBitOp_Xor,
};

//.............................................................................

bool
GetBit (
	const size_t* pMap,
	size_t PageCount,
	size_t Bit
	);

bool
SetBit (
	size_t* pMap,
	size_t PageCount,
	size_t Bit,
	bool Value = true
	);

bool
SetBitRange ( 
	size_t* pMap,
	size_t PageCount,
	size_t From,
	size_t To,
	bool Value = true
	);

bool
MergeBitMaps ( 
	size_t* pMap,
	const size_t* pMap2,
	size_t PageCount,
	EBitOp Op
	);

inline
void
InverseBitMap (
	size_t* pMap,
	size_t PageCount
	)
{
	for (size_t i = 0; i < PageCount; i++)
		pMap [i] = ~pMap [i];
}

size_t 
FindBit (
	const size_t* pMap,
	size_t PageCount,
	size_t From,
	bool Value = true
	);

//.............................................................................

template <size_t BitCount> 
class CBitMapT
{
public:
	enum
	{
		PageCount = BitCount / _AXL_PTR_BITNESS
	};

protected:
	size_t m_Map [PageCount];

public:
	CBitMapT ()
	{
		Clear ();
	}

	CBitMapT (const CBitMapT& Src)
	{
		Copy (Src);
	}

	void 
	Clear()
	{ 
		memset (m_Map, 0, sizeof (m_Map));
	}

	void
	Copy (const CBitMapT& Src)
	{
		memcpy (m_Map, Src.m_Map, sizeof (m_Map));
	}

	int 
	Cmp (const CBitMapT& Src)
	{
		return memcmp (m_Map, Src.m_Map, sizeof (m_Map));
	}

	const size_t* 
	GetMap () const
	{
		return m_Map;
	}

	size_t* 
	GetMap ()
	{
		return m_Map;
	}
	
	size_t 
	GetPageCount () const
	{
		return PageCount;
	}

	bool 
	GetBit (size_t Bit) const
	{
		return rtl::GetBit (m_Map, PageCount, Bit);
	}

	bool
	SetBit (
		size_t Bit, 
		bool Value = true
		)
	{
		return rtl::SetBit (m_Map, PageCount, Bit, Value);
	}

	bool
	SetBitRange ( 
		size_t From,
		size_t To,
		bool Value = true
		)
	{
		return rtl::SetBitRange (m_Map, PageCount, From, To, Value);
	}

	bool
	Merge ( 
		const CBitMapT& BitMap2,
		EBitOp Op
		)
	{
		return rtl::MergeBitMaps (m_Map, BitMap2.m_Map, PageCount, Op);
	}

	void 
	Inverse ()
	{ 
		rtl::InverseBitMap (m_Map, PageCount);
	}

	size_t 
	FindBit (
		size_t Start,
		bool Value = true
		) const
	{
		return rtl::FindBit (m_Map, PageCount, Start, Value);
	}
};

//.............................................................................

class CBitMap
{
protected:
	CArrayT <size_t> m_Map;

public:
	CBitMap (size_t BitCount = 0)
	{
		Create (BitCount);
	}

	const size_t* 
	GetMap () const
	{
		return m_Map;
	}

	size_t* 
	GetMap ()
	{
		return m_Map;
	}

	size_t 
	GetPageCount () const
	{
		return m_Map.GetCount ();
	}

	size_t 
	GetBitCount () const
	{
		return GetPageCount () * _AXL_PTR_BITNESS;
	}

	void 
	Clear()
	{
		memset (m_Map, 0, m_Map.GetCount () * sizeof (size_t));
	}

	void 
	Copy (const CBitMap& Src);

	int
	Cmp (const CBitMap& Src);
	
	bool 
	Create (size_t BitCount);

	bool 
	SetBitCount (size_t BitCount);

	bool 
	EnsureBitCount (size_t BitCount)
	{
		return BitCount > GetBitCount () ? SetBitCount (BitCount) : true;
	}

	bool 
	GetBit (size_t Bit) const
	{
		return rtl::GetBit (m_Map, m_Map.GetCount (), Bit);
	}

	bool
	SetBit (
		size_t Bit,
		bool Value = true
		)
	{
		return rtl::SetBit (m_Map, m_Map.GetCount (), Bit, Value);
	}

	bool
	SetBitResize (
		size_t Bit,
		bool Value = true
		)
	{
		EnsureBitCount (Bit + 1);
		return rtl::SetBit (m_Map, m_Map.GetCount (), Bit, Value);
	}

	bool
	SetBitRange (
		size_t From, 
		size_t To,
		bool Value = true
		)
	{
		return rtl::SetBitRange (m_Map, m_Map.GetCount (), From, To, Value);
	}

	bool
	SetBitRangeResize (
		size_t From, 
		size_t To,
		bool Value = true
		)
	{
		EnsureBitCount (To);
		return rtl::SetBitRange (m_Map, m_Map.GetCount (), From, To, Value);
	}

	bool 
	Merge (
		const CBitMap& BitMap2, 
		EBitOp Op
		);

	bool 
	MergeResize (
		const CBitMap& BitMap2, 
		EBitOp Op
		);

	void 
	Inverse ()
	{
		rtl::InverseBitMap (m_Map, m_Map.GetCount ());
	}

	size_t 
	FindBit (
		size_t Start,
		bool Value = true
		) const 
	{
		return rtl::FindBit (m_Map, m_Map.GetCount (), Start, Value);
	}
};

//.............................................................................

} // namespace gc
} // namespace axl
