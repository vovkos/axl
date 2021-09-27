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
run() {
	printf("hui govno i muravei\n");

	sl::HandleTable<int> handleTable(0xffffffff);

	uintptr_t h1 = handleTable.add(100);
	uintptr_t h2 = handleTable.add(200);
	uintptr_t h3 = handleTable.add(300);
	uintptr_t h4 = handleTable.add(400);

	sl::HandleTableIterator<int> it = handleTable.getHead();
	for (; it; it++) {
		printf("0x%p -> %d\n", it->getKey (), it->m_value);
	}
}

//..............................................................................

ADD_TEST_CASE("test_HandleTable", run)

}
