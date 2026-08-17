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

struct LineCount;
struct ZoneCount;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

	Counters&
	operator -= (const Counters& src) {
		m_lineCount -= src.m_lineCount;
		m_zoneCount -= src.m_zoneCount;
		return *this;
	}

	// the per-counter deltas -- this is the extra surface addSubStat/subSubStat
	// require, and the whole point of them: one counter moves and the other is
	// not even read. no unary minus anywhere (subSubStat uses -=, same as
	// StatBTree/StatBinTree)

	Counters&
	operator += (const LineCount& src);

	Counters&
	operator -= (const LineCount& src);

	Counters&
	operator += (const ZoneCount& src);

	Counters&
	operator -= (const ZoneCount& src);
};

// per-counter sum types: constructible from the aggregated Counters (extracts
// one counter), += Counters for calcPrefixSubStat, </<= and -= for the
// findPrefixSubStatGe descent, and == for findPrefixSubStatEq's exact-prefix test

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Counters&
Counters::operator += (const LineCount& src) {
	m_lineCount += src.m_value;
	return *this;
}

inline
Counters&
Counters::operator -= (const LineCount& src) {
	m_lineCount -= src.m_value;
	return *this;
}

inline
Counters&
Counters::operator += (const ZoneCount& src) {
	m_zoneCount += src.m_value;
	return *this;
}

inline
Counters&
Counters::operator -= (const ZoneCount& src) {
	m_zoneCount -= src.m_value;
	return *this;
}

//..............................................................................

typedef sl::FenwickTree<Counters> FenwickTree;

uint32_t
lcg(uint32_t* seed) {
	*seed = *seed * 1664525 + 1013904223;
	return *seed >> 16;
}

// verifies calcPrefixSubStat at every index (inclusive semantics) and
// findPrefixSubStatGe for every target from 0 through past the total, against the
// reference element array; findPrefixSubStatGe(target) = smallest index whose
// inclusive cumulative sum is
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
		TEST_ASSERT(tree.calcPrefixSubStat<LineCount>(i).m_value == lineCount);
		TEST_ASSERT(tree.calcPrefixSubStat<ZoneCount>(i).m_value == zoneCount);

		// calcSelfSubStat must reproduce the element itself, not a partial sum

		TEST_ASSERT(tree.calcSelfSubStat<LineCount>(i).m_value == reference[i].m_lineCount);
		TEST_ASSERT(tree.calcSelfSubStat<ZoneCount>(i).m_value == reference[i].m_zoneCount);
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
		size_t findIndex = tree.findPrefixSubStatGe(LineCount(target), &remainder);
		// findPrefixSubStatEq: the same index, but ONLY on an exact inclusive prefix

		size_t exactIndex = tree.findPrefixSubStatEq(LineCount(target));

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

	size_t index = tree.findPrefixSubStatGe(LineCount(7));
	TEST_ASSERT(index == -1);
}

// scalar value type exercising the non-template calcPrefixStat/findPrefixStatGe
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

	tree.addStat(3, 7);
	reference.rwi()[3] += 7;

	tree.subStat(3, 5);
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
		size_t findIndex = tree.findPrefixStatGe(target, &remainder);
		if (modelIndex < ElementCount) {
			TEST_ASSERT(findIndex == modelIndex);
			TEST_ASSERT(remainder == target - modelSum);
		} else {
			TEST_ASSERT(findIndex == -1); // target > total
		}
	}
}

// a small int literal against counts past INT_MAX: the literal must widen to Stat
// rather than pull the descent down to its own narrower type

void
test_ScalarLiteral() {
	sl::FenwickTree<size_t> tree;

	size_t big = 3000000000ULL; // past INT_MAX, so a descent typed `int` truncates
	tree.append(big);
	tree.append(big);
	tree.append(big);

	TEST_ASSERT(tree.findPrefixStatGe(7) == 0);
	TEST_ASSERT(tree.findPrefixStatGe(4000000000ULL) == 1);
	TEST_ASSERT(tree.findPrefixStatEq(big) == 0);
	TEST_ASSERT(tree.findPrefixStatEq(9000000000ULL) == 2);
	TEST_ASSERT(tree.calcPrefixStat(2) == big * 3);
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
		tree.addStat(index, delta);
		reference.rwi()[index] += delta;

		if ((i & 0xff) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);
}

