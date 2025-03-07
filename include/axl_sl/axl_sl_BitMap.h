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

#pragma once

#define _AXL_SL_BITMAP_H

#include "axl_sl_Array.h"
#include "axl_sl_Hash.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

// for merging bit maps

typedef Or<size_t>     BitMapOr;
typedef Xor<size_t>    BitMapXor;
typedef And<size_t>    BitMapAnd;
typedef AndNot<size_t> BitMapAndNot;

//..............................................................................

inline
bool
getBit_u(
	const size_t* map,
	size_t bit
) {
	size_t page = bit / AXL_PTR_BITS;
	size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
	return (map[page] & mask) != 0;
}

inline
bool
getBit(
	const size_t* map,
	size_t pageCount,
	size_t bit
) {
	size_t page = bit / AXL_PTR_BITS;
	if (page >= pageCount)
		return false;

	size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
	return (map[page] & mask) != 0;
}

inline
void
setBit_u(
	size_t* map,
	size_t bit
) {
	size_t page = bit / AXL_PTR_BITS;
	size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
	map[page] |= mask;
}

inline
void
setBit(
	size_t* map,
	size_t pageCount,
	size_t bit
) {
	size_t page = bit / AXL_PTR_BITS;
	if (page < pageCount) {
		size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
		map[page] |= mask;
	}
}

inline
void
clearBit_u(
	size_t* map,
	size_t bit
) {
	size_t page = bit / AXL_PTR_BITS;
	size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
	map[page] &= ~mask;
}

inline
void
clearBit(
	size_t* map,
	size_t pageCount,
	size_t bit
) {
	size_t page = bit / AXL_PTR_BITS;
	if (page < pageCount) {
		size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
		map[page] &= ~ mask;
	}
}

void
setBitRange_u(
	size_t* map,
	size_t from,
	size_t to
);

void
setBitRange(
	size_t* map,
	size_t pageCount,
	size_t from,
	size_t to
);

void
clearBitRange_u(
	size_t* map,
	size_t from,
	size_t to
);

void
clearBitRange(
	size_t* map,
	size_t pageCount,
	size_t from,
	size_t to
);

void
shrBitMap(
	size_t* map,
	size_t pageCount,
	size_t n
);

void
shlBitMap(
	size_t* map,
	size_t pageCount,
	size_t n
);

template <typename Op>
void
mergeBitMaps(
	size_t* p,
	const size_t* p2,
	size_t pageCount
) {
	size_t* end = p + pageCount;
	for (; p < end; p++, p2++)
		*p = Op()(*p, *p2);
}

template <typename Op>
bool
mergeCmpBitMaps(
	size_t* p,
	const size_t* p2,
	size_t pageCount
) {
	bool isChanged = false;

	size_t* end = p + pageCount;
	for (; p < end; p++, p2++) {
		size_t value = Op()(*p, *p2);
		if (*p != value)
			isChanged = true;

		*p = value;
	}

	return isChanged;
}

inline
void
invertBitMap(
	size_t* map,
	size_t pageCount
) {
	for (size_t i = 0; i < pageCount; i++)
		map[i] = ~map[i];
}

size_t
findBit(
	const size_t* map,
	size_t pageCount,
	size_t from
);

size_t
findBitReverse(
	const size_t* map,
	size_t pageCount,
	size_t from
);

size_t
findZeroBit(
	const size_t* map,
	size_t pageCount,
	size_t from
);

size_t
findZeroBitReverse(
	const size_t* map,
	size_t pageCount,
	size_t from
);

//..............................................................................

template <size_t N>
class BitMapN {
public:
	enum {
		BitCount  = N,
		PageCount = BitCount / AXL_PTR_BITS + BitCount % AXL_PTR_BITS ? 1 : 0
	};

protected:
	size_t m_map[PageCount];

public:
	BitMapN() {
		clear();
	}

	void
	clear() {
		memset(m_map, 0, sizeof(m_map));
	}

	void
	copy(const BitMapN& src) {
		memcpy(m_map, src.m_map, sizeof(m_map));
	}

	size_t
	hash() const {
		return djb2(m_map, sizeof(m_map));
	}

	bool
	isEqual(const BitMapN& src) const {
		return memcmp(m_map, src.m_map, sizeof(m_map)) == 0;
	}

	const size_t*
	p() const {
		return m_map;
	}

	size_t*
	p() {
		return m_map;
	}

	size_t
	getPageCount() const {
		return PageCount;
	}

	bool
	getBit(size_t bit) const {
		ASSERT(bit < BitCount);
		return sl::getBit_u(m_map, bit);
	}

	void
	setBit(size_t bit) {
		ASSERT(bit < BitCount);
		sl::setBit_u(m_map, bit);
	}

