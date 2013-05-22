#include "pch.h"

using namespace axl;

namespace test_StringTemplate { 

//.............................................................................

void
Run (const char* pFileName)
{
	bool Result;

	io::CMappedFile File;
	Result = File.Open (pFileName, io::EFileFlag_ReadOnly);
	if (!Result)
	{
		printf ("%s\n", err::GetError ()->GetDescription ());
		return;
	}

	char* p = (char*) File.View ();
	size_t Size = (size_t) File.GetSize ();
	
	lua::CStringTemplate St;

	rtl::CString ResultString;
	Result = St.Process (&ResultString, pFileName, p, Size);
	if (!Result)
	{
		printf ("error processing string template: %s\n", err::GetError ()->GetDescription ());
		return;
	}

	printf ("result string:\n%s\n", ResultString);
}

//.............................................................................

} // namespace test_StringTemplate
