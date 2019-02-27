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

#define _AXL_SL_SINGLETON_H

#include "axl_sl_SimpleSingleton.h"
#include "axl_ref_New.h"
#include "axl_g_Module.h"

namespace axl {
namespace sl {

//..............................................................................

template <class T>
class DestructSingleton:
	public ref::RefCount,
	public g::Finalizer
{
public:
	T* m_p;

public:
	virtual
	void
	finalize()
	{
		m_p->~T();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <class T>
class ConstructSingleton
{
public:
	typedef DestructSingleton<T> Destruct;

public:
	void
	operator () (void* p)
	{
		new(p)T;
		ref::Ptr<Destruct> destruct = AXL_REF_NEW(Destruct);
		destruct->m_p = (T*)p;
		g::getModule()->addFinalizer(destruct);
	}
};

//..............................................................................

template <typename T>
T*
getSingleton(volatile int32_t* flag = NULL)
{
	static uchar_t buffer[sizeof(T)] = { 0 };
	callOnce(ConstructSingleton<T> (), buffer, flag);
	return (T*)buffer;
}

//..............................................................................

} // namespace sl
} // namespace axl
