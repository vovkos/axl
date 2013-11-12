#include "pch.h"

using namespace axl;

namespace test_HashTable { 

//.............................................................................

void Run ()
{
	rtl::CHashTableMapT <int, int, rtl::CHashIdT <int> > Map;
	rtl::CIteratorT <rtl::CHashTableMapEntryT <int, int> > It;

	srand (GetTickCount ());

	for (size_t i = 0; i < 50; i++)
	{
		int Key = rand () % 50;
		It = Map.Goto (Key);
		It->m_Value = Key * 10;
	}

	It = Map.GetHead ();
	for (; It; It++)
		printf ("%d = %d\n", It->m_Key, It->m_Value);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
	{
		int Key = rand () % 50;
		It = Map.Find (Key);
		if (It)
			printf ("%d = %d\n", Key, It->m_Value);
		else
			printf ("%d not found\n", Key);
	}

	printf (".........\n");
}

//.............................................................................

} // namespace test_HashTable
