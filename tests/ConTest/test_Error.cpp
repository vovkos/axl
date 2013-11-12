#include "pch.h"

using namespace axl;

namespace test_Error { 

//.............................................................................

void 
Run ()
{
	err::CError Error;
	
	Error.Pack <rtl::CPackSeqT_4 <
		const char*,
		int,
		const char*,
		int
		> > (
		rtl::GUID_Null, 
		12345, 
		"hui", 0xdead, "govno", 0xbeaf
		);

	err::CError Error2;
	Error2.Format (
		rtl::GUID_Null, 
		12345, 
		"%s %d %s %d",
		"hui", 0xdead, "govno", 0xbeaf
		);

	printf ("...\n");
}


//.............................................................................

} // namespace test_Error
