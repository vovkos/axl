// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_SINGLETON_H

#include "axl_rtl_SimpleSingleton.h"
#include "axl_g_Module.h"

namespace axl {
namespace rtl {

//.............................................................................

template <class T>
class CDestructSingletonT: public g::IFinalizer
{
public:
	AXL_OBJ_SIMPLE_CLASS (CDestructSingletonT, g::IFinalizer)

public:
	T* m_p;

public:
	virtual 
	void 
	Finalize ()
	{
		m_p->~T ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <class T>
class CConstructSingletonT
{
public:
	void* m_pBuffer;

public:
	CConstructSingletonT (void* p)
	{
		m_pBuffer = p;
	}

	void
	operator () ()
	{
		new (m_pBuffer) T;			
		CDestructSingletonT <T> Destruct;
		Destruct.m_p = (T*) m_pBuffer;
		g::GetModule ()->AddFinalizer (&Destruct);
	}
};

//.............................................................................

template <typename T>
T*
GetSingleton (volatile long* pFlag = NULL)
{
	static uchar_t _Buffer [sizeof (T)] = { 0 };
	exe::CallOnce (CConstructSingletonT <T> (_Buffer), pFlag);
	return (T*) _Buffer;
}

//.............................................................................

} // namespace rtl
} // namespace axl

