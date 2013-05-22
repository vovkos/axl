// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	TKey m_Key;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey, 
	typename TValue
	>
class CKeyValueT: public CKeyT <TKey>
{
public:
	TValue m_Value;
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
	T* m_pParent;
	T* m_pLeft;
	T* m_pRight;

	CBinTreeNodeBaseT ()
	{
		m_pParent = NULL;
		m_pLeft = NULL;
		m_pRight = NULL; 
	}

	// overridable

	static 
	void 
	OnXcg (
		T* pNode1, 
		T* pNode2
		)
	{
	}
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
	CStdListT <CNode> m_NodeList;
	CNode* m_pRoot;

public:
	CBinTreeBaseT ()
	{ 
		m_pRoot = NULL; 
	}
	
	bool 
	IsEmpty () const
	{ 
		return m_NodeList.IsEmpty (); 
	}

	size_t 
	GetCount () const
	{ 
		return m_NodeList.GetCount (); 
	}

	CIterator 
	GetHead () const
	{ 
		return m_NodeList.GetHead (); 
	}

	CIterator 
	GetTail () const
	{ 
		return m_NodeList.GetTail (); 
	}

	CIterator 
	Find (const CKey& Key) const
	{
		CNode* pNode = m_pRoot;

		while (pNode) 
		{
			int Cmp = CCmp () (Key, pNode->m_Key);
			if (Cmp == 0) 
				return pNode;

			pNode = Cmp < 0 ? pNode->m_pLeft : pNode->m_pRight;
		}

		return NULL;
	}

	template <class R>
	CIterator 
	FindEx (const CKey& Key) const
	{
		CNode* pResult = NULL;
		CNode* pNode = m_pRoot;

		if (IsEmpty ())
			return NULL;

		while (pNode) 
		{
			int Cmp = CCmp () (pNode->m_Key, Key); // invert arg key and node key!
			
			if (R () (Cmp, 0))
				pResult = pNode;

			if (Cmp == 0) 
				break; // exact match

			pNode = Cmp < 0 ? pNode->m_pRight : pNode->m_pLeft; // invert arg key and node key!
		}

		return pResult;
	}

	CIterator 
	Goto (const CKey& Key)
	{
		CNode* pParent = NULL;
		CNode* pNode = m_pRoot;

		// find the place to insert

		int Cmp;

		while (pNode) 
		{
			Cmp = CCmp () (Key, pNode->m_Key);
			if (Cmp == 0) 
				return pNode;

			pParent = pNode;
			pNode = Cmp < 0 ? pNode->m_pLeft : pNode->m_pRight;
		}

		// create and insert new node

		pNode = AXL_MEM_NEW (CNode);
		pNode->m_Key = Key;
		pNode->m_pParent = pParent;
		pNode->m_pLeft = NULL;
		pNode->m_pRight = NULL;

		if (!pParent)
		{
			m_pRoot = pNode;
			m_NodeList.InsertTail (pNode);
		}
		else if (Cmp < 0)
		{
			pParent->m_pLeft = pNode;
			m_NodeList.InsertBefore (pNode, pParent);
		}
		else
		{
			pParent->m_pRight = pNode;
			m_NodeList.InsertAfter (pNode, pParent);
		}

		static_cast <T*> (this)->OnInsert (pNode);
		return pNode;
	}

	void 
	Delete (CIterator It)
	{
		CNode* pNode = *It;

		if (pNode->m_pLeft && pNode->m_pRight) 
		{
			CNode* pNext = (CNode*) pNode->m_pNext;
			ASSERT (pNext == GetLeftmostChild (pNode->m_pRight));
			Xcg (pNode, pNext);
		}

		static_cast <T*> (this)->OnDelete (pNode);
		m_NodeList.Delete (pNode);
	}

	bool 
	DeleteByKey (const CKey& Key)
	{
		CIterator It = Find (Key);
		if (!It)
			return false;

		Delete (It);
		return true;
	}

	void 
	Clear ()
	{ 
		m_NodeList.Clear ();
		m_pRoot = NULL;
	}

protected:
	CNode* 
	GetLeftmostChild (CNode* pNode)
	{
		while (pNode->m_pLeft)
			pNode = pNode->m_pLeft;
		
		return pNode;
	}

	CNode* 
	GetRightmostChild (CNode* pNode)
	{
		while (pNode->m_pRight)
			pNode = pNode->m_pRight;
		
		return pNode;
	}

