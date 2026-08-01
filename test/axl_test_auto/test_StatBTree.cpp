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

// Span exercises the T-vs-Stat split: m_id is payload that is NOT summable
// (and must survive every split/merge intact), the two counters are the
// summable part

struct Span {
	uint_t m_id;
	uint64_t m_lineCount;
	uint64_t m_charCount;

	Span():
		m_id(0),
		m_lineCount(0),
		m_charCount(0) {}

	Span(
		uint_t id,
		uint64_t lineCount,
		uint64_t charCount
	):
		m_id(id),
		m_lineCount(lineCount),
		m_charCount(charCount) {}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the aggregate: everything summable in a Span and nothing else. a Stat must
// provide the full arithmetic set -- unary -, binary + and -, += and -= -- the
// counters are unsigned, so negation wraps and cancels back out on the way up

struct SpanStat {
	uint64_t m_lineCount;
	uint64_t m_charCount;

	SpanStat():
		m_lineCount(0),
		m_charCount(0) {}

	SpanStat(const Span& span):
		m_lineCount(span.m_lineCount),
		m_charCount(span.m_charCount) {}

	SpanStat&
	operator += (const SpanStat& src) {
		m_lineCount += src.m_lineCount;
		m_charCount += src.m_charCount;
		return *this;
	}

	SpanStat&
	operator -= (const SpanStat& src) {
		m_lineCount -= src.m_lineCount;
		m_charCount -= src.m_charCount;
		return *this;
	}

	SpanStat
	operator + (const SpanStat& src) const {
		SpanStat result = *this;
		return result += src;
	}

	SpanStat
	operator - (const SpanStat& src) const {
		SpanStat result = *this;
		return result -= src;
	}

