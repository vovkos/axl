// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_SIMPLESINGLETON_H

#include "axl_mt_CallOnce.h"

namespace axl {
namespace mt {

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
	static uchar_t buffer [sizeof (T)] = { 0 };	
	callOnce (ConstructSimpleSingleton <T> (), buffer, flag);
	return (T*) buffer;
}

//.............................................................................

} // namespace mt
} // namespace axl