	void
	setBitRange(
		size_t from,
		size_t to
	) {
		ASSERT(from <= BitCount && to <= BitCount);
		sl::setBitRange_u(m_map, from, to);
	}

	void
	clearBit(size_t bit) {
		ASSERT(bit < BitCount);
		sl::clearBit_u(m_map, bit);
	}

	void
	clearBitRange(
		size_t from,
		size_t to
	) {
		ASSERT(from <= BitCount && to <= BitCount);
		sl::clearBitRange_u(m_map, from, to);
	}

	void
	shr(size_t n) {
		sl::shrBitMap(m_map, PageCount, n);
	}

	void
	shl(size_t n) {
		sl::shlBitMap(m_map, PageCount, n);
	}

	template <typename Op>
	void
	merge(const BitMapN& bitMap2) {
		sl::mergeBitMaps<Op>(m_map, bitMap2.m_map, PageCount);
	}

	template <typename Op>
	bool
	mergeCmp(const BitMapN& bitMap2) {
		return sl::mergeCmpBitMaps<Op>(m_map, bitMap2.m_map, PageCount);
	}

	void
	invert() {
		sl::invertBitMap(m_map, PageCount);
	}

	size_t
	findBit(size_t start = 0) const {
		return sl::findBit(m_map, PageCount, start);
	}

	size_t
	findBitReverse(size_t start = -1) const {
		return sl::findBitReverse(m_map, PageCount, start);
	}

	size_t
	findZeroBit(size_t start = 0) const {
		return sl::findZeroBit(m_map, PageCount, start);
	}

	size_t
	findZeroBitReverse(size_t start = -1) const {
		return sl::findZeroBitReverse(m_map, PageCount, start);
	}
};

//..............................................................................

class BitMap {
protected:
	Array<size_t> m_map;

public:
	BitMap() {}

	BitMap(size_t bitCount) {
		setBitCount(bitCount);
	}

	BitMap(
		rc::BufKind bufKind,
		void* p,
		size_t size
	): m_map(bufKind, p, size) {}

	bool
	operator == (const BitMap& src) const {
		return isEqual(src);
	}

	bool
	operator != (const BitMap& src) const {
		return !isEqual(src);
	}

	const size_t*
	cp() const {
		return m_map.cp();
	}

	size_t*
	p() {
		return m_map.p();
	}

	bool
	isNull() const { // name it 'isNull' to avoid confusion (isEmpty sounds like "no bits set")
		return m_map.isEmpty();
	}

	size_t
	getBitCount() const {
		return getPageCount() * AXL_PTR_BITS;
	}

	size_t
	getPageCount() const {
		return m_map.getCount();
	}

