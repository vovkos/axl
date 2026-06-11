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

#define _AXL_SL_ARGTYPE_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
struct ArgTypeSelector {
	typedef typename std::conditional<std::is_scalar<T>::value, T, const T&>::type Type;
};

template <typename T>
using ArgType = typename ArgTypeSelector<T>::Type;

template <typename... Ts>
using ArgTypes = std::tuple<ArgType<Ts>...>;

//..............................................................................

} // namespace sl
} // namespace axl
