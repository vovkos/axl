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

#define _AXL_SL_STATBINTREE_H

#include "axl_sl_BinTreeBase.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Value0,
	typename Stat0 = Value0,
	typename ValueStatArgs0 = ArgTypes<Value0, Stat0>
>
struct StatEntry: ListLink {
	typedef Value0 Value;
	typedef Stat0 Stat;
	typedef typename std::tuple_element<0, ValueStatArgs0>::type ValueArg;
	typedef typename std::tuple_element<1, ValueStatArgs0>::type StatArg;

protected:
	Value m_value;
	Stat m_stat;

public:
	StatEntry(ValueArg value):
		m_value(value),
		m_stat(value) {}

	ValueArg
	getValue() const {
		return m_value;
	}

	StatArg
	getStat() const {
		return m_stat;
	}

	Stat
	getChildStat() const {
		return m_stat - Stat(m_value);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Value,
	typename Stat,
	typename Balancer,
	typename ValueStatArgs = ArgTypes<Value, Stat>
>
struct StatBinTreeNode : public BinTreeNodeBase<
	StatBinTreeNode<Value, Stat, Balancer, ValueStatArgs>,
	StatEntry<Value, Stat, ValueStatArgs>,
	typename std::tuple_element<0, ValueStatArgs>::type,
	Balancer
> {
	typedef typename std::tuple_element<0, ValueStatArgs>::type ValueArg;
	typedef sl::StatEntry<Value, Stat, ValueStatArgs> StatEntry;
	typedef sl::BinTreeNodeBase<StatBinTreeNode, StatEntry, ValueArg, Balancer> BinTreeNodeBase;

	template <
		typename Node,
		typename Balancer2
	>
	friend class StatBinTree;

	StatBinTreeNode(
		ValueArg value,
		StatBinTreeNode* parent
	):
		BinTreeNodeBase(value, parent)
	{
	}
};

//..............................................................................

template <
	typename Node0,
	typename Balancer0
>
class StatBinTree: public BinTreeBase<
	StatBinTree<Node0, Balancer0>,
	Node0,
	Balancer0
> {
	friend BinTreeBase;
	friend Balancer;

public:
	typedef Node0 Node;
	typedef Balancer0 Balancer;
	typedef typename Node::Value Value;
	typedef typename Node::Stat Stat;
	typedef typename Node::ValueArg ValueArg;
	typedef typename Node::StatArg StatArg;
	typedef sl::BinTreeBase<StatBinTree, Node, Balancer> BinTreeBase;
	typedef sl::Iterator<Node> Iterator;
	typedef sl::ConstIterator<Node> ConstIterator;

public:
	// the grand total over every element

	Stat
	getFullStat() const {
		return getFullStat<Stat>();
	}

	template <typename Partial>
	Partial
	getFullStat() const {
		return this->m_root ? Partial(this->m_root->m_stat) : Partial(Stat());
	}

	// the total over everything up to (and including) Iterator it

	Stat
	calcStat(ConstIterator it) const {
		return calcStat<Stat>(it);
	}

	template <typename Partial>
	Partial
	calcStat(ConstIterator it) const {
		if (!it)
			return Partial(Stat());

		const Node* p = *it;
		Partial stat = Partial(p->m_stat);
		if (p->m_right)
			stat -= p->m_right->m_stat;

		for (; p->m_parent; p = p->m_parent)
			if (p == p->m_parent->m_right) {
				stat += p->m_parent->m_stat;
				stat -= p->m_stat;
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
		return const_cast<StatBinTree*>(this)->findByStat<Stat, StatArg>(targetStat, remainder);
	}

	// this overload descends on a single field of a multi-field stat

	template <
		typename Partial,
		typename PartialArg = ArgType<Partial>
	>
	Iterator
	findByStat(
		PartialArg targetStat,
		Partial* remainder = NULL
	) {
		Partial stat = targetStat;

		for (Node* p = this->m_root; p;) {
			if (p->m_left) {
				Partial leftStat = p->m_left->m_stat;
				if (!(leftStat < stat)) { // go left
					p = p->m_left;
					continue;
				}

				stat -= leftStat;
			}

			Partial midStat = Stat(p->m_value);
			if (!(midStat < stat)) { // found it
				if (remainder)
					*remainder = stat;

				return p;
			}

			stat -= midStat;
			p = p->m_right; // go right
		}

		return Iterator(); // past the grand total
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
		return const_cast<StatBinTree*>(this)->findByStat<Partial, PartialArg>(targetStat, remainder);
	}

	void
	modify(
		Iterator it,
		ValueArg value
	) {
		ASSERT(it);

		Node* node = *it;
		Stat delta = Stat(value) - Stat(node->m_value);
		node->m_value = value;

		for (Node* p = node; p; p = p->m_parent)
			p->m_stat += delta;
	}

	Iterator
	insertHead(ValueArg value) {
		if (!this->m_root)
			return insertRoot(value);
		else
			return insertLeft(value, this->findLeftmostChild(this->m_root));
	}

	Iterator
	insertTail(ValueArg value) {
		if (!this->m_root)
			return insertRoot(value);
		else
			return insertRight(value, this->findRightmostChild(this->m_root));
	}

	Iterator
	insertBefore(
		ValueArg value,
		Iterator beforeIt
	) {
		if (!beforeIt)
			return insertTail(value);

		Node* p = *beforeIt;
		if (!p->m_left)
			return insertLeft(value, p);
		else
			return insertRight(value, this->findRightmostChild(p->m_left));
	}

	Iterator
	insertAfter(
		ValueArg value,
		Iterator afterIt
	) {
		if (!afterIt)
			return insertHead(value);

		Node* p = *afterIt;
		if (!p->m_right)
			return insertRight(value, p);
		else
			return insertLeft(value, this->findLeftmostChild(p->m_right));
	}

#ifdef _AXL_DEBUG
	void
	assertValid() const {
		ASSERT(!this->m_root || !this->m_root->m_parent);

		ConstIterator it = this->getHead();
		size_t count = 0;
		assertValidNode(this->m_root, &it, &count);

		ASSERT(!it); // the chain must be exhausted, i.e. hold no extra nodes
		ASSERT(count == this->getCount());
	}
#endif

protected:
	Iterator
	insertRoot(ValueArg value) {
		ASSERT(!this->m_root);

		Node* node = new Node(value, NULL);
		this->m_root = node;
		return this->m_nodeList.insertTail(node); // the root skips onInsert
	}

	Iterator
	insertLeft(
		ValueArg value,
		Node* parent
	) {
		ASSERT(!parent->m_left);

		Node* node = new Node(value, parent);
		parent->m_left = node;
		addStat(node);
		Balancer::onInsert(this, node);
		return this->m_nodeList.insertBefore(node, parent);
	}

	Iterator
	insertRight(
		ValueArg value,
		Node* parent
	) {
		ASSERT(!parent->m_right);

		Node* node = new Node(value, parent);
		parent->m_right = node;
		addStat(node);
		Balancer::onInsert(this, node);
		return this->m_nodeList.insertAfter(node, parent);
	}

	void
	recalcStat(Node* p) {
		p->m_stat = p->m_value;

		if (p->m_left)
			p->m_stat += p->m_left->m_stat;

		if (p->m_right)
			p->m_stat += p->m_right->m_stat;
	}

	void
	rotateLeft(Node* p) {
		Node* right = p->m_right;
		ASSERT(right);

		BinTreeBase::rotateLeft(p);
		right->m_stat = p->m_stat; // promoted node gets the old total
		recalcStat(p);             // demoted node recomputes from children
	}

	void
	rotateRight(Node* p) {
		Node* left = p->m_left;
		ASSERT(left);

		BinTreeBase::rotateRight(p);
		left->m_stat = p->m_stat;  // promoted node gets the old total
		recalcStat(p);             // demoted node recomputes from children
	}

	void
	addStat(Node* p0) {
		Stat stat = p0->m_value;
		for (Node* p = p0->m_parent; p; p = p->m_parent)
			p->m_stat += stat;
	}

	void
	subStat(
		const Node* p0,
		const Node* end
	) {
		Stat stat = p0->m_value;
		for (Node* p = p0->m_parent; p != end; p = p->m_parent)
			p->m_stat -= stat;
	}

	void
	updateStat(
		Node* dst,
		const Node* src
	) {
		dst->m_stat = src->m_stat - src->m_value;
	}

#ifdef _AXL_DEBUG
	void
	assertValidNode(
		const Node* p,
		ConstIterator* it,
		size_t* count
	) const {
		if (!p)
			return;

		Stat stat = p->m_value;

		if (p->m_left) {
			ASSERT(p->m_left->m_parent == p);
			stat += p->m_left->m_stat;
		}

		if (p->m_right) {
			ASSERT(p->m_right->m_parent == p);
			stat += p->m_right->m_stat;
		}

		ASSERT(p->m_stat == stat);

		assertValidNode(p->m_left, it, count);

		ASSERT(*it && *(*it) == p); // in-order position must match the chain
		(*it)++;
		(*count)++;

		assertValidNode(p->m_right, it, count);
	}
#endif
};

//..............................................................................

} // namespace sl
} // namespace axl