	void
	clear() {
		memset(m_map.p(), 0, m_map.getCount() * sizeof(size_t));
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(BitMap&& src) {
		m_map.move(std::move(src.m_map));
	}
#endif

	bool
	forceCopy(const BitMap& src) {
		return m_map.forceCopy(src.m_map) != -1;
	}

	bool
	copy(const BitMap& src) {
		return m_map.copy(src.m_map) != -1;
	}

	bool
	copy(
		const size_t* p,
		size_t pageCount
	) {
		return m_map.copy(p, pageCount) != -1;
	}

	bool
	copy(
		const char* p,
		size_t size
	);

	size_t
	hash() const {
		return djb2(m_map, m_map.getCount() * sizeof(size_t));
	}

	bool
	isEqual(const BitMap& src) const;

	bool
	setBitCount(size_t bitCount)  {
		return m_map.setCountZeroConstruct(sl::align<AXL_PTR_BITS>(bitCount) / AXL_PTR_BITS);
	}

	bool
	ensureBitCount(size_t bitCount) {
		return bitCount > getBitCount() ? setBitCount(bitCount) : true;
	}

	bool
	getBit(size_t bit) const {
		return sl::getBit(m_map, m_map.getCount(), bit);
	}

	void
	setBit(size_t bit) {
		return sl::setBit(m_map.p(), m_map.getCount(), bit);
	}

	bool
	setBitResize(size_t bit) {
		return ensureBitCount(bit + 1) ? (sl::setBit_u(m_map.p(), bit), true) : false;
	}

	void
	setBitRange(
		size_t from,
		size_t to
	) {
		sl::setBitRange(m_map.p(), m_map.getCount(), from, to);
	}

	bool
	setBitRangeResize(
		size_t from,
		size_t to
	) {
		return from >= to ? true : ensureBitCount(to) ? (sl::setBitRange_u(m_map.p(), from, to), true) : false;
	}

	void
	clearBit(size_t bit) {
		return sl::clearBit(m_map.p(), m_map.getCount(), bit);
	}

	bool
	clearBitResize(size_t bit) {
		return ensureBitCount(bit + 1) ? (sl::clearBit_u(m_map.p(), bit), true) : false;
	}

	void
	clearBitRange(
		size_t from,
		size_t to
	) {
		return sl::clearBitRange(m_map.p(), m_map.getCount(), from, to);
	}

	bool
	clearBitRangeResize(
		size_t from,
		size_t to
	) {
		return from >= to ? true : ensureBitCount(to) ? (sl::clearBitRange_u(m_map.p(), from, to), true) : false;
	}

	void
	shr(size_t n) {
		sl::shrBitMap(m_map.p(), m_map.getCount(), n);
	}

	bool
	shrResize(size_t n);

	void
	shl(size_t n) {
		sl::shlBitMap(m_map.p(), m_map.getCount(), n);
	}

	bool
	shlResize(size_t n);

	template <typename Op>
	void
	merge(const BitMap& bitMap2);

	template <typename Op>
	bool
	mergeCmp(const BitMap& bitMap2);

	template <typename Op>
	bool
	mergeResize(const BitMap& bitMap2);

	template <typename Op>
	bool
	mergeCmpResize(const BitMap& bitMap2);

	void
	invert() {
		return sl::invertBitMap(m_map.p(), m_map.getCount());
	}

	size_t
	findBit(size_t start = 0) const {
		return sl::findBit(m_map, m_map.getCount(), start);
	}

	size_t
	findBitReverse(size_t start = -1) const {
		return sl::findBitReverse(m_map, m_map.getCount(), start);
	}

	size_t
	findZeroBit(size_t start = 0) const {
		return sl::findZeroBit(m_map, m_map.getCount(), start);
	}

	size_t
	findZeroBitReverse(size_t start = -1) const {
		return sl::findZeroBitReverse(m_map, m_map.getCount(), start);
	}

protected:
	static
	bool
	isEqualImpl(
		const size_t* p1,
		size_t count1,
		const size_t* p2,
		size_t count2
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
BitMap::isEqual(const BitMap& src) const {
	size_t count = m_map.getCount();
	size_t count2 = src.m_map.getCount();

	return
		count == count2 ? memcmp(m_map, src.m_map, count * sizeof(size_t)) == 0 :
		count < count2 ? isEqualImpl(m_map, count, src.m_map, count2) :
		isEqualImpl(src.m_map, count2, m_map, count);
}

inline
bool
BitMap::copy(
	const char* p,
	size_t size
) {
	size_t pageCount = sl::align<sizeof(size_t)>(size) / sizeof(size_t);
	bool result = m_map.setCountZeroConstruct(pageCount);
	if (!result)
		return false;

	memcpy(m_map.p(), p, size);
	return true;
}

inline
bool
BitMap::shlResize(size_t n) {
	size_t i = findBitReverse();
	if (i == -1)
		return true;

	if (!ensureBitCount(i + n + 1))
		return false;

	shlBitMap(m_map.p(), m_map.getCount(), n);
	return true;
}

inline
bool
BitMap::shrResize(size_t n) {
	shrBitMap(m_map.p(), m_map.getCount(), n);

	intptr_t i = m_map.getCount() - 1;
	for (; i > 0; i--)
		if (m_map[i])
			break;

	return m_map.setCount(i + 1);
}

template <typename Op>
void
BitMap::merge(const BitMap& bitMap2) {
	size_t pageCount = m_map.getCount();
	size_t pageCount2 = bitMap2.m_map.getCount();
	sl::mergeBitMaps<Op>(m_map.p(), bitMap2.m_map, AXL_MIN(pageCount, pageCount2));
}

template <typename Op>
bool
BitMap::mergeCmp(const BitMap& bitMap2) {
	size_t pageCount = m_map.getCount();
	size_t pageCount2 = bitMap2.m_map.getCount();
	return sl::mergeCmpBitMaps<Op>(m_map.p(), bitMap2.m_map, AXL_MIN(pageCount, pageCount2));
}

template <typename Op>
bool
BitMap::mergeResize(const BitMap& bitMap2) {
	size_t pageCount2 = bitMap2.m_map.getCount();
	size_t pageCount = m_map.ensureCountZeroConstruct(pageCount2);
	if (pageCount == -1)
		return false;

	sl::mergeBitMaps<Op>(m_map.p(), bitMap2.m_map, AXL_MIN(pageCount, pageCount2));
	return true;
}

template <typename Op>
bool
BitMap::mergeCmpResize(const BitMap& bitMap2) {
	size_t pageCount2 = bitMap2.m_map.getCount();
	size_t pageCount = m_map.ensureCountZeroConstruct(pageCount2);
	if (pageCount == -1)
		return false;

	return sl::mergeCmpBitMaps<Op>(m_map.p(), bitMap2.m_map, AXL_MIN(pageCount, pageCount2));
}

//..............................................................................

} // namespace gc
} // namespace axl
