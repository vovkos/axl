//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_sl_BitMap.h"
#include "axl_sl_BitIdx.h"

namespace axl {
namespace sl {

//..............................................................................

bool
getBit(
	const size_t* map,
	size_t pageCount,
	size_t bit) {
	size_t page = bit / AXL_PTR_BITS;
	if (page >= pageCount)
		return false;

	size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));

	return (map[page] & mask) != 0;
}

bool
setBit(
	size_t* map,
	size_t pageCount,
	size_t bit,
	bool value
) {
	size_t page = bit / AXL_PTR_BITS;
	if (page >= pageCount)
		return false;

	size_t* p = map + page;
	size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
	size_t old = *p;

	if (value)
		*p |= mask;
	else
		*p &= ~mask;

	return *p != old;
}

bool
setBitRange(
	size_t* map,
	size_t pageCount,
	size_t from,
	size_t to,
	bool value
) {
	bool hasChanged = false;

	size_t bitCount = pageCount * AXL_PTR_BITS;

	if (from >= bitCount)
		return false;

	if (to > bitCount)
		to = bitCount;

	size_t pageIndex = from / AXL_PTR_BITS;
	size_t* p = map + pageIndex;

	from -= pageIndex * AXL_PTR_BITS;
	to -= pageIndex * AXL_PTR_BITS;

	size_t mask;
	size_t old;

	if (value) {
		old = *p;

		if (to < AXL_PTR_BITS) {
			mask = getBitmask(from, to);
			*p |= mask;
			return *p != old;
		}

		mask = getHiBitmask(from);
		*p |= mask;

		if (*p != old)
			hasChanged = true;

		to -= AXL_PTR_BITS;
		p++;

		while (to >= AXL_PTR_BITS) {
			if (*p != -1)
				hasChanged = true;

			*p = -1;
			to -= AXL_PTR_BITS;
			p++;
		}

		if (to) {
			mask = getLoBitmask(to);
			old = *p;
			*p |= mask;

			if (*p != old)
				hasChanged = true;
		}
	} else {
		old = *p;

		if (to < AXL_PTR_BITS) {
			mask = getBitmask(from, to);
			*p &= ~mask;
			return *p != old;
		}

		mask = getHiBitmask(from);
		*p &= ~mask;

		if (*p != old)
			hasChanged = true;

		to -= AXL_PTR_BITS;
		p++;

		while (to >= AXL_PTR_BITS) {
			if (*p != 0)
				hasChanged = true;

			*p = 0;
			to -= AXL_PTR_BITS;
			p++;
		}

		if (to) {
			mask = getLoBitmask(to);
			old = *p;
			*p &= ~mask;

			if (*p != old)
				hasChanged = true;
		}
	}

	return hasChanged;
}

bool
mergeBitMaps(
	size_t* map,
	const size_t* map2,
	size_t pageCount,
	BitOpKind op
) {
	bool hasChanged = false;

	size_t old;

	size_t* p = map;
	size_t* end = p + pageCount;
	const size_t* p2 = map2;

	switch (op) {
	case BitOpKind_Or:
		for (; p < end; p++, p2++) {
			old = *p;
			*p |= *p2;
			if (*p != old)
				hasChanged = true;
		}

		break;

	case BitOpKind_Xor:
		for (; p < end; p++, p2++) {
			old = *p;
			*p ^= *p2;
			if (*p != old)
				hasChanged = true;
		}

		break;

	case BitOpKind_And:
		for (; p < end; p++, p2++) {
			old = *p;
			*p &= *p2;
			if (*p != old)
				hasChanged = true;
		}

		break;

	case BitOpKind_AndNot:
		for (; p < end; p++, p2++) {
			old = *p;
			*p &= ~*p2;
			if (*p != old)
				hasChanged = true;
		}

		break;

	default:
		ASSERT(false);
	}

	return hasChanged;
}

size_t
findBit(
	const size_t* map,
	size_t pageCount,
	size_t from,
	bool value
) {
	size_t i = from / AXL_PTR_BITS;
	if (i >= pageCount)
		return -1;

	const size_t* p = map + i;

	from -= i * AXL_PTR_BITS;

	if (value) {
		size_t x = *p & getHiBitmask(from);

		if (x)
			return i * AXL_PTR_BITS + getLoBitIdx(x);

		for (i++, p++; i < pageCount; i++, p++) {
			if (*p != 0)
				return i * AXL_PTR_BITS + getLoBitIdx(*p);
		}
	} else {
		size_t x = ~*p & getHiBitmask(from);

		if (x)
			return i * AXL_PTR_BITS + getLoBitIdx(x);

		for (i++, p++; i < pageCount; i++, p++) {
			if (*p != -1)
				return i * AXL_PTR_BITS + getLoBitIdx(~*p);
		}
	}

	return -1;
}

//..............................................................................

bool
BitMap::create(size_t bitCount) {
	bool result = setBitCount(bitCount);
	if (!result)
		return false;

	clear();
	return true;
}

bool
BitMap::setBitCount(size_t bitCount) {
	size_t pageCount = bitCount / AXL_PTR_BITS;
	if (bitCount & (AXL_PTR_BITS - 1))
		pageCount++;

	return m_map.setCountZeroConstruct(pageCount);
}

int
BitMap::cmp(const BitMap& src) const {
	size_t count = m_map.getCount();
	size_t count2 = src.m_map.getCount();

	return
		count < count2 ? -1 :
		count > count2 ? 1 :
		memcmp(m_map, src.m_map, count * sizeof(size_t));
}

bool
BitMap::merge(
	const BitMap& bitMap2,
	BitOpKind op
) {
	size_t pageCount = m_map.getCount();
	size_t pageCount2 = bitMap2.m_map.getCount();

	return sl::mergeBitMaps(m_map, bitMap2.m_map, AXL_MIN(pageCount, pageCount2), op);
}

bool
BitMap::mergeResize(
	const BitMap& bitMap2,
	BitOpKind op
) {
	size_t pageCount2 = bitMap2.m_map.getCount();
	size_t pageCount = m_map.ensureCountZeroConstruct(pageCount2);

	return sl::mergeBitMaps(m_map, bitMap2.m_map, AXL_MIN(pageCount, pageCount2), op);
}

//..............................................................................

} // namespace sl
} // namespace axl
