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

sl::StringRef
ref(const char* p) {
	return sl::StringRef(p, strlen(p));
}

size_t
naiveFind(
	const char* haystack,
	size_t haystackLength,
	const char* needle,
	size_t needleLength
) {
	if (needleLength > haystackLength)
		return -1;

	for (size_t i = 0; i <= haystackLength - needleLength; i++)
		if (!memcmp(haystack + i, needle, needleLength))
			return i;

	return -1;
}

size_t
naiveReverseFind(
	const char* haystack,
	size_t haystackLength,
	const char* needle,
	size_t needleLength
) {
	if (needleLength > haystackLength)
		return -1;

	for (size_t i = haystackLength - needleLength + 1; i > 0; i--)
		if (!memcmp(haystack + i - 1, needle, needleLength))
			return i - 1;

	return -1;
}

template <typename Find>
void
testBinForwardOne(
	const char* haystack,
	size_t haystackLength,
	const char* needle,
	size_t needleLength
) {
	Find find(needle, needleLength);
	TEST_ASSERT(find.find(haystack, haystackLength) == naiveFind(haystack, haystackLength, needle, needleLength));
}

template <typename Find>
void
testBinReverseOne(
	const char* haystack,
	size_t haystackLength,
	const char* needle,
	size_t needleLength
) {
	Find find(needle, needleLength);
	size_t expected = naiveReverseFind(haystack, haystackLength, needle, needleLength);
	size_t result = find.find(haystack, haystackLength);
	TEST_ASSERT(result == (expected == -1 ? (size_t)-1 : haystackLength - expected - needleLength));
}

template <typename Find>
void
testBinIncrementalForwardOne(
	const char* haystack,
	size_t haystackLength,
	const char* needle,
	size_t needleLength,
	size_t chunkSize
) {
	Find find(needle, needleLength);
	sl::BoyerMooreBinState state(needleLength);
	size_t expected = naiveFind(haystack, haystackLength, needle, needleLength);
	size_t result = -1;
	size_t offset = 0;
	while (offset < haystackLength) {
		size_t size = AXL_MIN(chunkSize, haystackLength - offset);
		result = find.find(&state, haystack + offset, size);
		if (result != -1)
			break;

		offset += size;
	}

	TEST_ASSERT(result == expected);
}

template <typename Find>
void
testBinIncrementalReverseOne(
	const char* haystack,
	size_t haystackLength,
	const char* needle,
	size_t needleLength,
	size_t chunkSize
) {
	Find find(needle, needleLength);
	sl::BoyerMooreBinState state(needleLength);
	size_t expected = naiveReverseFind(haystack, haystackLength, needle, needleLength);
	expected = expected == -1 ? (size_t)-1 : haystackLength - expected - needleLength;

	size_t result = -1;
	size_t end = haystackLength;
	while (end) {
		size_t size = AXL_MIN(chunkSize, end);
		result = find.find(&state, haystack + end - size, size);
		if (result != -1)
			break;

		end -= size;
	}

	TEST_ASSERT(result == expected);
}

void
test_BinDeterministic() {
	static const char haystack[] = "ababa aba xyz aba";
	static const char overlap[] = "aba";
	static const char middle[] = "xyz";
	static const char absent[] = "nope";
	static const char single[] = "a";
	static const char full[] = "ababa aba xyz aba";

	testBinForwardOne<sl::BoyerMooreHorspoolBinFind>(haystack, strlen(haystack), overlap, strlen(overlap));
	testBinForwardOne<sl::BoyerMooreBinFind>(haystack, strlen(haystack), overlap, strlen(overlap));
	testBinForwardOne<sl::BoyerMooreBinFind>(haystack, strlen(haystack), middle, strlen(middle));
	testBinForwardOne<sl::BoyerMooreBinFind>(haystack, strlen(haystack), absent, strlen(absent));
	testBinForwardOne<sl::BoyerMooreBinFind>(haystack, strlen(haystack), single, strlen(single));
	testBinForwardOne<sl::BoyerMooreBinFind>(haystack, strlen(haystack), full, strlen(full));

	testBinReverseOne<sl::BoyerMooreHorspoolReverseBinFind>(haystack, strlen(haystack), overlap, strlen(overlap));
	testBinReverseOne<sl::BoyerMooreReverseBinFind>(haystack, strlen(haystack), overlap, strlen(overlap));
	testBinReverseOne<sl::BoyerMooreReverseBinFind>(haystack, strlen(haystack), middle, strlen(middle));
	testBinReverseOne<sl::BoyerMooreReverseBinFind>(haystack, strlen(haystack), absent, strlen(absent));
	testBinReverseOne<sl::BoyerMooreReverseBinFind>(haystack, strlen(haystack), full, strlen(full));
}

