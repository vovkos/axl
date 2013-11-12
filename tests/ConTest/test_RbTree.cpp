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
Run ()
{
	rtl::CIteratorT <TDerived> ItDerived;
	rtl::CIteratorT <TBase> ItBase;

	ItBase = ItDerived;

	rtl::CRbTreeMapT <int, int> Tree;
	rtl::CRbTreeMapT <int, int>::CIterator It;

	srand (GetTickCount ());

	for (size_t i = 0; i < 50; i++)
		Tree.Goto (rand () % 50);

	It = Tree.GetHead ();
	for (; It; It++)
		printf ("%d\n", It->m_Key);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
		Tree.DeleteByKey (rand () % 50);

	It = Tree.GetHead ();
	for (; It; It++)
		printf ("%d\n", It->m_Key);

	printf (".........\n");
}


//.............................................................................

} // namespace test_RbTree