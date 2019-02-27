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

// for most aggregates, we want to pass it as a const-reference

template <typename T>
class ArgType
{
public:
	typedef const T& Type;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// for simple types, its more efficient to pass it as is

template <typename T>
class SimpleArgType
{
public:
	typedef T Type;
};

//..............................................................................

// specialization for primitive types

template <>
class ArgType<char>: public SimpleArgType<char>
{
};

template <>
class ArgType<wchar_t>: public SimpleArgType<wchar_t>
{
};

template <>
class ArgType<float>: public SimpleArgType<float>
{
};

template <>
class ArgType<double>: public SimpleArgType<double>
{
};

template <>
class ArgType<int8_t>: public SimpleArgType<int8_t>
{
};

template <>
class ArgType<uint8_t>: public SimpleArgType<uint8_t>
{
};

template <>
class ArgType<int16_t>: public SimpleArgType<int16_t>
{
};

template <>
class ArgType<uint16_t>: public SimpleArgType<uint16_t>
{
};

template <>
class ArgType<int32_t>: public SimpleArgType<int32_t>
{
};

template <>
class ArgType<uint32_t>: public SimpleArgType<uint32_t>
{
};

template <>
class ArgType<int64_t>: public SimpleArgType<int64_t>
{
};

template <>
class ArgType<uint64_t>: public SimpleArgType<uint64_t>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for pointers

template <typename T>
class ArgType<T*>: public SimpleArgType<T*>
{
};

//..............................................................................

} // namespace sl
} // namespace axl
