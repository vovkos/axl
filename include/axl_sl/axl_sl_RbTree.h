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

#define _AXL_SL_RBTREE_H

#include "axl_sl_BinTree.h"

namespace axl {
namespace sl {

//..............................................................................

enum RbColor
{
	RbColor_Black = 0,
	RbColor_Red   = 1
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename NodeData
	>
class RbTreeNodeBase: public BinTreeNodeBase <T, NodeData>
{
public:
	RbColor m_color;

	static
	void
	onXcg (
		T* node1,
		T* node2
		)
	{
		RbColor oldColor = node1->m_color;
		node1->m_color = node2->m_color;
		node2->m_color = oldColor;
	}

	static
	int
	getColor (T* node)
	{
		return node ? node->m_color : RbColor_Black;
	}

#ifdef _AXL_DEBUG
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

		if (node->m_color == RbColor_Black)
		{
			leftCount++;
		}
		else // red color cannot have red children
		{
			ASSERT (getColor (node->m_left) == RbColor_Black);
			ASSERT (getColor (node->m_right) == RbColor_Black);
		}

		return leftCount;
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Key>
class RbTreeNode: public RbTreeNodeBase <
	RbTreeNode <Key>,
	KeyNodeData <Key>
	>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
	>
class RbTreeMapNode: public RbTreeNodeBase <
	RbTreeMapNode <Key, Value>,
	KeyValueNodeData <Key, Value>
	>
{
};

//..............................................................................

template <typename Key>
class RbTreeIterator: public Iterator <RbTreeNode <Key> >
{
public:
	RbTreeIterator ()
	{
	}

