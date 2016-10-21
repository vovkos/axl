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

namespace test_RbTree {

//..............................................................................

struct Base: sl::ListLink
{
	int a, b;
};

struct Derived: Base
{
	int c, d;
};

void
run ()
{
	sl::Iterator <Derived> itDerived;
	sl::Iterator <Base> itBase;

	itBase = itDerived;

	sl::RbTreeMap <int, int> tree;
	sl::RbTreeMap <int, int>::Iterator it;

	for (size_t i = 0; i < 50; i++)
		tree.visit (rand () % 50);

	it = tree.getHead ();
	for (; it; it++)
		printf ("%d\n", it->m_key);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
		tree.eraseKey (rand () % 50);

	it = tree.getHead ();
	for (; it; it++)
		printf ("%d\n", it->m_key);

	printf (".........\n");
}


//..............................................................................

} // namespace test_RbTree
