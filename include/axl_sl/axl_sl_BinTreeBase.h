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

#define _AXL_SL_BINTREEBASE_H

#include "axl_sl_List.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Base, // MapEntry for BSTs, StatEntry for order-stat trees
	typename BaseInit,
	typename Balancer
>
struct BinTreeNodeBase:
	Base,
	Balancer::Meta {

	template <
		typename T2,
		typename Node0,
		typename Balancer0
	>
	friend class BinTreeBase;

	friend Balancer;

protected:
	T* m_parent;
	T* m_left;
	T* m_right;

protected:
	BinTreeNodeBase(
		BaseInit baseInit,
		T* parent
	):
		Base(baseInit) {
		m_parent = parent;
		m_left = NULL;
		m_right = NULL;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// base for both BSTs and stat-trees

template <
	typename T,
	typename Node0,
	typename Balancer0
>
class BinTreeBase {
	friend Balancer0;

public:
	typedef Node0 Node;
	typedef Balancer0 Balancer;
	typedef typename Balancer::Meta Meta;
	typedef sl::Iterator<Node> Iterator;
	typedef sl::ConstIterator<Node> ConstIterator;

	// everything the balancer needs to know to rebalance on erase

	struct EraseContext: Meta {
		Node* m_parent;
		Node* m_child;

		EraseContext(
			Node* parent,
			Node* child,
			const Meta& meta
		): Meta(meta) {
			ASSERT(!child || child->m_parent == parent);
			m_parent = parent;
			m_child = child;
		}
	};

protected:
	List<Node> m_nodeList;
	Node* m_root;

public:
	BinTreeBase() {
		m_root = NULL;
	}

	bool
	isEmpty() const {
		return m_nodeList.isEmpty();
	}

	size_t
	getCount() const {
		return m_nodeList.getCount();
	}

	Iterator
	getHead() {
		return m_nodeList.getHead();
	}

	ConstIterator
	getHead() const {
		return m_nodeList.getHead();
	}

	Iterator
	getTail() {
		return m_nodeList.getTail();
	}

	ConstIterator
	getTail() const {
		return m_nodeList.getTail();
	}

	void
	erase(Iterator it) {
		Node* node = *it;

		static_cast<T*>(this)->subStat(node, NULL);

		EraseContext context =
			!node->m_left ? replaceWithChild(node, node->m_right) :
			!node->m_right ? replaceWithChild(node, node->m_left) :
			replaceWithNext(node);

		Balancer::onErase(static_cast<T*>(this), context);
		m_nodeList.erase(node);
	}

	void
	clear() {
		m_nodeList.clear();
		m_root = NULL;
	}

protected:
	Node*
	findLeftmostChild(Node* p) {
		while (p->m_left)
			p = p->m_left;

		return p;
	}

	Node*
	findRightmostChild(Node* p) {
		while (p->m_right)
			p = p->m_right;

		return p;
	}

	EraseContext
	replaceWithChild(
		Node* p,
		Node* child
	) {
		Node* parent = p->m_parent;
		if (!parent) {
			ASSERT(p == m_root);
			m_root = child;
		} else if (parent->m_left == p)
			parent->m_left = child;
		else {
			ASSERT(parent->m_right == p);
			parent->m_right = child;
		}

		if (child)
			child->m_parent = parent;

		return EraseContext(parent, child, *p);
	}

	EraseContext
	replaceWithNext(Node* p) {
		// p has two children and is replaced by its in-order successor

		Node* next = (Node*)p->m_next;
		ASSERT(p->m_left && p->m_right && next && !next->m_left && next == findLeftmostChild(p->m_right));

		static_cast<T*>(this)->subStat(next, p);
		static_cast<T*>(this)->updateStat(next, p);

		Node* parent;
		Node* child = next->m_right;

		if (next == p->m_right) { // direct child
			next->m_parent = p->m_parent;
			next->m_left = p->m_left;
			next->m_left->m_parent = next;

			parent = next;
		} else {
			Node* nextParent = next->m_parent;
			ASSERT(nextParent && nextParent->m_left == next);

			next->m_parent = p->m_parent;
			next->m_left = p->m_left;
			next->m_right = p->m_right;
			next->m_left->m_parent = next;
			next->m_right->m_parent = next;

			nextParent->m_left = child;

			if (child)
				child->m_parent = nextParent;

			parent = nextParent;
		}

		// fixup parent

		if (!p->m_parent)
			m_root = next;
		else if (p->m_parent->m_left == p)
			p->m_parent->m_left = next;
		else {
			ASSERT(p->m_parent->m_right == p);
			p->m_parent->m_right = next;
		}

		EraseContext context(parent, child, *next);
		*(Meta*)next = *p;
		return context;
	}

	void
	rotateLeft(Node* p) {
		Node* right = p->m_right;
		ASSERT(right);

		p->m_right = right->m_left;

		if (right->m_left)
			right->m_left->m_parent = p;

		right->m_parent = p->m_parent;

		if (!p->m_parent)
			m_root = right;
		else if (p->m_parent->m_left == p)
			p->m_parent->m_left = right;
		else
			p->m_parent->m_right = right;

		right->m_left = p;
		p->m_parent = right;
	}

	void
	rotateRight(Node* p) {
		Node* left = p->m_left;
		ASSERT(left);

		p->m_left = left->m_right;

		if (left->m_right)
			left->m_right->m_parent = p;

		left->m_parent = p->m_parent;

		if (!p->m_parent)
			m_root = left;
		else if (p->m_parent->m_right == p)
			p->m_parent->m_right = left;
		else
			p->m_parent->m_left = left;

		left->m_right = p;
		p->m_parent = left;
	}

	// overridables for stat trees:

	void
	subStat(
		const Node* p,
		const Node* end
	) {
		// stat tree should update stat of all parents of `node` all the way up and stop before `end`
	}

	void
	updateStat(
		Node* dst,
		const Node* src
	) {
		// stat tree should update stat of `dst` using stat of `src` (that's being removed)
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
