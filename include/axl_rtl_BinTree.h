// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BINTREE_H

#include "axl_rtl_List.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename Key>
class KeyNodeData
{
public:
	Key m_key;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key, 
	typename Value
	>
class KeyValueNodeData: public KeyNodeData <Key>
{
public:
	Value m_value;
};  

//.............................................................................

template <
	typename T,
	typename NodeData
	>
class BinTreeNodeBase: 
	public rtl::ListLink,
	public NodeData
{
public:
	T* m_parent;
	T* m_left;
	T* m_right;

	BinTreeNodeBase ()
	{
		m_parent = NULL;
		m_left = NULL;
		m_right = NULL; 
	}

	// overridable

	static 
	void 
	onXcg (
		T* node1, 
		T* node2
		)
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum BinTreeFindExKind
{
	BinTreeFindExKind_Eq = 0,
	BinTreeFindExKind_Lt,
	BinTreeFindExKind_Le,
	BinTreeFindExKind_Gt,
	BinTreeFindExKind_Ge,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Key,
	typename Cmp,
	typename Node
	>
class BinTreeBase
{
public:
	typedef Key Key;
	typedef Cmp Cmp;
	typedef Node Node;

	typedef Iterator <Node> Iterator;

protected:
	StdList <Node> m_nodeList;
	Node* m_root;

public:
	BinTreeBase ()
	{ 
		m_root = NULL; 
	}
	
	bool 
	isEmpty () const
	{ 
		return m_nodeList.isEmpty (); 
	}

	size_t 
	getCount () const
	{ 
		return m_nodeList.getCount (); 
	}

	Iterator 
	getHead () const
	{ 
		return m_nodeList.getHead (); 
	}

	Iterator 
	getTail () const
	{ 
		return m_nodeList.getTail (); 
	}

	Iterator 
	find (const Key& key) const
	{
		Node* node = m_root;

		while (node) 
		{
			int cmp = Cmp () (key, node->m_key);
			if (cmp == 0) 
				return node;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		return NULL;
	}

	Iterator 
	findEx (
		const Key& key,
		BinTreeFindExKind findExKind
		) const
	{
		Node* node = m_root;
		Node* prevNode;
		int prevCmp;

		if (isEmpty ())
			return NULL;

		while (node) 
		{
			int cmp = Cmp () (key, node->m_key);			
			if (cmp == 0) 
				break; // exact match

			prevNode = node;
			prevCmp = cmp;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		ASSERT (node || prevNode);
		switch (findExKind)
		{
		case BinTreeFindExKind_Lt:
			return node ? node->m_left : prevCmp > 0 ? prevNode : Iterator (prevNode).getPrev ();

		case BinTreeFindExKind_Le:
			return node ? node : prevCmp > 0 ? prevNode : Iterator (prevNode).getPrev ();

		case BinTreeFindExKind_Gt:
			return node ? node->m_right : prevCmp < 0 ? prevNode : Iterator (prevNode).getNext ();

		case BinTreeFindExKind_Ge:
			return node ? node : prevCmp < 0 ? prevNode : Iterator (prevNode).getNext ();
		
		case BinTreeFindExKind_Eq:
		default:
			return node;
		}		
	}

	Iterator 
	visit (const Key& key)
	{
		Node* parent = NULL;
		Node* node = m_root;

		// find the place to insert

		int cmp;

		while (node) 
		{
			cmp = Cmp () (key, node->m_key);
			if (cmp == 0) 
				return node;

			parent = node;
			node = cmp < 0 ? node->m_left : node->m_right;
		}

		// create and insert new node

		node = AXL_MEM_NEW (Node);
		node->m_key = key;
		node->m_parent = parent;
		node->m_left = NULL;
		node->m_right = NULL;

		if (!parent)
		{
			m_root = node;
			m_nodeList.insertTail (node);
		}
		else if (cmp < 0)
		{
			parent->m_left = node;
			m_nodeList.insertBefore (node, parent);
		}
		else
		{
			parent->m_right = node;
			m_nodeList.insertAfter (node, parent);
		}

		static_cast <T*> (this)->onInsert (node);
		return node;
	}

	void 
	erase (Iterator it)
	{
		Node* node = *it;

		if (node->m_left && node->m_right) 
		{
			Node* next = (Node*) node->m_next;
			ASSERT (next == getLeftmostChild (node->m_right));
			xcg (node, next);
		}

		static_cast <T*> (this)->onErase (node);
		m_nodeList.erase (node);
	}

	bool 
	eraseByKey (const Key& key)
	{
		Iterator it = find (key);
		if (!it)
			return false;

		erase (it);
		return true;
	}

	void 
	clear ()
	{ 
		m_nodeList.clear ();
		m_root = NULL;
	}

protected:
	Node* 
	getLeftmostChild (Node* node)
	{
		while (node->m_left)
			node = node->m_left;
		
		return node;
	}

	Node* 
	getRightmostChild (Node* node)
	{
		while (node->m_right)
			node = node->m_right;
		
		return node;
	}

	void 
	xcg (
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

		Node::onXcg (node1, node2);
	}

	Node* 
	replaceWithChild (Node* node)
	{		
		Node* child = node->m_right ? node->m_right : node->m_left;
		 
		if (!node->m_parent)
		{
			ASSERT (node == m_root);
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
	rotateLeft (Node* x)
	{
		Node* y = x->m_right;
		ASSERT (y);

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
	rotateRight (Node* x)
	{
		Node* y = x->m_left;
		ASSERT (y);

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
	onInsert (Node* node)
	{
	}

	void 
	onErase (Node* node)
	{ 
		replaceWithChild (node); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

