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

// Span is pure PAYLOAD -- the tree never looks inside it and can't derive a
// stat from it. m_id must survive every split/merge/borrow intact; the counters
// are only here so the test can build the stat it passes in alongside

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

// the aggregate: everything summable in a Span and nothing else. the tree needs
// +=, -= and == (the last one for assertValid only) and NOTHING else -- no unary
// minus, no binary + or -, no <. those are deliberately absent here, so this
// file stops compiling if the tree ever starts requiring them again

struct SpanStat {
	uint64_t m_lineCount;
	uint64_t m_charCount;

	SpanStat():
		m_lineCount(0),
		m_charCount(0) {}

	SpanStat(const Span& span):
		m_lineCount(span.m_lineCount),
		m_charCount(span.m_charCount) {}

	bool
	operator == (const SpanStat& src) const {
		return m_lineCount == src.m_lineCount && m_charCount == src.m_charCount;
	}

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
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// per-field projections. a SubStat is deliberately NOT a full Stat -- it only
// needs what the projected paths use: constructible from the aggregated
// SpanStat (extracts one field), += SpanStat for calcPrefixStat, -= and < for
// the findGe descent, and == for find()'s exact-prefix test. keeping the surface
// narrow is what proves the projected overloads don't quietly depend on the full
// Stat arithmetic

struct LineStat {
	uint64_t m_value;

	LineStat(uint64_t value = 0):
		m_value(value) {}

	LineStat(const SpanStat& stat):
		m_value(stat.m_lineCount) {}

	bool
	operator == (const LineStat& src) const {
		return m_value == src.m_value;
	}

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

