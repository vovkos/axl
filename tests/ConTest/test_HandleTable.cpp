#include "pch.h"

using namespace axl;

namespace test_HandleTable { 

//.............................................................................

void
Run ()
{
	printf ("hui govno i muravei\n");

	rtl::CHandleTableT <int> HandleTable (0xffffffff);

	handle_t h1 = HandleTable.Add (100);
	handle_t h2 = HandleTable.Add (200);
	handle_t h3 = HandleTable.Add (300);
	handle_t h4 = HandleTable.Add (400);

	rtl::CIteratorT <rtl::CHandleTableT <int>::CEntry> It = HandleTable.GetList ().GetHead ();
	for (; It; It++)
	{
		printf ("0x%x = %d\n", It->GetHandle (), It->m_Value);
	}
}

//.............................................................................

} // namespace test_HandleTable
