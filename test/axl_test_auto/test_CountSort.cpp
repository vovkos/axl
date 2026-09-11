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
#include "test.h"
#include "axl_sl_CountSort.h"

namespace {

//..............................................................................

// a payload wider than the key, so stability is observable

struct Item {
	size_t m_key;
	size_t m_serial; // position in the original array
};

struct GetItemKey {
	size_t
	operator () (const Item& item) const {
		return m_key(item);
	}

	static
	size_t
	m_key(const Item& item) {
		return item.m_key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
checkSorted(
	const Item* dst,
	const size_t* indexes,
	const size_t* keys,
	size_t count,
	size_t limit
) {
	// 1. keys are non-descending, and equal keys kept their original order

	for (size_t i = 1; i < count; i++) {
		TEST_ASSERT(dst[i - 1].m_key <= dst[i].m_key);

		if (dst[i - 1].m_key == dst[i].m_key)
			TEST_ASSERT(dst[i - 1].m_serial < dst[i].m_serial); // stable
	}

	// 2. every element survived exactly once

	sl::Array<size_t> seen;
	seen.setCountZeroConstruct(count);

	sl::Array<size_t>::Rwi seenRwi = seen.rwi();
	for (size_t i = 0; i < count; i++) {
		TEST_ASSERT(dst[i].m_serial < count);
		seenRwi[dst[i].m_serial]++;
	}

	for (size_t i = 0; i < count; i++)
		TEST_ASSERT(seen[i] == 1);

	// 3. indexes[k] is the start of row k

	size_t base = 0;
	for (size_t k = 0; k <= limit; k++) {
		TEST_ASSERT(indexes[k] == base);

		for (size_t i = 0; i < count; i++)
			if (keys[i] == k)
				base++;
	}

	// 4. rows actually hold their key

	for (size_t k = 0; k <= limit; k++) {
		size_t end = k < limit ? indexes[k + 1] : count;
		for (size_t i = indexes[k]; i < end; i++)
			TEST_ASSERT(dst[i].m_key == k);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
runCase(
	const size_t* keys,
	size_t count,
	size_t limit
) {
	sl::Array<Item> src;
	src.setCount(count);

	sl::Array<Item>::Rwi srcRwi = src.rwi();
	for (size_t i = 0; i < count; i++) {
		srcRwi[i].m_key = keys[i];
		srcRwi[i].m_serial = i;
	}

	sl::Array<Item> dst;
	sl::Array<size_t> indexes;
	dst.setCount(count);
	indexes.setCount(limit + 1);

	sl::countSort<Item, GetItemKey>(dst.p(), src.cp(), count, indexes.p(), limit);
	checkSorted(dst.cp(), indexes.cp(), keys, count, limit);
}

//..............................................................................

void run() {
	// empty -- must not touch dst, and must zero the whole index table

	{
		size_t indexes[4];
		memset(indexes, 0xcd, sizeof(indexes));
		sl::countSort<Item, GetItemKey>(NULL, NULL, 0, indexes, 3);

		for (size_t i = 0; i <= 3; i++)
			TEST_ASSERT(indexes[i] == 0);
	}

	// single element

	{
		static size_t keys[] = { 0 };
		runCase(keys, countof(keys), 0);
	}

	{
		static size_t keys[] = { 2 };
		runCase(keys, countof(keys), 2);
	}

	// already sorted / reversed / interleaved

	{
		static size_t keys[] = { 0, 1, 2, 3, 4 };
		runCase(keys, countof(keys), 4);
	}

	{
		static size_t keys[] = { 4, 3, 2, 1, 0 };
		runCase(keys, countof(keys), 4);
	}

	{
		static size_t keys[] = { 2, 0, 1, 0, 2, 1, 0 };
		runCase(keys, countof(keys), 2);
	}

	// all the same key -- one full row, every other row empty

	{
		static size_t keys[] = { 3, 3, 3, 3, 3 };
		runCase(keys, countof(keys), 5);
	}

	// both ends of the range occupied, middle empty

	{
		static size_t keys[] = { 0, 7, 0, 7 };
		runCase(keys, countof(keys), 7);
	}

	// limit above the largest key -- the CSR sentinel trick: the extra row is
	// empty, so indexes[limit] retains the total and rows are [k, k + 1) for all k

	{
		static size_t keys[] = { 0, 2, 1, 2, 0 };
		size_t count = countof(keys);
		size_t limit = 3; // keys only go up to 2

		sl::Array<Item> src;
		src.setCount(count);

		sl::Array<Item>::Rwi srcRwi = src.rwi();
		for (size_t i = 0; i < count; i++) {
			srcRwi[i].m_key = keys[i];
			srcRwi[i].m_serial = i;
		}

		sl::Array<Item> dst;
		sl::Array<size_t> indexes;
		dst.setCount(count);
		indexes.setCount(limit + 1);

		sl::countSort<Item, GetItemKey>(dst.p(), src.cp(), count, indexes.p(), limit);
		checkSorted(dst.cp(), indexes.cp(), keys, count, limit);

		TEST_ASSERT(indexes[limit] == count); // sentinel
	}

	// default key accessor on a scalar array

	{
		static size_t src[] = { 3, 1, 2, 1, 0 };
		size_t dst[countof(src)];
		size_t indexes[4];

		sl::countSort(dst, src, countof(src), indexes, 3);

		TEST_ASSERT(dst[0] == 0);
		TEST_ASSERT(dst[1] == 1);
		TEST_ASSERT(dst[2] == 1);
		TEST_ASSERT(dst[3] == 2);
		TEST_ASSERT(dst[4] == 3);

		TEST_ASSERT(indexes[0] == 0);
		TEST_ASSERT(indexes[1] == 1);
		TEST_ASSERT(indexes[2] == 3);
		TEST_ASSERT(indexes[3] == 4);
	}

	// wide-ish range, many duplicates -- exercises the prefix sum

	{
		sl::Array<size_t> keys;
		size_t count = 1000;
		size_t limit = 63;

		keys.setCount(count);

		sl::Array<size_t>::Rwi keyRwi = keys.rwi();
		for (size_t i = 0; i < count; i++)
			keyRwi[i] = (i * 37) % (limit + 1);

		runCase(keys.cp(), count, limit);
	}
}

//..............................................................................

ADD_TEST_CASE("test_CountSort", run)

}
