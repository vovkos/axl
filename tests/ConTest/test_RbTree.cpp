#include "pch.h"

using namespace axl;

namespace test_RbTree {

//.............................................................................

struct TBase: rtl::TListLink
{
	int a, b;
};

struct TDerived: TBase
{
	int c, d;
};

void 
run ()
{
	rtl::CIteratorT <TDerived> itDerived;
	rtl::CIteratorT <TBase> itBase;

	itBase = itDerived;

	rtl::CRbTreeMapT <int, int> tree;
	rtl::CRbTreeMapT <int, int>::CIterator it;

	srand (getTickCount ());

	for (size_t i = 0; i < 50; i++)
		tree.goto (rand () % 50);

	it = tree.getHead ();
	for (; it; it++)
		printf ("%d\n", it->m_key);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
		tree.deleteByKey (rand () % 50);

	it = tree.getHead ();
	for (; it; it++)
		printf ("%d\n", it->m_key);

	printf (".........\n");
}


//.............................................................................

} // namespace test_RbTree