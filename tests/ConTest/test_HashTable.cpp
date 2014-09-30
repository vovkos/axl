#include "pch.h"

using namespace axl;

namespace test_HashTable { 

//.............................................................................

void run ()
{
	rtl::CHashTableMapT <int, int, rtl::CHashIdT <int> > map;
	rtl::CIteratorT <rtl::CHashTableMapEntryT <int, int> > it;

	srand (getTickCount ());

	for (size_t i = 0; i < 50; i++)
	{
		int key = rand () % 50;
		it = map.goto (key);
		it->m_value = key * 10;
	}

	it = map.getHead ();
	for (; it; it++)
		printf ("%d = %d\n", it->m_key, it->m_value);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
	{
		int key = rand () % 50;
		it = map.find (key);
		if (it)
			printf ("%d = %d\n", key, it->m_value);
		else
			printf ("%d not found\n", key);
	}

	printf (".........\n");
}

//.............................................................................

} // namespace test_HashTable
