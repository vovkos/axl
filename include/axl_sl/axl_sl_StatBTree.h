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

#pragma once

#define _AXL_SL_STATBTREE_H

#include "axl_sl_List.h"
#include "axl_sl_ArrayDetails.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Stat,
	size_t Fanout,
	typename StatArg
>
struct StatBTreeNode;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	size_t Fanout,
	typename StatArg
>
struct StatBTreeNodeRoot {
	StatBTreeNode<Stat, Fanout, StatArg>* m_parent;
	Stat m_stat; // aggregate of all children
	size_t m_count; // slots in use

	StatBTreeNodeRoot():
		m_stat() {
		m_parent = NULL;
		m_count = 0;
	}

	void
	addStat(StatArg delta) {
		addStat<AddAssign<Stat, StatArg> >(delta);
	}

	void
	subStat(StatArg delta) {
		addStat<SubAssign<Stat, StatArg> >(delta);
	}

	template <typename Op>
	void
	addStat(StatArg delta) {
		Op()(m_stat, delta);
		for (StatBTreeNodeRoot* p = m_parent; p; p = p->m_parent)
			Op()(p->m_stat, delta);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Stat,
	typename Child0,
	size_t Fanout,
	typename StatChildArgs
>
struct StatBTreeNodeBase: public StatBTreeNodeRoot<
	Stat,
	Fanout,
	typename std::tuple_element<0, StatChildArgs>::type
> {
	typedef StatBTreeNodeRoot<
		Stat,
		Fanout,
		typename std::tuple_element<0, StatChildArgs>::type
	> NodeRoot;

	typedef StatBTreeNodeBase NodeBase;
	typedef Child0 Child;
	typedef typename std::tuple_element<0, StatChildArgs>::type StatArg;
	typedef typename std::tuple_element<1, StatChildArgs>::type ChildArg;

	Child m_childArray[Fanout];

	StatBTreeNodeBase():
		m_childArray() {}

	void
	remove(size_t slot) {
		ASSERT(slot < this->m_count);

		Child* p = m_childArray + slot;
		ArrayDetails<Child>::copy(p, p + 1, this->m_count - slot - 1);
		this->m_count--;
	}

	void
	insert(
		size_t slot,
		ChildArg child
	) {
		ASSERT(slot <= this->m_count && this->m_count < Fanout);

		Child* p = m_childArray + slot;
		ArrayDetails<Child>::copy(p + 1, p, this->m_count - slot);
		*p = child;
		this->m_count++;
	}

	void
	insert(
		size_t slot,
		const T* src,
		size_t srcSlot,
		size_t count
	) {
		ASSERT(
			slot <= this->m_count &&
			this->m_count + count <= Fanout &&
			srcSlot + count <= src->m_count
		);

		Child* p = m_childArray + slot;
		ArrayDetails<Child>::copy(p + count, p, this->m_count - slot);
		ArrayDetails<Child>::copy(p, src->m_childArray + srcSlot, count);
		this->m_count += count;
	}

	void
	recalcStat() {
		ASSERT(this->m_count);

		Stat stat = static_cast<T*>(this)->getChildStat(0);
		for (size_t i = 1; i < this->m_count; i++)
			stat += static_cast<T*>(this)->getChildStat(i);

		this->m_stat = stat;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	size_t Fanout,
	typename StatArg
>
struct StatBTreeNode: StatBTreeNodeBase<
	StatBTreeNode<Stat, Fanout, StatArg>,
	Stat,
	StatBTreeNodeRoot<Stat, Fanout, StatArg>*,
	Fanout,
	std::tuple<StatArg, StatBTreeNodeRoot<Stat, Fanout, StatArg>*>
> {
	typedef typename StatBTreeNode::NodeRoot NodeRoot;
	typedef typename StatBTreeNode::NodeBase NodeBase;
	typedef typename StatBTreeNode::Child Child;

	StatArg
	getChildStat(size_t slot) const {
		ASSERT(slot < this->m_count);
		return this->m_childArray[slot]->m_stat;
	}

	size_t
	find(const NodeRoot* child) const {
		for (size_t i = 0; i < this->m_count; i++)
			if (this->m_childArray[i] == child)
				return i;

		ASSERT(false);
		return -1;
	}

	void
	insert(
		size_t slot,
		NodeRoot* child
	) {
		NodeBase::insert(slot, child);
		child->m_parent = this; // reparent
	}

	void
	insert(
		size_t slot,
		const StatBTreeNode* src,
		size_t srcSlot,
		size_t count = 1
	) {
		NodeBase::insert(slot, src, srcSlot, count);
		for (size_t i = slot, endSlot = slot + count; i < endSlot; i++)
			this->m_childArray[i]->m_parent = this; // reparent
	}

	void
	freeChildNodes(size_t level) {
		ASSERT(level > 1);
		if (--level > 1)
			for (size_t i = 0; i < this->m_count; i++) {
				StatBTreeNode* child = (StatBTreeNode*)this->m_childArray[i];
				child->freeChildNodes(level);
				delete child;
			}
		else // grandchildren are leaves
			for (size_t i = 0; i < this->m_count; i++)
				delete (StatBTreeNode*)this->m_childArray[i];
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	typename Value,
	size_t Fanout,
	typename StatValueArgs
>
struct StatBTreeLeaf:
	StatBTreeNodeBase<
		StatBTreeLeaf<Stat, Value, Fanout, StatValueArgs>,
		Stat,
		Stat, // a leaf's slots hold the per-element stats themselves
		Fanout,
		std::tuple<
			typename std::tuple_element<0, StatValueArgs>::type,
			typename std::tuple_element<0, StatValueArgs>::type
		>
	>,
	ListLink {

	typedef typename StatBTreeLeaf::NodeBase NodeBase;
	typedef typename StatBTreeLeaf::StatArg StatArg;
	typedef typename StatBTreeLeaf::Child Child;
	typedef typename std::tuple_element<1, StatValueArgs>::type ValueArg;

	Value m_valueArray[Fanout];

	StatBTreeLeaf():
		m_valueArray() {}

	StatArg
	getChildStat(size_t slot) const {
		ASSERT(slot < this->m_count);
		return this->m_childArray[slot];
	}

	void
	remove(size_t slot) {
		size_t copyCount = this->m_count - slot - 1; // before Node::remove updates m_count
		NodeBase::remove(slot);
		Value* p = m_valueArray + slot;
		ArrayDetails<Value>::copy(p, p + 1, copyCount);
	}

	void
	insert(
		size_t slot,
		StatArg stat
	) {
		size_t copyCount = this->m_count - slot; // before Node::insert updates m_count
		NodeBase::insert(slot, stat);
		Value* p = m_valueArray + slot;
		ArrayDetails<Value>::copy(p + 1, p, copyCount);
		*p = Value();
	}

	void
	insert(
		size_t slot,
		const StatBTreeLeaf* src,
		size_t srcSlot,
		size_t count = 1
	) {
		size_t copyCount = this->m_count - slot; // before Node::insert updates m_count
		NodeBase::insert(slot, src, srcSlot, count);
		Value* p = m_valueArray + slot;
		ArrayDetails<Value>::copy(p + count, p, copyCount);
		ArrayDetails<Value>::copy(p, src->m_valueArray + srcSlot, count);
	}
};

//..............................................................................

template <
	typename T,
	typename Stat,
	typename Value, // Value or const Value
	size_t Fanout,
	typename StatValueArgs
>
class StatBTreeIteratorBase {
public:
	typedef StatBTreeLeaf<
		Stat,
		typename std::remove_const<Value>::type,
		Fanout,
		StatValueArgs
	> Leaf;

	typedef typename Leaf::StatArg StatArg;

	// handing out a Value& takes a mutable leaf

	typedef typename std::conditional<
		std::is_const<Value>::value,
		sl::ConstIterator<Leaf>,
		sl::Iterator<Leaf>
	>::type LeafIterator;

protected:
	LeafIterator m_leafIt;
	size_t m_slot;

public:
	StatBTreeIteratorBase() {
		m_slot = 0;
	}

	StatBTreeIteratorBase(
		const LeafIterator& leafIt,
		size_t slot
	) {
		m_leafIt = leafIt;
		m_slot = slot;
	}

	explicit operator bool () const {
		return (bool)m_leafIt;
	}

	bool
	operator == (const StatBTreeIteratorBase& it) const {
		return m_leafIt == it.m_leafIt && m_slot == it.m_slot;
	}

	bool
	operator != (const StatBTreeIteratorBase& it) const {
		return !operator == (it);
	}

	Value&
	operator * () const {
		return m_leafIt->m_valueArray[m_slot];
	}

	Value*
	operator -> () const {
		return &m_leafIt->m_valueArray[m_slot];
	}

	T&
	operator ++ () {
		return next();
	}

	T&
	operator -- () {
		return prev();
	}

	T
	operator ++ (int) { // post increment
		T old = *(T*)this;
		next();
		return old;
	}

	T
	operator -- (int) { // post decrement
		T old = *(T*)this;
		prev();
		return old;
	}

	const LeafIterator&
	getLeafIterator() const {
		return m_leafIt;
	}

	size_t
	getSlot() const {
		return m_slot;
	}

	StatArg
	getStat() const {
		return m_leafIt->getChildStat(m_slot);
	}

	T&
	next() {
		if (!m_leafIt)
			return *(T*)this;

		m_slot++;
		if (m_slot >= m_leafIt->m_count) {
			m_leafIt.next();
			m_slot = 0;
		}

		return *(T*)this;
	}

	T&
	prev() {
		if (!m_leafIt)
			return *(T*)this;

		if (m_slot)
			m_slot--;
		else {
			m_leafIt.prev();
			m_slot = m_leafIt ? m_leafIt->m_count - 1 : 0;
		}

		return *(T*)this;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	typename Value,
	size_t Fanout,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
class StatBTreeIterator: public StatBTreeIteratorBase<
	StatBTreeIterator<Stat, Value, Fanout, StatValueArgs>,
	Stat,
	Value,
	Fanout,
	StatValueArgs
> {
public:
	typedef sl::StatBTreeIteratorBase<
		StatBTreeIterator,
		Stat,
		Value,
		Fanout,
		StatValueArgs
	> StatBTreeIteratorBase;

	typedef typename StatBTreeIteratorBase::LeafIterator LeafIterator;
	typedef typename StatBTreeIteratorBase::Leaf Leaf;
	typedef typename StatBTreeIteratorBase::StatArg StatArg;

	StatBTreeIterator() {}

	StatBTreeIterator(
		const LeafIterator& leafIt,
		size_t slot
	): StatBTreeIteratorBase(leafIt, slot) {}

	void
	addStat(StatArg delta) {
		addStat<AddAssign<Stat, StatArg> >(delta);
	}

	void
	subStat(StatArg delta) {
		addStat<SubAssign<Stat, StatArg> >(delta);
	}

protected:
	template <typename Op>
	void
	addStat(StatArg delta) {
		Leaf* leaf = *this->m_leafIt;
		ASSERT(leaf && this->m_slot < leaf->m_count);

		Op()(leaf->m_childArray[this->m_slot], delta);
		leaf->template addStat<Op>(delta);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	typename Value,
	size_t Fanout,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
class StatBTreeConstIterator: public StatBTreeIteratorBase<
	StatBTreeConstIterator<Stat, Value, Fanout, StatValueArgs>,
	Stat,
	const Value,
	Fanout,
	StatValueArgs
> {
public:
	typedef sl::StatBTreeIteratorBase<
		StatBTreeConstIterator,
		Stat,
		const Value,
		Fanout,
		StatValueArgs
	> StatBTreeIteratorBase;

	typedef typename StatBTreeIteratorBase::LeafIterator LeafIterator;
	typedef sl::StatBTreeIterator<Stat, Value, Fanout, StatValueArgs> Iterator;

	StatBTreeConstIterator() {}

	StatBTreeConstIterator(
		const LeafIterator& leafIt,
		size_t slot
	): StatBTreeIteratorBase(leafIt, slot) {}

	StatBTreeConstIterator(const Iterator& src):
		StatBTreeIteratorBase(src.getLeafIterator(), src.getSlot()) {}

	StatBTreeConstIterator&
	operator = (const Iterator& src) {
		this->m_leafIt = src.getLeafIterator();
		this->m_slot = src.getSlot();
		return *this;
	}
};

//..............................................................................

template <
	typename Stat0,
	typename Value0,
	size_t Fanout0 = 16,
	typename StatValueArgs = ArgTypes<Stat0, Value0>
>
class StatBTree {
	AXL_DISABLE_COPY(StatBTree)

public:
	static_assert(Fanout0 >= 4 && !(Fanout0 & 1), "StatBTree<> requires an even Fanout >= 4");

	enum: size_t {
		Fanout = Fanout0,
	};

	typedef Stat0 Stat;
	typedef Value0 Value;
	typedef typename std::tuple_element<0, StatValueArgs>::type StatArg;
	typedef typename std::tuple_element<1, StatValueArgs>::type ValueArg;
	typedef StatBTreeNodeRoot<Stat, Fanout, StatArg> NodeBase;
	typedef StatBTreeNode<Stat, Fanout, StatArg> Node;
	typedef StatBTreeLeaf<Stat, Value, Fanout, StatValueArgs> Leaf;
	typedef StatBTreeIterator<Stat, Value, Fanout, StatValueArgs> Iterator;
	typedef StatBTreeConstIterator<Stat, Value, Fanout, StatValueArgs> ConstIterator;

protected:
	sl::List<Leaf> m_leafList;
	NodeBase* m_root;
	size_t m_count;
	size_t m_height;

public:
	StatBTree() {
		m_root = NULL;
		m_count = 0;
		m_height = 0;
	}

	~StatBTree() {
		clear();
	}

	size_t
	getCount() const {
		return m_count;
	}

	bool
	isEmpty() const {
		return m_count == 0;
	}

	size_t
	getHeight() const {
		return m_height;
	}

	Iterator
	getHead() {
		return !m_leafList.isEmpty() ? Iterator(m_leafList.getHead(), 0) : Iterator();
	}

	ConstIterator
	getHead() const {
		return const_cast<StatBTree*>(this)->getHead();
	}

	Iterator
	getTail() {
		sl::Iterator<Leaf> it = m_leafList.getTail();
		return it ? Iterator(it, it->m_count - 1) : Iterator();
	}

	ConstIterator
	getTail() const {
		return const_cast<StatBTree*>(this)->getTail();
	}

	void
	clear() {
		if (m_height) { // otherwise m_root is a leaf
			Node* root = (Node*)m_root;
			if (m_height > 1)
				root->freeChildNodes(m_height);
			delete root;
		}

		m_leafList.clear();
		m_root = NULL;
		m_count = 0;
		m_height = 0;
	}

	Stat
	getFullStat() const {
		return getFullStat<Stat>();
	}

	template <typename SubStat>
	SubStat
	getFullStat() const {
		return m_root ? SubStat(m_root->m_stat) : SubStat(Stat());
	}

	// everything up to (and including) iterator

	Stat
	calcPrefixStat(ConstIterator it) const {
		return calcPrefixStat<Stat>(it);
	}

	template <typename SubStat>
	SubStat
	calcPrefixStat(ConstIterator it) const {
		SubStat stat = SubStat(Stat());
		if (!it)
			return stat;

		const Leaf* leaf = *it.getLeafIterator();
		size_t slot = it.getSlot();
		for (size_t i = 0; i <= slot; i++)
			stat += leaf->getChildStat(i);

		const NodeBase* node = leaf;
		for (const Node* parent = node->m_parent; parent; parent = parent->m_parent) {
			slot = parent->find(node);
			for (size_t i = 0; i < slot; i++)
				stat += parent->getChildStat(i);

			node = parent;
		}

		return stat;
	}

	// findGe(arg): first element whose PREFIX is >= arg -- the containing element
	// offset, if asked for, is arg minus the prefix BEFORE it

	Iterator
	findGe(
		StatArg targetStat,
		Stat* offset = NULL
	) {
		return findGe<Stat, StatArg>(targetStat, offset);
	}

	ConstIterator
	findGe(
		StatArg targetStat,
		Stat* offset = NULL
	) const {
		return findGe<Stat, StatArg>(targetStat, offset);
	}

	// this overload can operate on a specific field of a multi-field stat

	template <
		typename SubStat,
		typename SubStatArg = ArgType<SubStat>
	>
	Iterator
	findGe(
		SubStatArg targetStat,
		SubStat* offset = NULL
	) {
		if (!m_root)
			return Iterator();

		SubStat stat = targetStat;
		NodeBase* node = m_root;

		for (size_t level = m_height; level; level--) {
			Node* parent = (Node*)node;
			size_t i = 0;
			for (; i < parent->m_count - 1; i++) { // the last child is the fallback
				SubStat childStat = parent->getChildStat(i);
				if (!(childStat < stat))
					break;

				stat -= childStat;
			}

			node = parent->m_childArray[i];
		}

		Leaf* leaf = (Leaf*)node;
		size_t i = 0;
		for (; i < leaf->m_count; i++) {
			SubStat childStat = leaf->getChildStat(i);
			if (!(childStat < stat))
				break;

			stat -= childStat;
		}

		if (i >= leaf->m_count) // targetStat is past the grand total
			return Iterator();

		if (offset)
			*offset = stat;

		return Iterator(leaf, i);
	}

	template <
		typename SubStat,
		typename SubStatArg = ArgType<SubStat>
	>
	ConstIterator
	findGe(
		SubStatArg targetStat,
		SubStat* offset = NULL
	) const {
		return const_cast<StatBTree*>(this)->findGe<SubStat, SubStatArg>(targetStat, offset);
	}

	// findEq(arg): the element whose prefix is exactly arg, NULL if none
	// that is findGe landing with its offset exactly on the element's own stat

	Iterator
	findEq(StatArg targetStat) {
		return findEq<Stat, StatArg>(targetStat);
	}

	ConstIterator
	findEq(StatArg targetStat) const {
		return findEq<Stat, StatArg>(targetStat);
	}

	template <
		typename SubStat,
		typename SubStatArg = ArgType<SubStat>
	>
	Iterator
	findEq(SubStatArg targetStat) {
		SubStat offset;
		Iterator it = findGe<SubStat, SubStatArg>(targetStat, &offset);
		return it && offset == SubStat(it.getStat()) ? it : Iterator();
	}

	template <
		typename SubStat,
		typename SubStatArg = ArgType<SubStat>
	>
	ConstIterator
	findEq(SubStatArg targetStat) const {
		return const_cast<StatBTree*>(this)->findEq<SubStat, SubStatArg>(targetStat);
	}

	Iterator
	insertHead(StatArg stat) {
		return m_leafList.isEmpty() ?
			insertFirst(stat) :
			insert((Leaf*)*m_leafList.getHead(), 0, stat);
	}

	Iterator
	insertHead(
		StatArg stat,
		ValueArg value
	) {
		Iterator it = insertHead(stat);
		*it = value;
		return it;
	}

	Iterator
	insertTail(StatArg stat) {
		if (m_leafList.isEmpty())
			return insertFirst(stat);

		Leaf* leaf = (Leaf*)*m_leafList.getTail();
		return insert(leaf, leaf->m_count, stat);
	}

	Iterator
	insertTail(
		StatArg stat,
		ValueArg value
	) {
		Iterator it = insertTail(stat);
		*it = value;
		return it;
	}

	Iterator
	insertBefore(
		StatArg stat,
		Iterator beforeIt
	) {
		return beforeIt ?
			insert(*beforeIt.getLeafIterator(), beforeIt.getSlot(), stat) :
			insertTail(stat);
	}

	Iterator
	insertBefore(
		StatArg stat,
		ValueArg value,
		Iterator beforeIt
	) {
		Iterator it = insertBefore(stat, beforeIt);
		*it = value;
		return it;
	}

	Iterator
	insertAfter(
		StatArg stat,
		Iterator afterIt
	) {
		return afterIt ?
			insert(*afterIt.getLeafIterator(), afterIt.getSlot() + 1, stat) :
			insertHead(stat);
	}

	Iterator
	insertAfter(
		StatArg stat,
		ValueArg value,
		Iterator afterIt
	) {
		Iterator it = insertAfter(stat, afterIt);
		*it = value;
		return it;
	}

	void
	erase(Iterator it) {
		ASSERT(it);

		Leaf* leaf = *it.getLeafIterator();
		size_t slot = it.getSlot();
		ASSERT(slot < leaf->m_count);

		leaf->subStat(leaf->getChildStat(slot));
		leaf->remove(slot);
		m_count--;

		if (!leaf->m_parent) { // the root leaf is exempt from the underflow rules
			if (!leaf->m_count) {
				m_leafList.erase(leaf);
				m_root = NULL;
			}
		} else if (leaf->m_count < Fanout / 2) // borrow/merge only rearranges stats
			onUnderflow(leaf);
	}

#ifdef _AXL_DEBUG
	void
	assertValid() const {
		if (!m_root) {
			ASSERT(!m_count && !m_height && m_leafList.isEmpty());
			return;
		}

		ASSERT(!m_root->m_parent);

		typename sl::List<Leaf>::ConstIterator leafIt = m_leafList.getHead();
		size_t count = 0;
		assertValidNode(m_root, m_height, true, &leafIt, &count);

		ASSERT(!leafIt); // the chain must hold no leaves the tree doesn't
		ASSERT(count == m_count);
	}
#endif

protected:
	Iterator
	insertFirst(StatArg stat) {
		ASSERT(!m_root && !m_count);

		Leaf* leaf = new Leaf;
		leaf->m_childArray[0] = stat;
		leaf->m_count = 1;
		leaf->m_stat = stat;
		m_leafList.insertTail(leaf);

		m_root = leaf;
		m_count = 1;
		m_height = 0;

		return Iterator(leaf, 0);
	}

	Iterator
	insert(
		Leaf* leaf,
		size_t slot,
		StatArg stat
	) {
		m_count++;

		if (leaf->m_count < Fanout) {
			leaf->insert(slot, stat);
			leaf->addStat(stat);
			return Iterator(leaf, slot);
		}

		Leaf* sibling = split(leaf);
		Leaf* target;
		if (slot <= leaf->m_count)
			target = leaf;
		else {
			slot -= leaf->m_count;
			target = sibling;
		}

		target->insert(slot, stat);
		target->addStat(stat);
		return Iterator(target, slot);
	}

	static
	void
	postSplit(
		Node* node,
		Node* sibling
	) {
	}

	void
	postSplit(
		Leaf* node,
		Leaf* sibling
	) {
		m_leafList.insertAfter(sibling, node);
	}

	template <typename NodeType>
	NodeType*
	split(NodeType* node) {
		ASSERT(node->m_count == Fanout);

		Node* parent = node->m_parent;
		NodeType* sibling = new NodeType;
		size_t siblingCount = node->m_count - Fanout / 2;
		sibling->insert(0, node, Fanout / 2, siblingCount);
		sibling->recalcStat();
		node->m_count = Fanout / 2;
		postSplit(node, sibling);

		// we deliberately postpone update of node->m_stat so it doesn't affect split(parent) below

		if (!parent) {
			Node* root = new Node;
			root->m_childArray[0] = node;
			root->m_childArray[1] = sibling;
			root->m_count = 2;
			root->m_stat = node->m_stat;
			node->m_parent = root;
			sibling->m_parent = root;
			m_root = root;
			m_height++;
			node->m_stat -= sibling->m_stat;
			return sibling;
		}

		size_t slot = parent->find(node) + 1;
		if (parent->m_count < Fanout) {
			parent->insert(slot, sibling);
			node->m_stat -= sibling->m_stat;
			return sibling;
		}

		Node* siblingParent = split(parent);
		Node* target;
		if (slot <= parent->m_count)
			target = parent;
		else {
			target = siblingParent;
			slot -= parent->m_count;
		}

		target->insert(slot, sibling);
		node->m_stat -= sibling->m_stat;
		return sibling;
	}

	void
	postMerge(Node* node) {
		delete node;
	}

	void
	postMerge(Leaf* node) {
		m_leafList.erase(node);
	}

	template <typename NodeType>
	void
	merge(
		NodeType* left,
		NodeType* right,
		size_t rightSlot
	) {
		ASSERT(left->m_parent == right->m_parent);
		ASSERT(left->m_count + right->m_count <= Fanout);

		Node* parent = left->m_parent;
		left->insert(left->m_count, right, 0, right->m_count);
		left->m_stat += right->m_stat;
		parent->remove(rightSlot);
		postMerge(right);

		// parent's total stat is unchanged, so no stat propagation

		if (!parent->m_parent) { // parent is the root
			if (parent->m_count == 1) { // collapse
				NodeType* child = (NodeType*)parent->m_childArray[0];
				child->m_parent = NULL;
				m_root = child;
				m_height--;
				delete parent;
			}
		} else if (parent->m_count < Fanout / 2)
			onUnderflow(parent);
	}

	template <typename NodeType>
	void
	onUnderflow(NodeType* node) {
		Node* parent = node->m_parent;
		ASSERT(parent && parent->m_count >= 2);

		size_t slot = parent->find(node);
		if (slot) { // merge or borrow from left
			NodeType* prev = (NodeType*)parent->m_childArray[slot - 1];
			if (prev->m_count <= Fanout / 2)
				merge(prev, node, slot);
			else {
				size_t prevSlot = prev->m_count - 1;
				Stat stat = prev->getChildStat(prevSlot); // copy before the move
				node->insert(0, prev, prevSlot);
				prev->remove(prevSlot);
				prev->m_stat -= stat;
				node->m_stat += stat;
			}
		} else { // merge of borrow from right
			ASSERT(slot < parent->m_count - 1);
			NodeType* next = (NodeType*)parent->m_childArray[slot + 1];
			if (next->m_count <= Fanout / 2)
				merge(node, next, slot + 1);
			else {
				Stat stat = next->getChildStat(0); // copy before the move
				node->insert(node->m_count, next, 0);
				next->remove(0);
				next->m_stat -= stat;
				node->m_stat += stat;
			}
		}
	}

#ifdef _AXL_DEBUG
	void
	assertValidNode(
		const NodeBase* node,
		size_t level, // 0 == a leaf
		bool isRoot,
		typename sl::List<Leaf>::ConstIterator* leafIt,
		size_t* count
	) const {
		ASSERT(node->m_count <= Fanout);

		// the root is exempt from the underflow rules

		if (!isRoot)
			ASSERT(node->m_count >= Fanout / 2);
		else if (level)
			ASSERT(node->m_count >= 2);
		else
			ASSERT(node->m_count >= 1);

		Stat stat = Stat();
		if (!level) { // a leaf: its slots hold the per-element stats themselves
			const Leaf* leaf = (const Leaf*)node;

			ASSERT(*leafIt && *(*leafIt) == leaf); // chain order must match the walk
			(*leafIt)++;
			*count += leaf->m_count;

			for (size_t i = 0; i < leaf->m_count; i++)
				stat += leaf->getChildStat(i);
		} else {
			const Node* p = (const Node*)node;

			for (size_t i = 0; i < p->m_count; i++) {
				const NodeBase* child = p->m_childArray[i];
				ASSERT(child->m_parent == p); // the two-way link is intact
				assertValidNode(child, level - 1, false, leafIt, count);
				stat += child->m_stat;
			}
		}

		ASSERT(node->m_stat == stat);
	}
#endif
};

//..............................................................................

} // namespace sl
} // namespace axl