	SpanStat
	operator - () const {
		SpanStat result; // zero minus this -- avoids unary minus on unsigned
		return result -= *this;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// per-field projections. a Partial is deliberately NOT a full Stat -- it only
// needs what the projected paths use: constructible from the aggregated
// SpanStat (extracts one field), += SpanStat for calcStat, -= and < for the
// findByStat descent. keeping the surface narrow is what proves the projected
// overloads don't quietly depend on the full Stat arithmetic

struct LineStat {
	uint64_t m_value;

	LineStat(uint64_t value = 0):
		m_value(value) {}

	LineStat(const SpanStat& stat):
		m_value(stat.m_lineCount) {}

	LineStat&
	operator += (const SpanStat& src) {
		m_value += src.m_lineCount;
		return *this;
	}

	LineStat&
	operator -= (const LineStat& src) {
		m_value -= src.m_value;
		return *this;
	}

	bool
	operator < (const LineStat& src) const {
		return m_value < src.m_value;
	}
};

struct CharStat {
	uint64_t m_value;

	CharStat(uint64_t value = 0):
		m_value(value) {}

	CharStat(const SpanStat& stat):
		m_value(stat.m_charCount) {}

	CharStat&
	operator += (const SpanStat& src) {
		m_value += src.m_charCount;
		return *this;
	}

	CharStat&
	operator -= (const CharStat& src) {
		m_value -= src.m_value;
		return *this;
	}

	bool
	operator < (const CharStat& src) const {
		return m_value < src.m_value;
	}
};

//..............................................................................

uint32_t
lcg(uint32_t* seed) {
	*seed = *seed * 1664525 + 1013904223;
	return *seed >> 16;
}

template <typename Tree>
typename Tree::Iterator
getIteratorAt(
	Tree& tree,
	size_t index
) {
	typename Tree::Iterator it = tree.getHead();
	for (size_t i = 0; i < index; i++)
		it++;

	return it;
}

// verifies the whole tree against the reference element array:
//
// - forward and backward iteration reproduce the reference sequence (this is
//   what catches broken leaf links and lost/duplicated payload)
// - calcStat(it) equals the INCLUSIVE prefix at every position (walks UP the
//   spine, so it catches broken parent pointers and stale node stats)
// - getFullStat() equals the grand total
// - findByStat(target) for every target 0 .. total + 2 equals the smallest
//   index whose inclusive prefix is >= target (ties from zero-count elements
//   resolve to the first one), empty iterator if target > total; on success
//   *remainder == target minus the prefix before the element found

template <typename Tree>
void
verifyTree(
	const Tree& tree,
	const sl::Array<Span>& reference
) {
	typedef typename Tree::Iterator Iterator; // a const tree yields const iterators

	size_t count = reference.getCount();
	TEST_ASSERT(tree.getCount() == count);
	TEST_ASSERT(tree.isEmpty() == (count == 0));

	uint64_t lineTotal = 0;
	uint64_t charTotal = 0;
	size_t i = 0;

	for (Iterator it = tree.getHead(); it; it++, i++) {
		TEST_ASSERT(i < count);
		TEST_ASSERT(it->m_id == reference[i].m_id);
		TEST_ASSERT((*it).m_lineCount == reference[i].m_lineCount);

		lineTotal += reference[i].m_lineCount;
		charTotal += reference[i].m_charCount;

		SpanStat prefix = tree.calcStat(it); // inclusive -- it's own stat counts
		TEST_ASSERT(prefix.m_lineCount == lineTotal);
		TEST_ASSERT(prefix.m_charCount == charTotal);
		TEST_ASSERT(tree.template calcStat<LineStat>(it).m_value == lineTotal);
		TEST_ASSERT(tree.template calcStat<CharStat>(it).m_value == charTotal);
	}

	TEST_ASSERT(i == count);

	SpanStat total = tree.getFullStat();
	TEST_ASSERT(total.m_lineCount == lineTotal);
	TEST_ASSERT(total.m_charCount == charTotal);
	TEST_ASSERT(tree.template getFullStat<LineStat>().m_value == lineTotal);
	TEST_ASSERT(tree.template getFullStat<CharStat>().m_value == charTotal);

	i = count;
	for (Iterator it = tree.getTail(); it; it--) {
		TEST_ASSERT(i);
		i--;
		TEST_ASSERT(it->m_id == reference[i].m_id);
	}

	TEST_ASSERT(i == 0);

	size_t modelIndex = 0; // smallest index with an inclusive prefix >= target
	uint64_t modelSum = 0; // the prefix before modelIndex

	for (uint64_t target = 0; target <= lineTotal + 2; target++) {
		while (
			modelIndex < count &&
			modelSum + reference[modelIndex].m_lineCount < target
		) {
			modelSum += reference[modelIndex].m_lineCount;
			modelIndex++;
		}

		LineStat remainder(-1);
		Iterator it = tree.template findByStat<LineStat>(LineStat(target), &remainder);

		if (modelIndex < count) {
			uint64_t inclusive = modelSum + reference[modelIndex].m_lineCount;
			TEST_ASSERT(it);
			TEST_ASSERT(it->m_id == reference[modelIndex].m_id);
			TEST_ASSERT(remainder.m_value == target - modelSum);
			TEST_ASSERT(tree.template calcStat<LineStat>(it).m_value == inclusive);
			TEST_ASSERT(inclusive >= target); // findByStat's defining property
		} else {
			TEST_ASSERT(!it); // target is past the grand total
		}
	}
}

//..............................................................................

// the Stat arithmetic contract itself -- the tree leans on negation cancelling
// exactly (erase propagates -stat up the spine), which is the one identity that
// isn't obvious with unsigned counters

void
test_StatArithmetic() {
	Span a(1, 3, 40);
	Span b(2, 5, 70);

	SpanStat statA = a;
	SpanStat statB = b;

	SpanStat sum = statA + statB;
	TEST_ASSERT(sum.m_lineCount == 8 && sum.m_charCount == 110);

	SpanStat diff = sum - statB;
	TEST_ASSERT(diff.m_lineCount == statA.m_lineCount && diff.m_charCount == statA.m_charCount);

	SpanStat negated = -statA;
	TEST_ASSERT(negated.m_lineCount == (uint64_t)0 - 3 && negated.m_charCount == (uint64_t)0 - 40);

	SpanStat cancelled = statA;
	cancelled += negated; // the erase path: adding -stat must land back on zero
	TEST_ASSERT(!cancelled.m_lineCount && !cancelled.m_charCount);

	SpanStat viaNegation = sum + (-statB); // -= and + -(...) must agree
	TEST_ASSERT(viaNegation.m_lineCount == diff.m_lineCount);
	TEST_ASSERT(viaNegation.m_charCount == diff.m_charCount);

	SpanStat zero;
	TEST_ASSERT(!zero.m_lineCount && !zero.m_charCount);
	TEST_ASSERT(!(-zero).m_lineCount && !(-zero).m_charCount);
}

//..............................................................................

void
test_Empty() {
	typedef sl::StatBTree<Span, SpanStat, 4> Tree; // commas can't cross TEST_ASSERT

	Tree tree;
	Tree::Iterator nullIt;

	TEST_ASSERT(tree.getCount() == 0);
	TEST_ASSERT(tree.isEmpty());
	TEST_ASSERT(!tree.getHead());
	TEST_ASSERT(!tree.getTail());
	TEST_ASSERT(tree.getFullStat().m_lineCount == 0);
	TEST_ASSERT(tree.getFullStat<LineStat>().m_value == 0);
	TEST_ASSERT(!tree.findByStat<LineStat>(LineStat(0)));
	TEST_ASSERT(!tree.findByStat<LineStat>(LineStat(7)));
	TEST_ASSERT(!tree.calcStat<LineStat>(nullIt).m_value);

	sl::Array<Span> reference;
	verifyTree(tree, reference);

	tree.clear(); // must survive a no-op clear
	verifyTree(tree, reference);
}

// the Stat = T default path, plus the scalar (non-projected)
// calcStat/getFullStat/findByStat

void
test_Scalar() {
	enum {
		ElementCount = 200
	};

	uint32_t seed = 0x1a2b3c4d;

	sl::StatBTree<uint64_t> tree; // Stat == uint64_t, Fanout == 16
	sl::Array<uint64_t> reference;

	for (size_t i = 0; i < ElementCount; i++) {
		uint64_t value = lcg(&seed) % 4; // zeros are frequent -- ties on purpose
		tree.insertTail(value);
		reference.append(value);
	}

	TEST_ASSERT(tree.getCount() == ElementCount);

	uint64_t total = 0;
	size_t i = 0;

	for (sl::StatBTree<uint64_t>::Iterator it = tree.getHead(); it; it++, i++) {
		TEST_ASSERT(*it == reference[i]);
		total += reference[i];
		TEST_ASSERT(tree.calcStat(it) == total); // inclusive
	}

	TEST_ASSERT(i == ElementCount);
	TEST_ASSERT(tree.getFullStat() == total);

	size_t modelIndex = 0;
	uint64_t modelSum = 0;

	for (uint64_t target = 0; target <= total + 2; target++) {
		while (modelIndex < ElementCount && modelSum + reference[modelIndex] < target) {
			modelSum += reference[modelIndex];
			modelIndex++;
		}

		uint64_t remainder = -1;
		sl::StatBTree<uint64_t>::Iterator it = tree.findByStat(target, &remainder);

		if (modelIndex < ElementCount) {
			TEST_ASSERT(it);
			TEST_ASSERT(tree.calcStat(it) == modelSum + reference[modelIndex]);
			TEST_ASSERT(remainder == target - modelSum);
		} else {
			TEST_ASSERT(!it);
		}
	}
}

// grow one element at a time, verifying the WHOLE tree at every count -- so
// every leaf split, every node split and every root growth gets checked

template <size_t Fanout>
void
testAppendSweep(size_t elementCount) {
	uint32_t seed = 0x64536455 + (uint32_t)Fanout;

	sl::StatBTree<Span, SpanStat, Fanout> tree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		tree.insertTail(span);
		reference.append(span);
		verifyTree(tree, reference);
	}
}

// the same, growing from the head instead (every insert hits slot 0)

template <size_t Fanout>
void
testPrependSweep(size_t elementCount) {
	uint32_t seed = 0x1234abcd + (uint32_t)Fanout;

	sl::StatBTree<Span, SpanStat, Fanout> tree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		tree.insertHead(span);
		reference.insert(0, span);
		verifyTree(tree, reference);
	}
}

// random mid-tree inserts (insertBefore/insertAfter), then random erases all
// the way back down to empty -- this is what exercises borrow-from-sibling,
// merge and root collapse

template <size_t Fanout>
void
testRandomInsertErase(size_t elementCount) {
	uint32_t seed = 0x0badc0de + (uint32_t)Fanout;

	sl::StatBTree<Span, SpanStat, Fanout> tree;
	sl::Array<Span> reference;

	typedef typename sl::StatBTree<Span, SpanStat, Fanout>::Iterator Iterator;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		size_t count = reference.getCount();
		size_t index = count ? lcg(&seed) % count : 0;

		if (lcg(&seed) & 1) { // insertBefore
			tree.insertBefore(span, getIteratorAt(tree, index));
			reference.insert(index, span);
		} else { // insertAfter
			Iterator it = getIteratorAt(tree, index);
			tree.insertAfter(span, it);
			reference.insert(it ? index + 1 : 0, span);
		}

		verifyTree(tree, reference);
	}

