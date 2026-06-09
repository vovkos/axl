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

enum {
	EmptyValue = -1,
	SpaceSize = 128,
};

typedef sl::RangeTree<size_t, int> RangeTree;

void
clearReference(int* reference) {
	for (size_t i = 0; i < SpaceSize; i++)
		reference[i] = EmptyValue;
}

void
addReferenceRange(
	int* reference,
	size_t offset,
	size_t length,
	int value
) {
	TEST_ASSERT(length);
	TEST_ASSERT(offset + length <= SpaceSize);

	for (size_t i = offset; i < offset + length; i++)
		reference[i] = value;
}

void
addRange(
	RangeTree* tree,
	int* reference,
	size_t offset,
	size_t length,
	int value
) {
	RangeTree::ConstIterator it = tree->addRange(offset, length, value);
	TEST_ASSERT(it);
	TEST_ASSERT(it->getKey() <= offset);
	TEST_ASSERT(it->m_value.getEndOffset() >= offset + length);
	TEST_ASSERT(it->m_value.getValue() == value);

	addReferenceRange(reference, offset, length, value);
}

void
verifyTree(
	const RangeTree& tree,
	const int* reference
) {
	RangeTree::ConstIterator it = tree.getHead();
	size_t count = 0;
	size_t i = 0;

	while (i < SpaceSize) {
		if (reference[i] == EmptyValue) {
			i++;
			continue;
		}

		size_t offset = i;
		int value = reference[i];
		do
			i++;
		while (i < SpaceSize && reference[i] == value);

		TEST_ASSERT(it);
		TEST_ASSERT(it->getKey() == offset);
		TEST_ASSERT(it->m_value.getEndOffset() == i);
		TEST_ASSERT(it->m_value.getValue() == value);

		RangeTree::ConstIterator nextIt = it.getNext();
		if (nextIt) {
			TEST_ASSERT(nextIt->getKey() >= it->m_value.getEndOffset());
			if (nextIt->getKey() == it->m_value.getEndOffset())
				TEST_ASSERT(nextIt->m_value.getValue() != value);
		}

		it++;
		count++;
	}

	TEST_ASSERT(!it);
	TEST_ASSERT(tree.getCount() == count);
}

void
test_Deterministic() {
	RangeTree tree;
	int reference[SpaceSize];
	clearReference(reference);

	addRange(&tree, reference, 10, 10, 1); // new range
	verifyTree(tree, reference);

	addRange(&tree, reference, 20, 5, 1); // adjacent same-value merge
	verifyTree(tree, reference);

	addRange(&tree, reference, 5, 5, 1); // adjacent same-value merge from left
	verifyTree(tree, reference);

	addRange(&tree, reference, 12, 3, 2); // split existing range in three
	verifyTree(tree, reference);

	addRange(&tree, reference, 8, 10, 3); // overlap head, middle, tail
	verifyTree(tree, reference);

	addRange(&tree, reference, 0, 40, 4); // fully cover all existing ranges
	verifyTree(tree, reference);

	addRange(&tree, reference, 40, 10, 5); // adjacent different value
	verifyTree(tree, reference);

	addRange(&tree, reference, 35, 15, 4); // erase covered adjacent range tail
	verifyTree(tree, reference);

	addRange(&tree, reference, 60, 8, 6); // gap before range
	verifyTree(tree, reference);

	addRange(&tree, reference, 50, 10, 4); // bridge same-value range across a gap
	verifyTree(tree, reference);

	addRange(&tree, reference, 55, 20, 6); // overlap bridge and following range
	verifyTree(tree, reference);

	addRange(&tree, reference, 75, 5, 6); // adjacent same-value merge to the right
	verifyTree(tree, reference);
}

void
test_PrecedingMergeAtExistingKey() {
	RangeTree tree;
	int reference[SpaceSize];
	clearReference(reference);

	addRange(&tree, reference, 0, 10, 1);
	addRange(&tree, reference, 10, 10, 2);
	addRange(&tree, reference, 10, 5, 1); // merge with prev, preserve tail
	verifyTree(tree, reference);

	tree.clear();
	clearReference(reference);

	addRange(&tree, reference, 0, 10, 1);
	addRange(&tree, reference, 10, 10, 2);
	addRange(&tree, reference, 20, 10, 1);
	addRange(&tree, reference, 10, 10, 1); // merge prev + erase current + merge next
	verifyTree(tree, reference);

	tree.clear();
	clearReference(reference);

	addRange(&tree, reference, 0, 10, 1);
	addRange(&tree, reference, 10, 10, 2);
	addRange(&tree, reference, 20, 10, 3);
	addRange(&tree, reference, 10, 15, 1); // merge prev, cover current, trim next
	verifyTree(tree, reference);
}

void
test_SinglePointOverwrites() {
	RangeTree tree;
	int reference[SpaceSize];
	clearReference(reference);

	addRange(&tree, reference, 0, SpaceSize, 0);
	verifyTree(tree, reference);

	for (size_t i = 0; i < SpaceSize; i += 2) {
		addRange(&tree, reference, i, 1, (int)(i + 1));
		verifyTree(tree, reference);
	}

	for (size_t i = 1; i < SpaceSize; i += 2) {
		addRange(&tree, reference, i, 1, (int)i);
		verifyTree(tree, reference);
	}

	addRange(&tree, reference, 0, SpaceSize, 7);
	verifyTree(tree, reference);
}

void
test_Randomized() {
	enum {
		IterationCount = 20000,
	};

	RangeTree tree;
	int reference[SpaceSize];
	clearReference(reference);

	uint32_t seed = 0x18273645;

	for (size_t i = 0; i < IterationCount; i++) {
		seed = seed * 1664525 + 1013904223;
		size_t offset = (seed >> 16) % SpaceSize;

		seed = seed * 1664525 + 1013904223;
		size_t maxLength = SpaceSize - offset;
		size_t length = 1 + ((seed >> 16) % maxLength);

		seed = seed * 1664525 + 1013904223;
		int value = (int)((seed >> 16) % 9);

		addRange(&tree, reference, offset, length, value);

		if ((i & 0x1f) == 0)
			verifyTree(tree, reference);
	}

	verifyTree(tree, reference);
}

void
run() {
	test_Deterministic();
	test_PrecedingMergeAtExistingKey();
	test_SinglePointOverwrites();
	test_Randomized();
}

//..............................................................................

ADD_TEST_CASE("test_RangeTree", run)

} // namespace
