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
#include "test.h"

namespace {

//..............................................................................

void
run ()
{
	bool result;

	const char* fileName = "test.txt.in";

	io::MappedFile file;
	result = file.open (fileName, io::FileFlag_ReadOnly);
	if (!result)
	{
		printf ("error: %s\n", err::getLastErrorDescription ().sz ());
		return;
	}

	char* p = (char*) file.view ();
	size_t size = (size_t) file.getSize ();

	st::LuaStringTemplate st;

	sl::String resultString;
	result = st.process (&resultString, fileName, sl::StringRef (p, size));
	if (!result)
	{
		printf ("error processing string template: %s\n", err::getLastErrorDescription ().sz ());
		return;
	}

	printf ("result string:\n%s\n", resultString.sz ());
}

//..............................................................................

ADD_TEST_CASE ("test_StringTemplate", run)

} // namespace test_StringTemplate
