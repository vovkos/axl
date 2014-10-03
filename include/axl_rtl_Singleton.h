// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
class DestructSingleton: 
	public g::Finalizer,
	public ref::RefCount
{
public:
	T* m_p;

public:
	virtual 
	void 
	finalize ()
	{
		m_p->~T ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <class T>
class ConstructSingleton
{
public: 
	typedef DestructSingleton <T> Destruct;

public:
	void
	operator () (void* p)
	{
		new (p) T;			
		ref::Ptr <Destruct> destruct = AXL_REF_NEW (Destruct);
		destruct->m_p = (T*) p;
		g::getModule ()->addFinalizer (destruct);
	}
};

//.............................................................................

template <typename T>
T*
getSingleton (volatile int32_t* flag = NULL)
{
	static uchar_t buffer [sizeof (T)] = { 0 };
	mt::callOnce (ConstructSingleton <T> (), buffer, flag);
	return (T*) buffer;
}

//.............................................................................

} // namespace rtl
} // namespace axl

