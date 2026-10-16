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

#pragma once

#define _AXL_TEST_TEST_H

#include "axl_test_Pch.h"

namespace axl {
namespace test {

//..............................................................................

typedef
void
TestFunc();

typedef sl::StringHashTable<TestFunc*> TestSuite;

inline
TestSuite*
getTestSuite() {
	static int32_t flag = 0;
	return sl::getSingleton<TestSuite>(&flag);
}

//..............................................................................

// runs the tests named on the command line (all registered tests if none);
// a test fails by throwing (const char*, sl::StringRef and err::ErrorRef provide a message);
// returns the negated number of failed tests

int
runTests(
	int argc,
	char* argv[]
);

int
runTests(
	int argc,
	wchar_t* argv[]
);

//..............................................................................

} // namespace test
} // namespace axl

//..............................................................................

// registers a test at static-init time (use at namespace scope)

#define AXL_TEST_ADD(name, func) \
	const axl::test::TestSuite::Iterator g_test ## func = axl::test::getTestSuite()->add(name, func);

#define AXL_TEST_ASSERT(e) ((bool) (e) ? \
	(void) 0 : \
	(void)(throw "Assertion failure at: " __FILE__ "(" AXL_ITOA(__LINE__) "): " #e))

//..............................................................................
