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

//..............................................................................

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
	srand((uint_t)sys::getTimestamp());

#if (_AXL_OS_WIN)
	WSADATA wsaData;
	WORD versionRequested = MAKEWORD(2, 2);
	WSAStartup(versionRequested, &wsaData);
#endif
	lex::registerParseErrorProvider();

	return test::runTests(argc, argv);
}

//..............................................................................