	while (!reference.isEmpty()) {
		size_t index = lcg(&seed) % reference.getCount();
		tree.erase(getIteratorAt(tree, index));
		reference.remove(index);
		verifyTree(tree, reference);
	}

	TEST_ASSERT(tree.isEmpty());
	TEST_ASSERT(!tree.getHead());
	TEST_ASSERT(!tree.getTail());
}

// modify must re-aggregate the spine (including the zero-stat corner cases)

template <size_t Fanout>
void
testModify(size_t elementCount) {
	uint32_t seed = 0xfeedface + (uint32_t)Fanout;

	sl::StatBTree<Span, SpanStat, Fanout> tree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		tree.insertTail(span);
		reference.append(span);
	}

	verifyTree(tree, reference);

	for (size_t i = 0; i < elementCount; i++) {
		size_t index = lcg(&seed) % elementCount;
		Span span(reference[index].m_id, lcg(&seed) % 6, lcg(&seed) % 200);

		tree.modify(getIteratorAt(tree, index), span);
		reference.rwi()[index] = span;
		verifyTree(tree, reference);
	}
}

// interleaved inserts and erases -- keeps the tree hovering around the
// borrow/merge threshold instead of monotonically growing or shrinking

template <size_t Fanout>
void
testMixed(size_t opCount) {
	uint32_t seed = 0xdeadbeef + (uint32_t)Fanout;

	sl::StatBTree<Span, SpanStat, Fanout> tree;
	sl::Array<Span> reference;
	uint_t id = 0;

	for (size_t i = 0; i < opCount; i++) {
		size_t count = reference.getCount();

		if (!count || (lcg(&seed) % 3)) { // 2/3 inserts
			Span span(++id, lcg(&seed) % 4, lcg(&seed) % 100);
			size_t index = count ? lcg(&seed) % (count + 1) : 0;

			if (index == count) {
				tree.insertTail(span);
				reference.append(span);
			} else {
				tree.insertBefore(span, getIteratorAt(tree, index));
				reference.insert(index, span);
			}
		} else {
			size_t index = lcg(&seed) % count;
			tree.erase(getIteratorAt(tree, index));
			reference.remove(index);
		}

		verifyTree(tree, reference);
	}

	tree.clear();
	reference.clear();
	verifyTree(tree, reference);
}

//..............................................................................

void
run() {
	test_StatArithmetic();
	test_Empty();
	test_Scalar();

	// Fanout must be even; 4 is the minimum (so the borrow/merge threshold is
	// Fanout / 2 == 2). the small ones split and merge far more often than the
	// default 16, so a modest element count still produces a multi-level tree

	testAppendSweep<4>(80);
	testAppendSweep<6>(80);
	testAppendSweep<16>(120);

	testPrependSweep<4>(80);
	testPrependSweep<8>(80);

	testRandomInsertErase<4>(60);
	testRandomInsertErase<6>(60);
	testRandomInsertErase<8>(60);
	testRandomInsertErase<16>(60);

	testModify<4>(40);
	testModify<16>(40);

	testMixed<4>(150);
	testMixed<6>(150);
	testMixed<16>(150);
}

//..............................................................................

ADD_TEST_CASE("test_StatBTree", run)

} // namespace
