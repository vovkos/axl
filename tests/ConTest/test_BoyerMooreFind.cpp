#include "pch.h"

#include <stdint.h>
#include <stdlib.h>

namespace test_BoyerMooreFind { 

//.............................................................................

void run ()
{
	rtl::TextBoyerMooreFind find;

	char haystack [] = "hui govno i muravei";
	char needle [] = "MUR";

	find.setPattern (
		needle, 
		lengthof (needle),
		rtl::TextBoyerMooreFind::Flag_Reverse | rtl::TextBoyerMooreFind::Flag_CaseInsensitive
		);
	
	size_t result = -1;
	rtl::TextBoyerMooreFind::IncrementalContext incrementalContext;
#if 0
	for (size_t i = 0; i < lengthof (haystack); i++)
	{
		result = find.find (&incrementalContext, haystack + i, 1);
		if (result != -1)
			break;
	}
#else
	for (intptr_t i = lengthof (haystack) - 1; i >= 0; i--)
	{
		result = find.find (&incrementalContext, haystack + i, 1);
		if (result != -1)
			break;
	}
#endif

	printf ("result = %d\n", result);
}

//.............................................................................

} // namespace test_Vso