void
test_BinIncremental() {
	static const char haystack[] = "0123456789--needle--0123456789";
	static const char needle[] = "needle";

	for (size_t chunkSize = 1; chunkSize <= strlen(needle) + 2; chunkSize++) {
		testBinIncrementalForwardOne<sl::BoyerMooreBinFind>(haystack, strlen(haystack), needle, strlen(needle), chunkSize);
		testBinIncrementalForwardOne<sl::BoyerMooreHorspoolBinFind>(haystack, strlen(haystack), needle, strlen(needle), chunkSize);
		testBinIncrementalReverseOne<sl::BoyerMooreReverseBinFind>(haystack, strlen(haystack), needle, strlen(needle), chunkSize);
		testBinIncrementalReverseOne<sl::BoyerMooreHorspoolReverseBinFind>(haystack, strlen(haystack), needle, strlen(needle), chunkSize);
	}
}

void
test_BinRandomized() {
	enum {
		HaystackLength = 96,
		MaxNeedleLength = 12,
		IterationCount = 2000,
	};

	char haystack[HaystackLength];
	char needle[MaxNeedleLength];
	uint32_t seed = 0x2a77c0de;

	for (size_t i = 0; i < IterationCount; i++) {
		for (size_t j = 0; j < HaystackLength; j++) {
			seed = seed * 1664525 + 1013904223;
			haystack[j] = 'a' + ((seed >> 24) % 5);
		}

		seed = seed * 1664525 + 1013904223;
		size_t needleLength = 1 + ((seed >> 24) % MaxNeedleLength);

		if (seed & 1) {
			size_t offset = (seed >> 8) % (HaystackLength - needleLength + 1);
			memcpy(needle, haystack + offset, needleLength);
		} else {
			for (size_t j = 0; j < needleLength; j++) {
				seed = seed * 1664525 + 1013904223;
				needle[j] = 'a' + ((seed >> 24) % 6);
			}
		}

		testBinForwardOne<sl::BoyerMooreBinFind>(haystack, HaystackLength, needle, needleLength);
		testBinForwardOne<sl::BoyerMooreHorspoolBinFind>(haystack, HaystackLength, needle, needleLength);
		testBinReverseOne<sl::BoyerMooreReverseBinFind>(haystack, HaystackLength, needle, needleLength);
		testBinReverseOne<sl::BoyerMooreHorspoolReverseBinFind>(haystack, HaystackLength, needle, needleLength);
	}
}

