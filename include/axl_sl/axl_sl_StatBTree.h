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

namespace axl {
namespace sl {

//..............................................................................

// nodes & leaves

template <
	typename Stat,
	size_t Fanout
>
struct StatBTreeNode;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	size_t Fanout
>
struct StatBTreeNodeRoot {
	StatBTreeNode<Stat, Fanout>* m_parent;
	Stat m_stat; // aggregate of all children
	size_t m_count; // slots in use

	StatBTreeNodeRoot():
		m_stat() {
		m_parent = NULL;
		m_count = 0;
	}

	void
	addStat(const Stat& delta) {
		m_stat += delta;
		for (StatBTreeNodeRoot* p = m_parent; p; p = p->m_parent)
			p->m_stat += delta;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Child0,
	typename Stat,
	size_t Fanout
>
struct StatBTreeNodeBase: StatBTreeNodeRoot<Stat, Fanout>  {
	typedef Child0 Child;
	typedef ArgType<Child> ChildArg;

	Child m_childArray[Fanout];

	void
	insertChild(
		size_t slot,
		ChildArg child
	) {
		ASSERT(this->m_count < Fanout && slot <= this->m_count);

		Child* p = m_childArray + slot;
		ArrayDetails<Child>::copy(p + 1, p, this->m_count - slot);
		m_childArray[slot] = child;
		this->m_count++;
	}

	void
	removeChild(size_t slot) {
		ASSERT(slot < this->m_count);

		Child* p = m_childArray + slot;
		ArrayDetails<Child>::copy(p, p + 1, this->m_count - slot - 1);
		this->m_count--;
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
	size_t Fanout
>
struct StatBTreeNode: StatBTreeNodeBase<
	StatBTreeNode<Stat, Fanout>,
	StatBTreeNodeRoot<Stat, Fanout>*,
	Stat,
	Fanout
> {
	typedef StatBTreeNodeRoot<Stat, Fanout> NodeRoot;

	const Stat&
	getChildStat(size_t slot) const {
		return this->m_childArray[slot]->m_stat;
	}

	void
	insertChild(
		size_t slot,
		NodeRoot* child // ArgType<NodeRoot*> is the pointer itself
	) {
		StatBTreeNodeBase<StatBTreeNode, NodeRoot*, Stat, Fanout>::insertChild(slot, child);
		child->m_parent = this;
	}

	size_t
	findChild(const NodeRoot* child) const {
		for (size_t i = 0; i < this->m_count; i++)
			if (this->m_childArray[i] == child)
				return i;

		ASSERT(false);
		return -1;
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
	typename T,
	typename Stat,
	size_t Fanout
>
struct StatBTreeLeaf:
	StatBTreeNodeBase<
		StatBTreeLeaf<T, Stat, Fanout>,
		T,
		Stat,
		Fanout
	>,
	ListLink {

	Stat
	getChildStat(size_t slot) const {
		return Stat(this->m_childArray[slot]);
	}
};

//..............................................................................

// iterators are { leaf, slot }
// iterators are always const: modification through iterator would break
// parent-child stat invariants -- use StatBTree::modify instead

template <
	typename T,
	typename Stat,
	size_t Fanout
>
class StatBTreeIterator {
public:
	typedef T Value;
	typedef StatBTreeLeaf<T, Stat, Fanout> Leaf;

protected:
	ConstIterator<Leaf> m_leafIt;
	size_t m_slot;

public:
	StatBTreeIterator() {
		m_slot = 0;
	}

	StatBTreeIterator(
		const ConstIterator<Leaf>& leafIt,
		size_t slot
	) {
		m_leafIt = leafIt;
		m_slot = slot;
	}

	explicit operator bool () const {
		return (bool)m_leafIt;
	}

	bool
	operator == (const StatBTreeIterator& it) const {
		return m_leafIt == it.m_leafIt && m_slot == it.m_slot;
	}

	bool
	operator != (const StatBTreeIterator& it) const {
		return !operator == (it);
	}

	const Value&
	operator * () const {
		return m_leafIt->m_childArray[m_slot];
	}

	const Value*
	operator -> () const {
		return &m_leafIt->m_childArray[m_slot];
	}

	StatBTreeIterator&
	operator ++ () {
		return next();
	}

	StatBTreeIterator&
	operator -- () {
		return prev();
	}

	StatBTreeIterator
	operator ++ (int) { // post increment
		StatBTreeIterator old = *this;
		next();
		return old;
	}

	StatBTreeIterator
	operator -- (int) { // post decrement
		StatBTreeIterator old = *this;
		prev();
		return old;
	}

	const ConstIterator<Leaf>&
	getLeafIterator() const {
		return m_leafIt;
	}

	size_t
	getSlot() const {
		return m_slot;
	}

	StatBTreeIterator&
	next() {
		if (!m_leafIt)
			return *this;

		m_slot++;
		if (m_slot >= m_leafIt->m_count) {
			m_leafIt.next();
			m_slot = 0;
		}

		return *this;
	}

	StatBTreeIterator&
	prev() {
		if (!m_leafIt)
			return *this;

		if (m_slot)
			m_slot--;
		else {
			m_leafIt.prev();
			m_slot = m_leafIt ? m_leafIt->m_count - 1 : 0;
		}

		return *this;
	}
};

//..............................................................................

template <
	typename T,
	typename Stat0 = T,
	size_t Fanout0 = 16,
	typename ValueArg0 = ArgType<T>
>
class StatBTree {
	AXL_DISABLE_COPY(StatBTree)

public:
	static_assert(Fanout0 >= 4 && !(Fanout0 & 1), "StatBTree<> requires an even Fanout >= 4");

	enum: size_t {
		Fanout = Fanout0,
	};

	typedef T Value;
	typedef Stat0 Stat;
	typedef ValueArg0 ValueArg;
	typedef ArgType<Stat> StatArg;
	typedef StatBTreeNodeRoot<Stat, Fanout> NodeBase;
	typedef StatBTreeNode<Stat, Fanout> Node;
	typedef StatBTreeLeaf<T, Stat, Fanout> Leaf;
	typedef StatBTreeIterator<T, Stat, Fanout> Iterator;

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
	getHead() const {
		return !m_leafList.isEmpty() ? Iterator(m_leafList.getHead(), 0) : Iterator();
	}

	Iterator
	getTail() const {
		sl::ConstIterator<Leaf> it = m_leafList.getTail();
		return it ? Iterator(it, it->m_count - 1) : Iterator();
	}

	void
	clear() {
		if (m_height) { // otherwise m_root is a leaf
			Node* root = (Node*)m_root;
			if (m_height > 1)
				root->freeChildNodes(m_height);
			delete root;
		}

		m_leafList.clear(); // owns the leaves
		m_root = NULL;
		m_count = 0;
		m_height = 0;
	}

	// the grand total over all the elements

	Stat
	getFullStat() const {
		return getFullStat<Stat>();
	}

	template <typename Partial>
	Partial
	getFullStat() const {
		return m_root ? Partial(m_root->m_stat) : Partial(Stat());
	}

	// the total over everything up to (and including) Iterator it

	Stat
	calcStat(Iterator it) const {
		return calcStat<Stat>(it);
	}

	template <typename Partial>
	Partial
	calcStat(Iterator it) const {
		Partial stat = Partial(Stat());
		if (!it)
			return stat;

		const Leaf* leaf = *it.getLeafIterator();
		size_t slot = it.getSlot();
		for (size_t i = 0; i <= slot; i++)
			stat += leaf->getChildStat(i);

		const NodeBase* node = leaf;
		for (const Node* parent = node->m_parent; parent; parent = parent->m_parent) {
			slot = parent->findChild(node);
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
	) const {
		return findByStat<Stat, StatArg>(targetStat, remainder);
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
	) const {
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

	Iterator
	insertHead(ValueArg value) {
		return m_leafList.isEmpty() ? insertFirst(value) : insert((Leaf*)*m_leafList.getHead(), 0, value);
	}

	Iterator
	insertTail(ValueArg value) {
		if (m_leafList.isEmpty())
			return insertFirst(value);

		Leaf* leaf = (Leaf*)*m_leafList.getTail();
		return insert(leaf, leaf->m_count, value);
	}

	Iterator
	insertBefore(
		ValueArg value,
		Iterator beforeIt
	) {
		return beforeIt ?
			insert((Leaf*)*beforeIt.getLeafIterator(), beforeIt.getSlot(), value) :
			insertTail(value);
	}

	Iterator
	insertAfter(
		ValueArg value,
		Iterator afterIt
	) {
		return afterIt ?
			insert((Leaf*)*afterIt.getLeafIterator(), afterIt.getSlot() + 1, value) :
			insertHead(value);
	}

	void
	erase(Iterator it) {
		ASSERT(it);

		Leaf* leaf = (Leaf*)*it.getLeafIterator();
		size_t slot = it.getSlot();
		ASSERT(slot < leaf->m_count);

		leaf->addStat(-Stat(leaf->m_childArray[slot]));
		leaf->removeChild(slot);
		m_count--;

		if (!leaf->m_parent) { // the root leaf is exempt from the underflow rules
			if (!leaf->m_count) {
				m_leafList.erase(leaf);
				m_root = NULL;
			}
		} else if (leaf->m_count < Fanout / 2) // borrow/merge only rearranges stats
			onUnderflow(leaf);
	}

	void
	modify(
		Iterator it,
		ValueArg value
	) {
		ASSERT(it);
		Leaf* leaf = (Leaf*)*it.getLeafIterator();
		size_t slot = it.getSlot();
		ASSERT(slot < leaf->m_count);

		Stat delta = Stat(value) - Stat(leaf->m_childArray[slot]);
		leaf->m_childArray[slot] = value;
		leaf->addStat(delta);
	}

protected:
	Iterator
	insertFirst(ValueArg value) {
		ASSERT(!m_root && !m_count);

		Leaf* leaf = new Leaf;
		leaf->m_childArray[0] = value;
		leaf->m_count = 1;
		leaf->m_stat = Stat(value);
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
		ValueArg value
	) {
		m_count++;

		if (leaf->m_count < Fanout) {
			leaf->insertChild(slot, value);
			leaf->addStat(Stat(value));
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

		target->insertChild(slot, value);
		target->addStat(Stat(value));
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
		ArrayDetails<typename NodeType::Child>::copy(
			sibling->m_childArray,
			node->m_childArray + Fanout / 2,
			siblingCount
		);
		node->m_count = Fanout / 2;
		sibling->m_count = siblingCount;
		sibling->recalcStat();
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

		size_t slot = parent->findChild(node) + 1;
		if (parent->m_count < Fanout) {
			parent->insertChild(slot, sibling);
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

		target->insertChild(slot, sibling);
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
		typedef typename NodeType::Child Child;

		ASSERT(left->m_parent == right->m_parent);
		ASSERT(left->m_count + right->m_count <= Fanout);

		Node* parent = left->m_parent;
		ArrayDetails<Child>::copy(left->m_childArray + left->m_count, right->m_childArray, right->m_count);
		left->m_count += right->m_count;
		left->m_stat += right->m_stat;
		parent->removeChild(rightSlot);
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

		size_t slot = parent->findChild(node);
		if (slot) { // merge or borrow from left
			NodeType* prev = (NodeType*)parent->m_childArray[slot - 1];
			if (prev->m_count <= Fanout / 2)
				merge(prev, node, slot);
			else {
				Stat stat = prev->getChildStat(prev->m_count - 1);
				node->insertChild(0, prev->m_childArray[--prev->m_count]);
				prev->m_stat -= stat;
				node->m_stat += stat;
			}
		} else { // merge of borrow from right
			ASSERT(slot < parent->m_count - 1);
			NodeType* next = (NodeType*)parent->m_childArray[slot + 1];
			if (next->m_count <= Fanout / 2)
				merge(node, next, slot + 1);
			else {
				Stat stat = next->getChildStat(0);
				node->insertChild(node->m_count, next->m_childArray[0]);
				next->removeChild(0);
				next->m_stat -= stat;
				node->m_stat += stat;
			}
		}
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
