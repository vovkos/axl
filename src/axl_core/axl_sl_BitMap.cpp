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

void
setBitRange_u(
	size_t* map,
	size_t from,
	size_t to
) {
	size_t pageIndex = from / AXL_PTR_BITS;
	size_t* p = map + pageIndex;

	from -= pageIndex * AXL_PTR_BITS;
	to -= pageIndex * AXL_PTR_BITS;

	if (to <= AXL_PTR_BITS) {
		*p |= getBitmask(from, to);
		return;
	}

	*p |= getHiBitmask(from);
	to -= AXL_PTR_BITS;
	p++;

	while (to > AXL_PTR_BITS) {
		*p = -1;
		to -= AXL_PTR_BITS;
		p++;
	}

	*p |= getLoBitmask(to);
}

void
setBitRange(
	size_t* map,
	size_t pageCount,
	size_t from,
	size_t to
) {
	size_t bitCount = pageCount * AXL_PTR_BITS;

	if (from >= bitCount)
		return;

	if (to > bitCount)
		to = bitCount;

	setBitRange_u(map, from, to);
}

void
clearBitRange_u(
	size_t* map,
	size_t from,
	size_t to
) {
	size_t pageIndex = from / AXL_PTR_BITS;
	size_t* p = map + pageIndex;

	from -= pageIndex * AXL_PTR_BITS;
	to -= pageIndex * AXL_PTR_BITS;

	if (to <= AXL_PTR_BITS) {
		*p &= ~getBitmask(from, to);
		return;
	}

	*p &= ~getHiBitmask(from);
	to -= AXL_PTR_BITS;
	p++;

	while (to > AXL_PTR_BITS) {
		*p = 0;
		to -= AXL_PTR_BITS;
		p++;
	}

	*p &= ~getLoBitmask(to);
}

void
clearBitRange(
	size_t* map,
	size_t pageCount,
	size_t from,
	size_t to
) {
	size_t bitCount = pageCount * AXL_PTR_BITS;

	if (from >= bitCount)
		return;

	if (to > bitCount)
		to = bitCount;

	clearBitRange_u(map, from, to);
}

size_t
findBit(
	const size_t* map,
	size_t pageCount,
	size_t from
) {
	size_t i = from / AXL_PTR_BITS;
	if (i >= pageCount)
		return -1;

	const size_t* p = map + i;
	from -= i * AXL_PTR_BITS;
	size_t x = *p & getHiBitmask(from);

	if (x)
		return i * AXL_PTR_BITS + getLoBitIdx(x);

	for (i++, p++; i < pageCount; i++, p++)
		if (*p != 0)
			return i * AXL_PTR_BITS + getLoBitIdx(*p);

	return -1;
}

size_t
findZeroBit(
	const size_t* map,
	size_t pageCount,
	size_t from
) {
	size_t i = from / AXL_PTR_BITS;
	if (i >= pageCount)
		return -1;

	const size_t* p = map + i;
	from -= i * AXL_PTR_BITS;
	size_t x = ~*p & getHiBitmask(from);

	if (x)
		return i * AXL_PTR_BITS + getLoBitIdx(x);

	for (i++, p++; i < pageCount; i++, p++)
		if (*p != -1)
			return i * AXL_PTR_BITS + getLoBitIdx(~*p);

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
BitMap::isEqualImpl(
	const size_t* p1,
	size_t count1,
	const size_t* p2,
 	size_t count2
) {
	ASSERT(count1 < count2);

	if (memcmp(p1, p2, count1 * sizeof(size_t)))
		return false;

	for (size_t i = count1; i < count2; i++)
		if (p2[i])
			return false;

	return true;
}

//..............................................................................

} // namespace sl
} // namespace axl
