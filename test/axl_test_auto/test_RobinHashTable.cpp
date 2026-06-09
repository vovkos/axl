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

void
test_Basic() {
	sl::RobinHashTable<uint_t, uint_t, IdentityHash> map;

	TEST_ASSERT(map.isEmpty());
	TEST_ASSERT(map.getCount() == 0);
	TEST_ASSERT(map.find(1) == NULL);

	for (uint_t i = 0; i < 1000; i++) {
		sl::RobinHashTable<uint_t, uint_t, IdentityHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i * 10;
	}

	TEST_ASSERT(!map.isEmpty());
	TEST_ASSERT(map.getCount() == 1000);

	for (uint_t i = 0; i < 1000; i++) {
		sl::RobinHashTable<uint_t, uint_t, IdentityHash>::Iterator it = map.find(i);
		TEST_ASSERT(it && it->m_value == i * 10);
	}

	for (uint_t i = 1000; i < 1200; i++)
		TEST_ASSERT(map.find(i) == NULL);

	for (uint_t i = 0; i < 1000; i += 3) {
		sl::RobinHashTable<uint_t, uint_t, IdentityHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i * 20;
	}

	TEST_ASSERT(map.getCount() == 1000);

	for (uint_t i = 0; i < 1000; i++) {
		sl::RobinHashTable<uint_t, uint_t, IdentityHash>::Iterator it = map.find(i);
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
		sl::RobinHashTable<uint_t, uint_t, IdentityHash>::Iterator it = map.find(i);
		TEST_ASSERT(it && it->m_value == i + 1);
	}
}

void
test_Collisions() {
	sl::RobinHashTable<uint_t, uint_t, LowBitsHash> map;

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::RobinHashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.visit(key);
		TEST_ASSERT(it);
		it->m_value = i;
	}

	TEST_ASSERT(map.getCount() == 2048);

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::RobinHashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.find(key);
		TEST_ASSERT(it && it->m_value == i);
	}

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + ((i + 1) & 7);
		TEST_ASSERT(map.find(key) == NULL);
	}
}

void
test_Erase() {
	sl::RobinHashTable<uint_t, uint_t, ZeroHash> map;

	for (uint_t i = 0; i < 512; i++) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i * 10;
	}

	sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator stableIt = map.find(17);
	TEST_ASSERT(stableIt);
	TEST_ASSERT(stableIt->m_value == 170);

	for (uint_t i = 0; i < 512; i += 2) {
		if (i == 17)
			continue;

		TEST_ASSERT(map.eraseKey(i));
		TEST_ASSERT(map.find(i) == NULL);
	}

	TEST_ASSERT(map.getCount() == 256);
	TEST_ASSERT(stableIt);
	TEST_ASSERT(stableIt->getKey() == 17);
	TEST_ASSERT(stableIt->m_value == 170);
	TEST_ASSERT(map.find(17) == stableIt);

	for (uint_t i = 1; i < 512; i += 2) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.find(i);
		TEST_ASSERT(it);
		TEST_ASSERT(it->m_value == i * 10);
	}

	for (uint_t i = 0; i < 512; i += 2) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i + 10000;
	}

	TEST_ASSERT(map.getCount() == 512);

	for (uint_t i = 0; i < 512; i++) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.find(i);
		TEST_ASSERT(it);
		TEST_ASSERT(it->m_value == (i & 1 ? i * 10 : i + 10000));
	}

	sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.find(400);
	TEST_ASSERT(it);
	map.erase(it);
	TEST_ASSERT(map.find(400) == NULL);
	TEST_ASSERT(!map.eraseKey(400));
	TEST_ASSERT(map.getCount() == 511);
}

void
test_SaturatedPsl() {
	sl::RobinHashTable<uint_t, uint_t, ZeroHash> map;

	for (uint_t i = 0; i < 512; i++) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i ^ 0x5a5a;
	}

	TEST_ASSERT(map.getCount() == 512);

	for (uint_t i = 0; i < 512; i++) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.find(i);
		TEST_ASSERT(it && it->m_value == (i ^ 0x5a5a));
	}

	for (uint_t i = 0; i < 512; i += 17) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.visit(i);
		TEST_ASSERT(it);
		it->m_value = i;
	}

	TEST_ASSERT(map.getCount() == 512);

	for (uint_t i = 0; i < 512; i++) {
		sl::RobinHashTable<uint_t, uint_t, ZeroHash>::Iterator it = map.find(i);
		TEST_ASSERT(it);
		TEST_ASSERT(it->m_value == (i % 17 ? (i ^ 0x5a5a) : i));
	}

	TEST_ASSERT(map.find(700) == NULL);
}

void
test_Randomized() {
	enum {
		KeyCount = 4096,
		IterationCount = 20000,
	};

	sl::RobinHashTable<uint_t, uint_t, LowBitsHash> map;
	bool isPresent[KeyCount] = {};
	uint_t valueArray[KeyCount] = {};
	uint_t expectedCount = 0;
	uint32_t seed = 0x12345678;

	for (uint_t i = 0; i < IterationCount; i++) {
		seed = seed * 1664525 + 1013904223;
		uint_t key = (seed >> 8) & (KeyCount - 1);
		uint_t value = seed ^ i;

		sl::RobinHashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.visit(key);
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
		it = map.find(key);
		TEST_ASSERT((it != NULL) == isPresent[key]);
		if (it)
			TEST_ASSERT(it->m_value == valueArray[key]);
	}

	for (uint_t key = 0; key < KeyCount; key++) {
		sl::RobinHashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.find(key);
		TEST_ASSERT((it != NULL) == isPresent[key]);
		if (it)
			TEST_ASSERT(it->m_value == valueArray[key]);
	}
}

void
run() {
	test_Basic();
	test_Collisions();
	test_Erase();
	test_SaturatedPsl();
	test_Randomized();
}

//..............................................................................

ADD_TEST_CASE("test_RobinHashTable", run)

} // namespace
