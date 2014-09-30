// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_SIMPLESINGLETON_H

#include "axl_mt_CallOnce.h"

namespace axl {
namespace rtl {

//.............................................................................

// singleton that doesnt require destructor

template <class T>
class ConstructSimpleSingleton
{
public:
	void
	operator () (void* p)
	{
		new (p) T;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
getSimpleSingleton (volatile int32_t* flag = NULL)
{
	static uchar_t _Buffer [sizeof (T)] = { 0 };	
	mt::callOnce (ConstructSimpleSingleton <T> (), _Buffer, flag);
	return (T*) _Buffer;
}

//.............................................................................

} // namespace rtl
} // namespace axl

