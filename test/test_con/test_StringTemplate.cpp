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

namespace test_StringTemplate {

//..............................................................................

void
run (const char* fileName)
{
	bool result;

	io::MappedFile file;
	result = file.open (fileName, io::FileFlag_ReadOnly);
	if (!result)
	{
		printf ("%s\n", err::getError ()->getDescription ());
		return;
	}

	char* p = (char*) file.view ();
	size_t size = (size_t) file.getSize ();

	lua::StringTemplate st;

	sl::String resultString;
	result = st.process (&resultString, fileName, p, size);
	if (!result)
	{
		printf ("error processing string template: %s\n", err::getError ()->getDescription ());
		return;
	}

	printf ("result string:\n%s\n", resultString);
}

//..............................................................................

} // namespace test_StringTemplate