void
test_BinSkipTraps() {
	// regression: a proper suffix matches, then the mismatch falls on a position whose
	// character recurs to its right. keying the bad-skip on the mismatch char (instead of
	// the window-end char) yields a non-positive shift here -- the old Horspool spun forever.
	// naiveFind also pins down correctness, not merely termination.

	static const struct {
		const char* m_haystack;
		const char* m_needle;
	} caseTable[] = {
		{ "aab",    "bab"   }, // suffix "ab" matches, mismatch at 0 ('a' recurs right); absent
		{ "aabab",  "bab"   }, // same trap, but the needle is present (at 2)
		{ "bbc",    "cbc"   }, // suffix "bc" matches, mismatch at 0 ('b' recurs right); absent
		{ "bbcbc",  "cbc"   }, // present at 2
		{ "aax",    "xax"   }, // suffix "ax" matches, mismatch at 0 ('a' recurs right); absent
		{ "aaxax",  "xax"   }, // present at 2
		{ "aabab",  "abab"  }, // 4-char needle, present at 1
		{ "ababab", "babab" }, // deep partial matches, present at 1
	};

	for (size_t i = 0; i < countof(caseTable); i++) {
		const char* haystack = caseTable[i].m_haystack;
		const char* needle = caseTable[i].m_needle;
		size_t haystackLength = strlen(haystack);
		size_t needleLength = strlen(needle);

		testBinForwardOne<sl::BoyerMooreHorspoolBinFind>(haystack, haystackLength, needle, needleLength);
		testBinForwardOne<sl::BoyerMooreBinFind>(haystack, haystackLength, needle, needleLength);
		testBinReverseOne<sl::BoyerMooreHorspoolReverseBinFind>(haystack, haystackLength, needle, needleLength);
		testBinReverseOne<sl::BoyerMooreReverseBinFind>(haystack, haystackLength, needle, needleLength);
	}
}

void
test_TextBasic() {
	sl::BoyerMooreTextFind find(ref("aba"));
	sl::BoyerMooreTextFindResult result = find.find("xx aba ababa", strlen("xx aba ababa"));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cpOffset == 3);
	TEST_ASSERT(result.m_cuOffset == 3);
	TEST_ASSERT(result.m_cuEndOffset == 6);

	result = find.find("no match", strlen("no match"));
	TEST_ASSERT(!result.isValid());

	sl::BoyerMooreReverseTextFind reverseFind(ref("aba"));
	result = reverseFind.find("xx aba ababa", strlen("xx aba ababa"));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(strlen("xx aba ababa") - strlen("aba") - result.m_cuOffset == 9);
}

void
test_TextIncremental() {
	static const char haystack[] = "0123456789needle";
	static const char needle[] = "needle";

	sl::BoyerMooreTextFind find(ref(needle));
	for (size_t chunkSize = 1; chunkSize <= strlen(needle) + 2; chunkSize++) {
		sl::BoyerMooreTextState state(ref(needle));
		sl::BoyerMooreTextFindResult result;
		size_t offset = 0;
		while (offset < strlen(haystack)) {
			size_t size = AXL_MIN(chunkSize, strlen(haystack) - offset);
			result = find.find(&state, haystack + offset, size);
			if (result.isValid())
				break;

			offset += size;
		}

		TEST_ASSERT(result.isValid());
		TEST_ASSERT(result.m_cuOffset == 10);
		TEST_ASSERT(result.m_cuEndOffset == 16);
	}
}

void
test_TextCaseFoldedWholeWord() {
	sl::BoyerMooreCaseFoldedTextFind caseFoldedFind(ref("MurA"));
	sl::BoyerMooreTextFindResult result = caseFoldedFind.find("xx mura yy", strlen("xx mura yy"));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cuOffset == 3);

	sl::BoyerMooreWholeWordTextFind wholeWordFind(ref("cat"));
	result = wholeWordFind.find("bobcat cat cat1 cat", strlen("bobcat cat cat1 cat"));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cuOffset == 7);

	result = wholeWordFind.find("bobcat cat1 scat", strlen("bobcat cat1 scat"));
	TEST_ASSERT(!result.isValid());

	sl::BoyerMooreWholeWordTextFind eofFind(ref("cat"));
	sl::BoyerMooreTextState state(ref("cat"));
	result = eofFind.find(&state, "xx cat", strlen("xx cat"));
	TEST_ASSERT(!result.isValid());
	result = eofFind.eof(&state);
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cuOffset == 3);
}

