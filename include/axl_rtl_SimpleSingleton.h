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
class CConstructSimpleSingletonT
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
GetSimpleSingleton (volatile int32_t* pFlag = NULL)
{
	static uchar_t _Buffer [sizeof (T)] = { 0 };	
	mt::CallOnce (CConstructSimpleSingletonT <T> (), _Buffer, pFlag);
	return (T*) _Buffer;
}

//.............................................................................

} // namespace rtl
} // namespace axl

