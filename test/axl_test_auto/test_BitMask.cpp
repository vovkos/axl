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
	uint64_t lo64[][2] = {
		{ 0,  0x0000000000000000LL },
		{ 4,  0x000000000000000fLL },
		{ 8,  0x00000000000000ffLL },
		{ 12, 0x0000000000000fffLL },
		{ 16, 0x000000000000ffffLL },
		{ 31, 0x000000007fffffffLL },
		{ 32, 0x00000000ffffffffLL },
		{ 52, 0x000fffffffffffffLL },
		{ 56, 0x00ffffffffffffffLL },
		{ 60, 0x0fffffffffffffffLL },
		{ 63, 0x7fffffffffffffffLL },
		{ 64, 0xffffffffffffffffLL },
	};

	for (size_t i = 0; i < countof(lo64); i++) {
		uint64_t shift = lo64[i][0];
		uint64_t expected = lo64[i][1];
		uint64_t result = sl::getLoBitmask64(shift);
		printf("getLoBitmask64(%llu): 0x%016llx\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint64_t hi64[][2] = {
		{ 0,  0xffffffffffffffffLL },
		{ 4,  0xfffffffffffffff0LL },
		{ 8,  0xffffffffffffff00LL },
		{ 12, 0xfffffffffffff000LL },
		{ 16, 0xffffffffffff0000LL },
		{ 31, 0xffffffff80000000LL },
		{ 32, 0xffffffff00000000LL },
		{ 52, 0xfff0000000000000LL },
		{ 56, 0xff00000000000000LL },
		{ 60, 0xf000000000000000LL },
		{ 63, 0x8000000000000000LL },
		{ 64, 0x0000000000000000LL },
	};

	for (size_t i = 0; i < countof(hi64); i++) {
		uint64_t shift = hi64[i][0];
		uint64_t expected = hi64[i][1];
		uint64_t result = sl::getHiBitmask64(shift);
		printf("getHiBitmask64(%llu): 0x%016llx\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint32_t lo32[][2] = {
		{ 0,  0x00000000 },
		{ 4,  0x0000000f },
		{ 8,  0x000000ff },
		{ 12, 0x00000fff },
		{ 16, 0x0000ffff },
		{ 31, 0x7fffffff },
		{ 32, 0xffffffff },
	};

	for (size_t i = 0; i < countof(lo32); i++) {
		uint32_t shift = lo32[i][0];
		uint32_t expected = lo32[i][1];
		uint32_t result = sl::getLoBitmask32(shift);
		printf("getLoBitmask32(%llu): 0x%08x\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint32_t hi32[][2] = {
		{ 0,  0xffffffff },
		{ 4,  0xfffffff0 },
		{ 8,  0xffffff00 },
		{ 12, 0xfffff000 },
		{ 16, 0xffff0000 },
		{ 31, 0x80000000 },
		{ 32, 0x00000000 },
	};

	for (size_t i = 0; i < countof(hi32); i++) {
		uint64_t shift = hi32[i][0];
		uint64_t expected = hi32[i][1];
		uint64_t result = sl::getHiBitmask32(shift);
		printf("getHiBitmask32(%llu): 0x%08x\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint16_t lo16[][2] = {
		{ 0,  0x0000 },
		{ 4,  0x000f },
		{ 8,  0x00ff },
		{ 12, 0x0fff },
		{ 16, 0xffff },
	};

	for (size_t i = 0; i < countof(lo16); i++) {
		uint16_t shift = lo16[i][0];
		uint16_t expected = lo16[i][1];
		uint16_t result = sl::getLoBitmask16(shift);
		printf("getLoBitmask16(%llu): 0x%08x\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint16_t hi16[][2] = {
		{ 0,  0xffff },
		{ 4,  0xfff0 },
		{ 8,  0xff00 },
		{ 12, 0xf000 },
		{ 16, 0x0000 },
	};

	for (size_t i = 0; i < countof(hi16); i++) {
		uint64_t shift = hi16[i][0];
		uint64_t expected = hi16[i][1];
		uint64_t result = sl::getHiBitmask16(shift);
		printf("getHiBitmask16(%llu): 0x%08x\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint8_t lo8[][2] = {
		{ 0, 0x00 },
		{ 3, 0x07 },
		{ 4, 0x0f },
		{ 7, 0x7f },
		{ 8, 0xff },
	};

	for (size_t i = 0; i < countof(lo8); i++) {
		uint8_t shift = lo8[i][0];
		uint8_t expected = lo8[i][1];
		uint8_t result = sl::getLoBitmask8(shift);
		printf("getLoBitmask8(%llu): 0x%08x\n", shift, result);
		TEST_ASSERT(result == expected);
	}

	uint8_t hi8[][2] = {
		{ 0, 0xff },
		{ 3, 0xf8 },
		{ 4, 0xf0 },
		{ 7, 0x80 },
		{ 8, 0x00 },
	};

	for (size_t i = 0; i < countof(hi8); i++) {
		uint64_t shift = hi8[i][0];
		uint64_t expected = hi8[i][1];
		uint64_t result = sl::getHiBitmask8(shift);
		printf("getHiBitmask8(%llu): 0x%08x\n", shift, result);
		TEST_ASSERT(result == expected);
	}
}

//..............................................................................

ADD_TEST_CASE("test_BitMask", run)

}
