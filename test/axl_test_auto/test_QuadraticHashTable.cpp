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

namespace {

//..............................................................................

struct IdentityHash {
	size_t
	operator () (uint_t key) const {
		return key;
	}
};

struct LowBitsHash {
	size_t
	operator () (uint_t key) const {
		return key & 7;
	}
};

struct ZeroHash {
	size_t
	operator () (uint_t) const {
		return 0;
	}
};

typedef sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, IdentityHash> Table;

void
test_Basic() {
	Table map;

	TEST_ASSERT(map.isEmpty());
	TEST_ASSERT(map.getCount() == 0);
	TEST_ASSERT(map.find(1) == NULL);

	for (uint_t i = 0; i < 1000; i++) {
		Table::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i * 10;
	}

	TEST_ASSERT(!map.isEmpty());
	TEST_ASSERT(map.getCount() == 1000);
	TEST_ASSERT(map.getHead() && map.getHead()->getKey() == 0);
	TEST_ASSERT(map.getTail() && map.getTail()->getKey() == 999);

	for (uint_t i = 0; i < 1000; i++) {
		Table::Iterator it = map.find(i);
		TEST_ASSERT(it && it->m_value == i * 10);
	}

	for (uint_t i = 1000; i < 1200; i++)
		TEST_ASSERT(map.find(i) == NULL);

	for (uint_t i = 0; i < 1000; i += 3) {
		Table::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i * 20;
	}

	TEST_ASSERT(map.getCount() == 1000);

	for (uint_t i = 0; i < 1000; i++) {
		Table::Iterator it = map.find(i);
		TEST_ASSERT(it);
		TEST_ASSERT(it->m_value == (i % 3 ? i * 10 : i * 20));
	}

	map.clear();
	TEST_ASSERT(map.isEmpty());
	TEST_ASSERT(map.getCount() == 0);

	for (uint_t i = 0; i < 1000; i++)
		TEST_ASSERT(map.find(i) == NULL);

	for (uint_t i = 0; i < 128; i++)
		TEST_ASSERT(map.add(i, i + 1));

	for (uint_t i = 0; i < 128; i++) {
		Table::Iterator it = map.find(i);
		TEST_ASSERT(it && it->m_value == i + 1);
	}
}

void
test_CollisionsAndErase() {
	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash> map;

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash>::Iterator it = map.visit(key);
		TEST_ASSERT(it);
		it->m_value = i;
	}

	TEST_ASSERT(map.getCount() == 2048);

	for (uint_t i = 0; i < 2048; i += 2) {
		uint_t key = i * 8 + (i & 7);
		TEST_ASSERT(map.eraseKey(key));
		TEST_ASSERT(map.find(key) == NULL);
	}

	TEST_ASSERT(map.getCount() == 1024);

	for (uint_t i = 1; i < 2048; i += 2) {
		uint_t key = i * 8 + (i & 7);
		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash>::Iterator it = map.find(key);
		TEST_ASSERT(it && it->m_value == i);
	}

	for (uint_t i = 0; i < 2048; i += 2) {
		uint_t key = i * 8 + (i & 7);
		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash>::Iterator it = map.visit(key);
		TEST_ASSERT(it);
		it->m_value = i + 10000;
	}

	TEST_ASSERT(map.getCount() == 2048);

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash>::Iterator it = map.find(key);
		TEST_ASSERT(it);
		TEST_ASSERT(it->m_value == (i & 1 ? i : i + 10000));
	}
}

void
test_VisitContinuesPastTombstone() {
	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash> map;

	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator it0 = map.add(0, 0);
	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator it8 = map.add(8, 80);
	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator it16 = map.add(16, 160);
	TEST_ASSERT(it0 && it8 && it16);
	TEST_ASSERT(map.getCount() == 3);

	TEST_ASSERT(map.eraseKey(8));
	TEST_ASSERT(map.getCount() == 2);

	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator it = map.visit(16);
	TEST_ASSERT(it == it16);
	TEST_ASSERT(it->m_value == 160);
	TEST_ASSERT(map.getCount() == 2);

	it = map.visit(24);
	TEST_ASSERT(it);
	it->m_value = 240;
	TEST_ASSERT(map.getCount() == 3);
	TEST_ASSERT(map.find(16) == it16);
	TEST_ASSERT(map.find(24)->m_value == 240);
}

void
test_StableIterators() {
	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash> map;

	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator stableIt = map.visit(17);
	TEST_ASSERT(stableIt);
	stableIt->m_value = 1700;

	for (uint_t i = 0; i < 4096; i++) {
		if (i == 17)
			continue;

		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i;
	}

	TEST_ASSERT(stableIt);
	TEST_ASSERT(stableIt->getKey() == 17);
	TEST_ASSERT(stableIt->m_value == 1700);
	TEST_ASSERT(map.find(17) == stableIt);

	for (uint_t i = 0; i < 4096; i += 3) {
		if (i != 17)
			TEST_ASSERT(map.eraseKey(i));
	}

	for (uint_t i = 4096; i < 8192; i++) {
		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, ZeroHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i;
	}

	TEST_ASSERT(stableIt);
	TEST_ASSERT(stableIt->getKey() == 17);
	TEST_ASSERT(stableIt->m_value == 1700);
	TEST_ASSERT(map.find(17) == stableIt);
}

void
test_Randomized() {
	enum {
		KeyCount = 4096,
		IterationCount = 20000,
	};

	sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash> map;
	bool isPresent[KeyCount] = {};
	uint_t valueArray[KeyCount] = {};
	uint_t expectedCount = 0;
	uint32_t seed = 0x12345678;

	for (uint_t i = 0; i < IterationCount; i++) {
		seed = seed * 1664525 + 1013904223;
		uint_t key = (seed >> 8) & (KeyCount - 1);
		uint_t value = seed ^ i;

		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash>::Iterator it = map.visit(key);
		TEST_ASSERT(it);
		it->m_value = value;

		if (!isPresent[key]) {
			isPresent[key] = true;
			expectedCount++;
		}

		valueArray[key] = value;
		TEST_ASSERT(map.getCount() == expectedCount);

		seed = seed * 1664525 + 1013904223;
		key = (seed >> 8) & (KeyCount - 1);
		if (isPresent[key] && (seed & 4)) {
			TEST_ASSERT(map.eraseKey(key));
			isPresent[key] = false;
			expectedCount--;
		}

		it = map.find(key);
		TEST_ASSERT((it != NULL) == isPresent[key]);
		if (it)
			TEST_ASSERT(it->m_value == valueArray[key]);
	}

	for (uint_t key = 0; key < KeyCount; key++) {
		sl::QuadraticHashTable<uint_t, (uint_t)-1, (uint_t)-2, uint_t, LowBitsHash>::Iterator it = map.find(key);
		TEST_ASSERT((it != NULL) == isPresent[key]);
		if (it)
			TEST_ASSERT(it->m_value == valueArray[key]);
	}
}

void
run() {
	test_Basic();
	test_CollisionsAndErase();
	test_VisitContinuesPastTombstone();
	test_StableIterators();
	test_Randomized();
}

//..............................................................................

ADD_TEST_CASE("test_QuadraticHashTable", run)

} // namespace
