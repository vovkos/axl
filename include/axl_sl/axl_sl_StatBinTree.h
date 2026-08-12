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
#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Stat0,
	typename Value0,
	typename StatValueArgs0 = ArgTypes<Stat0, Value0>
>
struct StatEntry: ListLink {
	typedef Stat0 Stat;
	typedef Value0 Value;
	typedef typename std::tuple_element<0, StatValueArgs0>::type StatArg;
	typedef typename std::tuple_element<1, StatValueArgs0>::type ValueArg;

protected:
	Stat m_selfStat;
	Stat m_fullStat;

public:
	Value m_value;

public:
	StatEntry(StatArg stat):
		m_selfStat(stat),
		m_fullStat(stat),
		m_value() {}

	StatArg
	getSelfStat() const {
		return m_selfStat;
	}

	StatArg
	getFullStat() const {
		return m_fullStat;
	}

	Stat
	getChildStat() const {
		Stat stat = m_fullStat;
		stat -= m_selfStat; // this way Stat doesn't need a binary minus op
		return stat;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Stat,
	typename Value,
	typename Balancer,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
struct StatBinTreeNode: public BinTreeNodeBase<
	StatBinTreeNode<Stat, Value, Balancer, StatValueArgs>,
	StatEntry<Stat, Value, StatValueArgs>,
	typename std::tuple_element<0, StatValueArgs>::type,
	Balancer
> {
	typedef sl::StatEntry<Stat, Value, StatValueArgs> StatEntry;
	typedef typename StatEntry::StatArg StatArg;
	typedef sl::BinTreeNodeBase<StatBinTreeNode, StatEntry, StatArg, Balancer> BinTreeNodeBase;

	template <
		typename Node,
		typename Balancer2
	>
	friend class StatBinTree;

	StatBinTreeNode(
		StatArg stat,
		StatBinTreeNode* parent
	):
		BinTreeNodeBase(stat, parent)
	{
	}

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
		Op()(this->m_selfStat, delta);
		Op()(this->m_fullStat, delta);

		for (StatBinTreeNode* p = this->m_parent; p; p = p->m_parent)
			Op()(p->m_fullStat, delta);
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
	template <
		typename T2,
		typename Node2,
		typename Balancer2
	>
	friend class BinTreeBase;

	friend Balancer0;

public:
	typedef Node0 Node;
	typedef Balancer0 Balancer;
	typedef typename Node::Stat Stat;
	typedef typename Node::Value Value;
	typedef typename Node::StatArg StatArg;
	typedef typename Node::ValueArg ValueArg;
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
		return this->m_root ? Partial(this->m_root->m_fullStat) : Partial(Stat());
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
		Partial stat = Partial(p->m_fullStat);
		if (p->m_right)
			stat -= p->m_right->m_fullStat;

		for (; p->m_parent; p = p->m_parent)
			if (p == p->m_parent->m_right) {
				stat += p->m_parent->m_fullStat;
				stat -= p->m_fullStat;
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
				Partial leftStat = p->m_left->m_fullStat;
				if (!(leftStat < stat)) { // go left
					p = p->m_left;
					continue;
				}

				stat -= leftStat;
			}

			Partial midStat = p->m_selfStat;
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

	Iterator
	insertHead(
		StatArg stat,
		ValueArg value
	) {
		Iterator it = !this->m_root ?
			insertRoot(stat) :
			insertLeft(stat, this->findLeftmostChild(this->m_root));

		it->m_value = value;
		return it;
	}

	Iterator
	insertTail(
		StatArg stat,
		ValueArg value
	) {
		Iterator it = !this->m_root ?
			insertRoot(stat) :
			insertRight(stat, this->findRightmostChild(this->m_root));

		it->m_value = value;
		return it;
	}

	Iterator
	insertBefore(
		StatArg stat,
		ValueArg value,
		ConstIterator beforeIt
	) {
		if (!beforeIt)
			return insertTail(stat, value);

		Node* p = (Node*)*beforeIt;
		Iterator it = !p->m_left ?
			insertLeft(stat, p) :
			insertRight(stat, this->findRightmostChild(p->m_left));

		it->m_value = value;
		return it;
	}

	Iterator
	insertAfter(
		StatArg stat,
		ValueArg value,
		ConstIterator afterIt
	) {
		if (!afterIt)
			return insertHead(stat, value);

		Node* p = (Node*)*afterIt;
		Iterator it = !p->m_right ?
			insertRight(stat, p) :
			insertLeft(stat, this->findLeftmostChild(p->m_right));

		it->m_value = value;
		return it;
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
	insertRoot(StatArg stat) {
		ASSERT(!this->m_root);

		Node* node = new Node(stat, NULL);
		this->m_root = node;
		return this->m_nodeList.insertTail(node); // the root skips onInsert
	}

	Iterator
	insertLeft(
		StatArg stat,
		Node* parent
	) {
		ASSERT(!parent->m_left);

		Node* node = new Node(stat, parent);
		parent->m_left = node;
		addStat(node);
		Balancer::onInsert(this, node);
		return this->m_nodeList.insertBefore(node, parent);
	}

	Iterator
	insertRight(
		StatArg stat,
		Node* parent
	) {
		ASSERT(!parent->m_right);

		Node* node = new Node(stat, parent);
		parent->m_right = node;
		addStat(node);
		Balancer::onInsert(this, node);
		return this->m_nodeList.insertAfter(node, parent);
	}

	void
	recalcStat(Node* p) {
		p->m_fullStat = p->m_selfStat;

		if (p->m_left)
			p->m_fullStat += p->m_left->m_fullStat;

		if (p->m_right)
			p->m_fullStat += p->m_right->m_fullStat;
	}

	void
	rotateLeft(Node* p) {
		Node* right = p->m_right;
		ASSERT(right);

		BinTreeBase::rotateLeft(p);
		right->m_fullStat = p->m_fullStat; // promoted node gets the old total
		recalcStat(p);                     // demoted node recomputes from children
	}

	void
	rotateRight(Node* p) {
		Node* left = p->m_left;
		ASSERT(left);

		BinTreeBase::rotateRight(p);
		left->m_fullStat = p->m_fullStat;  // promoted node gets the old total
		recalcStat(p);                     // demoted node recomputes from children
	}

	void
	addStat(Node* p0) {
		Stat stat = p0->m_selfStat;
		for (Node* p = p0->m_parent; p; p = p->m_parent)
			p->m_fullStat += stat;
	}

	void
	subStat(
		const Node* p0,
		const Node* end
	) {
		Stat stat = p0->m_selfStat;
		for (Node* p = p0->m_parent; p != end; p = p->m_parent)
			p->m_fullStat -= stat;
	}

	void
	updateStat(
		Node* dst,
		const Node* src
	) {
		dst->m_fullStat = src->m_fullStat;
		dst->m_fullStat -= src->m_selfStat; // this way Stat doesn't need a binary minus op
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

		Stat stat = p->m_selfStat;

		if (p->m_left) {
			ASSERT(p->m_left->m_parent == p);
			stat += p->m_left->m_fullStat;
		}

		if (p->m_right) {
			ASSERT(p->m_right->m_parent == p);
			stat += p->m_right->m_fullStat;
		}

		ASSERT(p->m_fullStat == stat);

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