void
test_TextUtf8Offsets() {
	static const char haystack[] = "a \xd0\xb6\xd1\x83\xd0\xba b";
	static const char needle[] = "\xd0\xb6\xd1\x83\xd0\xba";

	sl::BoyerMooreTextFind find(ref(needle));
	sl::BoyerMooreTextFindResult result = find.find(haystack, strlen(haystack));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cpOffset == 2);
	TEST_ASSERT(result.m_cuOffset == 2);
	TEST_ASSERT(result.m_cuEndOffset == 8);

	for (size_t chunkSize = 1; chunkSize <= 4; chunkSize++) {
		sl::BoyerMooreTextState state(ref(needle));
		result = sl::BoyerMooreTextFindResult();
		size_t offset = 0;
		while (offset < strlen(haystack)) {
			size_t size = AXL_MIN(chunkSize, strlen(haystack) - offset);
			result = find.find(&state, haystack + offset, size);
			if (result.isValid())
				break;

			offset += size;
		}

		TEST_ASSERT(result.isValid());
		TEST_ASSERT(result.m_cpOffset == 2);
		TEST_ASSERT(result.m_cuOffset == 2);
		TEST_ASSERT(result.m_cuEndOffset == 8);
	}
}

void
test_TextUtf16Offsets() {
	static const utf16_t haystack[] = { 'a', ' ', 0x0436, 0x0443, 0x043a, ' ', 'b' };
	static const utf32_t needle[] = { 0x0436, 0x0443, 0x043a };

	sl::BoyerMooreTextFind_utf16 find(sl::StringRef_utf32(needle, countof(needle)));
	sl::BoyerMooreTextFindResult result = find.find(haystack, countof(haystack));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cpOffset == 2);
	TEST_ASSERT(result.m_cuOffset == 2);
	TEST_ASSERT(result.m_cuEndOffset == 5);

	for (size_t chunkSize = 1; chunkSize <= 3; chunkSize++) {
		sl::BoyerMooreTextState_utf16 state(sl::StringRef_utf32(needle, countof(needle)));
		result = sl::BoyerMooreTextFindResult();
		size_t offset = 0;
		while (offset < countof(haystack)) {
			size_t size = AXL_MIN(chunkSize, countof(haystack) - offset);
			result = find.find(&state, haystack + offset, size);
			if (result.isValid())
				break;

			offset += size;
		}

		TEST_ASSERT(result.isValid());
		TEST_ASSERT(result.m_cpOffset == 2);
		TEST_ASSERT(result.m_cuOffset == 2);
		TEST_ASSERT(result.m_cuEndOffset == 5);
	}
}

void
test_TextUtf32Offsets() {
	static const utf32_t haystack[] = { 'a', ' ', 0x1f642, 'x', 0x1f642, ' ' };
	static const utf32_t needle[] = { 0x1f642, 'x' };

	sl::BoyerMooreTextFind_utf32 find(sl::StringRef_utf32(needle, countof(needle)));
	sl::BoyerMooreTextFindResult result = find.find(haystack, countof(haystack));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(result.m_cpOffset == 2);
	TEST_ASSERT(result.m_cuOffset == 2);
	TEST_ASSERT(result.m_cuEndOffset == 4);

	sl::BoyerMooreReverseTextFind_utf32 reverseFind(sl::StringRef_utf32(needle, countof(needle)));
	result = reverseFind.find(haystack, countof(haystack));
	TEST_ASSERT(result.isValid());
	TEST_ASSERT(countof(haystack) - countof(needle) - result.m_cuOffset == 2);
}

void
run() {
	test_BinDeterministic();
	test_BinIncremental();
	test_BinRandomized();
	test_BinSkipTraps();
	test_TextBasic();
	test_TextIncremental();
	test_TextCaseFoldedWholeWord();
	test_TextUtf8Offsets();
	test_TextUtf16Offsets();
	test_TextUtf32Offsets();
}

//..............................................................................

ADD_TEST_CASE("test_BoyerMooreFind", run)

} // namespace
