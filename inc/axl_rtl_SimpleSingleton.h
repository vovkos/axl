// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_SIMPLESINGLETON_H

#include "axl_exe_CallOnce.h"

namespace axl {
namespace rtl {

//.............................................................................

// singleton that doesnt require destructor

template <class T>
class CConstructSimpleSingletonT
{
protected:
	void* m_pBuffer;

public:
	CConstructSimpleSingletonT (void* p)
	{
		m_pBuffer = p;
	}

	void
	operator () ()
	{
		new (m_pBuffer) T;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
GetSimpleSingleton (volatile long* pFlag = NULL)
{
	static uchar_t _Buffer [sizeof (T)] = { 0 };	
	exe::CallOnce (CConstructSimpleSingletonT <T> (_Buffer), pFlag);	
	return (T*) _Buffer;
}

//.............................................................................

} // namespace rtl
} // namespace axl

