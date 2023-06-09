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

struct MyStruct {
	sl::ListLink m_Link1;
	sl::ListLink m_Link2;

	int m_a;
	int m_b;
	int m_c;
};

struct MyStruct2: sl::ListLink {
	int m_x;
	int m_y;
	int m_z;
};

void run() {
	sl::List<
		MyStruct,
		sl::Offset<MyStruct, sl::ListLink, offsetof(MyStruct, m_Link1) >,
		mem::Delete<MyStruct>
	> list1;

	sl::AuxList<
		MyStruct,
		sl::Offset<MyStruct, sl::ListLink, offsetof(MyStruct, m_Link2) >
	> list2;

	MyStruct* p = new MyStruct;
	list1.insertHead(p);
	list2.insertHead(p);

	p = new MyStruct;
	list1.insertHead(p);
	list2.insertHead(p);

	sl::List<
		MyStruct2,
		sl::ExplicitCast<MyStruct2*, sl::ListLink*>,
		mem::Delete<MyStruct2>
	> list3;

	MyStruct2* p2 = new MyStruct2;
	list3.insertTail(p2);

	sl::BoxList<int> boxList;
	boxList.insertTail(10);
	boxList.insertHead(-10);
	boxList.insertTail(20);
	boxList.insertHead(-20);

	boxList.removeHead();

	sl::BoxIterator<int> it = boxList.getHead();
	for (; it; it++) {
		printf("%d\n", *it);
	}
};

//..............................................................................

ADD_TEST_CASE("test_List", run)

}
