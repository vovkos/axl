// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_LIST_H

#include "axl_rtl_ListBase.h"
#include "axl_rtl_Func.h"
#include "axl_mem_New.h"

namespace axl {
namespace rtl {

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

} // namespace rtl
} // namespace axl

