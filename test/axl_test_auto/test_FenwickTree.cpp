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

struct Counters {
	uint64_t m_lineCount;
	uint64_t m_zoneCount;

	Counters():
		m_lineCount(0),
		m_zoneCount(0) {}

	Counters(
		uint64_t lineCount,
		uint64_t zoneCount
	):
		m_lineCount(lineCount),
		m_zoneCount(zoneCount) {}

	Counters&
	operator += (const Counters& src) {
		m_lineCount += src.m_lineCount;
		m_zoneCount += src.m_zoneCount;
		return *this;
	}

	Counters
	operator - () const {
		return Counters(0 - m_lineCount, 0 - m_zoneCount);
	}
};

// per-counter sum types: constructible from the aggregated Counters (extracts
// one counter), += Counters for calcPrefixStat, </<= and -= for findGe, and ==
// for find()'s exact-prefix test

struct LineCount {
	uint64_t m_value;

	LineCount(uint64_t value = 0):
		m_value(value) {}

	LineCount(const Counters& counters):
		m_value(counters.m_lineCount) {}

	LineCount&
	operator += (const Counters& src) {
		m_value += src.m_lineCount;
		return *this;
	}

	LineCount&
	operator -= (const LineCount& src) {
		m_value -= src.m_value;
		return *this;
	}

	bool
	operator == (const LineCount& src) const {
		return m_value == src.m_value;
	}

	bool
	operator < (const LineCount& src) const {
		return m_value < src.m_value;
	}

	bool
	operator <= (const LineCount& src) const {
		return m_value <= src.m_value;
	}
};

struct ZoneCount {
	uint64_t m_value;

	ZoneCount(uint64_t value = 0):
		m_value(value) {}

	ZoneCount(const Counters& counters):
		m_value(counters.m_zoneCount) {}

	ZoneCount&
	operator += (const Counters& src) {
		m_value += src.m_zoneCount;
		return *this;
	}

	ZoneCount&
	operator -= (const ZoneCount& src) {
		m_value -= src.m_value;
		return *this;
	}

	bool
	operator == (const ZoneCount& src) const {
		return m_value == src.m_value;
	}

	bool
	operator < (const ZoneCount& src) const {
		return m_value < src.m_value;
	}

	bool
	operator <= (const ZoneCount& src) const {
		return m_value <= src.m_value;
	}
};

typedef sl::FenwickTree<Counters> FenwickTree;

uint32_t
lcg(uint32_t* seed) {
	*seed = *seed * 1664525 + 1013904223;
	return *seed >> 16;
}

// verifies calcPrefixStat at every index (inclusive semantics) and findGe
// every target from 0 through past the total, against the reference element
// array; findGe(target) = smallest index whose inclusive cumulative sum is
// >= target (ties from zero-count elements resolve to the first index), -1 if
// target exceeds the total; on success, *remainder = target minus the
// cumulative sum before the found element

void
verifyTree(
	const FenwickTree& tree,
	const sl::Array<Counters>& reference
) {
	size_t count = reference.getCount();
	TEST_ASSERT(tree.getCount() == count);

	uint64_t lineCount = 0;
	uint64_t zoneCount = 0;
	for (size_t i = 0; i < count; i++) {
		lineCount += reference[i].m_lineCount;
		zoneCount += reference[i].m_zoneCount;
		TEST_ASSERT(tree.calcPrefixStat<LineCount>(i).m_value == lineCount);
		TEST_ASSERT(tree.calcPrefixStat<ZoneCount>(i).m_value == zoneCount);

		// calcSelfStat must reproduce the element itself, not a partial sum

		TEST_ASSERT(tree.calcSelfStat<LineCount>(i).m_value == reference[i].m_lineCount);
		TEST_ASSERT(tree.calcSelfStat<ZoneCount>(i).m_value == reference[i].m_zoneCount);
	}

	size_t modelIndex = 0;  // smallest index with cumulative line count >= target
	uint64_t modelSum = 0;  // cumulative line count before modelIndex
	for (uint64_t target = 0; target <= lineCount + 2; target++) {
		while (
			modelIndex < count &&
			modelSum + reference[modelIndex].m_lineCount < target
		) {
			modelSum += reference[modelIndex].m_lineCount;
			modelIndex++;
		}

		LineCount remainder(-1);
		size_t findIndex = tree.findGe<LineCount>(LineCount(target), &remainder);
		// find(): the same index, but ONLY on an exact inclusive prefix

		size_t exactIndex = tree.findEq<LineCount>(LineCount(target));

		if (modelIndex < count) {
			TEST_ASSERT(findIndex == modelIndex);
			TEST_ASSERT(remainder.m_value == target - modelSum);

			uint64_t inclusive = modelSum + reference[modelIndex].m_lineCount;
			TEST_ASSERT(exactIndex == (inclusive == target ? modelIndex : -1));
		} else {
			TEST_ASSERT(findIndex == -1); // target > total
			TEST_ASSERT(exactIndex == -1);
		}
	}
}

void
test_Empty() {
	FenwickTree tree;
	TEST_ASSERT(tree.getCount() == 0);
	TEST_ASSERT(tree.isEmpty());

	size_t index = tree.findGe<LineCount>(LineCount(7));
	TEST_ASSERT(index == -1);
}

