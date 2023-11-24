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

#define _AXL_SL_UNSIGNEDTYPE_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

template <size_t N>
class UnsignedTypeImpl {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class UnsignedTypeImpl<1> {
public:
	typedef uint8_t Type;
};

template <>
class UnsignedTypeImpl<2> {
public:
	typedef uint16_t Type;
};

template <>
class UnsignedTypeImpl<4> {
public:
	typedef uint32_t Type;
};

template <>
class UnsignedTypeImpl<8> {
public:
	typedef uint64_t Type;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UnsignedType: public UnsignedTypeImpl<sizeof(T)> {
};

//..............................................................................

} // namespace sl
} // namespace axl
