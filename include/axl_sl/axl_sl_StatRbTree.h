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

#define _AXL_SL_STATRBTREE_H

#include "axl_sl_StatBinTree.h"
#include "axl_sl_RbBalancer.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Value,
	typename Stat = Value,
	typename ValueStatArgs = ArgTypes<Value, Stat>
>
using StatRbTreeNode = StatBinTreeNode<
	Value,
	Stat,
	RbBalancer,
	ValueStatArgs
>;

template <
	typename Value,
	typename Stat = Value,
	typename ValueStatArgs = ArgTypes<Value, Stat>
>
using StatRbTreeIterator = Iterator<StatRbTreeNode<Value, Stat, ValueStatArgs> >;

template <
	typename Value,
	typename Stat = Value,
	typename ValueStatArgs = ArgTypes<Value, Stat>
>
using ConstStatRbTreeIterator = ConstIterator<StatRbTreeNode<Value, Stat, ValueStatArgs> >;

template <
	typename Value,
	typename Stat = Value,
	typename ValueStatArgs = ArgTypes<Value, Stat>
>
using StatRbTree = StatBinTree<
	StatRbTreeNode<Value, Stat, ValueStatArgs>,
	RbBalancer
>;

//..............................................................................

} // namespace sl
} // namespace axl