// appends interleaved with stat updates -- append must aggregate previously
// updated nodes correctly

void
test_InterleavedAppendAddStat() {
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
		tree.addStat(index, delta);
		reference.rwi()[index] += delta;

		if ((i & 0x3f) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);
}

// subStat must exactly undo addStat (counters are unsigned; intermediate
// per-element values never go negative here)

void
test_AddSubRoundTrip() {
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
		tree.addStat(index, delta);
		updateIndexes.append(index);
		updateDeltas.append(delta);
	}

	for (size_t i = UpdateCount; i; i--)
		tree.subStat(updateIndexes[i - 1], updateDeltas[i - 1]);

	verifyTree(tree, reference); // back to the pre-update state
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the projected addSubStat/subSubStat methods: the delta is a
// single counter, so only that counter may move anywhere in the tree -- the
// other one must come out bit-identical. the Counters += LineCount ops are what
// make it laser-focused: the untouched counter is never even read

void
test_SubStatDelta() {
	enum {
		ElementCount = 200,
		UpdateCount  = 400,
	};

	uint32_t seed = 0x5b57a70d;

	FenwickTree tree;
	sl::Array<Counters> reference;

	for (size_t i = 0; i < ElementCount; i++) {
		Counters counters(lcg(&seed) % 8, lcg(&seed) % 4);
		tree.append(counters);
		reference.append(counters);
	}

	verifyTree(tree, reference);

	// line-only deltas: every zone count must survive untouched

	for (size_t i = 0; i < UpdateCount; i++) {
		size_t index = lcg(&seed) % ElementCount;
		uint64_t delta = lcg(&seed) % 8;

		tree.addSubStat(index, LineCount(delta));
		reference.rwi()[index].m_lineCount += delta;

		// shrink it back down by no more than what's there (unsigned counters)

		uint64_t back = reference[index].m_lineCount ?
			lcg(&seed) % reference[index].m_lineCount :
			0;

		tree.subSubStat(index, LineCount(back));
		reference.rwi()[index].m_lineCount -= back;

		if ((i & 0x3f) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);

	// and the other projection, on the same tree

	for (size_t i = 0; i < UpdateCount; i++) {
		size_t index = lcg(&seed) % ElementCount;
		uint64_t delta = lcg(&seed) % 4;

		tree.addSubStat(index, ZoneCount(delta));
		reference.rwi()[index].m_zoneCount += delta;

		uint64_t back = reference[index].m_zoneCount ?
			lcg(&seed) % reference[index].m_zoneCount :
			0;

		tree.subSubStat(index, ZoneCount(back));
		reference.rwi()[index].m_zoneCount -= back;

		if ((i & 0x3f) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);

	// a projected round trip must land back exactly where it started -- this is
	// what catches a subStat that quietly adds (the two loops above would both
	// pass if addStat and subStat were the same op with a matching reference)

	Counters before = tree.calcFullStat();

	for (size_t i = 0; i < UpdateCount; i++) {
		size_t index = lcg(&seed) % ElementCount;
		LineCount delta(lcg(&seed) % 8);

		tree.addSubStat(index, delta);
		tree.subSubStat(index, delta);
	}

	Counters after = tree.calcFullStat();
	TEST_ASSERT(before.m_lineCount == after.m_lineCount);
	TEST_ASSERT(before.m_zoneCount == after.m_zoneCount);
	verifyTree(tree, reference);
}

void
run() {
	test_Empty();
	test_Scalar();
	test_ScalarLiteral();
	test_SmallCountSweep();
	test_Randomized();
	test_InterleavedAppendAddStat();
	test_AddSubRoundTrip();
	test_SubStatDelta();
}

//..............................................................................

ADD_TEST_CASE("test_FenwickTree", run)

} // namespace
