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
getBit (
	const size_t* map,
	size_t pageCount,
	size_t bit
	);

bool
setBit (
	size_t* map,
	size_t pageCount,
	size_t bit,
	bool value = true
	);

bool
setBitRange ( 
	size_t* map,
	size_t pageCount,
	size_t from,
	size_t to,
	bool value = true
	);

bool
mergeBitMaps ( 
	size_t* map,
	const size_t* map2,
	size_t pageCount,
	EBitOp op
	);

inline
void
inverseBitMap (
	size_t* map,
	size_t pageCount
	)
{
	for (size_t i = 0; i < pageCount; i++)
		map [i] = ~map [i];
}

size_t 
findBit (
	const size_t* map,
	size_t pageCount,
	size_t from,
	bool value = true
	);

//.............................................................................

template <size_t bitCount> 
class CBitMapT
{
public:
	enum
	{
		pageCount = bitCount / _AXL_PTR_BITNESS
	};

protected:
	size_t m_map [pageCount];

public:
	CBitMapT ()
	{
		clear ();
	}

	CBitMapT (const CBitMapT& src)
	{
		copy (src);
	}

	void 
	clear()
	{ 
		memset (m_map, 0, sizeof (m_map));
	}

	void
	copy (const CBitMapT& src)
	{
		memcpy (m_map, src.m_map, sizeof (m_map));
	}

	int 
	cmp (const CBitMapT& src)
	{
		return memcmp (m_map, src.m_map, sizeof (m_map));
	}

	const size_t* 
	getMap () const
	{
		return m_map;
	}

	size_t* 
	getMap ()
	{
		return m_map;
	}
	
	size_t 
	getPageCount () const
	{
		return pageCount;
	}

	bool 
	getBit (size_t bit) const
	{
		return rtl::getBit (m_map, pageCount, bit);
	}

	bool
	setBit (
		size_t bit, 
		bool value = true
		)
	{
		return rtl::setBit (m_map, pageCount, bit, value);
	}

	bool
	setBitRange ( 
		size_t from,
		size_t to,
		bool value = true
		)
	{
		return rtl::setBitRange (m_map, pageCount, from, to, value);
	}

	bool
	merge ( 
		const CBitMapT& bitMap2,
		EBitOp op
		)
	{
		return rtl::mergeBitMaps (m_map, bitMap2.m_map, pageCount, op);
	}

	void 
	inverse ()
	{ 
		rtl::inverseBitMap (m_map, pageCount);
	}

	size_t 
	findBit (
		size_t start,
		bool value = true
		) const
	{
		return rtl::findBit (m_map, pageCount, start, value);
	}
};

//.............................................................................

class CBitMap
{
protected:
	CArrayT <size_t> m_map;

public:
	CBitMap (size_t bitCount = 0)
	{
		create (bitCount);
	}

	const size_t* 
	getMap () const
	{
		return m_map;
	}

	size_t* 
	getMap ()
	{
		return m_map;
	}

	size_t 
	getPageCount () const
	{
		return m_map.getCount ();
	}

	size_t 
	getBitCount () const
	{
		return getPageCount () * _AXL_PTR_BITNESS;
	}

	void 
	clear()
	{
		memset (m_map, 0, m_map.getCount () * sizeof (size_t));
	}

	void 
	copy (const CBitMap& src);

	int
	cmp (const CBitMap& src);
	
	bool 
	create (size_t bitCount);

	bool 
	setBitCount (size_t bitCount);

	bool 
	ensureBitCount (size_t bitCount)
	{
		return bitCount > getBitCount () ? setBitCount (bitCount) : true;
	}

	bool 
	getBit (size_t bit) const
	{
		return rtl::getBit (m_map, m_map.getCount (), bit);
	}

	bool
	setBit (
		size_t bit,
		bool value = true
		)
	{
		return rtl::setBit (m_map, m_map.getCount (), bit, value);
	}

	bool
	setBitResize (
		size_t bit,
		bool value = true
		)
	{
		ensureBitCount (bit + 1);
		return rtl::setBit (m_map, m_map.getCount (), bit, value);
	}

	bool
	setBitRange (
		size_t from, 
		size_t to,
		bool value = true
		)
	{
		return rtl::setBitRange (m_map, m_map.getCount (), from, to, value);
	}

	bool
	setBitRangeResize (
		size_t from, 
		size_t to,
		bool value = true
		)
	{
		ensureBitCount (to);
		return rtl::setBitRange (m_map, m_map.getCount (), from, to, value);
	}

	bool 
	merge (
		const CBitMap& bitMap2, 
		EBitOp op
		);

	bool 
	mergeResize (
		const CBitMap& bitMap2, 
		EBitOp op
		);

	void 
	inverse ()
	{
		rtl::inverseBitMap (m_map, m_map.getCount ());
	}

	size_t 
	findBit (
		size_t start,
		bool value = true
		) const 
	{
		return rtl::findBit (m_map, m_map.getCount (), start, value);
	}
};

//.............................................................................

} // namespace gc
} // namespace axl
