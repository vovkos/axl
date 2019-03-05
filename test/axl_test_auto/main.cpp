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

//.............................................................................

bool
runTest(
	size_t index,
	size_t total,
	const char* name,
	TestFunc* func
	)
{
	printf(
		"Test %d/%d: %s\n"
		"...................................................................\n",
		index,
		total,
		name
		);

	bool result = true;
	try
	{
		func();
	}
	catch(const char* message)
	{
		printf("%s\n", message);
		result = false;
	}
	catch(...)
	{
		result = false;
	}

	printf(result ? "\n>>> Passed\n\n" : "\n>>> FAILED\n\n");
	return result;
}

//.............................................................................

#if (_AXL_OS_WIN)
int
wmain(
	int argc,
	wchar_t* argv[]
	)
#else
int
main(
	int argc,
	char* argv[]
	)
#endif
{
#if (_AXL_OS_POSIX)
	setvbuf(stdout, NULL, _IOLBF, 1024);
#endif

#if (_AXL_OS_WIN)
	WSADATA wsaData;
	WORD versionRequested = MAKEWORD(2, 2);
	WSAStartup(versionRequested, &wsaData);
#endif
	srand((uint_t)sys::getTimestamp());

	g::getModule()->setTag("axl_test_auto");
	lex::registerParseErrorProvider();

	size_t total;
	size_t passed = 0;

	if (argc < 2) // run all
	{
		total = getTestCaseSet()->getCount();
		sl::StringHashTableIterator<TestFunc*> it = getTestCaseSet()->getHead();
		for (size_t i = 0; it; it++, i++)
			passed += runTest(i, total, it->getKey(), it->m_value);
	}
	else
	{
		total = argc - 1;
		sl::String name;

		for (int i = 1; i < argc; i++)
		{
			name = argv[i];
			sl::StringHashTableIterator<TestFunc*> it = getTestCaseSet()->find(name);
			if (it)
				passed += runTest(i, total, it->getKey(), it->m_value);
			else
				printf("TEST NOT FOUND: %s\n", name.sz());
		}
	}

	printf("%d%% tests passed, %d tests failed out of %d\n",
		total ? passed * 100 / total : 0,
		total - passed,
		total
		);

	return passed - total; // return negation of the number of failed tests
}

//..............................................................................
