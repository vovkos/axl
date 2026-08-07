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

#define _AXL_SL_RBBALANCER_H

#include "axl_sl_BinTreeBase.h"

namespace axl {
namespace sl {

//..............................................................................

enum RbColor {
	RbColor_Black = 0,
	RbColor_Red   = 1
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct RbMeta {
	friend class RbBalancer;

protected:
	RbColor m_color;

public:
	RbMeta() {
		m_color = RbColor_Black; // the root never goes through onInsert
	}

	RbColor
	getColor() const {
		return m_color;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class RbBalancer {
public:
	typedef RbMeta Meta;

public:
	template <typename Node>
	static
	RbColor
	getColor(const Node* p) {
		return p ? p->m_color : RbColor_Black;
	}

	template <typename T>
	static
	void
	onInsert(
		T* tree,
		typename T::Node* x
	) {
		typedef typename T::Node Node;

		x->m_color = RbColor_Red; // make new node red

		// check Red-Black properties
		while (x != tree->m_root && x->m_parent->m_color == RbColor_Red) {
			// we have a violation
			if (x->m_parent == x->m_parent->m_parent->m_left) {
				Node* y = x->m_parent->m_parent->m_right;
				if (getColor(y) == RbColor_Red) {
					// uncle is red
					x->m_parent->m_color = RbColor_Black;
					y->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
					x = x->m_parent->m_parent;
				} else {
					// uncle is black
					if (x == x->m_parent->m_right) {
						// make x a m_left child
						x = x->m_parent;
						tree->rotateLeft(x);
					}

					// recolor and rotate
					x->m_parent->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
					tree->rotateRight(x->m_parent->m_parent);
				}
			} else {
				// mirror image of above code
				Node* y = x->m_parent->m_parent->m_left;
				if (getColor(y) == RbColor_Red) {
					// uncle is red
					x->m_parent->m_color = RbColor_Black;
					y->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
					x = x->m_parent->m_parent;
				} else {
					// uncle is black
					if (x == x->m_parent->m_left) {
						x = x->m_parent;
						tree->rotateRight(x);
					}

					x->m_parent->m_color = RbColor_Black;
					x->m_parent->m_parent->m_color = RbColor_Red;
					tree->rotateLeft(x->m_parent->m_parent);
				}
			}
		}

		tree->m_root->m_color = RbColor_Black;

	#if (_AXL_SL_RBTREE_ASSERT_VALID)
		assertValid(tree->m_root);
	#endif
	}

	template <typename T>
	static
	void
	onErase(
		T* tree,
		const typename T::EraseContext& context
	) {
		typedef typename T::Node Node;

		// no need to fixup red node deletion
		if (context.m_color == RbColor_Red)
			return;

		Node* x = context.m_child;
		Node* p = context.m_parent;

		while (x != tree->m_root && getColor(x) == RbColor_Black) {
			// due to invariants of RB-tree black node must have a non-null sibling
			// if this sibling is red, both of its children are non-null black

			if (x == p->m_left) {
				Node* w = p->m_right; // non-null, see above

				if (w->m_color == RbColor_Red) {
					w->m_color = RbColor_Black;
					p->m_color = RbColor_Red;
					tree->rotateLeft(p);
					w = p->m_right; // non-null black, see above
				}

				// w is non-null black

				if (getColor(w->m_left) == RbColor_Black &&
					getColor(w->m_right) == RbColor_Black) {
					w->m_color = RbColor_Red;
					x = p;
				} else { // at least one of w children is red
					if (getColor(w->m_right) == RbColor_Black) {
						// w->m_left is red
						w->m_left->m_color = RbColor_Black;
						w->m_color = RbColor_Red;
						tree->rotateRight(w);
						w = p->m_right; // non-null black (just rotated here)
					}

					w->m_color = p->m_color;
					p->m_color = RbColor_Black;
					w->m_right->m_color = RbColor_Black; // w->m_right is non-null

					tree->rotateLeft(p);
					x = tree->m_root;
				}
			} else {
				Node* w = p->m_left; // non-null, see above

				if (w->m_color == RbColor_Red) {
					w->m_color = RbColor_Black;
					p->m_color = RbColor_Red;
					tree->rotateRight(p);
					w = p->m_left; // non-null black, see above
				}

				// w is non-null black

				if (getColor(w->m_left) == RbColor_Black &&
					getColor(w->m_right) == RbColor_Black) {
					w->m_color = RbColor_Red;
					x = p;
				} else { // at least one of w children is red
					if (getColor(w->m_left) == RbColor_Black) {
						// w->m_right is red
						w->m_right->m_color = RbColor_Black;
						w->m_color = RbColor_Red;
						tree->rotateLeft(w);
						w = p->m_left; // non-null black (just rotated)
					}

					w->m_color = p->m_color;
					p->m_color = RbColor_Black;
					w->m_left->m_color = RbColor_Black; // w->m_left is non-null

					tree->rotateRight(p);
					x = tree->m_root;
				}
			}

			p = x->m_parent;
		}

		if (x)
			x->m_color = RbColor_Black;

	#if (_AXL_SL_RBTREE_ASSERT_VALID)
		assertValid(tree->m_root);
	#endif
	}

#ifdef _AXL_DEBUG
	template <typename Node>
	static
	size_t
	assertValid(const Node* p) {
		if (!p)
			return 1;

		size_t leftCount = assertValid(p->m_left);
		size_t rightCount = assertValid(p->m_right);

		// should be the same count of black nodes on the way down

		ASSERT(leftCount == rightCount);

		if (p->m_color == RbColor_Black) {
			leftCount++;
		} else { // red color cannot have red children
			ASSERT(getColor(p->m_left) == RbColor_Black);
			ASSERT(getColor(p->m_right) == RbColor_Black);
		}

		return leftCount;
	}
#endif
};

//..............................................................................

} // namespace sl
} // namespace axl
