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
#include "axl_test_Test.h"

namespace axl {
namespace test {

//..............................................................................

static
bool
runTest(
	size_t index,
	size_t total,
	const sl::StringRef& name,
	TestFunc* func
) {
	printf(
		"Test %d/%d: %s\n"
		"...................................................................\n",
		index,
		total,
		name.sz()
	);

	bool result = true;
	uint64_t timestamp = sys::getPreciseTimestamp();

	try {
		func();
	} catch (const char* message) {
		printf("%s\n", message);
		result = false;
	} catch (const sl::StringRef& message) {
		printf("%s\n", message.sz());
		result = false;
	} catch (const err::ErrorRef& error) {
		printf("%s\n", error.getDescription().sz());
		result = false;
	} catch (...) {
		printf("unknown exception caught\n");
		result = false;
	}

	printf(
		"\n>>> %s (%.3f ms)\n\n",
		result ? "Passed" : "FAILED",
		(double)(sys::getPreciseTimestamp() - timestamp) / 10000 // timestamps are in 100-nanosecond intervals
	);

	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename C>
static
int
runTestsImpl(
	int argc,
	C* argv[]
) {
	size_t total;
	size_t passed = 0;
	uint64_t timestamp = sys::getPreciseTimestamp();

	const TestSuite* testSuite = getTestSuite();

	if (argc < 2) { // run all
		total = testSuite->getCount();
		sl::ConstStringHashTableIterator<TestFunc*> it = testSuite->getHead();
		for (size_t i = 0; it; it++, i++)
			passed += runTest(i + 1, total, it->getKey(), it->m_value);
	} else {
		total = argc - 1;
		sl::String name;

		for (int i = 1; i < argc; i++) {
			name = argv[i];
			TestFunc* func = testSuite->findValue(name, NULL);
			if (!func) {
				printf("TEST NOT FOUND: %s\n", name.sz());
				continue;
			}

			passed += runTest(i, total, name, func);
		}
	}

	printf(
		"%d%% tests passed, %d tests failed out of %d (%.3f ms)\n",
		total ? passed * 100 / total : 0,
		total - passed,
		total,
		(double)(sys::getPreciseTimestamp() - timestamp) / 10000
	);

	return passed - total;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int
runTests(
	int argc,
	char* argv[]
) {
	return runTestsImpl(argc, argv);
}

int
runTests(
	int argc,
	wchar_t* argv[]
) {
	return runTestsImpl(argc, argv);
}

//..............................................................................

} // namespace test
} // namespace axl
