// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_THREAD_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class CThreadAttr
{
protected:
	pthread_attr_t m_Attr;
	
public:
	CThreadAttr ()
	{
		pthread_attr_init (&m_Attr);
	}

	~CThreadAttr ()
	{
		pthread_attr_destroy (&m_Attr);
	}
		
	operator const pthread_attr_t* () const
	{
		return &m_Attr;
	}

	operator pthread_attr_t* ()
	{
		return &m_Attr;
	}
	
	cpu_set_t
	GetAffinity () const
	{
		cpu_set_t CpuSet = { 0 };
		pthread_attr_getaffinity_np (&m_Attr, sizeof (cpu_set_t), &CpuSet);
		return CpuSet;
	}
	
	bool
	SetAffinity (cpu_set_t CpuSet)
	{
		int Result = pthread_attr_setaffinity_np (&m_Attr, sizeof (cpu_set_t), &CpuSet);
		return Result == 0 ? true : err::Fail (Result);		
	}
	
	size_t 
	GetStackSize () const
	{
		size_t Size = -1;
		pthread_attr_getstacksize (&m_Attr, &Size);
		return Size;
	}

	bool
	SetStackSize (size_t Size)
	{
		int Result = pthread_attr_setstacksize (&m_Attr, Size);
		return Result == 0 ? true : err::Fail (Result);		
	}
};

//.............................................................................

typedef void* (FThreadProc) (void* pContext);

//.............................................................................

class CThread
{
protected:
	pthread_t m_ThreadId;
	bool m_IsOpen;
	
public:
	CThread ()
	{
		m_IsOpen = false;
	}
	
	~CThread ()
	{
		Detach ();
	}
	
	operator pthread_t ()
	{
		return m_ThreadId;
	}
	
	bool
	IsOpen ()
	{
		return m_IsOpen;
	}
	
	bool 
	Create (
		const pthread_attr_t* pAttr,
		FThreadProc pfThreadProc,
		void* pContext = NULL
		);

	bool 
	Create (
		FThreadProc pfThreadProc,
		void* pContext = NULL
		)
	{
		return Create (NULL, pfThreadProc, pContext);
	}

	bool
	Join (
		uint_t Timeout,
		void** ppRetVal = NULL
		);
		
	bool
	Join (void** ppRetVal = NULL)
	{
		return Join (-1, ppRetVal);
	}

	bool
	Detach ();
};

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
