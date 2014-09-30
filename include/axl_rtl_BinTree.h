// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BINTREE_H

#include "axl_rtl_List.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename TKey>
class CKeyT
{
public:
	TKey m_key;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey, 
	typename TValue
	>
class CKeyValueT: public CKeyT <TKey>
{
public:
	TValue m_value;
};  

//.............................................................................

template <
	typename T,
	typename TNodeData
	>
class CBinTreeNodeBaseT: 
	public rtl::TListLink,
	public TNodeData
{
public:
	T* m_parent;
	T* m_left;
	T* m_right;

	CBinTreeNodeBaseT ()
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

enum EBinTreeFindEx
{
	EBinTreeFindEx_Eq = 0,
	EBinTreeFindEx_Lt,
	EBinTreeFindEx_Le,
	EBinTreeFindEx_Gt,
	EBinTreeFindEx_Ge,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TKey,
	typename TCmp,
	typename TNode
	>
class CBinTreeBaseT
{
public:
	typedef TKey CKey;
	typedef TCmp CCmp;
	typedef TNode CNode;

	typedef CIteratorT <CNode> CIterator;

protected:
	CStdListT <CNode> m_nodeList;
	CNode* m_root;

public:
	CBinTreeBaseT ()
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

	CIterator 
	getHead () const
	{ 
		return m_nodeList.getHead (); 
	}

	CIterator 
	getTail () const
	{ 
		return m_nodeList.getTail (); 
	}

	CIterator 
	find (const CKey& key) const
	{
		CNode* node = m_root;

		while (node) 
		{
			int cmp = CCmp () (key, node->m_key);
			if (cmp == 0) 
				return node;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		return NULL;
	}

	CIterator 
	findEx (
		const CKey& key,
		EBinTreeFindEx findExKind
		) const
	{
		CNode* node = m_root;
		CNode* prevNode;
		int prevCmp;

		if (isEmpty ())
			return NULL;

		while (node) 
		{
			int cmp = CCmp () (key, node->m_key);			
			if (cmp == 0) 
				break; // exact match

			prevNode = node;
			prevCmp = cmp;

			node = cmp < 0 ? node->m_left : node->m_right;
		}

		ASSERT (node || prevNode);
		switch (findExKind)
		{
		case EBinTreeFindEx_Lt:
			return node ? node->m_left : prevCmp > 0 ? prevNode : CIterator (prevNode).getPrev ();

		case EBinTreeFindEx_Le:
			return node ? node : prevCmp > 0 ? prevNode : CIterator (prevNode).getPrev ();

		case EBinTreeFindEx_Gt:
			return node ? node->m_right : prevCmp < 0 ? prevNode : CIterator (prevNode).getNext ();

		case EBinTreeFindEx_Ge:
			return node ? node : prevCmp < 0 ? prevNode : CIterator (prevNode).getNext ();
		
		case EBinTreeFindEx_Eq:
		default:
			return node;
		}		
	}

	CIterator 
	visit (const CKey& key)
	{
		CNode* parent = NULL;
		CNode* node = m_root;

		// find the place to insert

		int cmp;

		while (node) 
		{
			cmp = CCmp () (key, node->m_key);
			if (cmp == 0) 
				return node;

			parent = node;
			node = cmp < 0 ? node->m_left : node->m_right;
		}

		// create and insert new node

		node = AXL_MEM_NEW (CNode);
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
	erase (CIterator it)
	{
		CNode* node = *it;

		if (node->m_left && node->m_right) 
		{
			CNode* next = (CNode*) node->m_next;
			ASSERT (next == getLeftmostChild (node->m_right));
			xcg (node, next);
		}

		static_cast <T*> (this)->onErase (node);
		m_nodeList.erase (node);
	}

	bool 
	eraseByKey (const CKey& key)
	{
		CIterator it = find (key);
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
	CNode* 
	getLeftmostChild (CNode* node)
	{
		while (node->m_left)
			node = node->m_left;
		
		return node;
	}

	CNode* 
	getRightmostChild (CNode* node)
	{
		while (node->m_right)
			node = node->m_right;
		
		return node;
	}

	void 
	xcg (
		CNode* node1, 
		CNode* node2
		)
	{
		CNode* oldParent = node1->m_parent;
		CNode* oldLeft = node1->m_left;
		CNode* oldRight = node1->m_right;

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

		CNode::onXcg (node1, node2);
	}

	CNode* 
	replaceWithChild (CNode* node)
	{		
		CNode* child = node->m_right ? node->m_right : node->m_left;
		 
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
	rotateLeft (CNode* x)
	{
		CNode* y = x->m_right;
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
	rotateRight (CNode* x)
	{
		CNode* y = x->m_left;
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
	onInsert (CNode* node)
	{
	}

	void 
	onErase (CNode* node)
	{ 
		replaceWithChild (node); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

