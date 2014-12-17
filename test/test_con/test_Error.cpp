#include "pch.h"

namespace test_Error { 

//.............................................................................

void 
run ()
{
	err::Error error;
	
	error.pack <rtl::PackSeq_4 <
		const char*,
		int,
		const char*,
		int
		> > (
		rtl::GUID_Null, 
		12345, 
		"hui", 0xdead, "govno", 0xbeaf
		);

	err::Error error2;
	error2.format (
		rtl::GUID_Null, 
		12345, 
		"%s %d %s %d",
		"hui", 0xdead, "govno", 0xbeaf
		);

	printf ("...\n");
}


//.............................................................................

} // namespace test_Error