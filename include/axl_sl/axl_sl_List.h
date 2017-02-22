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

#define _AXL_SL_LIST_H

#include "axl_sl_ListBase.h"
#include "axl_mem_New.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class StdList: public List <T, typename mem::StdDelete <T>, GetLink>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class CppList: public List <T, typename mem::CppDelete <T>, GetLink>
{
};

//..............................................................................

} // namespace sl
} // namespace axl
