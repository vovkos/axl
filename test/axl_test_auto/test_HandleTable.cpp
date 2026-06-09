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

void
test_Basic() {
	sl::HandleTable<int> table(0xffffffff);

	uintptr_t h1 = table.add(100);
	uintptr_t h2 = table.add(200);
	uintptr_t h3 = table.add(300);
	uintptr_t h4 = table.add(400);

	TEST_ASSERT(h1 == 0xffffffff);
	TEST_ASSERT(h2 == 0x100000000);
	TEST_ASSERT(h3 == 0x100000001);
	TEST_ASSERT(h4 == 0x100000002);
	TEST_ASSERT(table.getCount() == 4);

	TEST_ASSERT(table.find(h1)->m_value == 100);
	TEST_ASSERT(table.find(h2)->m_value == 200);
	TEST_ASSERT(table.find(h3)->m_value == 300);
	TEST_ASSERT(table.find(h4)->m_value == 400);
	TEST_ASSERT(!table.find(0));

	TEST_ASSERT(table.eraseKey(h2));
	TEST_ASSERT(!table.find(h2));
	TEST_ASSERT(table.getCount() == 3);

	uintptr_t h5 = table.add(h2, 500);
	TEST_ASSERT(h5 == h2);
	TEST_ASSERT(table.find(h5)->m_value == 500);
	TEST_ASSERT(table.getCount() == 4);

	uintptr_t h6 = table.add(h2, 600);
	TEST_ASSERT(h6 != h2);
	TEST_ASSERT(table.find(h2)->m_value == 500);
	TEST_ASSERT(table.find(h6)->m_value == 600);
	TEST_ASSERT(table.getCount() == 5);
}

void
test_ResetAndZero() {
	sl::HandleTable<int> table(0);

	uintptr_t h1 = table.add(10);
	uintptr_t h2 = table.add(20);

	TEST_ASSERT(h1 == 1);
	TEST_ASSERT(h2 == 2);
	TEST_ASSERT(table.find(h1)->m_value == 10);
	TEST_ASSERT(table.find(h2)->m_value == 20);

	table.reset(100);
	TEST_ASSERT(table.isEmpty());
	TEST_ASSERT(!table.find(h1));

	uintptr_t h3 = table.add(30);
	TEST_ASSERT(h3 == 100);
	TEST_ASSERT(table.getCount() == 1);
	TEST_ASSERT(table.find(h3)->m_value == 30);
}

void
test_Randomized() {
	enum {
		KeyCount = 4096,
		IterationCount = 20000,
	};

	sl::HandleTable<uint_t> table(1);
	bool isPresent[KeyCount] = {};
	uint_t valueArray[KeyCount] = {};
	uint_t expectedCount = 0;
	uint32_t seed = 0x13572468;

	for (uint_t i = 0; i < IterationCount; i++) {
		seed = seed * 1664525 + 1013904223;
		uintptr_t key = ((seed >> 8) & (KeyCount - 1)) + 1;

		if (seed & 1) {
			uint_t value = seed ^ i;
			uintptr_t handle = table.add(key, value);
			TEST_ASSERT(handle);
			if (handle < KeyCount + 1 && !isPresent[handle - 1]) {
				isPresent[handle - 1] = true;
				expectedCount++;
			}
			if (handle < KeyCount + 1)
				valueArray[handle - 1] = value;
		} else {
			bool result = table.eraseKey(key);
			TEST_ASSERT(result == isPresent[key - 1]);
			if (isPresent[key - 1]) {
				isPresent[key - 1] = false;
				expectedCount--;
			}
		}

		TEST_ASSERT(table.getCount() >= expectedCount);

		seed = seed * 1664525 + 1013904223;
		key = ((seed >> 8) & (KeyCount - 1)) + 1;
		sl::HandleTable<uint_t>::Iterator it = table.find(key);
		TEST_ASSERT((it != NULL) == isPresent[key - 1]);
		if (it)
			TEST_ASSERT(it->m_value == valueArray[key - 1]);
	}
}

void
run() {
	test_Basic();
	test_ResetAndZero();
	test_Randomized();
}

//..............................................................................

ADD_TEST_CASE("test_HandleTable", run)

} // namespace
