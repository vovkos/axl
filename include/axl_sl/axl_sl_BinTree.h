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

#include "axl_sl_BinTreeBase.h"
#include "axl_sl_MapEntry.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value,
	typename Balancer,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
struct BinTreeNode : public BinTreeNodeBase<
	BinTreeNode<Key, Value, Balancer, KeyValueArgs>,
	MapEntry<Key, Value, KeyValueArgs>,
	typename std::tuple_element<0, KeyValueArgs>::type,
	Balancer
> {
	typedef sl::MapEntry<Key, Value, KeyValueArgs> MapEntry;
	typedef typename MapEntry::KeyArg KeyArg;
	typedef sl::BinTreeNodeBase<BinTreeNode, MapEntry, KeyArg, Balancer> BinTreeNodeBase;

	template <
		typename Node,
		typename Balancer2,
		typename Cmp
	>
	friend class BinTree;

	BinTreeNode(
		typename MapEntry::KeyArg key,
		BinTreeNode* parent
	):
		BinTreeNodeBase(key, parent)
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Node0,
	typename Balancer,
	typename Cmp
>
class BinTree: public BinTreeBase<
	BinTree<Node0, Balancer, Cmp>,
	Node0,
	Balancer
> {
public:
	typedef Node0 Node;
	typedef typename Node::Key Key;
	typedef typename Node::Value Value;
	typedef typename Node::KeyArg KeyArg;
	typedef typename Node::ValueArg ValueArg;
	typedef sl::Iterator<Node> Iterator;
	typedef sl::ConstIterator<Node> ConstIterator;

protected:
	Cmp m_cmp;

public:
	explicit BinTree(const Cmp& cmp = Cmp()) {
		m_cmp = cmp;
	}

	Value&
	operator [] (KeyArg key) {
		return visit(key)->m_value;
	}

	const Value&
	operator [] (KeyArg key) const {
		return const_cast<BinTree*>(this)->visit(key)->m_value;
	}

	Iterator
	find(KeyArg key) {
		Node* leNode = findLeNode(key);
		return leNode && !m_cmp(leNode->m_key, key) ? leNode : NULL;
	}

	ConstIterator
	find(KeyArg key) const {
		return const_cast<BinTree*>(this)->find(key);
	}

	template <RelOpKind opKind>
	Iterator
	find(KeyArg key) {
		Node* leNode = findLeNode(key);
		switch (opKind) {
		case RelOpKind_Lt:
			return leNode ?
				m_cmp(leNode->m_key, key) ? leNode : Iterator(leNode).getPrev() :
				NULL;

		case RelOpKind_Le:
			return leNode;

		case RelOpKind_Gt:
			return leNode ? Iterator(leNode).getNext() : this->getHead();

		case RelOpKind_Ge:
			return leNode ?
				!m_cmp(leNode->m_key, key) ? leNode : Iterator(leNode).getNext() :
				this->getHead();

		default:
			return leNode && !m_cmp(leNode->m_key, key) ? leNode : NULL;
		}
	}

	template <RelOpKind opKind>
	ConstIterator
	find(KeyArg key) const {
		return const_cast<BinTree*>(this)->find<opKind>(key);
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
		Node* node = this->m_root;
		if (!node) {
			node = new Node(key, NULL);
			this->m_root = node;
			return this->m_nodeList.insertTail(node);
		}

		Node* child;
		for (;;)
			if (m_cmp(key, node->m_key)) {
				if (node->m_left)
					node = node->m_left;
				else {
					child = new Node(key, node);
					node->m_left = child;
					this->m_nodeList.insertBefore(child, node);
					break;
				}
			} else if (m_cmp(node->m_key, key)) {
				if (node->m_right)
					node = node->m_right;
				else {
					child = new Node(key, node);
					node->m_right = child;
					this->m_nodeList.insertAfter(child, node);
					break;
				}
			} else
				return node;

		Balancer::onInsert(this, child);
		return child;
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
		ASSERT(isNew);

		size_t prevCount = this->getCount();
		Iterator it = visit(key);
		it->m_value = value;
		*isNew = this->getCount() > prevCount;
		return it;
	}

	Iterator
	addIfNotExists(
		KeyArg key,
		ValueArg value
	) {
		size_t prevCount = this->getCount();
		Iterator it = visit(key);
		if (this->getCount() == prevCount)
			return NULL;

		it->m_value = value;
		return it;
	}

	bool
	eraseKey(KeyArg key) {
		Iterator it = find(key);
		if (!it)
			return false;

		this->erase(it);
		return true;
	}

#ifdef _AXL_DEBUG
	void
	assertValid() const {
		ASSERT(!this->m_root || !this->m_root->m_parent);

		ConstIterator it = this->getHead();
		size_t count = 0;
		assertValidNode(this->m_root, NULL, NULL, &it, &count);

		ASSERT(!it); // the chain must hold no nodes the tree doesn't
		ASSERT(count == this->getCount());
	}
#endif

protected:
	Node*
	findLeNode(KeyArg key) {
		Node* leNode = NULL;
		Node* node = this->m_root;
		while (node)
			if (m_cmp(key, node->m_key))
				node = node->m_left;
			else {
				leNode = node;
				node = node->m_right;
			}

		return leNode;
	}

	void
	adjustKey(
		Node* x,
		KeyArg key // the tree order must stays intact!
	) {
		ASSERT(!x->m_prev || m_cmp(((Node*)x->m_prev)->m_key, key));
		ASSERT(!x->m_next || m_cmp(key, ((Node*)x->m_next)->m_key));
		x->m_key = key;
	}

#ifdef _AXL_DEBUG
	void
	assertValidNode(
		const Node* p,
		const Node* lowerBound,
		const Node* higherBound,
		ConstIterator* it,
		size_t* count
	) const {
		if (!p)
			return;

		if (lowerBound)
			ASSERT(m_cmp(lowerBound->m_key, p->m_key)); // strictly greater than the lower bound

		if (higherBound)
			ASSERT(m_cmp(p->m_key, higherBound->m_key)); // strictly less than the upper bound

		if (p->m_left)
			ASSERT(p->m_left->m_parent == p);

		if (p->m_right)
			ASSERT(p->m_right->m_parent == p);

		assertValidNode(p->m_left, lowerBound, p, it, count);

		ASSERT(*it && *(*it) == p); // in-order position must match the chain
		(*it)++;
		(*count)++;

		assertValidNode(p->m_right, p, higherBound, it, count);
	}
#endif
};

//..............................................................................

} // namespace sl
} // namespace axl
