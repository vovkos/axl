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
run()
{
	err::Error error;

	error.pack<sl::PackSeq_4<
		const char*,
		int,
		const char*,
		int
		> > (
		sl::g_nullGuid,
		12345,
		"hui", 0xdead, "govno", 0xbeaf
		);

	sl::String s = enc::HexEncoding::encode(error.cp(), error.getSize());
	printf("error: %s\n", s.sz ());

	err::Error error2;
	error2.format(
		sl::g_nullGuid,
		12345,
		"%s %d %s %d",
		"hui", 0xdead, "govno", 0xbeaf
		);

	s = enc::HexEncoding::encode(error.cp(), error.getSize());
	printf("error: %s\n", s.sz ());
}

//..............................................................................

ADD_TEST_CASE("test_Error", run)

}
