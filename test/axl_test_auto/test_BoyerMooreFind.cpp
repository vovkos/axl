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

void run() {
#if (0)
	sl::TextBoyerMooreFind find;

	char haystack[] = "hui govno i muravei";
	char needle[] = "muravei";

	uint_t flags =
//		sl::TextBoyerMooreFlag_Reverse |
//		sl::TextBoyerMooreFlag_CaseInsensitive |
		sl::TextBoyerMooreFlag_WholeWord |
		0;

	find.setPattern(sl::StringRef(needle, lengthof(needle)), flags);

	size_t result = -1;
	sl::TextBoyerMooreFind::IncrementalContext incrementalContext;

#if 1
	for (size_t i = 0; i < lengthof(haystack); i++) {
		result = find.find(&incrementalContext, i, sl::StringRef(haystack + i, 1));
		if (result != -1)
			break;
	}

	if (result == -1 && (flags & sl::TextBoyerMooreFlag_WholeWord) != 0)
		result = find.find(&incrementalContext, lengthof(haystack), sl::StringRef(" ", 1));

#elif 0
	for (intptr_t i = lengthof(haystack) - 1; i >= 0; i--) {
		result = find.find(&incrementalContext, i, haystack + i, 1);
		if (result != -1)
			break;
	}

	if (result == -1 && (flags & sl::TextBoyerMooreFind::Flag_WholeWord) != 0)
		result = find.find(&incrementalContext, -1, " ", 1);
#else
	result = find.find(haystack, lengthof(haystack));
#endif

	printf("result = %d\n", result);

	result = find.find(sl::StringRef(haystack, lengthof(haystack)));
	printf("result = %d\n", result);
#endif
}

//..............................................................................

ADD_TEST_CASE("test_BoyerMooreFind", run)

}
