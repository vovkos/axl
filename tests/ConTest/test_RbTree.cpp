#include "pch.h"

namespace test_RbTree {

//.............................................................................

struct Base: rtl::ListLink
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
	rtl::Iterator <Derived> itDerived;
	rtl::Iterator <Base> itBase;

	itBase = itDerived;

	rtl::RbTreeMap <int, int> tree;
	rtl::RbTreeMap <int, int>::Iterator it;

	for (size_t i = 0; i < 50; i++)
		tree.visit (rand () % 50);

	it = tree.getHead ();
	for (; it; it++)
		printf ("%d\n", it->m_key);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
		tree.eraseByKey (rand () % 50);

	it = tree.getHead ();
	for (; it; it++)
		printf ("%d\n", it->m_key);

	printf (".........\n");
}


//.............................................................................

} // namespace test_RbTree