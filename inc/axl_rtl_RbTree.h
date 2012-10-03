// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_RBTREE_H

#include "axl_rtl_BinTree.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename T,
	typename TNodeData
	>
class CRbTreeNodeBaseT: public CBinTreeNodeBaseT <T, TNodeData>
{
public:
	enum EColor
	{
		EColor_Black = 0,
		EColor_Red   = 1
	};

	int m_Color;

	CRbTreeNodeBaseT ()
	{ 
		m_Color = EColor_Black; 
	}

	static 
	void 
	OnXcg (
		T* pNode1, 
		T* pNode2
		)
	{
		int OldColor = pNode1->m_Color;
		pNode1->m_Color = pNode2->m_Color;
		pNode2->m_Color = OldColor;
	}

	static 
	int 
	GetColor (T* pNode)
	{ 
		return pNode ? pNode->m_Color : EColor_Black; 
	}

#ifdef _DEBUG
	static 
	size_t 
	AssertValid (T* pNode)
	{
		if (!pNode)
			return 1;

		size_t LeftCount = AssertValid (pNode->m_pLeft);
		size_t RightCount = AssertValid (pNode->m_pRight);

		// should be the same count of black nodes on the way down

		ASSERT (LeftCount == RightCount);

		if (pNode->m_Color == CNode::EColor_Black)
		{
			LeftCount++;
		}
		else // red color cannot have red children
		{
			ASSERT (GetColor (pNode->m_pLeft) == EColor_Black);
			ASSERT (GetColor (pNode->m_pRight) == EColor_Black);
		}

		return LeftCount;
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename TKey>
class CRbTreeNodeT: public CRbTreeNodeBaseT <
	CRbTreeNodeT <TKey>, 
	CKeyT <TKey> 
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey,
	typename TValue
	>
class CRbTreeMapNodeT: public CRbTreeNodeBaseT <
	CRbTreeMapNodeT <TKey, TValue>, 
	CKeyValueT <TKey, TValue> 
	>
{
};

//.............................................................................

template <
	typename TKey,
	typename TCmp,
	typename TNode = CRbTreeNodeT <TKey>
	>
class CRbTreeT: public CBinTreeBaseT <
	CRbTreeT <TKey, TCmp, TNode>, 
	TKey,
	TCmp,
	TNode
	>
{
public:
#ifdef _DEBUG
	void 
	AssertValid ()
	{ 
		CNode::AssertValid (m_pRoot); 
	}
#endif

	void 
	OnInsert (CNode* x)
	{
		x->m_Color = CNode::EColor_Red; // make new node red

		// check Red-Black properties 
		while (x != m_pRoot && static_cast <CNode*>(x->m_pParent)->m_Color == CNode::EColor_Red) 
		{
			// we have a violation 
			if (x->m_pParent == x->m_pParent->m_pParent->m_pLeft) 
			{
				CNode *y = static_cast <CNode*>(x->m_pParent->m_pParent->m_pRight);
				if (CNode::GetColor (y) == CNode::EColor_Red) 
				{
					// uncle is red 
					static_cast <CNode*>(x->m_pParent)->m_Color = CNode::EColor_Black;
					y->m_Color = CNode::EColor_Black;
					static_cast <CNode*>(x->m_pParent->m_pParent)->m_Color = CNode::EColor_Red;
					x = static_cast <CNode*>(x->m_pParent->m_pParent);
				} 
				else 
				{
					// uncle is black
					if (x == x->m_pParent->m_pRight) 
					{
						// make x a m_pLeft child
						x = static_cast <CNode*>(x->m_pParent);
						RotateLeft (x);
					}

					// recolor and rotate
					static_cast <CNode*>(x->m_pParent)->m_Color = CNode::EColor_Black;
					static_cast <CNode*>(x->m_pParent->m_pParent)->m_Color = CNode::EColor_Red;
					RotateRight (static_cast <CNode*>(x->m_pParent->m_pParent));
				}
			} 
			else 
			{
				// mirror image of above code
				CNode *y = x->m_pParent->m_pParent->m_pLeft;
				if (CNode::GetColor (y) == CNode::EColor_Red) 
				{
					// uncle is red
					x->m_pParent->m_Color = CNode::EColor_Black;
					y->m_Color = CNode::EColor_Black;
					x->m_pParent->m_pParent->m_Color = CNode::EColor_Red;
					x = x->m_pParent->m_pParent;
				} 
				else 
				{
					// uncle is black
					if (x == x->m_pParent->m_pLeft) 
					{
						x = x->m_pParent;
						RotateRight (x);
					}

					x->m_pParent->m_Color = CNode::EColor_Black;
					x->m_pParent->m_pParent->m_Color = CNode::EColor_Red;
					RotateLeft (x->m_pParent->m_pParent);
				}
			}
		}

		m_pRoot->m_Color = CNode::EColor_Black;

	#if _AXL_RTL_RBTREE_ASSERT_VALID
		AssertValid ();
	#endif
	}

	void 
	OnDelete (CNode* pNode)
	{ 
		CNode* x = ReplaceWithChild (pNode);
		CNode* p = pNode->m_pParent;

		// no need to fixup red node deletion
		if (pNode->m_Color == CNode::EColor_Red) 
			return;

		while (x != m_pRoot && CNode::GetColor (x) == CNode::EColor_Black)
		{
			// due to invariants of RB-tree black node must have a non-null sibling 
			// if this sibling is red, both of its children are non-null black 

			if (x == p->m_pLeft) 
			{
				CNode *w = p->m_pRight; // non-null, see above

				if (w->m_Color == CNode::EColor_Red) 
				{
					w->m_Color = CNode::EColor_Black;
					p->m_Color = CNode::EColor_Red;
					RotateLeft (p);
					w = p->m_pRight; // non-null black, see above
				}

				// w is non-null black

				if (CNode::GetColor (w->m_pLeft) == CNode::EColor_Black && 
					CNode::GetColor (w->m_pRight) == CNode::EColor_Black) 
				{
					w->m_Color = CNode::EColor_Red;
					x = p;
				} 
				else // at least one of w children is red 
				{
					if (CNode::GetColor (w->m_pRight) == CNode::EColor_Black) 
					{
						// w->m_pLeft is red
						w->m_pLeft->m_Color = CNode::EColor_Black; 
						w->m_Color = CNode::EColor_Red;
						RotateRight (w);
						w = p->m_pRight; // non-null black (just rotated here)
					}
	                
					w->m_Color = p->m_Color;
					p->m_Color = CNode::EColor_Black;
					w->m_pRight->m_Color = CNode::EColor_Black; // w->m_pRight is non-null

					RotateLeft (p);
					x = m_pRoot;
				}
			} 
			else 
			{
				CNode *w = p->m_pLeft; // non-null, see above

				if (w->m_Color == CNode::EColor_Red) 
				{
					w->m_Color = CNode::EColor_Black;
					p->m_Color = CNode::EColor_Red;
					RotateRight (p);
					w = p->m_pLeft; // non-null black, see above
				}
	            
				// w is non-null black

				if (CNode::GetColor (w->m_pLeft) == CNode::EColor_Black && 
					CNode::GetColor (w->m_pRight) == CNode::EColor_Black) 
				{
					w->m_Color = CNode::EColor_Red;
					x = p;
				} 
				else // at least one of w children is red 
				{
					if (CNode::GetColor (w->m_pLeft) == CNode::EColor_Black) 
					{
						// w->m_pRight is red
						w->m_pRight->m_Color = CNode::EColor_Black;
						w->m_Color = CNode::EColor_Red;
						RotateLeft (w);
						w = p->m_pLeft; // non-null black (just rotated)
					}

					w->m_Color = p->m_Color;
					p->m_Color = CNode::EColor_Black;
					w->m_pLeft->m_Color = CNode::EColor_Black; // w->m_pLeft is non-null

					RotateRight (p);
					x = m_pRoot;
				}
			}

			p = x->m_pParent;
		}
	 
		if (x)
			x->m_Color = CNode::EColor_Black;

	#if _AXL_RTL_RBTREE_ASSERT_VALID
		AssertValid ();
	#endif
	}

};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TKey, 
	typename TValue, 
	typename TCmp = CCmpT <TKey>
	>
class CRbTreeMapT: public CRbTreeT <
	TKey, 
	TCmp, 
	CRbTreeMapNodeT <TKey, TValue> 
	>
{
};

//.............................................................................

} // namespace rtl
} // namespace axl
