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

void test_IntHashTable ()
{
	sl::HashTableMap <int, int, sl::HashId <int> > map;
	sl::HashTableMapIterator <int, int> it;

	for (size_t i = 0; i < 50; i++)
	{
		int key = rand () % 50;
		it = map.visit (key);
		if (it->m_value)
			ASSERT (it->m_value == key * 10);

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
		if (!it)
		{
			printf ("%d not found\n", key);
		}
		else
		{
			printf ("%d = %d\n", key, it->m_value);
			ASSERT (it->m_value == key * 10);
		}
	}

	printf (".........\n");
}

void test_StringHashTableIgnoreCase ()
{
	static char stringTable [] [80] =
	{
		"Multi-function",      // LIBUSB_CLASS_PER_INTERFACE = 0,
		"Audio",               // LIBUSB_CLASS_AUDIO = 1,
		"Communication",       // LIBUSB_CLASS_COMM = 2,
		"HID",                 // LIBUSB_CLASS_HID = 3,
		"Unknown",             // 4
		"Physical",            // LIBUSB_CLASS_PHYSICAL = 5,
		"Image",               // LIBUSB_CLASS_IMAGE = 6,
		"Printer",             // LIBUSB_CLASS_PRINTER = 7,
		"Mass Storage",        // LIBUSB_CLASS_MASS_STORAGE = 8,
		"USB HUB",             // LIBUSB_CLASS_HUB = 9,
		"Data",                // LIBUSB_CLASS_DATA = 10,
		"Smart Card",          // LIBUSB_CLASS_SMART_CARD = 0x0b,
		"Unknown",             // 0x0c,
		"Content Security",    // LIBUSB_CLASS_CONTENT_SECURITY = 0x0d,
		"Video",               // LIBUSB_CLASS_VIDEO = 0x0e,
		"Personal Healthcare", // LIBUSB_CLASS_PERSONAL_HEALTHCARE = 0x0f,
	};

	sl::StringHashTableMapIgnoreCase_pcu <int> map_pcu;
	sl::StringHashTableMapIgnoreCase_pcp <int> map_pcp;
	
	sl::StringHashTableMapIterator <int> it;
	for (size_t i = 0; i < countof (stringTable); i++)
	{
		it = map_pcu.visit (stringTable [i]);
		it->m_value = strlen (stringTable [i]);

		it = map_pcp.visit (stringTable [i]);
		it->m_value = strlen (stringTable [i]);
	}

	for (size_t i = 0; i < countof (stringTable); i++)
	{
		size_t length = strlen (stringTable [i]);
		for (size_t j = 0; j < length; j++)
			stringTable [i] [j] = (rand () & 1) ? tolower (stringTable [i] [j]) : toupper (stringTable [i] [j]);

		printf ("map_pcu: looking for %s...", stringTable [i]);
		it = map_pcu.find (stringTable [i]);
		ASSERT (it && it->m_value == length);

		printf ("found\n");

		printf ("map_pcp: looking for %s...", stringTable [i]);
		it = map_pcp.find (stringTable [i]);
		ASSERT (it && it->m_value == length);

		printf ("found\n");
	}
}

void run ()
{
	test_IntHashTable ();
	test_StringHashTableIgnoreCase ();
}

//..............................................................................

ADD_TEST_CASE ("test_HashTable", run)

}
