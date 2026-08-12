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
	typename Stat,
	typename Value,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
using StatRbTreeNode = StatBinTreeNode<
	Stat,
	Value,
	RbBalancer,
	StatValueArgs
>;

template <
	typename Stat,
	typename Value,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
using StatRbTreeIterator = Iterator<StatRbTreeNode<Stat, Value, StatValueArgs> >;

template <
	typename Stat,
	typename Value,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
using ConstStatRbTreeIterator = ConstIterator<StatRbTreeNode<Stat, Value, StatValueArgs> >;

template <
	typename Stat,
	typename Value,
	typename StatValueArgs = ArgTypes<Stat, Value>
>
using StatRbTree = StatBinTree<
	StatRbTreeNode<Stat, Value, StatValueArgs>,
	RbBalancer
>;

//..............................................................................

} // namespace sl
} // namespace axl
