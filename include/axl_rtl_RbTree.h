// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	ERbColor m_color;

	CRbTreeNodeBaseT ()
	{ 
		m_color = ERbColor_Black; 
	}

	static 
	void 
	onXcg (
		T* node1, 
		T* node2
		)
	{
		ERbColor oldColor = node1->m_color;
		node1->m_color = node2->m_color;
		node2->m_color = oldColor;
	}

	static 
	int 
	getColor (T* node)
	{ 
		return node ? node->m_color : ERbColor_Black; 
	}

#ifdef _DEBUG
	static 
	size_t 
	assertValid (T* node)
	{
		if (!node)
			return 1;

		size_t leftCount = assertValid (node->m_left);
		size_t rightCount = assertValid (node->m_right);

		// should be the same count of black nodes on the way down

		ASSERT (leftCount == rightCount);

		if (node->m_color == ERbColor_Black)
		{
			leftCount++;
		}
		else // red color cannot have red children
		{
			ASSERT (getColor (node->m_left) == ERbColor_Black);
			ASSERT (getColor (node->m_right) == ERbColor_Black);
		}

		return leftCount;
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
	assertValid ()
	{ 
		TNode::assertValid (this->m_root); // thanks a lot gcc
	}
#endif

	void 
	onInsert (TNode* x)
	{
		x->m_color = ERbColor_Red; // make new node red

		// check Red-Black properties 
		while (x != this->m_root && x->m_parent->m_color == ERbColor_Red) 
		{
			// we have a violation 
			if (x->m_parent == x->m_parent->m_parent->m_left) 
			{
				TNode *y = x->m_parent->m_parent->m_right;
				if (TNode::getColor (y) == ERbColor_Red) 
				{
					// uncle is red 
					x->m_parent->m_color = ERbColor_Black;
					y->m_color = ERbColor_Black;
					x->m_parent->m_parent->m_color = ERbColor_Red;
					x = x->m_parent->m_parent;
				} 
				else 
				{
					// uncle is black
					if (x == x->m_parent->m_right) 
					{
						// make x a m_pLeft child
						x = x->m_parent;
						this->rotateLeft (x);
					}

					// recolor and rotate
					x->m_parent->m_color = ERbColor_Black;
					x->m_parent->m_parent->m_color = ERbColor_Red;
					this->rotateRight (x->m_parent->m_parent);
				}
			} 
			else 
			{
				// mirror image of above code
				TNode *y = x->m_parent->m_parent->m_left;
				if (TNode::getColor (y) == ERbColor_Red) 
				{
					// uncle is red
					x->m_parent->m_color = ERbColor_Black;
					y->m_color = ERbColor_Black;
					x->m_parent->m_parent->m_color = ERbColor_Red;
					x = x->m_parent->m_parent;
				} 
				else 
				{
					// uncle is black
					if (x == x->m_parent->m_left) 
					{
						x = x->m_parent;
						this->rotateRight (x);
					}

					x->m_parent->m_color = ERbColor_Black;
					x->m_parent->m_parent->m_color = ERbColor_Red;
					this->rotateLeft (x->m_parent->m_parent);
				}
			}
		}

		this->m_root->m_color = ERbColor_Black;

	#if (_AXL_RTL_RBTREE_ASSERT_VALID)
		assertValid ();
	#endif
	}

	void 
	onErase (TNode* node)
	{ 
		TNode* x = this->replaceWithChild (node); 
		TNode* p = node->m_parent;

		// no need to fixup red node deletion
		if (node->m_color == ERbColor_Red) 
			return;

		while (x != this->m_root && TNode::getColor (x) == ERbColor_Black) 
		{
			// due to invariants of RB-tree black node must have a non-null sibling 
			// if this sibling is red, both of its children are non-null black 

			if (x == p->m_left) 
			{
				TNode *w = p->m_right; // non-null, see above

				if (w->m_color == ERbColor_Red) 
				{
					w->m_color = ERbColor_Black;
					p->m_color = ERbColor_Red;
					this->rotateLeft (p); 
					w = p->m_right; // non-null black, see above
				}

				// w is non-null black

				if (TNode::getColor (w->m_left) == ERbColor_Black && 
					TNode::getColor (w->m_right) == ERbColor_Black) 
				{
					w->m_color = ERbColor_Red;
					x = p;
				} 
				else // at least one of w children is red 
				{
					if (TNode::getColor (w->m_right) == ERbColor_Black) 
					{
						// w->m_pLeft is red
						w->m_left->m_color = ERbColor_Black; 
						w->m_color = ERbColor_Red;
						this->rotateRight (w); 
						w = p->m_right; // non-null black (just rotated here)
					}
	                
					w->m_color = p->m_color;
					p->m_color = ERbColor_Black;
					w->m_right->m_color = ERbColor_Black; // w->m_pRight is non-null

					this->rotateLeft (p); 
					x = this->m_root; 
				}
			} 
			else 
			{
				TNode *w = p->m_left; // non-null, see above

				if (w->m_color == ERbColor_Red) 
				{
					w->m_color = ERbColor_Black;
					p->m_color = ERbColor_Red;
					this->rotateRight (p); 
					w = p->m_left; // non-null black, see above
				}
	            
				// w is non-null black

				if (TNode::getColor (w->m_left) == ERbColor_Black && 
					TNode::getColor (w->m_right) == ERbColor_Black) 
				{
					w->m_color = ERbColor_Red;
					x = p;
				} 
				else // at least one of w children is red 
				{
					if (TNode::getColor (w->m_left) == ERbColor_Black) 
					{
						// w->m_pRight is red
						w->m_right->m_color = ERbColor_Black;
						w->m_color = ERbColor_Red;
						this->rotateLeft (w); 
						w = p->m_left; // non-null black (just rotated)
					}

					w->m_color = p->m_color;
					p->m_color = ERbColor_Black;
					w->m_left->m_color = ERbColor_Black; // w->m_pLeft is non-null

					this->rotateRight (p); 
					x = this->m_root; 
				}
			}

			p = x->m_parent;
		}
	 
		if (x)
			x->m_color = ERbColor_Black;

	#if (_AXL_RTL_RBTREE_ASSERT_VALID)
		assertValid ();
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
