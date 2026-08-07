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

struct Base: sl::ListLink {
	int a, b;
};

struct Derived: Base {
	int c, d;
};

void
test_IteratorConversions() {
	sl::Iterator<Derived> itDerived;
	sl::Iterator<Base> itBase;
	itBase = itDerived;
}

void
test_BasicOrderAndLookup() {
	static const int keyTable[] = {
		50, 20, 70, 10, 30, 60, 80, 25, 35, 65, 75,
	};

	sl::RbTree<int, int> tree;

	for (size_t i = 0; i < countof(keyTable); i++) {
		bool isNew = false;
		sl::RbTree<int, int>::Iterator it = tree.add(keyTable[i], keyTable[i] * 10, &isNew);
		TEST_ASSERT(it && isNew);
	}

	TEST_ASSERT(tree.getCount() == countof(keyTable));

	for (size_t i = 0; i < countof(keyTable); i++) {
		bool isNew = true;
		sl::RbTree<int, int>::Iterator it = tree.add(keyTable[i], keyTable[i] * 20, &isNew);
		TEST_ASSERT(it && !isNew);
		TEST_ASSERT(it->m_value == keyTable[i] * 20);
	}

	TEST_ASSERT(tree.getCount() == countof(keyTable));

	int prev = -1;
	size_t count = 0;
	sl::RbTree<int, int>::Iterator it = tree.getHead();
	for (; it; it++) {
		TEST_ASSERT(it->getKey() > prev);
		TEST_ASSERT(it->m_value == it->getKey() * 20);
		prev = it->getKey();
		count++;
	}

	TEST_ASSERT(count == tree.getCount());
	TEST_ASSERT(tree.findValue(25, -1) == 500);
	TEST_ASSERT(tree.findValue(26, -1) == -1);
	TEST_ASSERT(tree.find<sl::RelOpKind_Lt>(25)->getKey() == 20);
	TEST_ASSERT(tree.find<sl::RelOpKind_Le>(25)->getKey() == 25);
	TEST_ASSERT(tree.find<sl::RelOpKind_Gt>(25)->getKey() == 30);
	TEST_ASSERT(tree.find<sl::RelOpKind_Ge>(25)->getKey() == 25);
	TEST_ASSERT(tree.find<sl::RelOpKind_Ge>(26)->getKey() == 30);
	TEST_ASSERT(!tree.find<sl::RelOpKind_Lt>(10));
	TEST_ASSERT(!tree.find<sl::RelOpKind_Gt>(80));

	TEST_ASSERT(!tree.addIfNotExists(25, 1));
	it = tree.addIfNotExists(26, 260);
	TEST_ASSERT(it && it->getKey() == 26 && it->m_value == 260);
	TEST_ASSERT(tree.find(26)->m_value == 260);
}

void
test_ErasePatterns() {
	sl::RbTree<int, int> tree;

	for (int i = 0; i < 256; i++)
		tree.add(i, i + 1);

	TEST_ASSERT(tree.getCount() == 256);

	for (int i = 0; i < 256; i += 2)
		TEST_ASSERT(tree.eraseKey(i));

	TEST_ASSERT(tree.getCount() == 128);

	for (int i = 0; i < 256; i++) {
		sl::RbTree<int, int>::Iterator it = tree.find(i);
		TEST_ASSERT((it != NULL) == (i & 1));
		if (it)
			TEST_ASSERT(it->m_value == i + 1);
	}

	for (int i = 1; i < 256; i += 2)
		TEST_ASSERT(tree.eraseKey(i));

	TEST_ASSERT(tree.isEmpty());
	TEST_ASSERT(!tree.eraseKey(1));
}

void
test_Randomized() {
	enum {
		KeyCount = 4096,
		IterationCount = 20000,
	};

	sl::RbTree<uint_t, uint_t> tree;
	bool isPresent[KeyCount] = {};
	uint_t valueArray[KeyCount] = {};
	uint_t expectedCount = 0;
	uint32_t seed = 0x24681357;

	for (uint_t i = 0; i < IterationCount; i++) {
		seed = seed * 1664525 + 1013904223;
		uint_t key = (seed >> 8) & (KeyCount - 1);

		if (seed & 1) {
			uint_t value = seed ^ i;
			sl::RbTree<uint_t, uint_t>::Iterator it = tree.add(key, value);
			TEST_ASSERT(it);
			if (!isPresent[key]) {
				isPresent[key] = true;
				expectedCount++;
			}
			valueArray[key] = value;
		} else {
			bool result = tree.eraseKey(key);
			TEST_ASSERT(result == isPresent[key]);
			if (isPresent[key]) {
				isPresent[key] = false;
				expectedCount--;
			}
		}

		TEST_ASSERT(tree.getCount() == expectedCount);

#ifdef _AXL_DEBUG
		// full O(n) structural check -- periodic, so the debug run stays quick
		if ((i & 0xff) == 0)
			tree.assertValid();
#endif

		seed = seed * 1664525 + 1013904223;
		key = (seed >> 8) & (KeyCount - 1);
		sl::RbTree<uint_t, uint_t>::Iterator it = tree.find(key);
		TEST_ASSERT((it != NULL) == isPresent[key]);
		if (it)
			TEST_ASSERT(it->m_value == valueArray[key]);
	}

	uint_t prev = 0;
	bool hasPrev = false;
	sl::RbTree<uint_t, uint_t>::Iterator it = tree.getHead();
	for (; it; it++) {
		TEST_ASSERT(isPresent[it->getKey()]);
		TEST_ASSERT(it->m_value == valueArray[it->getKey()]);
		if (hasPrev)
			TEST_ASSERT(prev < it->getKey());
		prev = it->getKey();
		hasPrev = true;
	}

#ifdef _AXL_DEBUG
	tree.assertValid();
#endif
}

void
run() {
	test_IteratorConversions();
	test_BasicOrderAndLookup();
	test_ErasePatterns();
	test_Randomized();
}

//..............................................................................

ADD_TEST_CASE("test_RbTree", run)

} // namespace
