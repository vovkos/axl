#include "pch.h"

namespace test_HandleTable { 

//.............................................................................

void
run ()
{
	printf ("hui govno i muravei\n");

	sl::HandleTable <int> handleTable (0xffffffff);

	handle_t h1 = handleTable.add (100);
	handle_t h2 = handleTable.add (200);
	handle_t h3 = handleTable.add (300);
	handle_t h4 = handleTable.add (400);

	sl::Iterator <sl::HandleTable <int>::Entry> it = handleTable.getList ().getHead ();
	for (; it; it++)
	{
		printf ("0x%x = %d\n", it->getHandle (), it->m_value);
	}
}

//.............................................................................

} // namespace test_HandleTable
