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
	static size_t ranges[][2] = {
		{ 0,   0 },
		{ 0,   1 },
		{ 0,   2 },
		{ 0,   16 },
		{ 0,   32 },
		{ 0,   64 },
		{ 0,   128 },
		{ 0,   16 - 1 },
		{ 0,   32 - 1 },
		{ 0,   64 - 1 },
		{ 0,   128 - 1 },
		{ 0,   16 - 2 },
		{ 0,   32 - 2 },
		{ 0,   64 - 2 },
		{ 0,   128 - 2 },
		{ 0,   16 + 1 },
		{ 0,   32 + 1 },
		{ 0,   64 + 1 },
		{ 0,   128 + 1 },

		{ 128,  128 },
		{ 128,  128 + 1 },
		{ 128,  128 + 2 },
		{ 128,  128 + 16 },
		{ 128,  128 + 32 },
		{ 128,  128 + 64 },
		{ 128,  128 + 128 },
		{ 128,  128 + 16 - 1 },
		{ 128,  128 + 32 - 1 },
		{ 128,  128 + 64 - 1 },
		{ 128,  128 + 128 - 1 },
		{ 128,  128 + 16 - 2 },
		{ 128,  128 + 32 - 2 },
		{ 128,  128 + 64 - 2 },
		{ 128,  128 + 128 - 2 },
		{ 128,  128 + 16 + 1 },
		{ 128,  128 + 32 + 1 },
		{ 128,  128 + 64 + 1 },
		{ 128,  128 + 128 + 1 },

		{ 128 - 1,  128 - 1 },
		{ 128 - 1,  128 + 1 },
		{ 128 - 1,  128 + 2 },
		{ 128 - 1,  128 + 16 },
		{ 128 - 1,  128 + 32 },
		{ 128 - 1,  128 + 64 },
		{ 128 - 1,  128 + 128 },
		{ 128 - 1,  128 + 16 - 1 },
		{ 128 - 1,  128 + 32 - 1 },
		{ 128 - 1,  128 + 64 - 1 },
		{ 128 - 1,  128 + 128 - 1 },
		{ 128 - 1,  128 + 16 - 2 },
		{ 128 - 1,  128 + 32 - 2 },
		{ 128 - 1,  128 + 64 - 2 },
		{ 128 - 1,  128 + 128 - 2 },
		{ 128 - 1,  128 + 16 + 1 },
		{ 128 - 1,  128 + 32 + 1 },
		{ 128 - 1,  128 + 64 + 1 },
		{ 128 - 1,  128 + 128 + 1 },

		{ 128 - 2,  128 - 2 },
		{ 128 - 2,  128 + 1 },
		{ 128 - 2,  128 + 2 },
		{ 128 - 2,  128 + 16 },
		{ 128 - 2,  128 + 32 },
		{ 128 - 2,  128 + 64 },
		{ 128 - 2,  128 + 128 },
		{ 128 - 2,  128 + 16 - 1 },
		{ 128 - 2,  128 + 32 - 1 },
		{ 128 - 2,  128 + 64 - 1 },
		{ 128 - 2,  128 + 128 - 1 },
		{ 128 - 2,  128 + 16 - 2 },
		{ 128 - 2,  128 + 32 - 2 },
		{ 128 - 2,  128 + 64 - 2 },
		{ 128 - 2,  128 + 128 - 2 },
		{ 128 - 2,  128 + 16 + 1 },
		{ 128 - 2,  128 + 32 + 1 },
		{ 128 - 2,  128 + 64 + 1 },
		{ 128 - 2,  128 + 128 + 1 },

		{ 128 + 1,  128 + 1 },
		{ 128 + 1,  128 + 2 },
		{ 128 + 1,  128 + 16 },
		{ 128 + 1,  128 + 32 },
		{ 128 + 1,  128 + 64 },
		{ 128 + 1,  128 + 128 },
		{ 128 + 1,  128 + 16 - 1 },
		{ 128 + 1,  128 + 32 - 1 },
		{ 128 + 1,  128 + 64 - 1 },
		{ 128 + 1,  128 + 128 - 1 },
		{ 128 + 1,  128 + 16 - 2 },
		{ 128 + 1,  128 + 32 - 2 },
		{ 128 + 1,  128 + 64 - 2 },
		{ 128 + 1,  128 + 128 - 2 },
		{ 128 + 1,  128 + 16 + 1 },
		{ 128 + 1,  128 + 32 + 1 },
		{ 128 + 1,  128 + 64 + 1 },
		{ 128 + 1,  128 + 128 + 1 },

		{ 1,              1 },
		{ 1,              512 },
		{ 2,              512 },
		{ 16,             512 },
		{ 32,             512 },
		{ 64,             512 },
		{ 128,            512 },
		{ 16 - 1,         512 },
		{ 32 - 1,         512 },
		{ 64 - 1,         512 },
		{ 128 - 1,        512 },
		{ 16 - 2,         512 },
		{ 32 - 2,         512 },
		{ 64 - 2,         512 },
		{ 128 - 2,        512 },
		{ 16 + 1,         512 },
		{ 32 + 1,         512 },
		{ 64 + 1,         512 },
		{ 128 + 1,        512 },

		{ 128 + 1,        512 - 1 },
		{ 128 + 2,        512 - 1 },
		{ 128 + 16,       512 - 1 },
		{ 128 + 32,       512 - 1 },
		{ 128 + 64,       512 - 1 },
		{ 128 + 128,      512 - 1 },
		{ 128 + 16 - 1,   512 - 1 },
		{ 128 + 32 - 1,   512 - 1 },
		{ 128 + 64 - 1,   512 - 1 },
		{ 128 + 128 - 1,  512 - 1 },
		{ 128 + 16 - 2,   512 - 1 },
		{ 128 + 32 - 2,   512 - 1 },
		{ 128 + 64 - 2,   512 - 1 },
		{ 128 + 128 - 2,  512 - 1 },
		{ 128 + 16 + 1,   512 - 1 },
		{ 128 + 32 + 1,   512 - 1 },
		{ 128 + 64 + 1,   512 - 1 },
		{ 128 + 128 + 1,  512 - 1 },

		{ 128 + 1,        512 - 2 },
		{ 128 + 2,        512 - 2 },
		{ 128 + 16,       512 - 2 },
		{ 128 + 32,       512 - 2 },
		{ 128 + 64,       512 - 2 },
		{ 128 + 128,      512 - 2 },
		{ 128 + 16 - 1,   512 - 2 },
		{ 128 + 32 - 1,   512 - 2 },
		{ 128 + 64 - 1,   512 - 2 },
		{ 128 + 128 - 1,  512 - 2 },
		{ 128 + 16 - 2,   512 - 2 },
		{ 128 + 32 - 2,   512 - 2 },
		{ 128 + 64 - 2,   512 - 2 },
		{ 128 + 128 - 2,  512 - 2 },
		{ 128 + 16 + 1,   512 - 2 },
		{ 128 + 32 + 1,   512 - 2 },
		{ 128 + 64 + 1,   512 - 2 },
		{ 128 + 128 + 1,  512 - 2 },
	};

	static size_t shifts[] = {
		0,
		1,
		2,

		16,
		32,
		64,
		128,

		16 - 1,
		32 - 1,
		64 - 1,
		128 - 1,

		16 - 2,
		32 - 2,
		64 - 2,
		128 - 2,

		16 + 1,
		32 + 1,
		64 + 1,
		128 + 1,
	};

	enum {
		Overshoot = 128
	};

	for (size_t i = 0; i < countof(ranges); i++) {
		size_t from = ranges[i][0];
		size_t to = ranges[i][1];
		ASSERT(from <= to);

		printf("from: %d to %d\n", from, to);

		sl::BitMap bitMap;
		bitMap.setBitRangeResize(from, to);

		sl::BitMap bitMapInv;
		bitMapInv.ensureBitCount(to + Overshoot + 1);
		bitMapInv.invert();
		bitMapInv.clearBitRange(from, to);

		for (size_t j = 0; j < to + Overshoot; j++) {
			bool b = bitMap.getBit(j);
			bool b2 = bitMapInv.getBit(j);
			TEST_ASSERT(b == !b2);

			bool isInside = j >= from && j < to;
			TEST_ASSERT(b && isInside || !b && !isInside);
		}

		if (from >= to) {
			TEST_ASSERT(bitMap.findBit() == -1);
			TEST_ASSERT(bitMap.findBitReverse() == -1);
			TEST_ASSERT(bitMapInv.findZeroBit() == -1);
			TEST_ASSERT(bitMapInv.findZeroBitReverse() == -1);
		} else  {
			TEST_ASSERT(bitMap.findBit() == from);
			TEST_ASSERT(bitMap.findBitReverse() == to - 1);
			TEST_ASSERT(bitMapInv.findZeroBit() == from);
			TEST_ASSERT(bitMapInv.findZeroBitReverse() == to - 1);
		}

		printf("  SHR\n");
		for (size_t l = 0; l < countof(shifts); l++) {
			size_t shift = shifts[l];
			printf("    >> %d\n", shift);

			from = ranges[i][0];
			to = ranges[i][1];
			from = from > shift ? from - shift : 0;
			to = to > shift ? to - shift : 0;

			sl::BitMap bitMapShr = bitMap;
			bitMapShr.shr(shift);

			for (size_t j = 0; j < to + Overshoot; j++) {
				bool b = bitMapShr.getBit(j);
				bool isInside = j >= from && j < to;
				TEST_ASSERT(b && isInside || !b && !isInside);
			}

			if (from >= to || to == 0) { // all shifted out
				TEST_ASSERT(bitMapShr.findBit() == -1);
				TEST_ASSERT(bitMapShr.findBitReverse() == - 1);
			} else {
				TEST_ASSERT(bitMapShr.findBit() == from);
				TEST_ASSERT(bitMapShr.findBitReverse() == to - 1);
			}
		}

		printf("  SHL\n");
		for (size_t l = 0; l < countof(shifts); l++) {
			size_t shift = shifts[l];
			printf("    << %d\n", shift);

			from = ranges[i][0] + shift;
			to = ranges[i][1] + shift;

			sl::BitMap bitMapShl = bitMap;
			bitMapShl.shlResize(shift);

			for (size_t j = 0; j < to + Overshoot; j++) {
				bool b = bitMapShl.getBit(j);
				bool isInside = j >= from && j < to;
				ASSERT(b && isInside || !b && !isInside);
			}

			if (from >= to) {
				TEST_ASSERT(bitMapShl.findBit() == -1);
				TEST_ASSERT(bitMapShl.findBitReverse() == -1);
			} else {
				TEST_ASSERT(bitMapShl.findBit() == from);
				TEST_ASSERT(bitMapShl.findBitReverse() == to - 1);
			}
		}
	}
}

//..............................................................................

ADD_TEST_CASE("test_BitMap", run)

}
