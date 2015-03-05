#include "pch.h"
#include "axl_fsm_RegExp.h"
#include "axl_rtl_AutoPtrArray.h"

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)
int
wmain (
	int argc,
	wchar_t* argv []
	)
#else
int
main (
	int argc,
	char* argv []
	)
#endif
{
	printf ("main ()\n");

	char const* src [] = 
	{
		"'A'",
		"'B'",
		".",
	};

	fsm::RegExp regExp;	
	fsm::RegExpCompiler regExpCompiler (&regExp);

	for (size_t i = 0; i < countof (src); i++)
	{
		bool result = regExpCompiler.incrementalCompile (src [i]);
		if (!result)
		{
			printf ("error: %s\n", err::getLastError ().getDescription ().cc ());
			return -1;
		}
	}

	regExpCompiler.finalize ();

	regExp.print ();
	return 0;
}

//.............................................................................