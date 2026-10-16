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

namespace {

//..............................................................................

struct LowBitsHash {
	size_t
	operator () (uint_t key) const {
		return key & 7;
	}
};

void
test_IntHashTable() {
	sl::SimpleHashTable<int, int> map;

	AXL_TEST_ASSERT(map.isEmpty());
	AXL_TEST_ASSERT(map.getCount() == 0);
	AXL_TEST_ASSERT(!map.find(10));
	AXL_TEST_ASSERT(map.findValue(10, -1) == -1);

	for (int i = 0; i < 1000; i++) {
		bool isNew = false;
		sl::SimpleHashTable<int, int>::Iterator it = map.add(i, i * 10, &isNew);
		AXL_TEST_ASSERT(it && isNew);
		AXL_TEST_ASSERT(it->getKey() == i);
		AXL_TEST_ASSERT(it->m_value == i * 10);
	}

	AXL_TEST_ASSERT(!map.isEmpty());
	AXL_TEST_ASSERT(map.getCount() == 1000);

	for (int i = 0; i < 1000; i++) {
		sl::SimpleHashTable<int, int>::Iterator it = map.find(i);
		AXL_TEST_ASSERT(it && it->m_value == i * 10);
	}

	for (int i = 1000; i < 1200; i++)
		AXL_TEST_ASSERT(!map.find(i));

	for (int i = 0; i < 1000; i += 3) {
		bool isNew = true;
		sl::SimpleHashTable<int, int>::Iterator it = map.add(i, i * 20, &isNew);
		AXL_TEST_ASSERT(it && !isNew);
		AXL_TEST_ASSERT(it->m_value == i * 20);
	}

	AXL_TEST_ASSERT(map.getCount() == 1000);

	for (int i = 0; i < 1000; i++) {
		sl::SimpleHashTable<int, int>::Iterator it = map.find(i);
		AXL_TEST_ASSERT(it);
		AXL_TEST_ASSERT(it->m_value == (i % 3 ? i * 10 : i * 20));
	}

	AXL_TEST_ASSERT(!map.addIfNotExists(30, 3000));
	AXL_TEST_ASSERT(map.getCount() == 1000);

	sl::SimpleHashTable<int, int>::Iterator it = map.addIfNotExists(2000, 20000);
	AXL_TEST_ASSERT(it && it->m_value == 20000);
	AXL_TEST_ASSERT(map.getCount() == 1001);

	for (int i = 0; i < 1000; i += 2)
		AXL_TEST_ASSERT(map.eraseKey(i));

	for (int i = 0; i < 1000; i++) {
		sl::SimpleHashTable<int, int>::Iterator it2 = map.find(i);
		if (i % 2)
			AXL_TEST_ASSERT(it2);
		else
			AXL_TEST_ASSERT(!it2);
	}

	AXL_TEST_ASSERT(!map.eraseKey(0));
	AXL_TEST_ASSERT(map.eraseKey(2000));
	AXL_TEST_ASSERT(map.getCount() == 500);

	map.clear();
	AXL_TEST_ASSERT(map.isEmpty());
	AXL_TEST_ASSERT(map.getCount() == 0);
	AXL_TEST_ASSERT(!map.find(1));
}

void
test_Collisions() {
	sl::HashTable<uint_t, uint_t, LowBitsHash> map;

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::HashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.add(key, i);
		AXL_TEST_ASSERT(it && it->m_value == i);
	}

	AXL_TEST_ASSERT(map.getCount() == 2048);

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::HashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.find(key);
		AXL_TEST_ASSERT(it && it->m_value == i);
	}

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + ((i + 1) & 7);
		AXL_TEST_ASSERT(!map.find(key));
	}

	for (uint_t i = 0; i < 2048; i += 5) {
		uint_t key = i * 8 + (i & 7);
		AXL_TEST_ASSERT(map.eraseKey(key));
	}

	for (uint_t i = 0; i < 2048; i++) {
		uint_t key = i * 8 + (i & 7);
		sl::HashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.find(key);
		AXL_TEST_ASSERT((it != NULL) == (i % 5 != 0));
		if (it)
			AXL_TEST_ASSERT(it->m_value == i);
	}
}

void
test_Randomized() {
	enum {
		KeyCount = 4096,
		IterationCount = 20000,
	};

	sl::HashTable<uint_t, uint_t, LowBitsHash> map;
	bool isPresent[KeyCount] = {};
	uint_t valueArray[KeyCount] = {};
	uint_t expectedCount = 0;
	uint32_t seed = 0x87654321;

	for (uint_t i = 0; i < IterationCount; i++) {
		seed = seed * 1664525 + 1013904223;
		uint_t key = (seed >> 8) & (KeyCount - 1);

		if (seed & 1) {
			uint_t value = seed ^ i;
			sl::HashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.add(key, value);
			AXL_TEST_ASSERT(it);
			if (!isPresent[key]) {
				isPresent[key] = true;
				expectedCount++;
			}
			valueArray[key] = value;
		} else {
			bool result = map.eraseKey(key);
			AXL_TEST_ASSERT(result == isPresent[key]);
			if (isPresent[key]) {
				isPresent[key] = false;
				expectedCount--;
			}
		}

		AXL_TEST_ASSERT(map.getCount() == expectedCount);

		seed = seed * 1664525 + 1013904223;
		key = (seed >> 8) & (KeyCount - 1);
		sl::HashTable<uint_t, uint_t, LowBitsHash>::Iterator it = map.find(key);
		AXL_TEST_ASSERT((it != NULL) == isPresent[key]);
		if (it)
			AXL_TEST_ASSERT(it->m_value == valueArray[key]);
	}
}

void
test_StringHashTableIgnoreCase() {
	static const char* stringTable[] = {
		"Multi-function",
		"Audio",
		"Communication",
		"HID",
		"Unknown",
		"Physical",
		"Image",
		"Printer",
		"Mass Storage",
		"USB HUB",
		"Data",
		"Smart Card",
		"Content Security",
		"Video",
		"Personal Healthcare",
	};

	sl::StringHashTableIgnoreCase_pcu<int> map_pcu;
	sl::StringHashTableIgnoreCase_pcp<int> map_pcp;

	for (size_t i = 0; i < countof(stringTable); i++) {
		map_pcu.add(stringTable[i], (int)strlen(stringTable[i]));
		map_pcp.add(stringTable[i], (int)strlen(stringTable[i]));
	}

	AXL_TEST_ASSERT(map_pcu.find("multi-FUNCTION")->m_value == 14);
	AXL_TEST_ASSERT(map_pcp.find("multi-FUNCTION")->m_value == 14);
	AXL_TEST_ASSERT(map_pcu.find("usb hub")->m_value == 7);
	AXL_TEST_ASSERT(map_pcp.find("usb hub")->m_value == 7);
	AXL_TEST_ASSERT(!map_pcu.find("definitely absent"));
	AXL_TEST_ASSERT(!map_pcp.find("definitely absent"));
}

void
run() {
	test_IntHashTable();
	test_Collisions();
	test_Randomized();
	test_StringHashTableIgnoreCase();
}

//..............................................................................

AXL_TEST_ADD("test_HashTable", run)

} // namespace
