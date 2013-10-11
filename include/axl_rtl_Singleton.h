// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_SINGLETON_H

#include "axl_rtl_SimpleSingleton.h"
#include "axl_ref_Factory.h"
#include "axl_g_Module.h"

namespace axl {
namespace rtl {

//.............................................................................

template <class T>
class CDestructSingletonT: 
	public g::CFinalizer,
	public ref::CRefCount
{
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
	typedef CDestructSingletonT <T> CDestruct;

public:
	void
	operator () (void* p)
	{
		new (p) T;			
		ref::CPtrT <CDestruct> Destruct = AXL_REF_NEW (CDestruct);
		Destruct->m_p = (T*) p;
		g::GetModule ()->AddFinalizer (Destruct);
	}
};

//.............................................................................

template <typename T>
T*
GetSingleton (volatile int32_t* pFlag = NULL)
{
	static uchar_t _Buffer [sizeof (T)] = { 0 };
	mt::CallOnce (CConstructSingletonT <T> (), _Buffer, pFlag);
	return (T*) _Buffer;
}

//.............................................................................

} // namespace rtl
} // namespace axl

