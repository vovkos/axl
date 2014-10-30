#include "pch.h"

#include <stdint.h>
#include <stdlib.h>

namespace test_BoyerMooreFind { 

//.............................................................................

void run ()
{
	rtl::TextBoyerMooreFind find;

	char haystack [] = "hui govno i muravei";
	char needle [] = "MURavei";

	uint_t flags = 
		rtl::TextBoyerMooreFind::Flag_Reverse | 
//		rtl::TextBoyerMooreFind::Flag_CaseInsensitive |
		rtl::TextBoyerMooreFind::Flag_WholeWord |
		0;

	find.setPattern (needle, lengthof (needle), flags);
	
	size_t result = -1;
	rtl::TextBoyerMooreFind::IncrementalContext incrementalContext;

#if 0
	for (size_t i = 0; i < lengthof (haystack); i++)
	{
		result = find.find (&incrementalContext, i, haystack + i, 1);
		if (result != -1)
			break;
	}
#elif 1
	for (intptr_t i = lengthof (haystack) - 1; i >= 0; i--)
	{
		result = find.find (&incrementalContext, i, haystack + i, 1);
		if (result != -1)
			break;
	}
#else
	result = find.find (haystack, lengthof (haystack));
#endif

	printf ("result = %d\n", result);
}

//.............................................................................

} // namespace test_Vso
