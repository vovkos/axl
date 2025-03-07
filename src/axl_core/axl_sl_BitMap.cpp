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
	ASSERT(from <= to);

	size_t pageIndex = from / AXL_PTR_BITS;
	size_t* p = map + pageIndex;

	from -= pageIndex * AXL_PTR_BITS;
	to -= pageIndex * AXL_PTR_BITS;

	if (to <= AXL_PTR_BITS) {
		*p |= getBitmask(from, to);
		return;
	}

	const size_t* end = p + to / AXL_PTR_BITS;
	*p++ |= getHiBitmask(from);
	while (p < end)
		*p++ = -1;

	to &= AXL_PTR_BITS - 1;
	if (to)
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
	ASSERT(from <= to);

	size_t pageIndex = from / AXL_PTR_BITS;
	size_t* p = map + pageIndex;

	from -= pageIndex * AXL_PTR_BITS;
	to -= pageIndex * AXL_PTR_BITS;

	if (to <= AXL_PTR_BITS) {
		*p &= ~getBitmask(from, to);
		return;
	}

	const size_t* end = p + to / AXL_PTR_BITS;
	*p++ &= ~getHiBitmask(from);
	while (p < end)
		*p++ = 0;

	to &= AXL_PTR_BITS - 1;
	if (to)
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

void
shrBitMap(
	size_t* map,
	size_t pageCount,
	size_t n
) {
	if (!n)
		return;

	size_t i = n / AXL_PTR_BITS;
	if (i >= pageCount) {
		memset(map, 0, pageCount * sizeof(size_t));
		return;
	}

	n &= AXL_PTR_BITS - 1;
	if (!n) {
		memmove(map, map + i, (pageCount - i) * sizeof(size_t));
		memset(map + pageCount - i, 0, i * sizeof(size_t));
		return;
	}

	size_t c = AXL_PTR_BITS - n;
	size_t* dst = map;
	size_t* src = dst + i;
	size_t* next = src + 1;
	size_t* end = map + pageCount;
	while (next < end) {
		*dst++ = (*src >> n) | (*next << c);
		src = next++;
	}

	*dst = *src >> n;

	if (i)
		memset(map + pageCount - i, 0, i * sizeof(size_t));
}

void
shlBitMap(
	size_t* map,
	size_t pageCount,
	size_t n
) {
	if (!n)
		return;

	size_t i = n / AXL_PTR_BITS;
	if (i >= pageCount) {
		memset(map, 0, pageCount * sizeof(size_t));
		return;
	}

	n &= AXL_PTR_BITS - 1;
	if (!n) {
		memmove(map + i, map, (pageCount - i) * sizeof(size_t));
		memset(map, 0, i * sizeof(size_t));
		return;
	}

	size_t c = AXL_PTR_BITS - n;
	size_t* dst = map + pageCount - 1;
	size_t* src = dst - i;
	size_t* prev = src - 1;
	while (prev >= map) {
		*dst-- = (*src << n) | (*prev >> c);
		src = prev--;
	}

	*dst = *src << n;

	if (i)
		memset(map, 0, i * sizeof(size_t));
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
	from &= AXL_PTR_BITS - 1;
	size_t x = *p & getHiBitmask(from);
	if (x)
		return i * AXL_PTR_BITS + getLoBitIdx(x);

	const size_t* end = map + pageCount;
	for (p++; p < end; p++)
		if (*p != 0)
			return (p - map) * AXL_PTR_BITS + getLoBitIdx(*p);

	return -1;
}

size_t
findBitReverse(
	const size_t* map,
	size_t pageCount,
	size_t from
) {
	if (!pageCount)
		return -1;

	intptr_t i = from / AXL_PTR_BITS;
	if (i >= pageCount)
		i = pageCount - 1;

	const size_t* p = map + i;
	from &= AXL_PTR_BITS - 1;
	size_t x = *p & getLoBitmask(from + 1);
	if (x)
		return i * AXL_PTR_BITS + getHiBitIdx(x);

	for (p--; p >= map; p--)
		if (*p != 0)
			return (p - map) * AXL_PTR_BITS + getHiBitIdx(*p);

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
	from &= AXL_PTR_BITS - 1;
	size_t x = ~*p & getHiBitmask(from);
	if (x)
		return i * AXL_PTR_BITS + getLoBitIdx(x);

	const size_t* end = map + pageCount;
	for (p++; p < end; p++)
		if (*p != -1)
			return (p - map) * AXL_PTR_BITS + getLoBitIdx(~*p);

	return -1;
}

size_t
findZeroBitReverse(
	const size_t* map,
	size_t pageCount,
	size_t from
) {
	if (!pageCount)
		return -1;

	size_t i = from / AXL_PTR_BITS;
	if (i >= pageCount)
		i = pageCount -1;

	const size_t* p = map + i;
	from &= AXL_PTR_BITS - 1;
	size_t x = ~*p & getLoBitmask(from + 1);
	if (x)
		return i * AXL_PTR_BITS + getHiBitIdx(x);

	const size_t* end = map + pageCount;
	for (p--; p >= map; p--)
		if (*p != -1)
			return (p - map) * AXL_PTR_BITS + getHiBitIdx(~*p);

	return -1;
}

//..............................................................................

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
