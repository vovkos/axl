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
	ref::Ptr<mem::Block> package = sl::formatPackage(
		"%s %d %s %d",
		"hui", 10, "govno", 20
		);

	sl::Unpacker unpacker(package->m_p, package->m_size);

	const char* s1;
	long l1;
	char* s2;
	long l2;

	unpacker.unpack(&s1);
	unpacker.unpack(&l1);
	unpacker.unpack(&s2);
	unpacker.unpack(&l2);

	printf("%s %d %s %d\n", s1, l1, s2, l2);
}

//..............................................................................

ADD_TEST_CASE("test_Packer", run)

}
