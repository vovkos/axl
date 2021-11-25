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

namespace axl {
namespace sl {

//..............................................................................

enum BitOpKind {
	BitOpKind_Or = 0,
	BitOpKind_Xor,
	BitOpKind_And,
	BitOpKind_AndNot,
};

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
mergeBitMaps(
	size_t* map,
	const size_t* map2,
	size_t pageCount,
	BitOpKind op
);

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
findZeroBit(
	const size_t* map,
	size_t pageCount,
	size_t from
);

//..............................................................................

template <size_t bitCount>
class BitMapN {
public:
	enum {
		PageCount = bitCount / AXL_PTR_BITS + bitCount % AXL_PTR_BITS ? 1 : 0
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
		return sl::getBit(m_map, PageCount, bit);
	}

	void
	setBit(size_t bit) {
		sl::setBit(m_map, PageCount, bit);
	}

	void
	setBitRange(
		size_t from,
		size_t to
	) {
		sl::setBitRange(m_map, PageCount, from, to);
	}

	void
	merge(
		const BitMapN& bitMap2,
		BitOpKind op
	) {
		sl::mergeBitMaps(m_map, bitMap2.m_map, PageCount, op);
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
	findZeroBit(size_t start = 0) const {
		return sl::findZeroBit(m_map, PageCount, start);
	}
};

//..............................................................................

class BitMap {
protected:
	Array<size_t> m_map;

public:
	BitMap() {}

	BitMap(size_t bitCount) {
		create(bitCount);
	}

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
	getPageCount() const {
		return m_map.getCount();
	}

	size_t
	getBitCount() const {
		return getPageCount() * AXL_PTR_BITS;
	}

	void
	clear() {
		memset(m_map, 0, m_map.getCount() * sizeof(size_t));
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

	size_t
	hash() const {
		return djb2(m_map, m_map.getCount() * sizeof(size_t));
	}

	bool
	isEqual(const BitMap& src) const;

	bool
	create(size_t bitCount);

	bool
	setBitCount(size_t bitCount)  {
		return m_map.setCountZeroConstruct(sl::align<AXL_PTR_BITS>(bitCount));
	}

	bool
	ensureBitCount(size_t bitCount) {
		return bitCount > getBitCount() ? setBitCount(bitCount) : true;
	}

	bool
	getBit(size_t bit) const {
		return sl::getBit(m_map, m_map.getCount(), bit);
	}

	bool
	setBit(size_t bit) {
		return m_map.ensureExclusive() ? (sl::setBit(m_map, m_map.getCount(), bit), true) : false;
	}

	bool
	setBitResize(size_t bit) {
		return ensureBitCount(bit + 1) ? (sl::setBit(m_map, m_map.getCount(), bit), true) : false;
	}

	void
	setBitRange(
		size_t from,
		size_t to
	) {
		sl::setBitRange(m_map, m_map.getCount(), from, to);
	}

	bool
	setBitRangeResize(
		size_t from,
		size_t to
	) {
		return ensureBitCount(to) ? (sl::setBitRange(m_map, m_map.getCount(), from, to), true) : false;
	}

	bool
	clearBit(size_t bit) {
		return m_map.ensureExclusive() ? (sl::clearBit(m_map, m_map.getCount(), bit), true) : false;
	}

	bool
	clearBitResize(size_t bit) {
		return ensureBitCount(bit + 1) ? (sl::clearBit(m_map, m_map.getCount(), bit), true) : false;
	}

	bool
	clearBitRange(
		size_t from,
		size_t to
	) {
		return m_map.ensureExclusive() ? (sl::clearBitRange(m_map, m_map.getCount(), from, to), true) : false;
	}

	bool
	clearBitRangeResize(
		size_t from,
		size_t to
	) {
		return ensureBitCount(to) ? (sl::clearBitRange(m_map, m_map.getCount(), from, to), true) : false;
	}

	bool
	merge(
		const BitMap& bitMap2,
		BitOpKind op
	);

	bool
	mergeResize(
		const BitMap& bitMap2,
		BitOpKind op
	);

	bool
	invert() {
		return m_map.ensureExclusive() ? (sl::invertBitMap(m_map, m_map.getCount()), true) : false;
	}

	size_t
	findBit(size_t start = 0) const {
		return sl::findBit(m_map, m_map.getCount(), start);
	}

	size_t
	findZeroBit(size_t start = 0) const {
		return sl::findZeroBit(m_map, m_map.getCount(), start);
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

//..............................................................................

} // namespace gc
} // namespace axl