	void 
	Xcg (
		CNode* pNode1, 
		CNode* pNode2
		)
	{
		CNode* pOldParent = pNode1->m_pParent;
		CNode* pOldLeft = pNode1->m_pLeft;
		CNode* pOldRight = pNode1->m_pRight;

		// special cases: direct parent-child relations

		if (pNode1 == pNode2->m_pLeft)
		{
			pNode1->m_pLeft = pNode2;
			pNode1->m_pRight = pNode2->m_pRight;
			pNode1->m_pParent = pNode2->m_pParent;

			pNode2->m_pLeft = pOldLeft;
			pNode2->m_pRight = pOldRight;
			pNode2->m_pParent = pNode1;
		}
		else if (pNode1 == pNode2->m_pRight)
		{
			pNode1->m_pLeft = pNode2->m_pLeft;
			pNode1->m_pRight = pNode2;
			pNode1->m_pParent = pNode2->m_pParent;

			pNode2->m_pLeft = pOldLeft;
			pNode2->m_pRight = pOldRight;
			pNode2->m_pParent = pNode1;
		}
		else if (pNode2 == pNode1->m_pLeft)
		{
			pNode1->m_pLeft = pNode2->m_pLeft;
			pNode1->m_pRight = pNode2->m_pRight;
			pNode1->m_pParent = pNode2;

			pNode2->m_pLeft = pNode1;
			pNode2->m_pRight = pOldRight;
			pNode2->m_pParent = pOldParent;
		}
		else if (pNode2 == pNode1->m_pRight)
		{
			pNode1->m_pLeft = pNode2->m_pLeft;
			pNode1->m_pRight = pNode2->m_pRight;
			pNode1->m_pParent = pNode2;

			pNode2->m_pLeft = pOldLeft;
			pNode2->m_pRight = pNode1;
			pNode2->m_pParent = pOldParent;
		}
		else 
		{
			pNode1->m_pLeft = pNode2->m_pLeft;
			pNode1->m_pRight = pNode2->m_pRight;
			pNode1->m_pParent = pNode2->m_pParent;

			pNode2->m_pLeft = pOldLeft;
			pNode2->m_pRight = pOldRight;
			pNode2->m_pParent = pOldParent;
		}

		// fixup parents

		if (!pNode1->m_pParent)
			m_pRoot = pNode1;
		else if (pNode1->m_pParent->m_pLeft == pNode2)
			pNode1->m_pParent->m_pLeft = pNode1;
		else
			pNode1->m_pParent->m_pRight = pNode1;

		if (!pNode2->m_pParent)
			m_pRoot = pNode2;
		else if (pNode2->m_pParent->m_pLeft == pNode1)
			pNode2->m_pParent->m_pLeft = pNode2;
		else
			pNode2->m_pParent->m_pRight = pNode2;

		// fixup children

		if (pNode1->m_pLeft)
			pNode1->m_pLeft->m_pParent = pNode1;

		if (pNode1->m_pRight)
			pNode1->m_pRight->m_pParent = pNode1;

		if (pNode2->m_pLeft)
			pNode2->m_pLeft->m_pParent = pNode2;

		if (pNode2->m_pRight)
			pNode2->m_pRight->m_pParent = pNode2;

		CNode::OnXcg (pNode1, pNode2);
	}

	CNode* 
	ReplaceWithChild (CNode* pNode)
	{		
		CNode* pChild = pNode->m_pRight ? pNode->m_pRight : pNode->m_pLeft;
		 
		if (!pNode->m_pParent)
		{
			ASSERT (pNode == m_pRoot);
			m_pRoot = pChild;
		}
		else if (pNode == pNode->m_pParent->m_pLeft)
			pNode->m_pParent->m_pLeft = pChild;
		else
			pNode->m_pParent->m_pRight = pChild;

		if (pChild)
			pChild->m_pParent = pNode->m_pParent;

		return pChild;
	}

	void 
	RotateLeft (CNode* x)
	{
		CNode* y = x->m_pRight;
		ASSERT (y);

		x->m_pRight = y->m_pLeft;

		if (y->m_pLeft) 
			y->m_pLeft->m_pParent = x;

		y->m_pParent = x->m_pParent;
	    
		if (!x->m_pParent) 
			m_pRoot = y;
		else if (x == x->m_pParent->m_pLeft)
			x->m_pParent->m_pLeft = y;
		else
			x->m_pParent->m_pRight = y;

		y->m_pLeft = x;
		x->m_pParent = y;
	}

	void 
	RotateRight (CNode* x)
	{
		CNode* y = x->m_pLeft;
		ASSERT (y);

		x->m_pLeft = y->m_pRight;

		if (y->m_pRight) 
			y->m_pRight->m_pParent = x;

		y->m_pParent = x->m_pParent;
	    
		if (!x->m_pParent) 
			m_pRoot = y;
		else if (x == x->m_pParent->m_pRight)
			x->m_pParent->m_pRight = y;
		else
			x->m_pParent->m_pLeft = y;

		y->m_pRight = x;
		x->m_pParent = y;
	}

	// overridables: tree rebalancing on insert/delete

	void 
	OnInsert (CNode* pNode)
	{
	}

	void 
	OnDelete (CNode* pNode)
	{ 
		ReplaceWithChild (pNode); 
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

