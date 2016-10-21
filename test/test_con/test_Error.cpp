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

namespace test_Error {

//..............................................................................

void
run ()
{
	err::Error error;

	error.pack <sl::PackSeq_4 <
		const char*,
		int,
		const char*,
		int
		> > (
		sl::GUID_Null,
		12345,
		"hui", 0xdead, "govno", 0xbeaf
		);

	err::Error error2;
	error2.format (
		sl::GUID_Null,
		12345,
		"%s %d %s %d",
		"hui", 0xdead, "govno", 0xbeaf
		);

	printf ("...\n");
}


//..............................................................................

} // namespace test_Error