	RbTreeIterator (const Iterator <RbTreeNode <Key> >& src)
	{
		this->m_p = src.getEntry ();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
	>
class RbTreeMapIterator: public Iterator <RbTreeMapNode <Key, Value> >
{
public:
	RbTreeMapIterator ()
	{
	}

	RbTreeMapIterator (const Iterator <RbTreeMapNode <Key, Value> >& src)
	{
		this->m_p = src.getEntry ();
	}
};

//..............................................................................

template <
	typename Key,
	typename Cmp = Cmp <Key>,
	typename KeyArg = ArgType <Key>,
	typename Node = RbTreeNode <Key>
	>
class RbTree: public BinTreeBase <
	RbTree <Key, Cmp, KeyArg, Node>,
	Key,
	Cmp,
	KeyArg,
	Node
	>
{
public:
#ifdef _AXL_DEBUG
	void
	assertValid ()
	{
		Node::assertValid (this->m_root);
	}
#endif

	void
	onInsert (Node* x)
	{
		x->m_color = RbColor_Red; // make new node red

		// check Red-Black properties
		while (x != this->m_root && x->m_parent->m_color == RbColor_Red)
		{
			// we have a violation
			if (x->m_parent == x->m_parent->m_parent->m_left)
			{
				Node *y = x->m_parent->m_parent->m_right;
				if (Node::getColor (y) == RbColor_Red)
				{
					// uncle is red
					x->m_parent->m_color = RbColor_Black;
					y->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
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
					x->m_parent->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
					this->rotateRight (x->m_parent->m_parent);
				}
			}
			else
			{
				// mirror image of above code
				Node *y = x->m_parent->m_parent->m_left;
				if (Node::getColor (y) == RbColor_Red)
				{
					// uncle is red
					x->m_parent->m_color = RbColor_Black;
					y->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
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

					x->m_parent->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
					this->rotateLeft (x->m_parent->m_parent);
				}
			}
		}

		this->m_root->m_color = RbColor_Black;

	#if (_AXL_SL_RBTREE_ASSERT_VALID)
		assertValid ();
	#endif
	}

	void
	onErase (Node* node)
	{
		Node* x = this->replaceWithChild (node);
		Node* p = node->m_parent;

		// no need to fixup red node deletion
		if (node->m_color == RbColor_Red)
			return;

		while (x != this->m_root && Node::getColor (x) == RbColor_Black)
		{
			// due to invariants of RB-tree black node must have a non-null sibling
			// if this sibling is red, both of its children are non-null black

			if (x == p->m_left)
			{
				Node *w = p->m_right; // non-null, see above

				if (w->m_color == RbColor_Red)
				{
					w->m_color = RbColor_Black;
					p->m_color = RbColor_Red;
					this->rotateLeft (p);
					w = p->m_right; // non-null black, see above
				}

				// w is non-null black

				if (Node::getColor (w->m_left) == RbColor_Black &&
					Node::getColor (w->m_right) == RbColor_Black)
				{
					w->m_color = RbColor_Red;
					x = p;
				}
				else // at least one of w children is red
				{
					if (Node::getColor (w->m_right) == RbColor_Black)
					{
						// w->m_pLeft is red
						w->m_left->m_color = RbColor_Black;
						w->m_color = RbColor_Red;
						this->rotateRight (w);
						w = p->m_right; // non-null black (just rotated here)
					}

					w->m_color = p->m_color;
					p->m_color = RbColor_Black;
					w->m_right->m_color = RbColor_Black; // w->m_pRight is non-null

					this->rotateLeft (p);
					x = this->m_root;
				}
			}
			else
			{
				Node *w = p->m_left; // non-null, see above

				if (w->m_color == RbColor_Red)
				{
					w->m_color = RbColor_Black;
					p->m_color = RbColor_Red;
					this->rotateRight (p);
					w = p->m_left; // non-null black, see above
				}

				// w is non-null black

				if (Node::getColor (w->m_left) == RbColor_Black &&
					Node::getColor (w->m_right) == RbColor_Black)
				{
					w->m_color = RbColor_Red;
					x = p;
				}
				else // at least one of w children is red
				{
					if (Node::getColor (w->m_left) == RbColor_Black)
					{
						// w->m_pRight is red
						w->m_right->m_color = RbColor_Black;
						w->m_color = RbColor_Red;
						this->rotateLeft (w);
						w = p->m_left; // non-null black (just rotated)
					}

					w->m_color = p->m_color;
					p->m_color = RbColor_Black;
					w->m_left->m_color = RbColor_Black; // w->m_pLeft is non-null

					this->rotateRight (p);
					x = this->m_root;
				}
			}

			p = x->m_parent;
		}

		if (x)
			x->m_color = RbColor_Black;

	#if (_AXL_SL_RBTREE_ASSERT_VALID)
		assertValid ();
	#endif
	}

};

//..............................................................................

template <
	typename Key,
	typename Value,
	typename Cmp = Cmp <Key>,
	typename KeyArg = typename ArgType <Key>::Type,
	typename ValueArg = typename ArgType <Value>::Type
	>
class RbTreeMap: public RbTree <
	Key,
	Cmp,
	KeyArg,
	RbTreeMapNode <Key, Value>
	>
{
public:
	typedef sl::Iterator <RbTreeMapNode <Key, Value> > Iterator;

public:
	Value&
	operator [] (KeyArg key)
	{
		return this->visit (key)->m_value;
	}

	Value
	findValue (
		KeyArg key,
		ValueArg undefinedValue
		) const
	{
		Iterator it = this->find (key);
		return it ? it->m_value : undefinedValue;
	}

	Iterator
	add (
		KeyArg key,
		ValueArg value,
		bool* isNew = NULL
		)
	{
		size_t prevCount = this->getCount ();

		Iterator it = this->visit (key);
		it->m_value = value;

		if (isNew)
			*isNew = this->getCount () > prevCount;

		return it;
	}

	Iterator
	addIfNotExists (
		KeyArg key,
		ValueArg value
		)
	{
		size_t prevCount = this->getCount ();

		Iterator it = this->visit (key);

		if (this->getCount () == prevCount)
			return NULL;

		it->m_value = value;
		return it;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
