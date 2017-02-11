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

void run ()
{
	sl::HashTableMap <int, int, sl::HashId <int> > map;
	sl::Iterator <sl::HashTableMapEntry <int, int> > it;

	for (size_t i = 0; i < 50; i++)
	{
		int key = rand () % 50;
		it = map.visit (key);
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

//..............................................................................

ADD_TEST_CASE ("test_HashTable", run)

}
