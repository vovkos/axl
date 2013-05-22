// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_RBTREE_H

#include "axl_rtl_BinTree.h"

namespace axl {
namespace rtl {

//.............................................................................

enum ERbColor
{
	ERbColor_Black = 0,
	ERbColor_Red   = 1
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TNodeData
	>
class CRbTreeNodeBaseT: public CBinTreeNodeBaseT <T, TNodeData>
{
public:
	ERbColor m_Color;

	CRbTreeNodeBaseT ()
	{ 
		m_Color = ERbColor_Black; 
	}

	static 
	void 
	OnXcg (
		T* pNode1, 
		T* pNode2
		)
	{
		ERbColor OldColor = pNode1->m_Color;
		pNode1->m_Color = pNode2->m_Color;
		pNode2->m_Color = OldColor;
	}

	static 
	int 
	GetColor (T* pNode)
	{ 
		return pNode ? pNode->m_Color : ERbColor_Black; 
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

		if (pNode->m_Color == ERbColor_Black)
		{
			LeftCount++;
		}
		else // red color cannot have red children
		{
			ASSERT (GetColor (pNode->m_pLeft) == ERbColor_Black);
			ASSERT (GetColor (pNode->m_pRight) == ERbColor_Black);
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
		TNode::AssertValid (this->m_pRoot); // thanks a lot gcc
	}
#endif

	void 
	OnInsert (TNode* x)
	{
		x->m_Color = ERbColor_Red; // make new node red

		// check Red-Black properties 
		while (x != this->m_pRoot && x->m_pParent->m_Color == ERbColor_Red) 
		{
			// we have a violation 
			if (x->m_pParent == x->m_pParent->m_pParent->m_pLeft) 
			{
				TNode *y = x->m_pParent->m_pParent->m_pRight;
				if (TNode::GetColor (y) == ERbColor_Red) 
				{
					// uncle is red 
					x->m_pParent->m_Color = ERbColor_Black;
					y->m_Color = ERbColor_Black;
					x->m_pParent->m_pParent->m_Color = ERbColor_Red;
					x = x->m_pParent->m_pParent;
				} 
				else 
				{
					// uncle is black
					if (x == x->m_pParent->m_pRight) 
					{
						// make x a m_pLeft child
						x = x->m_pParent;
						this->RotateLeft (x);
					}

					// recolor and rotate
					x->m_pParent->m_Color = ERbColor_Black;
					x->m_pParent->m_pParent->m_Color = ERbColor_Red;
					this->RotateRight (x->m_pParent->m_pParent);
				}
			} 
			else 
			{
				// mirror image of above code
				TNode *y = x->m_pParent->m_pParent->m_pLeft;
				if (TNode::GetColor (y) == ERbColor_Red) 
				{
					// uncle is red
					x->m_pParent->m_Color = ERbColor_Black;
					y->m_Color = ERbColor_Black;
					x->m_pParent->m_pParent->m_Color = ERbColor_Red;
					x = x->m_pParent->m_pParent;
				} 
				else 
				{
					// uncle is black
					if (x == x->m_pParent->m_pLeft) 
					{
						x = x->m_pParent;
						this->RotateRight (x);
					}

					x->m_pParent->m_Color = ERbColor_Black;
					x->m_pParent->m_pParent->m_Color = ERbColor_Red;
					this->RotateLeft (x->m_pParent->m_pParent);
				}
			}
		}

		this->m_pRoot->m_Color = ERbColor_Black;

	#if (_AXL_RTL_RBTREE_ASSERT_VALID)
		AssertValid ();
	#endif
	}

	void 
	OnDelete (TNode* pNode)
	{ 
		TNode* x = this->ReplaceWithChild (pNode); 
		TNode* p = pNode->m_pParent;

		// no need to fixup red node deletion
		if (pNode->m_Color == ERbColor_Red) 
			return;

		while (x != this->m_pRoot && TNode::GetColor (x) == ERbColor_Black) 
		{
			// due to invariants of RB-tree black node must have a non-null sibling 
			// if this sibling is red, both of its children are non-null black 

			if (x == p->m_pLeft) 
			{
				TNode *w = p->m_pRight; // non-null, see above

				if (w->m_Color == ERbColor_Red) 
				{
					w->m_Color = ERbColor_Black;
					p->m_Color = ERbColor_Red;
					this->RotateLeft (p); 
					w = p->m_pRight; // non-null black, see above
				}

				// w is non-null black

				if (TNode::GetColor (w->m_pLeft) == ERbColor_Black && 
					TNode::GetColor (w->m_pRight) == ERbColor_Black) 
				{
					w->m_Color = ERbColor_Red;
					x = p;
				} 
				else // at least one of w children is red 
				{
					if (TNode::GetColor (w->m_pRight) == ERbColor_Black) 
					{
						// w->m_pLeft is red
						w->m_pLeft->m_Color = ERbColor_Black; 
						w->m_Color = ERbColor_Red;
						this->RotateRight (w); 
						w = p->m_pRight; // non-null black (just rotated here)
					}
	                
					w->m_Color = p->m_Color;
					p->m_Color = ERbColor_Black;
					w->m_pRight->m_Color = ERbColor_Black; // w->m_pRight is non-null

					this->RotateLeft (p); 
					x = this->m_pRoot; 
				}
			} 
			else 
			{
				TNode *w = p->m_pLeft; // non-null, see above

				if (w->m_Color == ERbColor_Red) 
				{
					w->m_Color = ERbColor_Black;
					p->m_Color = ERbColor_Red;
					this->RotateRight (p); 
					w = p->m_pLeft; // non-null black, see above
				}
	            
				// w is non-null black

				if (TNode::GetColor (w->m_pLeft) == ERbColor_Black && 
					TNode::GetColor (w->m_pRight) == ERbColor_Black) 
				{
					w->m_Color = ERbColor_Red;
					x = p;
				} 
				else // at least one of w children is red 
				{
					if (TNode::GetColor (w->m_pLeft) == ERbColor_Black) 
					{
						// w->m_pRight is red
						w->m_pRight->m_Color = ERbColor_Black;
						w->m_Color = ERbColor_Red;
						this->RotateLeft (w); 
						w = p->m_pLeft; // non-null black (just rotated)
					}

					w->m_Color = p->m_Color;
					p->m_Color = ERbColor_Black;
					w->m_pLeft->m_Color = ERbColor_Black; // w->m_pLeft is non-null

					this->RotateRight (p); 
					x = this->m_pRoot; 
				}
			}

			p = x->m_pParent;
		}
	 
		if (x)
			x->m_Color = ERbColor_Black;

	#if (_AXL_RTL_RBTREE_ASSERT_VALID)
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