	bool
	operator == (const CharStat& src) const {
		return m_value == src.m_value;
	}

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
// - calcPrefixStat(it) equals the INCLUSIVE prefix at every position (walks UP
//   the spine, so it catches broken parent pointers and stale node stats)
// - getFullStat() equals the grand total
// - findGe(target) for every target 0 .. total + 2 equals the smallest
//   index whose inclusive prefix is >= target (ties from zero-count elements
//   resolve to the first one), empty iterator if target > total; on success
//   *remainder == target minus the prefix before the element found

template <typename Tree>
void
verifyTree(
	const Tree& tree,
	const sl::Array<Span>& reference
) {
	typedef typename Tree::ConstIterator ConstIterator; // a const tree yields const iterators

#ifdef _AXL_DEBUG
	tree.assertValid(); // the per-node invariant the public API can't see
#endif

	size_t count = reference.getCount();
	TEST_ASSERT(tree.getCount() == count);
	TEST_ASSERT(tree.isEmpty() == (count == 0));

	uint64_t lineTotal = 0;
	uint64_t charTotal = 0;
	size_t i = 0;

	for (ConstIterator it = tree.getHead(); it; it++, i++) {
		TEST_ASSERT(i < count);
		TEST_ASSERT(it->m_id == reference[i].m_id);
		TEST_ASSERT((*it).m_lineCount == reference[i].m_lineCount);

		// the element's stat is STORED alongside the value, not derived from it

		SpanStat elementStat = it.getStat();
		TEST_ASSERT(elementStat.m_lineCount == reference[i].m_lineCount);
		TEST_ASSERT(elementStat.m_charCount == reference[i].m_charCount);

		lineTotal += reference[i].m_lineCount;
		charTotal += reference[i].m_charCount;

		SpanStat prefix = tree.calcPrefixStat(it); // inclusive -- its own counts
		TEST_ASSERT(prefix.m_lineCount == lineTotal);
		TEST_ASSERT(prefix.m_charCount == charTotal);
		TEST_ASSERT(tree.template calcPrefixStat<LineStat>(it).m_value == lineTotal);
		TEST_ASSERT(tree.template calcPrefixStat<CharStat>(it).m_value == charTotal);
	}

	TEST_ASSERT(i == count);

	SpanStat total = tree.getFullStat();
	TEST_ASSERT(total.m_lineCount == lineTotal);
	TEST_ASSERT(total.m_charCount == charTotal);
	TEST_ASSERT(tree.template getFullStat<LineStat>().m_value == lineTotal);
	TEST_ASSERT(tree.template getFullStat<CharStat>().m_value == charTotal);

	i = count;
	for (ConstIterator it = tree.getTail(); it; it--) {
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
		ConstIterator it = tree.template findGe<LineStat>(LineStat(target), &remainder);

		// find() is findGe() landing exactly on an element's inclusive prefix

		ConstIterator exactIt = tree.template findEq<LineStat>(LineStat(target));

		if (modelIndex < count) {
			uint64_t inclusive = modelSum + reference[modelIndex].m_lineCount;
			TEST_ASSERT(it);
			TEST_ASSERT(it->m_id == reference[modelIndex].m_id);
			TEST_ASSERT(remainder.m_value == target - modelSum);
			TEST_ASSERT(tree.template calcPrefixStat<LineStat>(it).m_value == inclusive);
			TEST_ASSERT(inclusive >= target); // findGe's defining property

			if (inclusive == target) {
				TEST_ASSERT(exactIt);
				TEST_ASSERT(exactIt->m_id == reference[modelIndex].m_id);
			} else {
				TEST_ASSERT(!exactIt);
			}
		} else {
			TEST_ASSERT(!exactIt); // past the grand total
			TEST_ASSERT(!it); // target is past the grand total
		}
	}
}

//..............................................................................

// the Stat arithmetic contract itself -- the tree leans on -= undoing += exactly
// (erase and borrow-from-sibling both subtract a stat back out of a running
// aggregate), which is the one identity that has to hold on unsigned counters

void
test_StatArithmetic() {
	Span a(1, 3, 40);
	Span b(2, 5, 70);

	SpanStat statA = a;
	SpanStat statB = b;

	SpanStat sum = statA;
	sum += statB;
	TEST_ASSERT(sum.m_lineCount == 8 && sum.m_charCount == 110);

	SpanStat diff = sum;
	diff -= statB; // the erase path: subtracting back out must land on statA
	TEST_ASSERT(diff.m_lineCount == statA.m_lineCount && diff.m_charCount == statA.m_charCount);

	SpanStat cancelled = statA;
	cancelled -= statA;
	TEST_ASSERT(!cancelled.m_lineCount && !cancelled.m_charCount);

	// intermediate underflow must cancel back out: the merge path can subtract a
	// sibling's total before adding it back on the other side

	SpanStat wrapped;
	wrapped -= statB;
	wrapped += statB;
	TEST_ASSERT(!wrapped.m_lineCount && !wrapped.m_charCount);

	SpanStat zero;
	TEST_ASSERT(!zero.m_lineCount && !zero.m_charCount);
}

//..............................................................................

void
test_Empty() {
	typedef sl::StatBTree<SpanStat, Span, 4> Tree; // commas can't cross TEST_ASSERT

	Tree tree;
	Tree::Iterator nullIt;

	TEST_ASSERT(tree.getCount() == 0);
	TEST_ASSERT(tree.isEmpty());
	TEST_ASSERT(!tree.getHead());
	TEST_ASSERT(!tree.getTail());
	TEST_ASSERT(tree.getFullStat().m_lineCount == 0);
	TEST_ASSERT(tree.getFullStat<LineStat>().m_value == 0);
	TEST_ASSERT(!tree.findGe<LineStat>(LineStat(0)));
	TEST_ASSERT(!tree.findGe<LineStat>(LineStat(7)));
	TEST_ASSERT(!tree.calcPrefixStat<LineStat>(nullIt).m_value);

	sl::Array<Span> reference;
	verifyTree(tree, reference);

	tree.clear(); // must survive a no-op clear
	verifyTree(tree, reference);
}

// a scalar Stat with a payload of its own, plus the non-projected
// calcPrefixStat/getFullStat/findGe. the payload here is an id with nothing
// to do with the stat -- so it also pins that the two are tracked independently

void
test_Scalar() {
	enum {
		ElementCount = 200
	};

	typedef sl::StatBTree<uint64_t, uint_t> Tree; // Fanout == 16

	uint32_t seed = 0x1a2b3c4d;

	Tree tree;
	sl::Array<uint64_t> reference;

	for (size_t i = 0; i < ElementCount; i++) {
		uint64_t value = lcg(&seed) % 4; // zeros are frequent -- ties on purpose
		tree.insertTail(value, (uint_t)i + 1);
		reference.append(value);
	}

	TEST_ASSERT(tree.getCount() == ElementCount);

	uint64_t total = 0;
	size_t i = 0;

	for (Tree::Iterator it = tree.getHead(); it; it++, i++) {
		TEST_ASSERT(*it == (uint_t)i + 1); // the payload
		TEST_ASSERT(it.getStat() == reference[i]); // the stat
		total += reference[i];
		TEST_ASSERT(tree.calcPrefixStat(it) == total); // inclusive
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
		Tree::Iterator it = tree.findGe(target, &remainder);

		if (modelIndex < ElementCount) {
			TEST_ASSERT(it);
			TEST_ASSERT(tree.calcPrefixStat(it) == modelSum + reference[modelIndex]);
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

	sl::StatBTree<SpanStat, Span, Fanout> tree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		tree.insertTail(SpanStat(span), span);
		reference.append(span);
		verifyTree(tree, reference);
	}
}

// the same, growing from the head instead (every insert hits slot 0)

template <size_t Fanout>
void
testPrependSweep(size_t elementCount) {
	uint32_t seed = 0x1234abcd + (uint32_t)Fanout;

	sl::StatBTree<SpanStat, Span, Fanout> tree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		tree.insertHead(SpanStat(span), span);
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

	sl::StatBTree<SpanStat, Span, Fanout> tree;
	sl::Array<Span> reference;

	typedef typename sl::StatBTree<SpanStat, Span, Fanout>::Iterator Iterator;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		size_t count = reference.getCount();
		size_t index = count ? lcg(&seed) % count : 0;

		if (lcg(&seed) & 1) { // insertBefore
			tree.insertBefore(SpanStat(span), span, getIteratorAt(tree, index));
			reference.insert(index, span);
		} else { // insertAfter
			Iterator it = getIteratorAt(tree, index);
			tree.insertAfter(SpanStat(span), span, it);
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

// addStat/subStat must re-aggregate the spine (including the zero-stat corner
// cases); modifyValue must touch nothing but the value. replacing a stat is
// subStat(old) + addStat(new) -- no negative delta, hence no unary minus on Stat

template <size_t Fanout>
void
testModify(size_t elementCount) {
	uint32_t seed = 0xfeedface + (uint32_t)Fanout;

	sl::StatBTree<SpanStat, Span, Fanout> tree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		tree.insertTail(SpanStat(span), span);
		reference.append(span);
	}

	verifyTree(tree, reference);

	for (size_t i = 0; i < elementCount; i++) {
		size_t index = lcg(&seed) % elementCount;
		Span span(reference[index].m_id, lcg(&seed) % 6, lcg(&seed) % 200);

		typename sl::StatBTree<SpanStat, Span, Fanout>::Iterator it =
			getIteratorAt(tree, index);

		it.subStat(SpanStat(reference[index]));
		it.addStat(SpanStat(span));
		*it = span; // the value is writable straight through the iterator

		reference.rwi()[index] = span;
		verifyTree(tree, reference);
	}

	// a value-only write must leave every aggregate alone -- and it needs no tree
	// call at all, not even a whole-value assignment

	SpanStat before = tree.getFullStat();

	for (size_t i = 0; i < elementCount; i++) {
		getIteratorAt(tree, i)->m_id += 1000;
		reference.rwi()[i].m_id += 1000;
	}

	SpanStat after = tree.getFullStat();
	TEST_ASSERT(before.m_lineCount == after.m_lineCount);
	TEST_ASSERT(before.m_charCount == after.m_charCount);
	verifyTree(tree, reference);
}

// interleaved inserts and erases -- keeps the tree hovering around the
// borrow/merge threshold instead of monotonically growing or shrinking

template <size_t Fanout>
void
testMixed(size_t opCount) {
	uint32_t seed = 0xdeadbeef + (uint32_t)Fanout;

	sl::StatBTree<SpanStat, Span, Fanout> tree;
	sl::Array<Span> reference;
	uint_t id = 0;

	for (size_t i = 0; i < opCount; i++) {
		size_t count = reference.getCount();

		if (!count || (lcg(&seed) % 3)) { // 2/3 inserts
			Span span(++id, lcg(&seed) % 4, lcg(&seed) % 100);
			size_t index = count ? lcg(&seed) % (count + 1) : 0;

			if (index == count) {
				tree.insertTail(SpanStat(span), span);
				reference.append(span);
			} else {
				tree.insertBefore(SpanStat(span), span, getIteratorAt(tree, index));
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// insertX(stat) leaves the Value default-constructed, for the caller to fill in
// stat and value are independent, so the totals are checked BEFORE the payload is written
// a small Fanout also proves the returned iterator survives a split

template <size_t Fanout>
void
testInsertNoValue(size_t elementCount) {
	typedef sl::StatBTree<SpanStat, Span, Fanout> Tree;
	typedef typename Tree::Iterator Iterator;

	uint32_t seed = 0x5eed0001 + (uint32_t)Fanout;

	Tree tree;
	sl::Array<Span> reference;
	uint64_t lineTotal = 0;
	uint64_t charTotal = 0;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		size_t index;
		Iterator it;

		switch (i % 4) {
		case 0:
			index = reference.getCount();
			it = tree.insertTail(SpanStat(span));
			break;

		case 1:
			index = 0;
			it = tree.insertHead(SpanStat(span));
			break;

		case 2:
			// getIteratorAt(count) is a NULL iterator, so this also covers the
			// insertBefore(NULL) == append fallback

			index = lcg(&seed) % (reference.getCount() + 1);
			it = tree.insertBefore(SpanStat(span), getIteratorAt(tree, index));
			break;

		default:
			// ...and index == 0 covers insertAfter(NULL) == prepend

			index = lcg(&seed) % (reference.getCount() + 1);
			it = tree.insertAfter(SpanStat(span), index ? getIteratorAt(tree, index - 1) : Iterator());
			break;
		}

		lineTotal += span.m_lineCount;
		charTotal += span.m_charCount;

		TEST_ASSERT(it);

		// default-constructed, not garbage and not a neighbour's payload

		TEST_ASSERT((*it).m_id == 0);
		TEST_ASSERT((*it).m_lineCount == 0 && (*it).m_charCount == 0);

		// the stat landed on this slot, and aggregated, with no value yet

		SpanStat elementStat = it.getStat();
		TEST_ASSERT(elementStat.m_lineCount == span.m_lineCount);
		TEST_ASSERT(elementStat.m_charCount == span.m_charCount);
		TEST_ASSERT(tree.getFullStat().m_lineCount == lineTotal);
		TEST_ASSERT(tree.getFullStat().m_charCount == charTotal);

		*it = span; // only now the payload

		reference.insert(index, span);
		verifyTree(tree, reference);
	}

	// and it all survives being torn down again

	while (!reference.isEmpty()) {
		size_t index = lcg(&seed) % reference.getCount();
		tree.erase(getIteratorAt(tree, index));
		reference.remove(index);
		verifyTree(tree, reference);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// insertX(stat) + fill must be indistinguishable from insertX(stat, value)

template <size_t Fanout>
void
testInsertNoValueEquivalence(size_t elementCount) {
	typedef sl::StatBTree<SpanStat, Span, Fanout> Tree;

	uint32_t seed = 0x5eed0002 + (uint32_t)Fanout;

	Tree noValueTree;
	Tree valueTree;
	sl::Array<Span> reference;

	for (size_t i = 0; i < elementCount; i++) {
		Span span((uint_t)i + 1, lcg(&seed) % 4, lcg(&seed) % 100);
		size_t index = lcg(&seed) % (reference.getCount() + 1);

		typename Tree::Iterator it = noValueTree.insertBefore(SpanStat(span), getIteratorAt(noValueTree, index));
		*it = span;

		valueTree.insertBefore(SpanStat(span), span, getIteratorAt(valueTree, index));
		reference.insert(index, span);
	}

	verifyTree(noValueTree, reference);
	verifyTree(valueTree, reference);

	typename Tree::ConstIterator it1 = noValueTree.getHead();
	typename Tree::ConstIterator it2 = valueTree.getHead();
	for (; it1 && it2; it1++, it2++) {
		TEST_ASSERT(it1->m_id == it2->m_id);
		TEST_ASSERT(it1->m_lineCount == it2->m_lineCount);
		TEST_ASSERT(it1.getStat().m_charCount == it2.getStat().m_charCount);
	}

	TEST_ASSERT(!it1 && !it2);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// REGRESSION: interior stat drift on a cascading split (slot test must be <=, not <)
// invisible in getFullStat(): the drift moves stat between subtrees, so the total stays right
// needs a small Fanout, all four insert forms so slot sweeps across Fanout / 2, and assertValid() after each op
// verifyTree() is O(total) in findGe targets, so it runs periodically instead

template <size_t Fanout>
void
testSplitCascade(size_t elementCount) {
	typedef sl::StatBTree<SpanStat, Span, Fanout> Tree;

	for (uint32_t i = 1; i <= 4; i++) {
		uint32_t seed = i * 0x9e3779b9 + (uint32_t)Fanout;

		Tree tree;
		sl::Array<Span> reference;

		for (size_t j = 0; j < elementCount; j++) {
			// sequenced deliberately: two lcg() calls in one expression would
			// leave the sequence up to the compiler's argument evaluation order

			uint64_t lineCount = lcg(&seed) % 4;
			uint64_t charCount = lcg(&seed) % 100;
			Span span((uint_t)j + 1, lineCount, charCount);
			size_t index;

			switch (j % 4) {
			case 0:
				index = reference.getCount();
				tree.insertTail(SpanStat(span), span);
				break;

			case 1:
				index = 0;
				tree.insertHead(SpanStat(span), span);
				break;

			case 2:
				index = lcg(&seed) % (reference.getCount() + 1);
				tree.insertBefore(SpanStat(span), span, getIteratorAt(tree, index));
				break;

			default:
				index = lcg(&seed) % (reference.getCount() + 1);
				tree.insertAfter(
					SpanStat(span),
					span,
					index ? getIteratorAt(tree, index - 1) : typename Tree::Iterator()
				);
				break;
			}

			reference.insert(index, span);

#ifdef _AXL_DEBUG
			tree.assertValid();
#endif
			if (j % 32 == 0)
				verifyTree(tree, reference);
		}

		verifyTree(tree, reference);

		// ...and it must survive the way back down, too

		while (!reference.isEmpty()) {
			size_t index = lcg(&seed) % reference.getCount();
			tree.erase(getIteratorAt(tree, index));
			reference.remove(index);

#ifdef _AXL_DEBUG
			tree.assertValid();
#endif
			if (reference.getCount() % 32 == 0)
				verifyTree(tree, reference);
		}

		TEST_ASSERT(tree.isEmpty() && !tree.getHead() && !tree.getTail());
	}
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

	testSplitCascade<4>(200); // deep enough that splits cascade past one level
	testSplitCascade<6>(200);
	testSplitCascade<8>(200);

	testInsertNoValue<4>(80);  // small Fanout: the returned iterator must survive splits
	testInsertNoValue<16>(80);
	testInsertNoValueEquivalence<4>(80);
	testInsertNoValueEquivalence<16>(80);

	testModify<4>(40);
	testModify<16>(40);

	testMixed<4>(150);
	testMixed<6>(150);
	testMixed<16>(150);
}

//..............................................................................

ADD_TEST_CASE("test_StatBTree", run)

} // namespace
