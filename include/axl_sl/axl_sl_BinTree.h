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

#define _AXL_SL_BINTREE_H

#include "axl_sl_List.h"
#include "axl_sl_MapEntry.h"
#include "axl_sl_Cmp.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Key,
	typename Value
>
struct BinTreeNodeBase: MapEntry<Key, Value> {
protected:
	T* m_parent;
	T* m_left;
	T* m_right;

protected:
	// overridable

	static
	void
	onXcg(
		T* node1,
		T* node2
	) {}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Node,
	typename Key,
	typename Value,
	typename Cmp,
	typename KeyArg,
	typename ValueArg
>
class BinTreeBase {
public:
	typedef sl::Iterator<Node> Iterator;
	typedef sl::ConstIterator<Node> ConstIterator;

protected:
	List<Node> m_nodeList;
	Node* m_root;
	Cmp m_cmp;

public:
	explicit BinTreeBase(const Cmp& cmp = Cmp()) {
		m_root = NULL;
		m_cmp = cmp;
	}

	Value&
	operator [] (KeyArg key) {
		return visit(key)->m_value;
	}

	const Value&
	operator [] (KeyArg key) const {
		return visit(key)->m_value;
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

	Iterator
	find(KeyArg key) {
		Node* node = m_root;

		while (node) {
			int cmp = m_cmp(key, node->m_key);
			if (cmp == 0)
				return node;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		return NULL;
	}

	ConstIterator
	find(KeyArg key) const {
		return ((BinTreeBase*)this)->find(key); // a simple const-cast
	}

	template <RelOpKind opKind>
	Iterator
	find(KeyArg key) {
		Node* node = m_root;
		Node* prevNode;
		int prevCmp;

		if (isEmpty())
			return NULL;

		while (node) {
			int cmp = m_cmp(key, node->m_key);
			if (cmp == 0)
				break; // exact match

			prevNode = node;
			prevCmp = cmp;
			node = cmp < 0 ? node->m_left : node->m_right;
		}

		ASSERT(node || prevNode);
		switch (opKind) {
		case RelOpKind_Lt:
			return node ? node->m_left : prevCmp > 0 ? prevNode : Iterator(prevNode).getPrev();

		case RelOpKind_Le:
			return node ? node : prevCmp > 0 ? prevNode : Iterator(prevNode).getPrev();

		case RelOpKind_Gt:
			return node ? node->m_right : prevCmp < 0 ? prevNode : Iterator(prevNode).getNext();

		case RelOpKind_Ge:
			return node ? node : prevCmp < 0 ? prevNode : Iterator(prevNode).getNext();

		default:
			ASSERT(opKind == RelOpKind_Eq);
			return node;
		}
	}

	template <RelOpKind opKind>
	ConstIterator
	find(KeyArg key) const {
		return ((BinTreeBase*)this)->find<opKind>(key); // a simple const-cast
	}

	Value
	findValue(
		KeyArg key,
		ValueArg undefinedValue
	) const {
		ConstIterator it = find(key);
		return it ? it->m_value : undefinedValue;
	}

	template <RelOpKind opKind>
	Value
	findValue(
		KeyArg key,
		ValueArg undefinedValue
	) const {
		ConstIterator it = find<opKind>(key);
		return it ? it->m_value : undefinedValue;
	}

	Iterator
	visit(KeyArg key) {
		Node* parent = NULL;
		Node* node = m_root;

		// find the place to insert

		int cmp;

		while (node) {
			cmp = m_cmp(key, node->m_key);
			if (cmp == 0)
				return node;

			parent = node;
			node = cmp < 0 ? node->m_left : node->m_right;
		}

		// create and insert new node

		node = new (mem::ZeroInit) Node;
		node->m_key = key;
		node->m_parent = parent;

		if (!parent) {
			m_root = node;
			m_nodeList.insertTail(node);
		} else if (cmp < 0) {
			parent->m_left = node;
			m_nodeList.insertBefore(node, parent);
		} else {
			parent->m_right = node;
			m_nodeList.insertAfter(node, parent);
		}

		static_cast<T*>(this)->onInsert(node);
		return node;
	}

	Iterator
	add(
		KeyArg key,
		ValueArg value
	) {
		Iterator it = visit(key);
		it->m_value = value;
		return it;
	}

	Iterator
	add(
		KeyArg key,
		ValueArg value,
		bool* isNew
	) {
		size_t prevCount = getCount();

		Iterator it = visit(key);
		it->m_value = value;

		if (isNew)
			*isNew = getCount() > prevCount;

		return it;
	}

	Iterator
	addIfNotExists(
		KeyArg key,
		ValueArg value
	) {
		size_t prevCount = getCount();

		Iterator it = visit(key);

		if (getCount() == prevCount)
			return NULL;

		it->m_value = value;
		return it;
	}

	void
	erase(Iterator it) {
		Node* node = *it;

		if (node->m_left && node->m_right) {
			ASSERT(node->m_prev == getRightmostChild(node->m_left));
			ASSERT(node->m_next == getLeftmostChild(node->m_right));

			xcg(node, (Node*)node->m_next);
			ASSERT(!node->m_left);

			// same effect:
			// xcg((Node*)node->m_prev, node);
			// ASSERT(!node->m_right);
		}

		Node* child = replaceWithChild(node);
		static_cast<T*>(this)->onErase(node, child);
		m_nodeList.erase(node);
	}

	bool
	eraseKey(KeyArg key) {
		Iterator it = find(key);
		if (!it)
			return false;

		erase(it);
		return true;
	}

	void
	clear() {
		m_nodeList.clear();
		m_root = NULL;
	}

protected:
	Node*
	getLeftmostChild(Node* node) {
		while (node->m_left)
			node = node->m_left;

		return node;
	}

	Node*
	getRightmostChild(Node* node) {
		while (node->m_right)
			node = node->m_right;

		return node;
	}

	void
	xcg(
		Node* prev,
		Node* next
	) {
		ASSERT(prev != next->m_right);
		ASSERT(next != prev->m_left);

		// handle direct parent-child relations

		if (prev == next->m_left) {
			Node* oldLeft = prev->m_left;
			Node* oldRight = prev->m_right;

			prev->m_left = next;
			prev->m_right = next->m_right;
			prev->m_parent = next->m_parent;

			next->m_left = oldLeft;
			next->m_right = oldRight;
			next->m_parent = prev;
		} else if (next == prev->m_right) {
			Node* oldParent = prev->m_parent;
			Node* oldLeft = prev->m_left;

			prev->m_left = next->m_left;
			prev->m_right = next->m_right;
			prev->m_parent = next;

			next->m_left = oldLeft;
			next->m_right = prev;
			next->m_parent = oldParent;
		} else {
			Node* oldParent = prev->m_parent;
			Node* oldLeft = prev->m_left;
			Node* oldRight = prev->m_right;

			prev->m_left = next->m_left;
			prev->m_right = next->m_right;
			prev->m_parent = next->m_parent;

			next->m_left = oldLeft;
			next->m_right = oldRight;
			next->m_parent = oldParent;
		}

		// fixup parents

		if (!prev->m_parent)
			m_root = prev;
		else if (prev->m_parent->m_left == next)
			prev->m_parent->m_left = prev;
		else
			prev->m_parent->m_right = prev;

		if (!next->m_parent)
			m_root = next;
		else if (next->m_parent->m_left == prev)
			next->m_parent->m_left = next;
		else
			next->m_parent->m_right = next;

		// fixup children

		if (prev->m_left)
			prev->m_left->m_parent = prev;

		if (prev->m_right)
			prev->m_right->m_parent = prev;

		if (next->m_left)
			next->m_left->m_parent = next;

		if (next->m_right)
			next->m_right->m_parent = next;

		Node::onXcg(prev, next);
	}

	Node*
	replaceWithChild(Node* node) {
		Node* child = node->m_right ? node->m_right : node->m_left;

		if (!node->m_parent) {
			ASSERT(node == m_root);
			m_root = child;
		} else if (node == node->m_parent->m_left)
			node->m_parent->m_left = child;
		else
			node->m_parent->m_right = child;

		if (child)
			child->m_parent = node->m_parent;

		return child;
	}

	void
	rotateLeft(Node* x) {
		Node* y = x->m_right;
		ASSERT(y);

		x->m_right = y->m_left;

		if (y->m_left)
			y->m_left->m_parent = x;

		y->m_parent = x->m_parent;

		if (!x->m_parent)
			m_root = y;
		else if (x == x->m_parent->m_left)
			x->m_parent->m_left = y;
		else
			x->m_parent->m_right = y;

		y->m_left = x;
		x->m_parent = y;
	}

	void
	rotateRight(Node* x) {
		Node* y = x->m_left;
		ASSERT(y);

		x->m_left = y->m_right;

		if (y->m_right)
			y->m_right->m_parent = x;

		y->m_parent = x->m_parent;

		if (!x->m_parent)
			m_root = y;
		else if (x == x->m_parent->m_right)
			x->m_parent->m_right = y;
		else
			x->m_parent->m_left = y;

		y->m_right = x;
		x->m_parent = y;
	}

	void
	adjustKey(
		Node* x,
		KeyArg key
	) {
		ASSERT(!x->m_prev || m_cmp(((Node*)x->m_prev)->m_key, key) < 0);
		ASSERT(!x->m_next || m_cmp(((Node*)x->m_next)->m_key, key) > 0);
		x->m_key = key;
	}

	// overridables: tree rebalancing on insert/delete

	void
	onInsert(Node* node) {}

	void
	onErase(
		Node* node,
		Node* child
	) {
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
