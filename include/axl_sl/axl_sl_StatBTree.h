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
		ASSERT(this->m_count < Fanout && slot <= this->m_count);

		Child* p = m_childArray + slot;
		ArrayDetails<Child>::copy(p + 1, p, this->m_count - slot);
		*p = child;
		this->m_count++;
	}

	void
	appendFromSibling(
		const T* src,
		size_t srcSlot,
		size_t count
	) {
		ASSERT(
			(!this->m_parent || this->m_parent == src->m_parent) && // split fills a sibling NOT linked to the parent yet
			this->m_count + count <= Fanout &&
			srcSlot + count <= src->m_count
		);

		ArrayDetails<Child>::copy(this->m_childArray + this->m_count, src->m_childArray + srcSlot, count);
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

	void
	insert(
		size_t slot,
		NodeRoot* child
	) {
		NodeBase::insert(slot, child);
		child->m_parent = this;
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
	insertFromSibling(
		size_t slot,
		const StatBTreeNode* src,
		size_t srcSlot
	) {
		ASSERT(
			this->m_parent == src->m_parent &&
			slot <= this->m_count &&
			srcSlot < src->m_count
		);

		insert(slot, src->m_childArray[srcSlot]);
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
		StatArg stat,
		ValueArg value
	) {
		size_t copyCount = this->m_count - slot; // before Node::insert updates m_count
		NodeBase::insert(slot, stat);
		Value* p = m_valueArray + slot;
		ArrayDetails<Value>::copy(p + 1, p, copyCount);
		*p = value;
	}

	void
	insertFromSibling(
		size_t slot,
		const StatBTreeLeaf* src,
		size_t srcSlot
	) {
		ASSERT(
			this->m_parent == src->m_parent && // borrow on underflow always between two siblings
			slot <= this->m_count && // slot == m_count appends
			srcSlot < src->m_count
		);

		insert(slot, src->m_childArray[srcSlot], src->m_valueArray[srcSlot]);
	}

	void
	appendFromSibling(
		const StatBTreeLeaf* src,
		size_t srcSlot,
		size_t count
	) {
		size_t dstSlot = this->m_count; // before Node::appendFromSibling updates m_count
		NodeBase::appendFromSibling(src, srcSlot, count);
		ArrayDetails<Value>::copy(m_valueArray + dstSlot, src->m_valueArray + srcSlot, count);
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

	template <typename Partial>
	Partial
	getFullStat() const {
		return m_root ? Partial(m_root->m_stat) : Partial(Stat());
	}

	// everything up to (and including) iterator

	Stat
	calcStat(ConstIterator it) const {
		return calcStat<Stat>(it);
	}

	template <typename Partial>
	Partial
	calcStat(ConstIterator it) const {
		Partial stat = Partial(Stat());
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

	// finds first iterator so that targetStat <= calcStat(it)

	Iterator
	findByStat(
		StatArg targetStat,
		Stat* remainder = NULL
	) {
		return findByStat<Stat, StatArg>(targetStat, remainder);
	}

	ConstIterator
	findByStat(
		StatArg targetStat,
		Stat* remainder = NULL
	) const {
		return const_cast<StatBTree*>(this)->findByStat<Stat, StatArg>(targetStat, remainder);
	}

	// this overload can operate on a specific field of a multi-field stat

	template <
		typename Partial,
		typename PartialArg = ArgType<Partial>
	>
	Iterator
	findByStat(
		PartialArg targetStat,
		Partial* remainder = NULL
	) {
		if (!m_root)
			return Iterator();

		Partial stat = targetStat;
		NodeBase* node = m_root;

		for (size_t level = m_height; level; level--) {
			Node* parent = (Node*)node;
			size_t i = 0;
			for (; i < parent->m_count - 1; i++) { // the last child is the fallback
				Partial childStat = parent->getChildStat(i);
				if (!(childStat < stat))
					break;

				stat -= childStat;
			}

			node = parent->m_childArray[i];
		}

		Leaf* leaf = (Leaf*)node;
		size_t i = 0;
		for (; i < leaf->m_count; i++) {
			Partial childStat = leaf->getChildStat(i);
			if (!(childStat < stat))
				break;

			stat -= childStat;
		}

		if (i >= leaf->m_count) // targetStat is past the grand total
			return Iterator();

		if (remainder)
			*remainder = stat;

		return Iterator(leaf, i);
	}

	template <
		typename Partial,
		typename PartialArg = ArgType<Partial>
	>
	ConstIterator
	findByStat(
		PartialArg targetStat,
		Partial* remainder = NULL
	) const {
		return const_cast<StatBTree*>(this)->findByStat<Partial, PartialArg>(targetStat, remainder);
	}

	Iterator
	insertHead(
		StatArg stat,
		ValueArg value
	) {
		return m_leafList.isEmpty() ?
			insertFirst(stat, value) :
			insert((Leaf*)*m_leafList.getHead(), 0, stat, value);
	}

	Iterator
	insertTail(
		StatArg stat,
		ValueArg value
	) {
		if (m_leafList.isEmpty())
			return insertFirst(stat, value);

		Leaf* leaf = (Leaf*)*m_leafList.getTail();
		return insert(leaf, leaf->m_count, stat, value);
	}

	Iterator
	insertBefore(
		StatArg stat,
		ValueArg value,
		Iterator beforeIt
	) {
		return beforeIt ?
			insert(*beforeIt.getLeafIterator(), beforeIt.getSlot(), stat, value) :
			insertTail(stat, value);
	}

	Iterator
	insertAfter(
		StatArg stat,
		ValueArg value,
		Iterator afterIt
	) {
		return afterIt ?
			insert(*afterIt.getLeafIterator(), afterIt.getSlot() + 1, stat, value) :
			insertHead(stat, value);
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

protected:
	Iterator
	insertFirst(
		StatArg stat,
		ValueArg value
	) {
		ASSERT(!m_root && !m_count);

		Leaf* leaf = new Leaf;
		leaf->m_childArray[0] = stat;
		leaf->m_valueArray[0] = value;
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
		StatArg stat,
		ValueArg value
	) {
		m_count++;

		if (leaf->m_count < Fanout) {
			leaf->insert(slot, stat, value);
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

		target->insert(slot, stat, value);
		target->addStat(stat);
		return Iterator(target, slot);
	}

	void
	postSplit(
		Leaf* node,
		Leaf* sibling
	) {
		m_leafList.insertAfter(sibling, node);
	}

	static
	void
	postSplit(
		Node* node,
		Node* sibling
	) {
		for (size_t i = 0; i < sibling->m_count; i++)
			sibling->m_childArray[i]->m_parent = sibling;
	}

	template <typename NodeType>
	NodeType*
	split(NodeType* node) {
		ASSERT(node->m_count == Fanout);

		Node* parent = node->m_parent;
		NodeType* sibling = new NodeType;
		size_t siblingCount = node->m_count - Fanout / 2;
		sibling->appendFromSibling(node, Fanout / 2, siblingCount);
		sibling->recalcStat();
		node->m_count = Fanout / 2;
		node->m_stat -= sibling->m_stat; // not recalcStat! not the same when we are splitting parent
		postSplit(node, sibling);

		if (!parent) {
			Node* root = new Node;
			root->m_childArray[0] = node;
			root->m_childArray[1] = sibling;
			root->m_count = 2;
			root->m_stat = node->m_stat;
			root->m_stat += sibling->m_stat;
			node->m_parent = root;
			sibling->m_parent = root;
			m_root = root;
			m_height++;
			return sibling;
		}

		size_t slot = parent->find(node) + 1;
		if (parent->m_count < Fanout) {
			parent->insert(slot, sibling);
			return sibling;
		}

		Node* parentSibling = split(parent);
		Node* target;
		if (slot < parent->m_count) {
			target = parent;
		} else {
			parent->m_stat -= sibling->m_stat;
			parentSibling->m_stat += sibling->m_stat;
			target = parentSibling;
			slot -= parent->m_count;
		}

		target->insert(slot, sibling);
		return sibling;
	}

	void
	postMerge(
		Leaf* left,
		Leaf* right
	) {
		m_leafList.erase(right);
	}

	void
	postMerge(
		Node* left,
		Node* right
	) {
		for (size_t i = 0; i < right->m_count; i++)
			right->m_childArray[i]->m_parent = left;

		delete right;
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
		left->appendFromSibling(right, 0, right->m_count);
		left->m_stat += right->m_stat;
		parent->remove(rightSlot);
		postMerge(left, right);

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
				node->insertFromSibling(0, prev, prevSlot);
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
				node->insertFromSibling(node->m_count, next, 0);
				next->remove(0);
				next->m_stat -= stat;
				node->m_stat += stat;
			}
		}
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
