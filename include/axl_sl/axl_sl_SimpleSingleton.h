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

#define _AXL_SL_SIMPLESINGLETON_H

#include "axl_sl_CallOnce.h"

namespace axl {
namespace sl {

//..............................................................................

// singleton that doesnt require destructor

template <class T>
class ConstructSimpleSingleton
{
public:
	void
	operator () (void* p)
	{
		new(p)T;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
getSimpleSingleton(volatile int32_t* flag = NULL)
{
	static uchar_t buffer[sizeof(T)] = { 0 };
	callOnce(ConstructSimpleSingleton<T> (), buffer, flag);
	return (T*)buffer;
}

//..............................................................................

} // namespace sl
} // namespace axl
