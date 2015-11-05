// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_LIST_H

#include "axl_sl_ListBase.h"
#include "axl_sl_Func.h"
#include "axl_mem_New.h"

namespace axl {
namespace sl {

//.............................................................................

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class StdList: public List <T, typename mem::StdDelete <T>, GetLink>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitCast <T*, ListLink*>
	>
class CppList: public List <T, typename mem::CppDelete <T>, GetLink>
{
};

//.............................................................................

} // namespace sl
} // namespace axl