// scalar value type exercising the non-template calcPrefixStat/findGe
// wrappers

void
test_Scalar() {
	enum {
		ElementCount = 64,
	};

	uint32_t seed = 0x64536455;

	sl::FenwickTree<uint64_t> tree;
	sl::Array<uint64_t> reference;

	for (size_t i = 0; i < ElementCount; i++) {
		uint64_t value = lcg(&seed) % 4;
		tree.append(value);
		reference.append(value);
	}

	tree.inc(3, 7);
	reference.rwi()[3] += 7;

	tree.dec(3, 5);
	reference.rwi()[3] -= 5;

	uint64_t total = 0;
	for (size_t i = 0; i < ElementCount; i++) {
		total += reference[i];
		TEST_ASSERT(tree.calcPrefixStat(i) == total);
	}

	size_t modelIndex = 0;  // smallest index with cumulative sum >= target
	uint64_t modelSum = 0;  // cumulative sum before modelIndex
	for (uint64_t target = 0; target <= total + 2; target++) {
		while (
			modelIndex < ElementCount &&
			modelSum + reference[modelIndex] < target
		) {
			modelSum += reference[modelIndex];
			modelIndex++;
		}

		uint64_t remainder = -1;
		size_t findIndex = tree.findGe(target, &remainder);
		if (modelIndex < ElementCount) {
			TEST_ASSERT(findIndex == modelIndex);
			TEST_ASSERT(remainder == target - modelSum);
		} else {
			TEST_ASSERT(findIndex == -1); // target > total
		}
	}
}

// exhaustively covers the boundary math of the implicit tree at every count
// crossing power-of-2 boundaries (where node coverage ranges change shape)

void
test_SmallCountSweep() {
	enum {
		MaxElementCount = 65,
	};

	uint32_t seed = 0x18273645;

	FenwickTree tree;
	sl::Array<Counters> reference;

	for (size_t count = 0; count <= MaxElementCount; count++) {
		tree.clear();
		reference.clear();

		for (size_t i = 0; i < count; i++) {
			Counters counters(lcg(&seed) % 4, lcg(&seed) % 3); // zeros are frequent
			size_t index = tree.append(counters);
			TEST_ASSERT(index == i);
			reference.append(counters);
		}

		verifyTree(tree, reference);
	}
}

void
test_Randomized() {
	enum {
		ElementCount = 1000,
		UpdateCount  = 1000,
	};

	uint32_t seed = 0x36455445;

	FenwickTree tree;
	sl::Array<Counters> reference;

	for (size_t i = 0; i < ElementCount; i++) {
		Counters counters(lcg(&seed) % 8, lcg(&seed) % 4);
		size_t index = tree.append(counters);
		TEST_ASSERT(index == i);
		reference.append(counters);
	}

	verifyTree(tree, reference);

	for (size_t i = 0; i < UpdateCount; i++) {
		size_t index = lcg(&seed) % ElementCount;
		Counters delta(lcg(&seed) % 8, lcg(&seed) % 4);
		tree.inc(index, delta);
		reference.rwi()[index] += delta;

		if ((i & 0xff) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);
}

// appends interleaved with increments -- append must aggregate previously
// incremented nodes correctly

void
test_InterleavedAppendInc() {
	enum {
		IterationCount = 500,
	};

	uint32_t seed = 0x45544536;

	FenwickTree tree;
	sl::Array<Counters> reference;

	for (size_t i = 0; i < IterationCount; i++) {
		Counters counters(lcg(&seed) % 4, lcg(&seed) % 3);
		tree.append(counters);
		reference.append(counters);

		size_t index = lcg(&seed) % reference.getCount();
		Counters delta(lcg(&seed) % 4, lcg(&seed) % 3);
		tree.inc(index, delta);
		reference.rwi()[index] += delta;

		if ((i & 0x3f) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);
}

// dec must exactly undo inc (counters are unsigned; intermediate per-element
// values never go negative here, only deltas are negated)

void
test_IncDecRoundTrip() {
	enum {
		ElementCount = 100,
		UpdateCount  = 200,
	};

	uint32_t seed = 0x54453645;

	FenwickTree tree;
	sl::Array<Counters> reference;

	for (size_t i = 0; i < ElementCount; i++) {
		Counters counters(lcg(&seed) % 8, lcg(&seed) % 4);
		tree.append(counters);
		reference.append(counters);
	}

	sl::Array<size_t> updateIndexes;
	sl::Array<Counters> updateDeltas;

	for (size_t i = 0; i < UpdateCount; i++) {
		size_t index = lcg(&seed) % ElementCount;
		Counters delta(lcg(&seed) % 8, lcg(&seed) % 4);
		tree.inc(index, delta);
		updateIndexes.append(index);
		updateDeltas.append(delta);
	}

	for (size_t i = UpdateCount; i; i--)
		tree.dec(updateIndexes[i - 1], updateDeltas[i - 1]);

	verifyTree(tree, reference); // back to the pre-update state
}

void
run() {
	test_Empty();
	test_Scalar();
	test_SmallCountSweep();
	test_Randomized();
	test_InterleavedAppendInc();
	test_IncDecRoundTrip();
}

//..............................................................................

ADD_TEST_CASE("test_FenwickTree", run)

} // namespace
