#include "pch.h"

using namespace axl;

namespace test_StringTemplate { 

//.............................................................................

void
run (const char* fileName)
{
	bool result;

	io::CMappedFile file;
	result = file.open (fileName, io::EFileFlag_ReadOnly);
	if (!result)
	{
		printf ("%s\n", err::getError ()->getDescription ());
		return;
	}

	char* p = (char*) file.view ();
	size_t size = (size_t) file.getSize ();
	
	lua::CStringTemplate st;

	rtl::CString resultString;
	result = st.process (&resultString, fileName, p, size);
	if (!result)
	{
		printf ("error processing string template: %s\n", err::getError ()->getDescription ());
		return;
	}

	printf ("result string:\n%s\n", resultString);
}

//.............................................................................

} // namespace test_StringTemplate
