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

template <
	typename Key,
	typename Value
>
class MapIteratorX: public sl::Iterator<sl::MapEntry<Key, Value> > {
public:
	MapIteratorX() {}

	template <typename T>
	MapIteratorX(const sl::Iterator<T>& src) {
		this->m_p = src.getEntry();
	}
};

void
run() {
	sl::Iterator<Derived> itDerived;
	sl::Iterator<Base> itBase;

	itBase = itDerived;

	sl::RbTree<int, int> tree;
	sl::RbTree<int, int>::Iterator it;

	MapIteratorX<int, int> it2 = tree.find(100);

	it2 = it;

	for (size_t i = 0; i < 50; i++)
		tree.visit(rand() % 50);

	it = tree.getHead();
	for (; it; it++)
		printf("%d\n", it->getKey ());

	printf(".........\n");

	for (size_t i = 0; i < 50; i++)
		tree.eraseKey(rand() % 50);

	it = tree.getHead();
	for (; it; it++)
		printf("%d\n", it->getKey ());

	printf(".........\n");
}


//..............................................................................

ADD_TEST_CASE("test_RbTree", run)

}
