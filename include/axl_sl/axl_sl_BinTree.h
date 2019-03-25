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
struct BinTreeNodeBase: MapEntry<Key, Value>
{
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
		)
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum BinTreeFindRelOp
{
	BinTreeFindRelOp_Eq = 0,
	BinTreeFindRelOp_Lt,
	BinTreeFindRelOp_Le,
	BinTreeFindRelOp_Gt,
	BinTreeFindRelOp_Ge,
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
class BinTreeBase
{
public:
	typedef sl::Iterator<Node> Iterator;
	typedef sl::ConstIterator<Node> ConstIterator;

protected:
	List<Node> m_nodeList;
	Node* m_root;
	Cmp m_cmp;

public:
	explicit BinTreeBase(const Cmp& cmp = Cmp())
	{
		m_root = NULL;
		m_cmp = cmp;
	}

	Value&
	operator [] (KeyArg key)
	{
		return visit(key)->m_value;
	}

	const Value&
	operator [] (KeyArg key) const
	{
		return visit(key)->m_value;
	}

	bool
	isEmpty() const
	{
		return m_nodeList.isEmpty();
	}

	size_t
	getCount() const
	{
		return m_nodeList.getCount();
	}

	Iterator
	getHead()
	{
		return m_nodeList.getHead();
	}

	ConstIterator
	getHead() const
	{
		return m_nodeList.getHead();
	}

	Iterator
	getTail()
	{
		return m_nodeList.getTail();
	}

	ConstIterator
	getTail() const
	{
		return m_nodeList.getTail();
	}

	Iterator
	find(KeyArg key)
	{
		Node* node = m_root;

		while (node)
		{
			int cmp = m_cmp(key, node->m_key);
			if (cmp == 0)
				return node;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		return NULL;
	}

	ConstIterator
	find(KeyArg key) const
	{
		return ((BinTreeBase*)this)->find(key); // a simple const-cast
	}

	Iterator
	find(
		KeyArg key,
		BinTreeFindRelOp relOp
		)
	{
		Node* node = m_root;
		Node* prevNode;
		int prevCmp;

		if (isEmpty())
			return NULL;

		while (node)
		{
			int cmp = m_cmp(key, node->m_key);
			if (cmp == 0)
				break; // exact match

			prevNode = node;
			prevCmp = cmp;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		ASSERT(node || prevNode);
		switch (relOp)
		{
		case BinTreeFindRelOp_Lt:
			return node ? node->m_left : prevCmp > 0 ? prevNode : Iterator(prevNode).getPrev();

		case BinTreeFindRelOp_Le:
			return node ? node : prevCmp > 0 ? prevNode : Iterator(prevNode).getPrev();

		case BinTreeFindRelOp_Gt:
			return node ? node->m_right : prevCmp < 0 ? prevNode : Iterator(prevNode).getNext();

		case BinTreeFindRelOp_Ge:
			return node ? node : prevCmp < 0 ? prevNode : Iterator(prevNode).getNext();

		case BinTreeFindRelOp_Eq:
		default:
			return node;
		}
	}

	ConstIterator
	find(
		KeyArg key,
		BinTreeFindRelOp relOp
		) const
	{
		return ((BinTreeBase*)this)->find(key, relOp); // a simple const-cast
	}

	Value
	findValue(
		KeyArg key,
		ValueArg undefinedValue
		) const
	{
		ConstIterator it = find(key);
		return it ? it->m_value : undefinedValue;
	}

	Value
	findValue(
		KeyArg key,
		BinTreeFindRelOp relOp,
		ValueArg undefinedValue
		) const
	{
		ConstIterator it = find(key, relOp);
		return it ? it->m_value : undefinedValue;
	}

	Iterator
	visit(KeyArg key)
	{
		Node* parent = NULL;
		Node* node = m_root;

		// find the place to insert

		int cmp;

		while (node)
		{
			cmp = m_cmp(key, node->m_key);
			if (cmp == 0)
				return node;

			parent = node;
			node = cmp < 0 ? node->m_left : node->m_right;
		}

		// create and insert new node

		node = AXL_MEM_ZERO_NEW(Node);
		node->m_key = key;
		node->m_parent = parent;
		node->m_left = NULL;
		node->m_right = NULL;

		if (!parent)
		{
			m_root = node;
			m_nodeList.insertTail(node);
		}
		else if (cmp < 0)
		{
			parent->m_left = node;
			m_nodeList.insertBefore(node, parent);
		}
		else
		{
			parent->m_right = node;
			m_nodeList.insertAfter(node, parent);
		}

		static_cast<T*> (this)->onInsert(node);
		return node;
	}

	Iterator
	add(
		KeyArg key,
		ValueArg value,
		bool* isNew = NULL
		)
	{
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
		)
	{
		size_t prevCount = getCount();

		Iterator it = visit(key);

		if (getCount() == prevCount)
			return NULL;

		it->m_value = value;
		return it;
	}

	void
	erase(Iterator it)
	{
		Node* node = *it;

		if (node->m_left && node->m_right)
		{
			Node* next = (Node*)node->m_next;
			ASSERT(next == getLeftmostChild(node->m_right));
			xcg(node, next);
		}

		static_cast<T*> (this)->onErase(node);
		m_nodeList.erase(node);
	}

	bool
	eraseKey(KeyArg key)
	{
		Iterator it = find(key);
		if (!it)
			return false;

		erase(it);
		return true;
	}

	void
	clear()
	{
		m_nodeList.clear();
		m_root = NULL;
	}

protected:
	Node*
	getLeftmostChild(Node* node)
	{
		while (node->m_left)
			node = node->m_left;

		return node;
	}

	Node*
	getRightmostChild(Node* node)
	{
		while (node->m_right)
			node = node->m_right;

		return node;
	}

	void
	xcg(
		Node* node1,
		Node* node2
		)
	{
		Node* oldParent = node1->m_parent;
		Node* oldLeft = node1->m_left;
		Node* oldRight = node1->m_right;

		// special cases: direct parent-child relations

		if (node1 == node2->m_left)
		{
			node1->m_left = node2;
			node1->m_right = node2->m_right;
			node1->m_parent = node2->m_parent;

			node2->m_left = oldLeft;
			node2->m_right = oldRight;
			node2->m_parent = node1;
		}
		else if (node1 == node2->m_right)
		{
			node1->m_left = node2->m_left;
			node1->m_right = node2;
			node1->m_parent = node2->m_parent;

			node2->m_left = oldLeft;
			node2->m_right = oldRight;
			node2->m_parent = node1;
		}
		else if (node2 == node1->m_left)
		{
			node1->m_left = node2->m_left;
			node1->m_right = node2->m_right;
			node1->m_parent = node2;

			node2->m_left = node1;
			node2->m_right = oldRight;
			node2->m_parent = oldParent;
		}
		else if (node2 == node1->m_right)
		{
			node1->m_left = node2->m_left;
			node1->m_right = node2->m_right;
			node1->m_parent = node2;

			node2->m_left = oldLeft;
			node2->m_right = node1;
			node2->m_parent = oldParent;
		}
		else
		{
			node1->m_left = node2->m_left;
			node1->m_right = node2->m_right;
			node1->m_parent = node2->m_parent;

			node2->m_left = oldLeft;
			node2->m_right = oldRight;
			node2->m_parent = oldParent;
		}

		// fixup parents

		if (!node1->m_parent)
			m_root = node1;
		else if (node1->m_parent->m_left == node2)
			node1->m_parent->m_left = node1;
		else
			node1->m_parent->m_right = node1;

		if (!node2->m_parent)
			m_root = node2;
		else if (node2->m_parent->m_left == node1)
			node2->m_parent->m_left = node2;
		else
			node2->m_parent->m_right = node2;

		// fixup children

		if (node1->m_left)
			node1->m_left->m_parent = node1;

		if (node1->m_right)
			node1->m_right->m_parent = node1;

		if (node2->m_left)
			node2->m_left->m_parent = node2;

		if (node2->m_right)
			node2->m_right->m_parent = node2;

		Node::onXcg(node1, node2);
	}

	Node*
	replaceWithChild(Node* node)
	{
		Node* child = node->m_right ? node->m_right : node->m_left;

		if (!node->m_parent)
		{
			ASSERT(node == m_root);
			m_root = child;
		}
		else if (node == node->m_parent->m_left)
			node->m_parent->m_left = child;
		else
			node->m_parent->m_right = child;

		if (child)
			child->m_parent = node->m_parent;

		return child;
	}

	void
	rotateLeft(Node* x)
	{
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
	rotateRight(Node* x)
	{
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

	// overridables: tree rebalancing on insert/delete

	void
	onInsert(Node* node)
	{
	}

	void
	onErase(Node* node)
	{
		replaceWithChild(node);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
