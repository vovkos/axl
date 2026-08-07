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
#include "axl_sl_RbBalancer.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using RbTreeNode = BinTreeNode<
	Key,
	Value,
	RbBalancer,
	KeyValueArgs
>;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using RbTreeIterator = Iterator<RbTreeNode<Key, Value, KeyValueArgs> >;

template <
	typename Key,
	typename Value,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using ConstRbTreeIterator = ConstIterator<RbTreeNode<Key, Value, KeyValueArgs> >;

template <
	typename Key,
	typename Value,
	typename Cmp = Lt<Key>,
	typename KeyValueArgs = ArgTypes<Key, Value>
>
using RbTree = BinTree<
	RbTreeNode<Key, Value, KeyValueArgs>,
	RbBalancer,
	Cmp
>;

//..............................................................................

} // namespace sl
} // namespace axl
