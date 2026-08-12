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

// Item is pure PAYLOAD -- the tree never looks inside it and cannot derive a stat
// from it. m_id must survive every rotation intact; the counters are only here so
// the test can build the ItemStat it passes in alongside.
//
// the Stat concept, as required by StatBinTree -- and nothing more:
//   Stat()                      -- the additive identity
//   Stat& operator += (Stat)
//   Stat& operator -= (Stat)
//   bool  operator == (Stat)    -- assertValid only
//
// no unary minus, no binary + or -: they are deliberately absent below, so this
// file stops compiling if the tree ever starts reaching for them again

struct Item {
	uint_t m_id;
	uint64_t m_lineCount;
	uint64_t m_charCount;

	Item():
		m_id(0),
		m_lineCount(0),
		m_charCount(0) {}

	Item(
		uint_t id,
		uint64_t lineCount,
		uint64_t charCount
	):
		m_id(id),
		m_lineCount(lineCount),
		m_charCount(charCount) {}

	bool
	operator == (const Item& item) const {
		return
			m_id == item.m_id &&
			m_lineCount == item.m_lineCount &&
			m_charCount == item.m_charCount;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct ItemStat {
	uint64_t m_lineCount;
	uint64_t m_charCount;

	ItemStat():
		m_lineCount(0),
		m_charCount(0) {}

	ItemStat(const Item& item): // implicit -- covers `m_stat = m_value`
		m_lineCount(item.m_lineCount),
		m_charCount(item.m_charCount) {}

	bool
	operator == (const ItemStat& stat) const {
		return m_lineCount == stat.m_lineCount && m_charCount == stat.m_charCount;
	}

	ItemStat&
	operator += (const ItemStat& stat) {
		m_lineCount += stat.m_lineCount;
		m_charCount += stat.m_charCount;
		return *this;
	}

	ItemStat&
	operator -= (const ItemStat& stat) {
		m_lineCount -= stat.m_lineCount;
		m_charCount -= stat.m_charCount;
		return *this;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// LineStat/CharStat are deliberately NARROW projections -- they define only what
// the <Partial> overloads are allowed to reach for (ctor-from-ItemStat, ctor from
// the raw field for the search target, operator <, += and -=). if calcStat or
// findByStat ever reached for full Stat arithmetic, these would stop compiling

struct LineStat {
	uint64_t m_lineCount;

	LineStat():
		m_lineCount(0) {}

	LineStat(const ItemStat& stat): // implicit -- Partial is built FROM Stat
		m_lineCount(stat.m_lineCount) {}

	explicit LineStat(uint64_t lineCount):
		m_lineCount(lineCount) {}

	bool
	operator == (const LineStat& stat) const {
		return m_lineCount == stat.m_lineCount;
	}

	bool
	operator < (const LineStat& stat) const {
		return m_lineCount < stat.m_lineCount;
	}

	LineStat&
	operator += (const LineStat& stat) {
		m_lineCount += stat.m_lineCount;
		return *this;
	}

	LineStat&
	operator -= (const LineStat& stat) {
		m_lineCount -= stat.m_lineCount;
		return *this;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct CharStat {
	uint64_t m_charCount;

	CharStat():
		m_charCount(0) {}

	CharStat(const ItemStat& stat):
		m_charCount(stat.m_charCount) {}

	explicit CharStat(uint64_t charCount):
		m_charCount(charCount) {}

	bool
	operator == (const CharStat& stat) const {
		return m_charCount == stat.m_charCount;
	}

	bool
	operator < (const CharStat& stat) const {
		return m_charCount < stat.m_charCount;
	}

	CharStat&
	operator += (const CharStat& stat) {
		m_charCount += stat.m_charCount;
		return *this;
	}

	CharStat&
	operator -= (const CharStat& stat) {
		m_charCount -= stat.m_charCount;
		return *this;
	}
};

//..............................................................................

// spelled through the StatRbTree aliases -- this is how a consumer writes it,
// and it is the only thing that instantiates axl_sl_StatRbTree.h

typedef sl::StatRbTreeNode<ItemStat, Item> ItemNode;
typedef sl::StatRbTree<ItemStat, Item> ItemTree;

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

// the tree is checked against a flat reference array after EVERY operation: the
// in-order sequence must match element for element, and the grand total must
// match the sum over the reference.
//
// assertValid() covers what the public API can't see -- the augmentation
// invariant at every INTERIOR node. getFullStat() alone only proves the root is
// right, which is exactly the kind of thing that stays right while an interior
// node drifts (that is how the StatBTree split-cascade bug hid)

template <typename Tree, typename Stat, typename Value>
void
verifyTree(
	Tree& tree,
	const sl::Array<Value>& reference
) {
#ifdef _AXL_DEBUG
	tree.assertValid();
#endif

	size_t count = reference.getCount();
	TEST_ASSERT(tree.getCount() == count);
	TEST_ASSERT(tree.isEmpty() == (count == 0));

	Stat total = Stat();
	size_t i = 0;

	typename Tree::ConstIterator it = tree.getHead();
	for (; it; it++, i++) {
		TEST_ASSERT(i < count);
		TEST_ASSERT(it->m_value == reference[i]);
		total += Stat(reference[i]);
	}

	TEST_ASSERT(i == count);
	TEST_ASSERT(tree.getFullStat() == total);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// calcStat(it) against a running prefix sum over the reference, then findByStat
// exhaustively for EVERY target in [0, total + 2] against a naive scan -- both
// the resulting element and the remainder

template <typename Partial>
void
verifyQueries(
	ItemTree& tree,
	const sl::Array<Item>& reference,
	uint64_t Item::* field
) {
	size_t count = reference.getCount();

	// 1. calcStat is the inclusive prefix sum

	uint64_t running = 0;
	size_t i = 0;

	ItemTree::ConstIterator it = tree.getHead();
	for (; it; it++, i++) {
		running += reference[i].*field;
		Partial stat = tree.template calcStat<Partial>(it);
		TEST_ASSERT(stat == Partial(running));
	}

	TEST_ASSERT(i == count);
	TEST_ASSERT(tree.template getFullStat<Partial>() == Partial(running));

	// 2. findByStat: first element whose inclusive prefix sum is >= target

	uint64_t total = running;
	for (uint64_t target = 0; target <= total + 2; target++) {
		ItemTree::ConstIterator foundIt =
			((const ItemTree&)tree).template findByStat<Partial>(Partial(target));

		// naive oracle

		uint64_t cum = 0;
		size_t expected = -1;
		for (size_t k = 0; k < count; k++) {
			cum += reference[k].*field;
			if (target <= cum) {
				expected = k;
				break;
			}
		}

		if (expected == -1) {
			TEST_ASSERT(!foundIt);
			continue;
		}

		TEST_ASSERT(foundIt);
		TEST_ASSERT(foundIt->m_value == reference[expected]);

		// remainder == target - (cumulative BEFORE the found element)

		Partial remainder;
		((const ItemTree&)tree).template findByStat<Partial>(Partial(target), &remainder);

		uint64_t before = 0;
		for (size_t k = 0; k < expected; k++)
			before += reference[k].*field;

		TEST_ASSERT(remainder == Partial(target - before));
	}
}

//..............................................................................

void
test_Empty() {
	typedef sl::StatBinTreeNode<uint64_t, uint64_t, sl::RbBalancer> Node;
	typedef sl::StatBinTree<Node, sl::RbBalancer> Tree;

	Tree tree;
	TEST_ASSERT(tree.isEmpty());
	TEST_ASSERT(tree.getCount() == 0);
	TEST_ASSERT(tree.getFullStat() == 0);
	TEST_ASSERT(!tree.getHead());
	TEST_ASSERT(!tree.getTail());

	tree.clear(); // clearing an empty tree must be a no-op
	TEST_ASSERT(tree.isEmpty());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the default-Stat path: Stat == Value == uint64_t

void
test_Scalar(size_t elementCount) {
	typedef sl::StatBinTreeNode<uint64_t, uint64_t, sl::RbBalancer> Node;
	typedef sl::StatBinTree<Node, sl::RbBalancer> Tree;

	Tree tree;
	sl::Array<uint64_t> reference;
	uint32_t seed = 1;

	for (size_t i = 0; i < elementCount; i++) {
		uint64_t value = lcg(&seed) % 4; // zeros are frequent -- ties on purpose
		tree.insertTail(value, value);
		reference.append(value);
		verifyTree<Tree, uint64_t, uint64_t>(tree, reference);
	}

	tree.clear();
	reference.clear();
	verifyTree<Tree, uint64_t, uint64_t>(tree, reference);
}

//..............................................................................

Item
makeItem(uint32_t* seed) {
	uint_t id = lcg(seed);
	return Item(id, lcg(seed) % 8, lcg(seed) % 64);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testAppendSweep(size_t elementCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 2;

	for (size_t i = 0; i < elementCount; i++) {
		Item item = makeItem(&seed);
		ItemTree::Iterator it = tree.insertTail(ItemStat(item), item);
		TEST_ASSERT(it && it->m_value == item);
		reference.append(item);
		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testPrependSweep(size_t elementCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 3;

	for (size_t i = 0; i < elementCount; i++) {
		Item item = makeItem(&seed);
		ItemTree::Iterator it = tree.insertHead(ItemStat(item), item);
		TEST_ASSERT(it && it->m_value == item);
		reference.insert(0, item);
		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// insertBefore/insertAfter at random interior positions -- this is what drives
// both attach cases: the free-slot one, and the one that has to descend to the
// rightmost/leftmost node of a subtree

void
testRandomInsert(size_t elementCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 4;

	for (size_t i = 0; i < elementCount; i++) {
		Item item = makeItem(&seed);

		if (reference.isEmpty()) {
			tree.insertTail(ItemStat(item), item);
			reference.append(item);
		} else {
			size_t index = lcg(&seed) % reference.getCount();
			ItemTree::Iterator it = getIteratorAt(tree, index);

			if (i & 1) {
				tree.insertBefore(ItemStat(item), item, it);
				reference.insert(index, item);
			} else {
				tree.insertAfter(ItemStat(item), item, it);
				reference.insert(index + 1, item);
			}
		}

		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// a NULL iterator mirrors sl::List: insertBefore appends, insertAfter prepends

void
testNullIterator() {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 5;

	for (size_t i = 0; i < 8; i++) {
		Item item = makeItem(&seed);
		tree.insertBefore(ItemStat(item), item, ItemTree::Iterator());
		reference.append(item);
		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}

	for (size_t i = 0; i < 8; i++) {
		Item item = makeItem(&seed);
		tree.insertAfter(ItemStat(item), item, ItemTree::Iterator());
		reference.insert(0, item);
		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// drain from random positions -- hits both erase branches (0/1-child via
// replaceWithChild, two-children via replaceWithNext, in both its direct-child
// and general forms)

void
testRandomErase(size_t elementCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 6;

	for (size_t i = 0; i < elementCount; i++) {
		Item item = makeItem(&seed);
		tree.insertTail(ItemStat(item), item);
		reference.append(item);
	}

	verifyTree<ItemTree, ItemStat, Item>(tree, reference);

	while (!reference.isEmpty()) {
		size_t index = lcg(&seed) % reference.getCount();
		tree.erase(getIteratorAt(tree, index));
		reference.remove(index);
		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}

	TEST_ASSERT(tree.isEmpty());
	TEST_ASSERT(tree.getFullStat() == ItemStat());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testMixed(size_t stepCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 7;

	for (size_t i = 0; i < stepCount; i++) {
		size_t count = reference.getCount();

		if (count && lcg(&seed) % 3 == 0) { // erase
			size_t index = lcg(&seed) % count;
			tree.erase(getIteratorAt(tree, index));
			reference.remove(index);
		} else {
			Item item = makeItem(&seed);

			if (!count) {
				tree.insertHead(ItemStat(item), item);
				reference.insert(0, item);
			} else {
				size_t index = lcg(&seed) % count;
				tree.insertAfter(ItemStat(item), item, getIteratorAt(tree, index));
				reference.insert(index + 1, item);
			}
		}

		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}
}

//..............................................................................

// exercises calcStat/findByStat on BOTH fields of the same tree -- the whole
// point of the <Partial> overloads is that one tree answers either question

void
testQueries(size_t elementCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 8;

	for (size_t i = 0; i < elementCount; i++) {
		Item item = makeItem(&seed);

		if (reference.isEmpty()) {
			tree.insertTail(ItemStat(item), item);
			reference.append(item);
		} else {
			size_t index = lcg(&seed) % reference.getCount();
			tree.insertAfter(ItemStat(item), item, getIteratorAt(tree, index));
			reference.insert(index + 1, item);
		}
	}

	verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	verifyQueries<LineStat>(tree, reference, &Item::m_lineCount);
	verifyQueries<CharStat>(tree, reference, &Item::m_charCount);

	// and again after churning the shape with erases

	while (reference.getCount() > elementCount / 2) {
		size_t index = lcg(&seed) % reference.getCount();
		tree.erase(getIteratorAt(tree, index));
		reference.remove(index);
	}

	verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	verifyQueries<LineStat>(tree, reference, &Item::m_lineCount);
	verifyQueries<CharStat>(tree, reference, &Item::m_charCount);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the scalar path: Stat == Value, so the plain (non-projected) overloads apply

void
testScalarQueries(size_t elementCount) {
	typedef sl::StatBinTreeNode<uint64_t, uint64_t, sl::RbBalancer> Node;
	typedef sl::StatBinTree<Node, sl::RbBalancer> Tree;

	Tree tree;
	sl::Array<uint64_t> reference;
	uint32_t seed = 9;

	for (size_t i = 0; i < elementCount; i++) {
		uint64_t value = lcg(&seed) % 4; // zeros on purpose -- ties in the descent
		tree.insertTail(value, value);
		reference.append(value);
	}

	uint64_t running = 0;
	size_t i = 0;

	Tree::ConstIterator it = tree.getHead();
	for (; it; it++, i++) {
		running += reference[i];
		TEST_ASSERT(tree.calcStat(it) == running);
	}

	uint64_t total = running;
	TEST_ASSERT(tree.getFullStat() == total);
	TEST_ASSERT(tree.calcStat(Tree::ConstIterator()) == 0); // NULL iterator -> identity

	for (uint64_t target = 0; target <= total + 2; target++) {
		uint64_t remainder = 0;
		Tree::Iterator foundIt = tree.findByStat(target, &remainder);

		uint64_t cum = 0;
		uint64_t before = 0;
		size_t expected = -1;

		for (size_t k = 0; k < reference.getCount(); k++) {
			cum += reference[k];
			if (target <= cum) {
				expected = k;
				break;
			}
			before = cum;
		}

		if (expected == -1) {
			TEST_ASSERT(!foundIt);
			continue;
		}

		TEST_ASSERT(foundIt);
		TEST_ASSERT(tree.calcStat(foundIt) >= target);
		TEST_ASSERT(remainder == target - before);
	}
}

// replacing an element must not touch the shape -- only the stat delta rides up.
// with a delta-only API that is subStat(old) + addStat(new), and the value goes
// straight through the iterator. the sweep deliberately includes shrinking
// values (so the intermediate underflows) and no-op rewrites

void
testModify(size_t elementCount) {
	ItemTree tree;
	sl::Array<Item> reference;
	uint32_t seed = 10;

	for (size_t i = 0; i < elementCount; i++) {
		Item item = makeItem(&seed);
		tree.insertTail(ItemStat(item), item);
		reference.append(item);
	}

	verifyTree<ItemTree, ItemStat, Item>(tree, reference);

	for (size_t i = 0; i < elementCount * 2; i++) {
		size_t index = lcg(&seed) % reference.getCount();
		Item item = makeItem(&seed);

		ItemTree::Iterator it = getIteratorAt(tree, index);
		it->subStat(ItemStat(reference[index]));
		it->addStat(ItemStat(item));
		it->m_value = item; // no propagation at all -- the value carries no stat

		reference.rwi()[index] = item;

		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}

	// shrink every element to zero, one at a time -- every delta underflows

	for (size_t i = 0; i < reference.getCount(); i++) {
		Item item(reference[i].m_id, 0, 0);

		ItemTree::Iterator it = getIteratorAt(tree, i);
		it->subStat(ItemStat(reference[i]));
		it->m_value = item;

		reference.rwi()[i] = item;
		verifyTree<ItemTree, ItemStat, Item>(tree, reference);
	}

	TEST_ASSERT(tree.getFullStat() == ItemStat());

	// queries must still hold after all that rewriting

	verifyQueries<LineStat>(tree, reference, &Item::m_lineCount);
	verifyQueries<CharStat>(tree, reference, &Item::m_charCount);
}

//..............................................................................

void
run() {
	test_Empty();
	test_Scalar(120);

	testAppendSweep(120);
	testPrependSweep(120);
	testRandomInsert(120);
	testNullIterator();
	testRandomErase(80);
	testMixed(250);

	testScalarQueries(120);
	testQueries(60); // exhaustive findByStat -- keep the element count modest
	testModify(40);

	// AXL_TODO: once calcStat(Iterator)/findByStat<Partial>/modify land, add --
	//   - calcStat(it) against a running prefix sum over the reference
	//   - findByStat exhaustively for every target in [0, total + 2], checking
	//     both the iterator and the remainder (the FenwickTree/StatBTree pattern)
	//   - a narrow Partial projection (ctor-from-Stat, operator<, operator-=)
	//     to prove the projected overloads don't reach for full Stat arithmetic
	//   - modify() round-trips
}

//..............................................................................

ADD_TEST_CASE("test_StatBinTree", run)

} // namespace
